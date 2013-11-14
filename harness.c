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
    plan(1);
    initDisk();
    ok(formatDisk() == -1, "formatDisk works");
}
