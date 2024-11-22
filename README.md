# Usage

```
#define SIMPLE_CALC_IMPLEMENTATION
#include "simple_calc.h"

int main(void)
{
	double six = sc_calculate("3 + 3", -1); // -1 for null terminated strings
	double one = sc_calculate("3 + -2/1", 8); // >0 for sized strings
	return 0;
}
```

# Testing

```
gcc -o test test.c -lm // or clang
./test
```
