TARGET=http_server

CFLAGS=-Wall -Werror -g
OBJS=$(patsubst %.c, %.o, $(wildcard *.c))
HEADERS=http_server.h http_server_config.h

$(TARGET): $(OBJS)
	gcc -o $@ $^

%.o: %.c $(HEADERS)
	gcc -c $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGET) *.o

.PHONY: clean
