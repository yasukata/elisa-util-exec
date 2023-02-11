PROGS = a.out

CC = gcc

CLEANFILES = $(PROGS) *.o *.d

LIBELISA_DIR ?= ../../lib/libelisa

CFLAGS = -O3 -pipe
CFLAGS += -g -rdynamic
CFLAGS += -Werror
CFLAGS += -Wall -Wunused-function
CFLAGS += -Wextra

CFLAGS += -I$(LIBELISA_DIR)/include

LDFLAGS += -ldl

C_SRCS = main.c

C_OBJS = $(C_SRCS:.c=.o)

OBJS = $(C_OBJS)

CLEANFILES += $(C_OBJS)

.PHONY: all
all: $(PROGS)

$(PROGS): $(OBJS) $(LIBELISA_DIR)/libelisa.a
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	-@rm -rf $(CLEANFILES)
