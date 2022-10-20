all: test
	./test

test: $(wildcard *.c) ../target/$(TARGET)/libkettle.so
	gcc -g -o test $^
