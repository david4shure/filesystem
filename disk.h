#include <stdio.h>
#define NUM_BLOCKS 100
#define BLOCK_SIZE 50

#define RELIABILITY 0.95
#define PERIOD 2147483647.0
#define ERROR 0
#define NO_ERROR 1

void initDisk();
int dRead(int addr, char *buf);
int dWrite(int addr, char *buf);
