CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic
SRC = main.c request.c print.c route.c response.c todo.c json.c

.PHONY: all clean

all: todo_api_0 todo_api_1 todo_api_2 todo_api_3 todo_api_4 todo_api_5 todo_api_6 todo_api_7

todo_api_0: $(SRC) todo_api.h
	$(CC) $(CFLAGS) -DLEVEL=0 $(SRC) -o todo_api_0

todo_api_1: $(SRC) todo_api.h
	$(CC) $(CFLAGS) -DLEVEL=1 $(SRC) -o todo_api_1

todo_api_2: $(SRC) todo_api.h
	$(CC) $(CFLAGS) -DLEVEL=2 $(SRC) -o todo_api_2

todo_api_3: $(SRC) todo_api.h
	$(CC) $(CFLAGS) -DLEVEL=3 $(SRC) -o todo_api_3

todo_api_4: $(SRC) todo_api.h
	$(CC) $(CFLAGS) -DLEVEL=4 $(SRC) -o todo_api_4

todo_api_5: $(SRC) todo_api.h
	$(CC) $(CFLAGS) -DLEVEL=5 $(SRC) -o todo_api_5

todo_api_6: $(SRC) todo_api.h
	$(CC) $(CFLAGS) -DLEVEL=6 $(SRC) -o todo_api_6

todo_api_7: $(SRC) todo_api.h
	$(CC) $(CFLAGS) -DLEVEL=7 $(SRC) -o todo_api_7

clean:
	rm -f todo_api_0 todo_api_1 todo_api_2 todo_api_3 todo_api_4 todo_api_5 todo_api_6 todo_api_7
