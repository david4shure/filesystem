
build : disk.c disk.h filesystem.c filesystem.h
	gcc -std=c99 disk.c filesystem.c

