#include <stdlib.h>
#include "usage.h"
#include "convert.h"

int
main(int argc, char *argv[]) {

    int errs = 0;

    if (argc <= 1) {
        usage(argv[0]);
        return EXIT_FAILURE;
    }

    for (int i=1; i<argc; i++) {
        if (convert_file(argv[i])) {
            errs++;
        }

    }

    return errs;
}
