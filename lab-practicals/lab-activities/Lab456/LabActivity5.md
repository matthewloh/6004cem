# Reduction Clause

## Example 1

```cpp
sum = 0;
#pragma omp parallel for reduction(+:sum)
for (int i = 0; i < 10; i++) {
    sum += i;
}
<!-- printf("sum = %d\n", sum); -->
```

Pls modify the above code to print out the content of sum.

## Example 2 – Multiple Reduction Clauses

```cpp
sum = 0;
#pragma omp parallel for reduction(+:sa, sb) reduction(max:m)
for (int i = 0; i < 10; i++) {
    sa += A[i];
    sb += B[i];
    if (A[i] > m) {
        m = A[i];
    }
    m = B[i] > m ? B[i] : m; // same result as: if (B[i] > m) m = B[i];
}
```

Pls modify the above code to print out sa, sb and m

Pls differentiate between Example 3 and Example 4:

## Example 3

```cpp
#pragma omp parallel
{
  #pragma for shared(x, sum) private(i)
  for (i = 0; i < 10000; i++) {
    sum += x[i];
  }
}
```

## Example 4

```cpp
#pragma omp parallel
{
  #pragma for shared(x, sum) private(i) reduction(+:sum)
  for (i = 0; i < 10000; i++) {
    sum += x[i];
  }
}
```

## Example 5

Run the following code. Then, modify the code to identify which threads are doing which part of the loop.

```cpp
#pragma omp parallel
{
  #pragma for private(i) lastprivate(k)
  for (i = 0; i < 10; i++) {
    k = i*i;
  }
  printf("k = %d\n", k); // k == 81
}
```
