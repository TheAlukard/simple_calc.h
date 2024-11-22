# Usage

```
#define SIMPLE_CALC_IMPLEMENTATION
#include "simple_calc.h"

int main(void)
{
	double six = sc_calculate("3 + 3");
	double one = sc_calculate("3 + -2/1");
	return 0;
}
```

# Testing

```
gcc -o test test.c -lm // or clang
./test
```
