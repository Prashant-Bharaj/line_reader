#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include "get_next_line/get_next_line.h"

int main(void)
{
    const char *expected[] = {"Line 1\n", "Line 2\n", "\n", "Last line"};
    const int count = sizeof(expected) / sizeof(expected[0]);
    int fd = open("tests/input.txt", O_RDONLY);
    if (fd < 0)
    {
        perror("open tests/input.txt");
        return 2;
    }

    for (int i = 0; i < count; ++i)
    {
        char *line = get_next_line(fd);
        if (!line)
        {
            fprintf(stderr, "Test failed: unexpected NULL at index %d\n", i);
            close(fd);
            return 1;
        }
        if (strcmp(line, expected[i]) != 0)
        {
            fprintf(stderr, "Test failed at index %d:\n expected: \"%s\"\n got: \"%s\"\n", i, expected[i], line);
            free(line);
            close(fd);
            return 1;
        }
        free(line);
    }

    /* After reading expected lines, next call should return NULL (EOF) */
    char *line = get_next_line(fd);
    if (line != NULL)
    {
        fprintf(stderr, "Test failed: expected NULL at EOF but got: \"%s\"\n", line);
        free(line);
        close(fd);
        return 1;
    }

    close(fd);
    printf("OK: get_next_line passed %d checks\n", count);
    return 0;
}
