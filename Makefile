TARGET=occ
SRC := $(wildcard c/*.c)

.PHONY: cmpl clean
cmpl:$(SRC)
	gcc -I../c $(SRC) -o $(TARGET) -lncurses

clean:
	rm -f $(TARGET)
