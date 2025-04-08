CC = gcc 
CFLAGS = -Wall -I$(INCDIR)

MAINTARGET = $(LIBDIR)libdatastructs.a

SRCDIR = src/
INCDIR = include/ 
OBJDIR = obj/
BINDIR = bin/
LIBDIR = lib/
SAMPLEDIR = samples/

SRCS = $(wildcard $(SRCDIR)*.c)
OBJS = $(patsubst $(SRCDIR)%.c, $(OBJDIR)%.o, $(SRCS))
SRCSAMPLES = $(wildcard $(SAMPLEDIR)*.c)

all: $(OBJS)
	@ar rcs $(MAINTARGET) $^

$(OBJDIR)%.o: $(SRCDIR)%.c 
	@$(CC) $(CFLAGS) -c $< -o $@

$(BINDIR)%: $(SAMPLEDIR)%.c
	@$(CC) $(CFLAGS) $< -o $@

clean: 
	@rm -f $(OBJDIR)*
	@rm -f $(BINDIR)*
	@rm -f $(LIBDIR)*