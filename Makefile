CXX = clang++
CFLAGS = --std=c++17 -stdlib=libc++ -Wall -Wno-c++98-compat -Wno-global-constructors \
         -Wno-exit-time-destructors -Wno-zero-as-null-pointer-constant \
         -Wno-padded -Wno-unused-macros -o main  -Iinc/ -Igen/
PROJ = lisp
SRC = src/
INC = inc/

all: dirs main

test: all
	./main

dirs:
	@if [ ! -d gen ] ; then mkdir gen; fi

main: gen/lex.yy.cpp gen/tab.cpp $(SRC)ast.cpp
	@$(CXX) $(CFLAGS) -o main gen/lex.yy.cpp gen/tab.cpp $(SRC)ast.cpp


gen/lex.yy.cpp: $(SRC)$(PROJ).l
	@flex -o gen/lex.yy.cpp $(SRC)$(PROJ).l

gen/tab.cpp: $(SRC)$(PROJ).y $(INC)ast.h
	@bison -d -o gen/tab.cpp $(SRC)$(PROJ).y

clean:
	@rm -f main
	@rm -r gen


.PHONY: dirs main test clean
