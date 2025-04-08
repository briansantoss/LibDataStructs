CC = gcc 
CFLAGS = -Wall -I$(INCDIR)

MAINTARGET = datastructs
SAMPLE = $(BINDIR)sample

SRCDIR = src/
INCDIR = include/ 
OBJDIR = obj/
BINDIR = bin/
LIBDIR = lib/
SAMPLEDIR = sample/

SRCS = $(wildcard $(SRCDIR)*.c)
OBJS = $(patsubst $(SRCDIR)%.c, $(OBJDIR)%.o, $(SRCS))

lib: $(OBJS)
	@ar rcs $(LIBDIR)lib$(MAINTARGET).a $^

$(OBJDIR)%.o: $(SRCDIR)%.c 
	@$(CC) $(CFLAGS) -c $< -o $@


run: $(SAMPLE)
	@./$(SAMPLE)

$(SAMPLE): $(SAMPLEDIR)sample.c $(LIBDIR)lib$(MAINTARGET).a
	@$(CC) $(CFLAGS) -L$(LIBDIR) $< -o $@ -l$(MAINTARGET)

clean: 
	@rm -f $(OBJDIR)*
	@rm -f $(BINDIR)*
	@rm -f $(LIBDIR)*