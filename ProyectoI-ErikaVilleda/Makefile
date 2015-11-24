all: DeCaf

DeCaf: ast.o DeCaf_parser.o DeCaf_lexer.o main.o
	g++ -o $@ $^

ast.o: ast.cpp ast.h
	g++ -c -o $@ $<

main.o: main.cpp ast.h
	g++ -c -o $@ $<

DeCaf_lexer.o: DeCaf_lexer.cpp
	g++ -c -o $@ $<

DeCaf_lexer.cpp: DeCaf.l ast.h
	flex -o $@ $<

DeCaf_parser.cpp: DeCaf.y ast.h
	bison --defines=tokens.h -o $@ $<

DeCaf_parse.o: DeCaf_parser.cpp
	g++ -c -o $@ $<

clean:
	rm -f *.o
	rm -f DeCaf_lexer.cpp
	rm -f DeCaf_parser.cpp
	rm -f tokens.h
	rm -f DeCaf
