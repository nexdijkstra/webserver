server: process.c server.c queue.c
	gcc -o server process.c server.c queue.c -pthread
