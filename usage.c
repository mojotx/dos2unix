//
// Created by Michael Jarvis on 7/15/22.
//

#include <stdio.h>
#include "usage.h"

void usage(const char *pn) {
    fprintf(stderr, "usage: %s <file1> [ <file2> .. <fileN> ]\n", pn);
}
