#define SIMPLE_CALC_IMPLEMENTATION
#include "simple_calc.h"
#include "stdio.h"

static int success = 0;
static int total = 0;

#define EXPECTED(expr, expected)\
    do {\
        total += 1;\
        double eps = 1e-5;\
        double result = scalc_calculate(expr);\
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
    } while (0)

int main(void)
{
    EXPECTED(
        "3 + 3",
        6
    );
    EXPECTED(
        "3 - 3",
        0
    );
    EXPECTED(
        "3 * 3",
        9
    );
    EXPECTED(
        "3 / 3",
        1
    );
    EXPECTED(
        "3 ^ 3",
        27
    );
    EXPECTED(
        "5 - -3",
        8
    );
    EXPECTED(
        "5 + -(2 * 3)",
        -1
    );
    EXPECTED(
        "5 * -1",
        -5
    );
    EXPECTED(
        "5 * 2 ^ 3 / 4 + 1 * 5",
        15
    );
    EXPECTED(
        "5 * 2 ^ 3 ^ 4 + 1 +  79",
        20560
    );
    EXPECTED(
        "1+2*3",
        7
    );
    EXPECTED(
        "1+3*9",
        28
    );
    EXPECTED(
        "64 ^ 0 / 2 + 6",
        6.5f
    );
    EXPECTED(
        "4 ^ 2 / 8 + 1",
        3
    );
    EXPECTED(
        "1 / 0.5 + 6",
        8
    );
    EXPECTED(
        "3 + 5 / 3 ^ 4 * 9 - 2 * 1",
        1.55555555555556
    );
    EXPECTED(
        "3 + 3 + 3 + 3 + 3 ^ 0 + 3",
        16
    );
    EXPECTED(
        "1 / 0.3 - 0.1 * 3 ^ 2",
        2.43333333333333
    );
    EXPECTED(
        "0 / 2 * 1",
        0
    );
    EXPECTED(
        "0/1*2",
        0
    );
    EXPECTED(
        "2 ^ 3 + 2",
        10
    );
    EXPECTED(
        "2 ^ (3 + 2)",
        32
    );
    EXPECTED(
        "9 / 3 - 2",
        1
    );
    EXPECTED(
        "9 / (3 - 2)",
        9
    );
    EXPECTED(
        "9 + 2 ^ 2 + 9 * 3 - 1 - 1",
        9 + pow(2, 2) + 9 * 3 - 1 - 1
    );
    EXPECTED(
        "9 + 2 ^ (2 + 9 * 3 - 1 - 1)",
        9 + pow(2, 2 + 9 * 3 - 1 - 1)
    );
    EXPECTED(
        "9 + 2 ^ (2 + 9 * (3 - 1) - 1)",
        9 + pow(2, 2 + 9 * (3 - 1) - 1)
    );
    EXPECTED(
        "0 / (3 - 1) + 9 * (2 ^ (1+1) + 3 * (3 * (1 - 1 + (3 / 1))))",
        0.f / (3 - 1) + 9 * (pow(2, 1 + 1) + 3 * (3 * (1 - 1 + (3.f / 1))))
    );
    EXPECTED(
        "10 + 5",
        15
    );
    EXPECTED(
        "10 - 5",
        5
    );
    EXPECTED(
        "10 * 5",
        50
    );
    EXPECTED(
        "10 / 2",
        5
    );
    EXPECTED(
        "10 ^ 2",
        100
    );
    EXPECTED(
        "20 - -10",
        30
    );
    EXPECTED(
        "4 + -(3 * 2)",
        -2
    );
    EXPECTED(
        "7 * -2",
        -14
    );
    EXPECTED(
        "2 * 3 ^ 2 + 4",
        22
    );
    EXPECTED(
        "8 / 2 + 3 ^ 2",
        13
    );
    EXPECTED(
        "(2 + 3) * 4",
        20
    );
    EXPECTED(
        "5 + 2 * (3 + 4)",
        19
    );
    EXPECTED(
        "10 / (2 + 3)",
        2
    );
    EXPECTED(
        "3 ^ 3 - 2 * 4",
        19
    );
    EXPECTED(
        "9 / 3 + 7",
        10
    );
    EXPECTED(
        "(4 ^ 2) / (8 / 2)",
        4
    );
    EXPECTED(
        "(2 + 3) ^ 2",
        25
    );
    EXPECTED(
        "6 * (1 + 2)",
        18
    );
    EXPECTED(
        "10 + 5 * 0",
        10
    );
    EXPECTED(
        "0 ^ 5",
        0
    );
    EXPECTED(
        "5 ^ 0",
        1
    );
    EXPECTED(
        "(2 + 3) * (4 - 1)",
        15
    );
    EXPECTED(
        "5 * 2 ^ (1 + 1)",
        20
    );
    EXPECTED(
        "6 - 4 / 2",
        4
    );
    EXPECTED(
        "9 + 3 ^ 2",
        18
    );
    EXPECTED(
        "8 / 4 * 2",
        4
    );
    EXPECTED(
        "7 - 2 + 3",
        8
    );
    EXPECTED(
        "2 * (3 + 4) - 5",
        9
    );
    EXPECTED(
        "3 + 5 * 2 - 4",
        9
    );
    EXPECTED(
        "10 / (2 ^ 2)",
        2.5f
    );
    EXPECTED(
        "10 / 4 * 2",
        5
    );
    EXPECTED(
        "5 + 3 - 2 * 4",
        0
    );
    EXPECTED(
        "2 ^ 3 ^ 2",
        64
    );
    EXPECTED(
        "6 / 3 + 2 * 5",
        12
    );
    EXPECTED(
        "3 * 3 ^ 2",
        27
    );
    EXPECTED(
        "(3 + 2) * (4 - 1)",
        15
    );
    EXPECTED(
        "2 ^ (3 - 1) * 3",
        12
    );
    EXPECTED(
        "10 - 5 + 3",
        8
    );
    EXPECTED(
        "(5 + 3) * (2 - 1)",
        8
    );
    EXPECTED(
        "(8 - 4) * (2 + 3)",
        20
    );
    EXPECTED(
        "3 + 3 * 3 - 3",
        9
    );
    EXPECTED(
        "5 * 5 - 3 ^ 2",
        16
    );
    EXPECTED(
        "4 ^ 2 - 2 * 3",
        10
    );
    EXPECTED(
        "6 / (3 - 1)",
        3
    );
    EXPECTED(
        "10 / 2 + 3",
        8
    );
    EXPECTED(
        "9 / (3 + 3)",
        1.5f
    );
    EXPECTED(
        "25 / 5 * 2",
        10
    );
    EXPECTED(
        "10 + 2 * (3 + 4)",
        24
    );
    EXPECTED(
        "7 * (2 + 3) - 4",
        31
    );
    EXPECTED(
        "(4 ^ 2) / (2 + 2)",
        4
    );
    EXPECTED(
        "1 / 2",
        0.5f
    );
    EXPECTED(
        "3 / 4",
        0.75f
    );
    EXPECTED(
        "1 / 3 + 2 / 3",
        1.0f
    );
    EXPECTED(
        "2 / 5 + 3 / 10",
        0.7f
    );
    EXPECTED(
        "5 / 2 - 1 / 2",
        2.0f
    );
    EXPECTED(
        "1 / 4 + 1 / 4",
        0.5f
    );
    EXPECTED(
        "1 / 3 * 3",
        1.0f
    );
    EXPECTED(
        "2 / 5 * 5 / 2",
        1.0f
    );
    EXPECTED(
        "3 / 4 / 2",
        0.375f
    );
    EXPECTED(
        "1 / 2 + 3 / 4 - 1 / 4",
        1.0f
    );
    EXPECTED(
        "3 / 4 * 4 / 3",
        1.0f
    );
    EXPECTED(
        "2 / 3 ^ 2",
        2.0f / 9.0f
    );
    EXPECTED(
        "4 / 5 + 1 / 5",
        1.0f
    );
    EXPECTED(
        "5 / 6 - 1 / 3",
        0.5f
    );
    EXPECTED(
        "1 / 3 + 1 / 6",
        0.5f
    );
    EXPECTED(
        "3 / 8 + 5 / 8",
        1.0f
    );
    EXPECTED(
        "7 / 10 - 1 / 2",
        0.2f
    );
    EXPECTED(
        "9 / 10 * 2 / 3",
        0.6f
    );
    EXPECTED(
        "2 / 5 + 3 / 5",
        1.0f
    );
    EXPECTED(
        "(1 / 3) + (1 / 3) + (1 / 3)",
        1.0f
    );
    EXPECTED(
        "(1 / 2) * (3 / 4)",
        0.375f
    );
    EXPECTED(
        "(3 / 5) / (6 / 5)",
        0.5f
    );
    EXPECTED(
        "1 / 2 ^ 2",
        0.25f
    );
    EXPECTED(
        "9 / (3 ^ 2)",
        1.0f
    );
    EXPECTED(
        "1 / (2 + 3)",
        0.2f
    );
    EXPECTED(
        "(1 / 2) + (2 / 3) * (3 / 4)",
        1.0f
    );
    EXPECTED(
        "(4 / 5) * (3 / 4)",
        0.6f
    );
    EXPECTED(
        "1 / (5 / 2)",
        0.4f
    );
    EXPECTED(
        "1 / 2 + 1 / 4 + 1 / 4",
        1.0f
    );
    EXPECTED(
        "(1 / 2) - (1 / 4)",
        0.25f
    );
    EXPECTED(
        "(2 / 3) * (3 / 2)",
        1.0f
    );
    EXPECTED(
        "(5 / 6) + (1 / 6)",
        1.0f
    );
    EXPECTED(
        "1 / 3 * (3 / 4)",
        0.25f
    );
    EXPECTED(
        "3 / 4 + (1 / 4) * (2 / 2)",
        1.0f
    );
    EXPECTED(
        "1 / (1 / 2)",
        2.0f
    );
    EXPECTED(
        "(1 / 3) + (1 / 3)",
        2.0f / 3.0f
    );
    EXPECTED(
        "1 / 3 + (1 / 6)",
        0.5f
    );
    EXPECTED(
        "3 / 4 - 1 / 2",
        0.25f
    );
    EXPECTED(
        "4 / (2 * 2)",
        1.f
    );
    EXPECTED(
        "2 / 3 * 3 / 2 ^ 2",
        0.5f
    );
    EXPECTED(
        "4 / 5 ^ 2",
        0.16f
    );
    EXPECTED(
        "5 / (2 + 3)",
        1.0f
    );
    EXPECTED(
        "1 / (4 / 5)",
        1.25f
    );
    EXPECTED(
        "(3 / 4) * (4 / 3)",
        1.0f
    );
    EXPECTED(
        "(2 / 3) + (1 / 3)",
        1.0f
    );
    EXPECTED(
        "(2 / 5) * (5 / 2)",
        1.0f
    );
    EXPECTED(
        "(4 / 5) / (1 / 5)",
        4.0f
    );
    EXPECTED(
        "1 / (2 + 3)",
        0.2f
    );
    EXPECTED(
        "(1 / 4) * (2 / 3)",
        0.16666666666666666f
    );
    EXPECTED(
        "(5 / 6) / (2 / 3)",
        1.25f
    );
    EXPECTED(
        "(3 / 8) + (5 / 8)",
        1.0f
    );
    EXPECTED(
        "4 / (2 ^ 2)",
        1.0f
    );

    printf("%d out of %d tests succeeded\n", success, total);

    return 0;
}
