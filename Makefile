CC = gcc
CFLAGS += -Wall -Werror -Wextra
CFLAGS_DEBUG = $(CFLAGS)
MAIN = my_bc
SRCS = my_bc.c stack.c queue.c tokens.c helper.c parser.c shunt_yard.c
OBJS = $(SRCS:.c=.o) 

all: $(MAIN)

$(MAIN): $(OBJS)
		$(CC) $(CFLAGS) -o $@ $(LINKERFLAG) $^

clean:
		$(RM) -f $(OBJS)

fclean: clean
		$(RM) -f $(MAIN) $(DEBUG)

re: fclean all