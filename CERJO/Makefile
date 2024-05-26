CC = gcc
CFLAGS = -Wall -Wextra -Werror
DEPS = performance.h utils.h athlete.h
OBJ = main.o performance.o athlete.o utils.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(OBJ) main main-debug

