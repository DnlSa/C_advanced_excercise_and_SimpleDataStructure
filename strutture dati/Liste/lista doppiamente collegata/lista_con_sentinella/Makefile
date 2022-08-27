CFLAGS=-Wall -Wextra -Wpedantic -g
TARGET=list
SRCS=main.c listac.c
OBJS=$(SRCS:.c=.o)

%.o:%.c
	$(CC) $(CFLAGS) $< -c -o $@


all: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

.PHONY:clean
clean:
	-$(RM) $(TARGET) $(OBJS)
