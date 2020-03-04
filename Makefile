TARGET=occ
SRC := $(wildcard c/*.c)

.PHONY: cmpl clean tests
cmpl:$(SRC)
	gcc -I../c $(SRC) -o $(TARGET) -lncurses

clean:
	rm -f $(TARGET)
	OUTPUT_PATH=test.taa

tests: 
	@make -C test 

