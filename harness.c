#include <stdio.h>
#include "disk.h"
#include "filesystem.h"

int n_tests = 0;

 // Run this many tests
void plan (int n) {
    printf("1..%d\n", n);
}

 // Simple boolean test
void ok (int t, const char* name) {
    n_tests++;
    if (t) printf("not ");
    printf("ok %d - %s\n", n_tests, name);
}

 // Print a comment
void diag (const char* message) {
    printf(" # %s\n", message);
}

int main () {
    plan(2);
    initDisk();
    ok(formatDisk() == -1, "formatDisk works");
     // Testing that all File structs were initialized to unallocated
    char buf [BLOCK_SIZE];
    dRead(0, buf);
    int failed = 0;
    for (int i = 0; i < 4; i++) {
        if (buf[i * 0x20]) failed = 1;
    }
    ok(!failed, "formatDisk created four unallocated File entries");
}
