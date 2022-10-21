all: test
	./test

test: $(wildcard *.c) ../target/$(TARGET)/libkettle.so
	gcc -g -o test $^

clean:
	rm test 2> /dev/null || true
