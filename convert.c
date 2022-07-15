//
// Created by Michael Jarvis on 7/15/22.
//

#include <stdio.h>
#include <fcntl.h>
#include <assert.h>
#include <limits.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include "convert.h"

static char *
get_tmpfn(const char *s) {
    static char buffer[PATH_MAX + 1];
    const char suffix[] = ".XXXXXX";
    char *tmpfn;

    assert(strlen(s) > 0);

    if (strlen(s) + strlen(suffix) + 1 > sizeof(buffer)) {
        fprintf(stderr, "filename too long: \"%s\"\n", s);
        return NULL;
    }

    // Zero the buffer
    memset(buffer, 0, sizeof(buffer));

    // put the filename in place
    strncat(buffer, s, sizeof(buffer) - strlen(buffer) - 1);
    strncat(buffer, suffix, sizeof(buffer) - strlen(buffer) - 1);

    tmpfn = mktemp(buffer);

    return tmpfn;
}

int convert_file(const char *fn) {

    int in, out;  // input and output file descriptors
    char *tmpfn;
    char c;
    struct stat statBuf;


    // Open input file
    if ((in = open(fn, O_RDONLY | O_SHLOCK)) == -1) {
        fprintf(stderr, "Cannot read from %s: %s\n", fn, strerror(errno));
        return 1;
    }

    // Get temp file name
    if ((tmpfn = get_tmpfn(fn)) == NULL) {
        // Handle error
        close(in);
        return 1;
    }

    // get perms from original file
    if (fstat(in, &statBuf)) {
        fprintf(stderr, "could not stat input fd %d: %s\n", in, strerror(errno));
        close(in);
        return 1;
    }

    // open temp output file descriptor
    if ((out = open(tmpfn, (O_WRONLY | O_CREAT | O_TRUNC | O_EXCL | O_EXLOCK), (S_IRUSR|S_IWUSR)) == -1) {
        fprintf(stderr, "Cannot open temp file %s: %s\n", tmpfn, strerror(errno));
        close(in);
        return 1;
    }

    // Read from in, write to out, only if char is not a ^M
    while ((read(in, &c, sizeof(c))) == 1) {
        if (c != 0x0d)
            write(out, &c, sizeof(c));
    }

    if (!errno) {
        fprintf(stderr, "Read or write error: %s\n", strerror(errno));
        close(in);
        close(out);
        return 1;
    }

    // Close file descriptors
    close(in);
    close(out);

    // Rename temp file to original
    if (rename(tmpfn, fn)) {
        fprintf(stderr, "Could not rename %s => %s: %s\n", tmpfn, fn, strerror(errno));
        return 1;
    }

    // Restore old file permission
    if (chmod(fn, statBuf.st_mode)) {
        fprintf(stderr, "could not restore old file permissions %o to %s: %s\n", statBuf.st_mode, fn, strerror(errno));
        return 1;
    }

    return 0;
}
