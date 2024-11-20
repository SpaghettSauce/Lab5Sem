# Лабораторная работа №1

## Пункт 1.1 Разминка
string s = "01234"
for(unsigned int i = s.size() - 1; i >= 0; i-- )
{
   std::cout<<s[i]<<std::endl;
}
Что выведет программа и почему?
## Пункт 1.2 Упражнение 1
Вычислить факториал 10! - 10 млн. раз. Замерить время выполнения программы в режиме debug и release версии. Объяснить результаты.

В Visual Studio создать консольное приложение С++. Для измерения времени использовать:

#include <time.h>
…
clock_t start = clock();
…
clock_t end = clock();
double seconds = (double)(end - start) / CLOCKS_PER_SEC;
https://docs.microsoft.com/ru-ru/cpp/c-runtime-library/reference/clock?view=vs-2019

## Пункт 1.3 Упражнение 2
Создать 2 потока следующим способом:
#include <thread>
#include <mutex>
#include <string>

void Func(string name)
{
   ...
}

int main()
{
   std::thread thread1(Func, "t1");
   std::thread thread2(Func, "t2");
   thread1.join();
   thread2.join();
   system("pause");
}
где Func функция, вычисления факториала.

Замерьте время выполнения всей программы.

Запустите вызов функции Func два раза последовательно. Замерьте время и сравните с запуском в параллельных потоках.
 
