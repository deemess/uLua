# c compiler and linker options
CC=g++
CFLAGS=-c -Wall
LDFLAGS=

# ulua interpreter
ULUA_SRC=platform_PC.c native.c gc.c vm.c ulua.c
ULUA_EXE=ulua
ULUA_OBJ=$(ULUA_SRC:.cpp=.o)

# lemon parse generator
LEMON_SRC=lemon.c
LEMON_EXE=lemon
LEMON_OBJ=$(LEMON_SRC:.cpp=.o)

# ulua compiler
ULUAC_SRC=platform_PC.c native.c gc.c vm.c ulua.c
ULUAC_EXE=uluac
ULUAC_OBJ=$(ULUA_SRC:.cpp=.o)

all: ulua_i lemon_p ulua_c

ulua_i: $(ULUA_SRC) $(ULUA_EXE)
lemon_p: $(LEMON_SRC) $(LEMON_EXE)
ulua_c:  $(ULUAC_SRC) $(ULUAC_EXE)

$(ULUA_EXE): $(ULUA_OBJ) 
	$(CC) $(LDFLAGS) $(ULUA_OBJ) -o $@
$(LEMON_EXE): $(LEMON_OBJ) 
	$(CC) $(LDFLAGS) $(LEMON_OBJ) -o $@
$(ULUAC_EXE): $(ULUAC_OBJ) 
	$(CC) $(LDFLAGS) $(ULUAC_OBJ) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(ULUA_EXE) $(LEMON_EXE)