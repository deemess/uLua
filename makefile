# c compiler and linker options
CC=gcc
CFLAGS=-g -c -Wall
LDFLAGS=-lm

# lemon parse generator
LEMON_SRC=lemon.c
LEMON_EXE=lemon
LEMON_OBJ=$(LEMON_SRC:.c=.o)

# ulua interpreter
ULUA_SRC=udebug.c native.c gc.c vm.c ulexer.c ucodegen.c uparser.c ulua.c
ULUA_EXE=ulua
ULUA_OBJ=$(ULUA_SRC:.c=.o)

# ulua compiler
ULUAC_SRC=udebug.c ulexer.c ucodegen.c uparser.c uluac.c
ULUAC_EXE=uluac
ULUAC_OBJ=$(ULUAC_SRC:.c=.o)

# ulua interpreter
ULUAI_SRC=native.c gc.c vm.c uluai.c
ULUAI_EXE=uluai
ULUAI_OBJ=$(ULUAI_SRC:.c=.o)
	
# all projects
all: clean lemon_p ulua_i ulua_c ulua_r

alldebug: CFLAGS += -DDEBUG -g
alldebug: LDFLAGS += -DDEBUG -g
alldebug: lemon_p ulua_i ulua_c ulua_r

# project dependencies
lemon_p: $(LEMON_SRC) $(LEMON_EXE)
ulua_i: $(ULUAI_SRC) $(ULUAI_EXE)
ulua_c:  lemon_g $(ULUAC_SRC) $(ULUAC_EXE)
ulua_r:	$(ULUA_SRC) $(ULUA_EXE)

# projects build configurations
$(ULUA_EXE): $(ULUA_OBJ) 
	$(CC) $(LDFLAGS) $(ULUA_OBJ) -o $@
$(LEMON_EXE): $(LEMON_OBJ) 
	$(CC) $(LDFLAGS) $(LEMON_OBJ) -o $@
$(ULUAC_EXE): $(ULUAC_OBJ) 
	$(CC) $(LDFLAGS) $(ULUAC_OBJ) -o $@
$(ULUAI_EXE): $(ULUAI_OBJ)
	$(CC) $(LDFLAGS) $(ULUAI_OBJ) -o $@

lemon_g:
	-./lemon uparser.y

# common
.c.o:
	$(CC) $(CFLAGS) $< -o $@
clean:
	rm -f *.o  $(ULUA_EXE) $(LEMON_EXE) $(ULUAC_EXE) $(ULUAI_EXE)
