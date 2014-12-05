#include "ulexer.h"

ERROR_CODE error = E_NONE;
static reserved keywords[] = {
	{TK_AND, "and"},
	{TK_BREAK, "break"},
	{TK_DO, "do"},
	{TK_ELSEIF, "elseif"},
	{TK_ELSE, "else"},
	{TK_END, "end"},
	{TK_FALSE, "false"},
	{TK_FOR, "for"},
	{TK_FUNCTION, "function"},
	{TK_IF, "if"},
	{TK_IN, "in"},
	{TK_LOCAL, "local"},
	{TK_NIL, "nil"},
	{TK_NOT, "not"},
	{TK_OR, "or"},
	{TK_REPEAT, "repeat"},
	{TK_RETURN, "return"},
	{TK_THEN, "then"},
	{TK_TRUE, "true"},
	{TK_UNTIL, "until"},
	{TK_WHILE, "while"},
/*	{TK_CONCAT, ".."},
	{TK_DOTS, "..."},
	{TK_EQ, "=="},
	{TK_GE, ">="},
	{TK_LE, "<="},
	{TK_NE, "~="},
	{TK_NAME, "<name>"},
	{TK_NUMBER, "<number>"},
	{TK_STRING, "<string>"},
	{TK_EOS, "<eof>"},*/
};

void lexerror(ERROR_CODE code) {
	error = code;
}

ERROR_CODE getLastULexError() {
	return error;
}

u08 nextchar(LexState* ls) {
	u08 c = ls->z[ls->zp++];
	ls->current = c;
	return c;
}

void setInput(LexState* ls, u08* stream) {
	error = E_NONE;

	ls->decpoint = (u08)'.';
	ls->lookahead.token = TK_EOS;
	ls->z = stream;
	ls->zp = 0;
	ls->linenumber = 1;
	ls->lastline = 1;
	ls->buff.bempty=TRUE;
	ls->t.semInfo.bempty=TRUE;
	ls->lookahead.semInfo.bempty=TRUE;
	nextchar(ls);
}

BOOL currIsNewline(LexState* ls) {
	return ls->current == '\n' || ls->current == '\r';
}

void inclinenumber(LexState* ls) {
	u08 old = ls->current;
	nextchar(ls);  // skip '\n' or '\r'
	if (currIsNewline(ls) && ls->current != old) {
		nextchar(ls);  // skip '\n\r' or '\r\n'
	}
	ls->linenumber++;
}


void save(LexState* ls, u08 c) {
	if(ls->buff.bempty == TRUE) { //if buff empty - start counting new one
		ls->buff.bempty = FALSE;
		ls->buff.bp = ls->zp-1;
		ls->buff.bplen = 0;
	}
	ls->buff.bplen++;
}

u08 save_and_next(LexState* ls) {
	save(ls, ls->current);
	return nextchar(ls);
}

BOOL check_next (LexState *ls, u08 c1, u08 c2) {
	if(c2 == 0) {
		if (!ls->current != c1)
			return FALSE;
	} else {
		if (!ls->current != c1 || !ls->current != c2)
			return FALSE;
	}
	save_and_next(ls);
	return TRUE;
}

u16 skip_sep(LexState* ls) {
	u08 count = 0;
	u08 c = ls->current;
	save_and_next(ls);
	while (ls->current == '=') {
		save_and_next(ls);
		count++;
	}
	return (ls->current == c) ? count : (-count) - 1;
}

u08 str2f (Token *t, u08 *stream) {
	char *endptr;
	t->number.fvalue = (float)strtod((const char*)&stream[t->semInfo.bp], &endptr);
	if (endptr == (char*)&stream[t->semInfo.bp]) 
		return 0;  /* conversion failed */
	if (*endptr == 'x' || *endptr == 'X')  /* maybe an hexadecimal constant? */
		t->number.fvalue = (float)strtoul((const char*)&stream[t->semInfo.bp], &endptr, 16);
	if (endptr == (char*)&stream[t->semInfo.bp + t->semInfo.bplen]) 
		return 1;  /* most common case */
	while (isspace((unsigned char) *endptr)) endptr++;
	if (*endptr != '\0') return 0;  /* invalid trailing characters? */
		return 1;
}

