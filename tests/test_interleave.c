#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include "get_next_line/get_next_line.h"

int main(void)
{
    int fa = open("tests/interleave_a.txt", O_RDONLY);
    int fb = open("tests/interleave_b.txt", O_RDONLY);
    if (fa < 0 || fb < 0)
    {
        perror("open interleave inputs");
        if (fa >= 0) close(fa);
        if (fb >= 0) close(fb);
        return 2;
    }

    /* Read alternately and check correctness */
    const char *expected_a[] = {"A1\n", "A2 longer\n", "A3\n"};
    const char *expected_b[] = {"B1\n", "B2\n"};

    char *la = get_next_line(fa);
    char *lb = get_next_line(fb);
    if (!la || !lb) { fprintf(stderr, "initial reads failed\n"); free(la); free(lb); close(fa); close(fb); return 1; }
    if (strcmp(la, expected_a[0]) != 0 || strcmp(lb, expected_b[0]) != 0)
    { fprintf(stderr, "initial mismatch\n expected A: '%s' got '%s'\n expected B: '%s' got '%s'\n", expected_a[0], la ? la : "(null)", expected_b[0], lb ? lb : "(null)"); free(la); free(lb); close(fa); close(fb); return 1; }
    free(la); free(lb);

    la = get_next_line(fa); /* A2 */
    lb = get_next_line(fb); /* B2 */
    if (!la || !lb) { fprintf(stderr, "second reads failed\n"); free(la); free(lb); close(fa); close(fb); return 1; }
    if (strcmp(la, expected_a[1]) != 0 || strcmp(lb, expected_b[1]) != 0)
    { fprintf(stderr, "second mismatch\n expected A(len=%zu): '%s' got(len=%zu): '%s'\n expected B(len=%zu): '%s' got(len=%zu): '%s'\n", strlen(expected_a[1]), expected_a[1], la ? strlen(la) : 0, la ? la : "(null)", strlen(expected_b[1]), expected_b[1], lb ? strlen(lb) : 0, lb ? lb : "(null)"); free(la); free(lb); close(fa); close(fb); return 1; }
    free(la); free(lb);

    la = get_next_line(fa); /* A3 */
    if (!la || strcmp(la, expected_a[2]) != 0)
    { fprintf(stderr, "third read mismatch for A\n"); free(la); close(fa); close(fb); return 1; }
    free(la);

    /* Now both should be at EOF */
    if (get_next_line(fa) != NULL || get_next_line(fb) != NULL)
    { fprintf(stderr, "expected EOF on both fds\n"); close(fa); close(fb); return 1; }

    close(fa); close(fb);
    printf("OK: interleave test passed\n");
    return 0;
}
