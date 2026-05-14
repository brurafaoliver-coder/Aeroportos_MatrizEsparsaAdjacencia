CC      = gcc
CFLAGS  = -Wall -Wextra -g -std=c11
TARGET  = aeroportos
SRCS    = main.c sparse_matrix.c airport_vector.c airport_graph.c
OBJS    = $(SRCS:.c=.o)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

run: all
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