void read_numeral (LexState *ls, Token *t) {
	do {
		save_and_next(ls);
	} while (isdigit(ls->current) || ls->current == '.');
	if (check_next(ls, 'E','e'))  // `E'? 
		check_next(ls, '+','-');  // optional exponent sign 
	while (isalnum(ls->current) || ls->current == '_')
		save_and_next(ls);

	t->semInfo.bempty = FALSE;
	t->semInfo.bp = ls->buff.bp;
	t->semInfo.bplen = ls->buff.bplen;
	if(!str2f(t, ls->z))
		lexerror(E_NUMBERFORMAT);
}

void read_long_string(LexState* ls, Token* t, u16 sep) {
	save_and_next(ls);//  -- skip 2nd '['
	if(currIsNewline(ls)) { // string starts with a newline?
		inclinenumber(ls);  // skip it
	}
	while(TRUE) {
		u08 c = ls->current;
		if(c == 0) { //end of stream
			lexerror(E_LONGSTR);
		} else if(c == ']') { 
			if (skip_sep(ls) == sep) {
				save_and_next(ls);  // skip 2nd ']'
				break;
			}
		} else if(currIsNewline(ls)) {
			save(ls, '\n');
			inclinenumber(ls);
			//if(!t)  ls->buff.bempty=TRUE;  // avoid wasting space
		} else { //default
			if(t) {
				save_and_next(ls);
			} else {
				nextchar(ls);
			}
		}
	}
	if(t) {
		t->semInfo.bempty = FALSE;
		t->semInfo.bp = ls->buff.bp;
		t->semInfo.bplen = ls->buff.bplen;
	}
}

void read_string (LexState *ls, int del, Token *t) {
  save_and_next(ls);
  while (ls->current != del) {
    switch (ls->current) {
      case 0:
        lexerror(E_UNFINISHSTR);
        continue;  // to avoid warnings 
      case '\n':
      case '\r':
        lexerror(E_UNFINISHSTR);
        continue;  // to avoid warnings 
      case '\\': {
        int c;
        nextchar(ls);  // do not save the `\'
        switch (ls->current) {
          case 'a': c = '\a'; break;
          case 'b': c = '\b'; break;
          case 'f': c = '\f'; break;
          case 'n': c = '\n'; break;
          case 'r': c = '\r'; break;
          case 't': c = '\t'; break;
          case 'v': c = '\v'; break;
          case '\n':  /* go through */
          case '\r': save(ls, '\n'); inclinenumber(ls); continue;
          case 0: continue; // will raise an error next loop 
          default: {
            if (!isdigit(ls->current))
              save_and_next(ls);  /* handles \\, \", \', and \? */
            else {  /* \xxx */
              int i = 0;
              c = 0;
              do {
                c = 10*c + (ls->current-'0');
                nextchar(ls);
              } while (++i<3 && isdigit(ls->current));
              save(ls, c);
            }
            continue;
          }
        }
        save(ls, c);
        nextchar(ls);
        continue;
      }
      default:
        save_and_next(ls);
    }
  }
	save_and_next(ls);  // skip delimiter 
	t->semInfo.bempty = FALSE;
	t->semInfo.bp = ls->buff.bp+1;
	t->semInfo.bplen = ls->buff.bplen-2;  
}

