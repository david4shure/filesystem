#include <stdio.h>
#include "disk.h"
#include "filesystem.h"

int n_tests = 0;

void plan (int n) {
    printf("1..%d\n", n);
}

void ok (int t, const char* name) {
    n_tests++;
    if (t) printf("not ");
    printf("ok %d - %s\n", n_tests, name);
}
void diag (const char* message) {
    printf(" # %s\n", message);
}

int main () {
    plan(1);
    initDisk();
    ok(formatDisk() == -1, "formatDisk works");
}
