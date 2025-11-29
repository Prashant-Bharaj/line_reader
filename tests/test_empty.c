#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "get_next_line/get_next_line.h"

int main(void)
{
    int fd = open("tests/empty.txt", O_RDONLY);
    if (fd < 0)
    {
        perror("open tests/empty.txt");
        return 2;
    }
    char *line = get_next_line(fd);
    if (line != NULL)
    {
        fprintf(stderr, "empty file test failed: expected NULL, got '%s'\n", line);
        free(line);
        close(fd);
        return 1;
    }
    close(fd);
    printf("OK: empty file test passed\n");
    return 0;
}
