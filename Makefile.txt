CC      = gcc
CFLAGS  = -Wall -Wextra -Wpedantic -Werror -O0 -g
LIBS	= -lm -lxml2 -lcurl -I/usr/include/libxml2

# If dcc is available, use it
ifneq ($(shell which dcc),)
	CC = dcc
endif

.PHONY: all clean

all: crawler 

crawler: crawler.c graph.c list.c
	$(CC) $(CFLAGS) $(POSIX) -o $@ $^ $(LIBS)

clean:
	rm -f crawler
	rm -rf *.dSYM
