Reduction Clause
Example A

```cpp
#include <omp.h>
#include <stdio.h>

int main(void) {
  int sum = 100;
#pragma omp parallel for
  for (int i = 1; i <= 4; i++) {
    sum += i;
  }

  printf("sum is %d\n", sum);
  return 0;
}
```

Example B

```cpp
#include <omp.h>
#include <stdio.h>

int main(void) {
  int sum = 100;
#pragma omp parallel for
  for (int i = 1; i <= 4; i++) {
#pragma omp critical
    sum += i;
  }

  printf("sum is %d\n", sum);
  return 0;
}
```

Example C

```cpp
#include <omp.h>
#include <stdio.h>

int main(void) {
  int sum = 100;
  printf("Before parallelism, sum's adress is %p\n", &sum);
#pragma omp parallel for reduction(+ : sum)
  for (int i = 1; i <= 4; i++) {
    printf("sum's address in thread %d is %p, value is %d\n",
           omp_get_thread_num(), &sum, sum);
    sum += i;
  }

  printf("After parallelism, sum's address is %p, and value is %d\n", &sum,
         sum);
  return 0;
}
```

Answer the following questions:

1. Run each sample and screenshot the output
Example A
╰─ g++ -fopenmp Lab6/a.cpp -o Lab6/a && Lab6/a                                                        ─╯
sum is 110
Example B
╰─ g++ -fopenmp Lab6/b.cpp -o Lab6/b && Lab6/b                                                        ─╯
sum is 110
Example C
╰─ g++ -fopenmp Lab6/c.cpp -o Lab6/c && Lab6/c                                                        ─╯
Before parallelism, sum's adress is 0x7ffecdf1ba00
sum's address in thread 3 is 0x79a01ebfcdd0, value is 0
sum's address in thread 2 is 0x79a01fbfddd0, value is 0
sum's address in thread 0 is 0x7ffecdf1b9a0, value is 0
sum's address in thread 1 is 0x79a020bfedd0, value is 0
After parallelism, sum's address is 0x7ffecdf1ba00, and value is 110

2. In your opinion, which option is better in terms of:
   a) Data race condition
   b) Performance