u16 llex(LexState* ls, Token* t) {
	u16 c,sep ;
	u08 i;

	ls->buff.bempty = TRUE;
	ls->buff.bp = 0;
	ls->buff.bplen = 0;
	t->semInfo.bempty = TRUE;

	while(TRUE) {
		c = ls->current;
		if(currIsNewline(ls)) {
			inclinenumber(ls);
		} else if(c == '-') { //minus ------------------------------------------------------
			c = nextchar(ls);
			if(c != '-') {
				return TK_MINUS;
			} else {//is a comment
				sep = -1;
				if(nextchar(ls) == '[') {
					sep = skip_sep(ls);
					//ls.buff = ""  -- 'skip_sep' may dirty the buffer
				}
				if (sep >= 0) {
					read_long_string(ls, NULL, sep);  // long comment
					//ls.buff = ""
				} else { // else short comment
					while (!currIsNewline(ls) && ls->current != 0) { //EOS
						nextchar(ls);
					}
				}
			}
		} else if(c == '[') { //----------------------------------------------------------------
			sep = skip_sep(ls);
			if(sep >= 0) {
				read_long_string(ls, t, sep);
				return TK_STRING;
			} else if(sep == -1) {
				return TK_SLPAREN;
			} else {
				lexerror(E_LONGSTRDELIM);
			}
		} else if(c == '=') { //----------------------------------------------------------------
			c = nextchar(ls);
			if (c != '=') {
				return TK_SET;
			} else {
				nextchar(ls); 
				return TK_EQ; 
			}
		} else if (c == '<') { //----------------------------------------------------------------
			c = nextchar(ls);
			if (c != '=') {
				return TK_L;
			} else { 
				nextchar(ls); 
				return TK_LE;
			}
		} else if (c == '>') { //----------------------------------------------------------------
			c = nextchar(ls);
			if (c != '=') {
				return TK_G;
			} else { 
				nextchar(ls); 
				return TK_GE;
			}
		} else if (c == '~') { //----------------------------------------------------------------
			c = nextchar(ls);
			if (c != '=') {
				return '~';
			} else { 
				nextchar(ls); 
				return TK_NE;
			}
		} else if(c == '\\' || c == '\'') { //----------------------------------------------------------------
			read_string(ls, c, t);
			return TK_STRING;
		} else if(c == '.') {  //----------------------------------------------------------------
			save_and_next(ls);
			if (check_next(ls, '.', 0)) {
				if (check_next(ls, '.', 0))
					return TK_DOTS;   // ... 
				else 
					return TK_CONCAT;   // .. 
			} else if (!isdigit(ls->current))  {
				return TK_DOT;
			} else {
				read_numeral(ls, t);
				return TK_NUMBER;
			}
		} else if(c == 0) {
			return TK_EOS;
		}  else { // default ----------------------------------------------------------------
			if (isspace(ls->current)) {
			  //lua_assert(!currIsNewline(ls));
			  nextchar(ls);
			  continue;
			}
			else if (isdigit(ls->current)) {
			  read_numeral(ls, t);
			  return TK_NUMBER;
			}
			else if (isalpha(ls->current) || ls->current == '_') {
			  /* identifier or reserved word */
			  do {
				save_and_next(ls);
			  } while (isalnum(ls->current) || ls->current == '_');

			  t->semInfo.bempty = ls->buff.bempty;
			  t->semInfo.bp = ls->buff.bp;
			  t->semInfo.bplen = ls->buff.bplen;

			  //try to match word with reserved keyword
			  for(i=0; i<21; i++) {
				  u08 p = 0;
				  BOOL matched = FALSE;
				  while(keywords[i].keyword[p] != 0 && p <10) {
					  if(keywords[i].keyword[p] == ls->z[ls->buff.bp+p]) {
						matched = TRUE;
						p++;
						continue;
					  } else {
						  matched = FALSE;
						  break;
					  }
				  }

				  if(matched) {
					  return keywords[i].type;
				  }
			  }

			  //not matched with reserved
			  return TK_NAME;
			} else {
			  int c = ls->current;
			  nextchar(ls);
			  // single-char tokens (+ - / ...)
			  //return c;  
			  switch(c) {
			  case '+':
				  return TK_PLUS;
			  case '-':
				  return TK_MINUS;
			  case '*':
				  return TK_TIMES;
			  case '/':
			  case '\\':
				  return TK_DIVIDE;
			  case '(':
				  return TK_LPAREN;
			  case ')':
				  return TK_RPAREN;
			  case ';':
				  return TK_SEMICOL;
			  case ':':
				  return TK_COLON;
			  case ',':
				  return TK_COMMA;
			  case ']':
				  return TK_SRPAREN;
			  case '{':
				  return TK_LBRACE;
			  case '}':
				  return TK_RBRACE;
			  case '#':
				  return TK_HASH;
			  case '%':
				  return TK_MOD;
			  case '^':
				  return TK_POW;
			  default:
				  return c;
			  }
			}
		}
	}
}

void next(LexState* ls) {
	ls->lastline = ls->linenumber;
	if(ls->lookahead.token != TK_EOS) {
		ls->t.token = ls->lookahead.token;
		ls->lookahead.token = TK_EOS;
	} else {
		ls->t.token = llex(ls, &ls->t);
	}
}
