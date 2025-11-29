#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "get_next_line/get_next_line.h"

/* Create a temporary file with many random-length lines, then read them with get_next_line */
int main(void)
{
    char tmpname[] = "tests/stressXXXXXX";
    int fd = mkstemp(tmpname);
    if (fd < 0)
    {
        perror("mkstemp");
        return 2;
    }
    /* We'll write deterministic "random" data */
    unsigned int seed = 42;
    const int LINES = 2000;
    const int MAXLEN = 512;
    int expected_lines = 0;
    for (int i = 0; i < LINES; ++i)
    {
        seed = seed * 1664525u + 1013904223u;
        int len = (seed % MAXLEN) + 1; /* 1..MAXLEN */
        for (int j = 0; j < len; ++j)
            write(fd, (j % 2) ? "x" : "y", 1);
        /* end with newline for all lines except some */
        if ((i % 10) != 0)
        {
            write(fd, "\n", 1);
            ++expected_lines;
        }
    }
    close(fd);

    /* Reopen for reading */
    int rfd = open(tmpname, O_RDONLY);
    if (rfd < 0)
    {
        perror("open tmp");
        unlink(tmpname);
        return 3;
    }

    int count = 0;
    char *line;
    while ((line = get_next_line(rfd)) != NULL)
    {
        ++count;
        free(line);
    }

    close(rfd);
    unlink(tmpname);

    if (count != expected_lines)
    {
        fprintf(stderr, "stress test failed: expected %d lines (newlines written), got %d\n", expected_lines, count);
        return 1;
    }
    printf("OK: stress test passed (%d lines)\n", count);
    return 0;
}
