#define SIMPLE_CALC_IMPLEMENTATION
#include "simple_calc.h"
#include "stdio.h"
#include <stdlib.h>

#define EXPECTED(expr, expected)\
    do {\
        double eps = 1e-5;\
        double result = sc_calculate(expr);\
        bool equal;\
        double diff = expected - result;\
        if (diff < 0) diff *= -1;\
        if (diff <= eps) {\
            success += 1;\
            equal = true;\
        }\
        else {\
            equal = false;\
        }\
        printf("{\n");\
        printf("  input  : %s\n", expr);\
        printf("  answer : %lf\n", (double)expected);\
        printf("  output : %lf\n", result);\
        printf("  status : %s\n", equal ? "SUCCESS" : "FAILURE");\
        printf("}\n");\
        if (!equal) return 0;\
    } while (0)

typedef struct {
    char *input;
    double answer;
} TestCase;

bool get_test_cases(TestCase* *output, size_t *count)
{
    FILE *f = fopen("test_cases.txt", "rb");

    if (f == NULL) return false;

    bool failed = false;
    const size_t max_len = 1024;
    size_t digits = 0;
    char temp[max_len];
    char c;
    char *endptr;

    #define FAILED() \
        do {\
            failed = true;\
            goto CLEANUP;\
        } while (0)

    *output = NULL;

    c = fgetc(f);

    while (c != '\0' && c != '\n') {
        if (digits >= max_len) FAILED();
        temp[digits++] = c;
        c = fgetc(f);
    }
    if (digits >= max_len) FAILED();

    temp[digits] = '\0';

    if (c == '\0' || c != '\n' || digits <= 0) FAILED();

    *count = strtoull(temp, &endptr, 0);

    if (*count <= 0 || *count > 10000) {
        *count = 0;
        FAILED();
    }

    *output = calloc(*count, sizeof(TestCase));

    if (*output == NULL) FAILED();

    for (size_t i = 0; i < *count; i++) {
        c = fgetc(f);
        if (c != '"') FAILED();
        size_t len = 0;
        c = fgetc(f);
        while (c != '\0' && c != '"') {
            if (len >= max_len) FAILED();
            temp[len++] = c;
            c = fgetc(f);
        }
        if (c != '"') FAILED();

        (*output)[i].input = malloc(sizeof(char) * (len + 1));

        if ((*output)[i].input == NULL) FAILED();

        memcpy((*output)[i].input, temp, sizeof(char) * len);
        (*output)[i].input[len] = '\0';

        len = 0;
        c = fgetc(f);

        while (c != '\0' && c != '\n') {
            if (len >= max_len) FAILED();
            temp[len++] = c;
            c = fgetc(f);
        }

        if (len >= max_len || len <= 0) FAILED();

        temp[len] = '\0';

        (*output)[i].answer = strtod(temp, &endptr);

        if (i < (*count) - 1 && c != '\n') FAILED();
    }

CLEANUP:
    #undef FAILED
    fclose(f);

    if (failed) {
        if (*output != NULL) {
            for (size_t i = 0; i < *count; i++) {
                free((*output)[i].input);
            }
        }
        free(*output);
        *output = NULL;
        *count = 0;

        return false;
    }

    return true;
}

int main(void)
{
    size_t success = 0;

    TestCase *cases;
    size_t total;

    if (!get_test_cases(&cases, &total)) {
        fprintf(stderr, "Failed to get test cases\n");
        return 1;
    }

    for (size_t i = 0; i < total; i++) {
        EXPECTED(cases[i].input, cases[i].answer);
    }

    printf("%zu out of %zu tests succeeded\n", success, total);

    return 0;
}
