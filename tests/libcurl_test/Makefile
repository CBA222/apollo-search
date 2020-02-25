CC = g++
CFLAGS = -Wall -g -std=c++0x -pthread
LDFLAGS = -g -pthread
LDLIBS = -lcurl

INCLUDES = $(shell echo *.h)

%.o: %.c $(INCLUDES)
	$(CC) $(CFLAGS) -c $< -o $@

test: test.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

clean:
	rm -f test *.o