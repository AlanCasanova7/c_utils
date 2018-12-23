CC=clang
CFLAGS=-O2 -Wall -Werror -I.
BINARY_TESTS=run_tests.exe

all: run_tests.exe

run_tests.exe: tests.o dictionary.o murmur3.o
	$(CC) -o $(BINARY_TESTS) $^
	./$(BINARY_TESTS)

tests.o: tests.c aiv_unit_test.h
	$(CC) -c -o $@ $(CFLAGS) $<

dictionary.o: dictionary.c dictionary.h
	$(CC) -c -o $@ $(CFLAGS) $<

murmur3.o: murmur3.c murmur3.h
	$(CC) -c -o $@ $(CFLAGS) $<