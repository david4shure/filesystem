
test : harness
	./harness

harness : disk.c disk.h filesystem.c filesystem.h harness.c
	gcc -std=c99 disk.c filesystem.c harness.c -o harness

.PHONY : test
