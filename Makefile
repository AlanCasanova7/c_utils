CC=clang
CFLAGS=-O2 -Wall -I. -Wno-pragma-pack
LDFLAGS=-L. -lSDL2
BINARY_TESTS= main.exe

all: run_tests.exe

run_tests.exe: tests.o dictionary.o hash.o
	$(CC) -o $(BINARY_TESTS) $^
	./$(BINARY_TESTS)

tests.o: tests.c aiv_unit_test.h my_math.h
	$(CC) -c -o $@ $(CFLAGS) $<

main.exe: main.o obj_parser.o rasterizer.o
	$(CC) -o $(BINARY_TESTS) $(LDFLAGS) $^
	./$(BINARY_TESTS)

main.o: main.c
	$(CC) -c -o $@ $(CFLAGS) $^

dictionary.o: dictionary.c dictionary.h
	$(CC) -c -o $@ $(CFLAGS) $<

hash.o: hash.c hash.h
	$(CC) -c -o $@ $(CFLAGS) $<

rasterizer.o: rasterizer.c rasterizer.h my_math.h
	$(CC) -c -o $@ $(CFLAGS) $<

obj_parser.o: obj_parser.c obj_parser.h rasterizer.o
	$(CC) -c -o $@ $(CFLAGS) $<