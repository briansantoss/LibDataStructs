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
TESTDIR = test/

SRCS = $(wildcard $(SRCDIR)*.c)
OBJS = $(patsubst $(SRCDIR)%.c, $(OBJDIR)%.o, $(SRCS))
SAMPLES_SRCS = $(wildcard $(SAMPLEDIR)*.c)
SAMPLES = $(patsubst $(SAMPLEDIR)%.c, $(BINDIR)%, $(SAMPLES_SRCS))
TESTS_SRCS = $(wildcard $(TESTDIR)*.c)
TESTS = $(patsubst $(TESTDIR)%.c,$(BINDIR)%, $(TESTS_SRCS))

.PHONY: all lib samples tests run_tests debug clean

all: lib samples

lib: $(OBJS) | $(LIBDIR)
	@echo "Creating static library..."
	@ar rcs $(LIBDIR)lib$(LIB_STATIC) $^

	@ar rcs $(LIBDIR)lib$(LIB_STATIC) $^

$(OBJDIR)%.o: $(SRCDIR)%.c | $(OBJDIR)
	@$(CC) $(CFLAGS) -c $< -o $@

samples: $(SAMPLES)

$(BINDIR)%: $(SAMPLEDIR)%.c lib | $(BINDIR)
	@$(CC) $(CFLAGS) $< -L$(LIBDIR) -l$(BASENAME) -o $@

run_tests : tests
	@echo "========================"
	@echo "Running tests..."
	@for test in $(TESTS); do echo "Running $$test:"; ./$$test; done
	@echo -e "\n========================"

tests: $(TESTS)

$(BINDIR)%: $(TESTDIR)%.c lib | $(BINDIR)
	@$(CC) $(CFLAGS) $< -L$(LIBDIR) -l$(BASENAME) -o $@

$(OBJDIR):
	@mkdir -p $@

$(BINDIR):
	@mkdir -p $@

$(LIBDIR):
	@mkdir -p $@

debug:
	@$(MAKE) -s DEBUG=1

clean: 
	@rm -f $(OBJDIR)*  $(BINDIR)* $(LIBDIR)*