FLAGS := -o3 -Werror -Wall -fomit-frame-pointer
CC := gcc
main: main.c getData.h
	$(CC) $(FLAGS) main.c -o zap
clean:
	rm zap
	rm a.out

