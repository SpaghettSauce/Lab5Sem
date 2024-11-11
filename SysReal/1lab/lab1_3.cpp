#include <iostream>
#include <thread>
#include <mutex>
#include <time.h>
#include <string>
using namespace std;

void Func(string name) {
	int value = 1;
	for (int i = 0; i < 10000000; i++) {
		for (int j = 0; j < 10; j++) {
			value *= j;
		}
	}
}

int main() {
	clock_t start_1 = clock();

	thread thread_1(Func, "t1");
	thread thread_2(Func, "t2");
	thread_1.join();
	thread_2.join();

	clock_t end_1 = clock();
	double time1 = (double)(end_1 - start_1) / CLOCKS_PER_SEC;

	cout << "Время параллельно: " << time1 <<"\n";

	clock_t start_2 = clock();

	Func("first");
	Func("second");

	clock_t end_2 = clock();

	double time2 = (double)(end_2 - start_2) / CLOCKS_PER_SEC;
	cout << "Время последовательно: " << time2 << "\n";

}
