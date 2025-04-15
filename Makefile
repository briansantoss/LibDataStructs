CC = gcc 
CFLAGS = -Wall -I$(INCDIR)

MAINTARGET = datastructs

SRCDIR = src/
INCDIR = include/ 
OBJDIR = obj/
BINDIR = bin/
LIBDIR = lib/
SAMPLEDIR = sample/

SRCS = $(wildcard $(SRCDIR)*.c)
OBJS = $(patsubst $(SRCDIR)%.c, $(OBJDIR)%.o, $(SRCS))
SAMPLESSRCS = $(wildcard $(SAMPLEDIR)*.c)
SAMPLES = $(patsubst $(SAMPLEDIR)%.c, $(BINDIR)%, $(SAMPLESSRCS))

lib: $(OBJS)
	@ar rcs $(LIBDIR)lib$(MAINTARGET).a $^

$(OBJDIR)%.o: $(SRCDIR)%.c 
	@$(CC) $(CFLAGS) -c $< -o $@

samples: $(SAMPLES) 

$(BINDIR)%: $(SAMPLEDIR)%.c lib
	@$(CC) $(CFLAGS) $< -L$(LIBDIR) -l$(MAINTARGET) -o $@

clean: 
	@rm -f $(OBJDIR)*
	@rm -f $(BINDIR)*
	@rm -f $(LIBDIR)*