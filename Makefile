CC = gcc 
CFLAGS = -Wall -I$(INCDIR)

ifeq ($(DEBUG),1)
	CFLAGS += -DDEBUG
endif

BASENAME = datastructs
LIB_STATIC = $(BASENAME).a

SRCDIR = src/
INCDIR = include/ 
OBJDIR = obj/
BINDIR = bin/
LIBDIR = lib/
SAMPLEDIR = sample/

SRCS = $(wildcard $(SRCDIR)*.c)
OBJS = $(patsubst $(SRCDIR)%.c, $(OBJDIR)%.o, $(SRCS))
SAMPLES_SRCS = $(wildcard $(SAMPLEDIR)*.c)
SAMPLES = $(patsubst $(SAMPLEDIR)%.c, $(BINDIR)%, $(SAMPLES_SRCS))

lib: $(OBJS)
	@ar rcs $(LIBDIR)lib$(LIB_STATIC) $^

$(OBJDIR)%.o: $(SRCDIR)%.c 
	@$(CC) $(CFLAGS) -c $< -o $@

samples: $(SAMPLES) 

$(BINDIR)%: $(SAMPLEDIR)%.c lib
	@$(CC) $(CFLAGS) $< -L$(LIBDIR) -l$(BASENAME) -o $@

debug:
	@make DEBUG=1

clean: 
	@rm -f $(OBJDIR)*
	@rm -f $(BINDIR)*
	@rm -f $(LIBDIR)*