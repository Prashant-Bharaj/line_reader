#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include "get_next_line/get_next_line.h"

static int test_invalid_fd(void)
{
    char *line = get_next_line(-1);
    if (line != NULL)
    {
        fprintf(stderr, "invalid fd test failed: expected NULL, got '%s'\n", line);
        free(line);
        return 1;
    }
    return 0;
}

static int test_multiple_fds(void)
{
    int fd1 = open("tests/input.txt", O_RDONLY);
    int fd2 = open("tests/input2.txt", O_RDONLY);
    if (fd1 < 0 || fd2 < 0)
    {
        perror("open inputs");
        if (fd1 >= 0) close(fd1);
        if (fd2 >= 0) close(fd2);
        return 1;
    }

    fprintf(stderr, "debug: fd1=%d fd2=%d\n", fd1, fd2);
    char path[64], buf[512];
    snprintf(path, sizeof(path), "/proc/self/fd/%d", fd1);
    ssize_t r = readlink(path, buf, sizeof(buf)-1);
    if (r > 0) { buf[r] = '\0'; fprintf(stderr, "fd1 -> %s\n", buf); }
    snprintf(path, sizeof(path), "/proc/self/fd/%d", fd2);
    r = readlink(path, buf, sizeof(buf)-1);
    if (r > 0) { buf[r] = '\0'; fprintf(stderr, "fd2 -> %s\n", buf); }
    char *l1 = get_next_line(fd1);
    fprintf(stderr, "debug: after read fd1 -> '%s'\n", l1 ? l1 : "(null)");
    char *l2 = get_next_line(fd2);
    fprintf(stderr, "debug: after read fd2 -> '%s'\n", l2 ? l2 : "(null)");
    if (!l1 || !l2)
    {
        fprintf(stderr, "multiple fds: unexpected NULL\n");
        free(l1); free(l2);
        close(fd1); close(fd2);
        return 1;
    }

    if (strcmp(l1, "Line 1\n") != 0 || strcmp(l2, "Other 1\n") != 0)
    {
        fprintf(stderr, "multiple fds: mismatch\n expected fd1: 'Line 1\\n', fd2: 'Other 1\\n'\n got fd1: '%s', fd2: '%s'\n", l1 ? l1 : "(null)", l2 ? l2 : "(null)");
        free(l1); free(l2);
        close(fd1); close(fd2);
        return 1;
    }

    free(l1); free(l2);

    /* Alternate reads to ensure state kept per-fd */
    l1 = get_next_line(fd1); /* should be "Line 2\n" */
    l2 = get_next_line(fd2); /* should be "Other 2" (no newline) */
    if (!l1 || !l2)
    {
        fprintf(stderr, "multiple fds (2): unexpected NULL\n");
        free(l1); free(l2);
        close(fd1); close(fd2);
        return 1;
    }
    if (strcmp(l1, "Line 2\n") != 0 || strcmp(l2, "Other 2") != 0)
    {
        fprintf(stderr, "multiple fds (2): mismatch\n got fd1: '%s', fd2: '%s'\n", l1, l2);
        free(l1); free(l2);
        close(fd1); close(fd2);
        return 1;
    }

    free(l1); free(l2);

    /* Drain remaining data for these fds so their internal buffers are freed
       before the fd numbers may be reused later in the tests. */
    char *tmp;
    while ((tmp = get_next_line(fd1)) != NULL)
        free(tmp);
    while ((tmp = get_next_line(fd2)) != NULL)
        free(tmp);

    close(fd1); close(fd2);
    return 0;
}

static int test_long_line(void)
{
    int fd = open("tests/long_input.txt", O_RDONLY);
    if (fd < 0)
    {
        perror("open long_input.txt");
        return 1;
    }
    char *line = get_next_line(fd);
    if (!line)
    {
        fprintf(stderr, "long line: got NULL\n");
        close(fd);
        return 1;
    }
    size_t len = strlen(line);
    fprintf(stderr, "debug long: len=%zu firstbytes=", len);
    for (size_t i = 0; i < (len < 16 ? len : 16); ++i)
        fprintf(stderr, "%02x", (unsigned char)line[i]);
    fprintf(stderr, "\n");
    /* Use fstat to determine expected file size (the line should equal file size) */
    struct stat st;
    if (fstat(fd, &st) == -1)
    {
        perror("fstat");
        free(line);
        close(fd);
        return 1;
    }
    if (len != (size_t)st.st_size)
    {
        fprintf(stderr, "long line: length mismatch: expected %lld got %zu\n", (long long)st.st_size, len);
        free(line);
        close(fd);
        return 1;
    }
    free(line);
    close(fd);
    return 0;
}

int main(void)
{
    if (test_invalid_fd())
        return 1;
    if (test_multiple_fds())
        return 2;
    if (test_long_line())
        return 3;
    printf("OK: edge tests passed\n");
    return 0;
}
