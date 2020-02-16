.PHONY: cmpl
cmpl:
	gcc -I../c c/nv-text.c c/nv-error.c c/nv-lex.c c/nv-compiler.c -o nv-lex
