# Target library
lib := libuthread.a
file := queue.o context.o uthread.o preempt.o

all: $(lib)

# Current directory
CUR_PWD := $(shell pwd)

# Define compilation toolchain
CC	= gcc

# General gcc options
CFLAGS	:= -Wall -Werror
CFLAGS	+= -pipe
## Debug flag
ifneq ($(D),1)
CFLAGS	+= -O2
else
CFLAGS	+= -O0
CFLAGS	+= -g
endif

# Generate dependencies
DEPFLAGS = -MMD -MF $(@:.o=.d)

# Include dependencies
deps := $(patsubst %.o,%.d,$(file))
-include $(deps)

# Generic rule for linking final applications
$(lib): $(file) 
	ar rcs $@ $^

# Generic rule for compiling objects
%.o: %.c
	@echo "CC	$@"
	$(CC) $(CFLAGS) -c -o $@ $< $(DEPFLAGS)

# Cleaning rule
clean:
	@echo "CLEAN	$(CUR_PWD)"
	rm -rf $(file) $(deps) $(lib)