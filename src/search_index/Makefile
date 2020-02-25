CC = g++
CFLAGS = -Wall -g -std=c++0x -pthread
LDFLAGS = -g -pthread
LDLIBS = -lcurl -lcmph

INCLUDES = $(shell echo *.h)

%.o: %.c $(INCLUDES)
	$(CC) $(CFLAGS) -c $< -o $@

main: main.o index.o fast_hash.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

test: test.o index.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

clean:
	rm -f main *.o