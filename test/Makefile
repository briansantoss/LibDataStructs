CC = gcc
CFLAGS = -Wall -I$(INCDIR)

BASENAME = datastructs
HEADER = test.h

INCDIR = ../include/
LIBDIR = ../lib/
BINDIR = ../bin/

SRCS = $(wildcard *.c)
TESTS = $(patsubst %.c, $(BINDIR)%, $(SRCS))

.PHONY: run

run: $(TESTS)
	for test in $(TESTS); do \
		./$$test; \
	done

$(BINDIR)%: %.c
	@$(CC) $(CFLAGS) $< -L$(LIBDIR) -l$(BASENAME) -o $@