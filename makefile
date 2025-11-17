FLAGS :=  -Werror -Wall -O3 -fomit-frame-pointer
CC := gcc
main: main.c zapFuncs.h
	$(CC) $(FLAGS) main.c -o zap
clean:
	rm zap
	clear
