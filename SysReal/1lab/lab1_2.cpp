#include <iostream>
#include <time.h>
using namespace std;

int factorial(int n) {
    int faktr = 1;
    for (int i = 1; i <= n; ++i) {
        faktr *= i;
    }
    return faktr;
}

int main() {
    clock_t start = clock();
    for (int i = 0; i < 10000000; i++) {
        int result = factorial(10);
    }
    clock_t end = clock();
    double seconds = (double)(end - start) / CLOCKS_PER_SEC;
    cout << seconds << endl;
}
