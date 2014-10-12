# c compiler and linker options
CC=gcc
CFLAGS=-c -Wall
LDFLAGS=

# ulua interpreter
ULUA_SRC=platform_PC.c native.c gc.c vm.c ulua.c
ULUA_EXE=ulua
ULUA_OBJ=$(ULUA_SRC:.c=.o)

# lemon parse generator
LEMON_SRC=lemon.c
LEMON_EXE=lemon
LEMON_OBJ=$(LEMON_SRC:.c=.o)

# ulua compiler
ULUAC_SRC=ulexer.c ucodegen.c uparser.c uluac.c
ULUAC_EXE=uluac
ULUAC_OBJ=$(ULUAC_SRC:.c=.o)

# ulua repl
ULUAR_SRC=platform_PC.c native.c gc.c vm.c ulexer.c ucodegen.c uparser.c uluar.c
ULUAR_EXE=uluar
ULUAR_OBJ=$(ULUAR_SRC:.c=.o)
	
# all projects
all: ulua_i lemon_p ulua_c ulua_r

alldebug: CFLAGS += -DDEBUG -g
alldebug: LDFLAGS += -DDEBUG -g
alldebug: ulua_i lemon_p ulua_c ulua_r

# project dependencies
ulua_i: $(ULUA_SRC) $(ULUA_EXE)
lemon_p: $(LEMON_SRC) $(LEMON_EXE)
ulua_c:  lemon_g $(ULUAC_SRC) $(ULUAC_EXE)
ulua_r:	$(ULUAR_SRC) $(ULUAR_EXE)

# projects build configurations
$(ULUA_EXE): $(ULUA_OBJ) 
	$(CC) $(LDFLAGS) $(ULUA_OBJ) -o $@
$(LEMON_EXE): $(LEMON_OBJ) 
	$(CC) $(LDFLAGS) $(LEMON_OBJ) -o $@
$(ULUAC_EXE): $(ULUAC_OBJ) 
	$(CC) $(LDFLAGS) $(ULUAC_OBJ) -o $@
$(ULUAR_EXE): $(ULUAR_OBJ)
	$(CC) $(LDFLAGS) $(ULUAR_OBJ) -o $@

lemon_g:
	-./lemon uparser.y

# common
.c.o:
	$(CC) $(CFLAGS) $< -o $@
clean:
	rm -f *.o  $(ULUA_EXE) $(LEMON_EXE) $(ULUAC_EXE) $(ULUAR_EXE)
