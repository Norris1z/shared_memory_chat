CC=clang
FLAGS= -Wall -Wextra -pedantic

.PHONY: user1 user2 clean

user1: 
		$(CC) user1.c $(FLAGS) -o user1 -std=c11
user2: 
		$(CC) user2.c $(FLAGS) -o user2 -std=c11

user1_leakcheck: 
		$(CC) user1.c $(FLAGS) -fsanitize=address -o user1 -O0 -ggdb3 -std=c11

user2_leakcheck: 
		$(CC) user2.c $(FLAGS) -fsanitize=address -o user2 -O0 -ggdb3 -std=c11
		
clean:
	rm -f user1 && rm -f user2 