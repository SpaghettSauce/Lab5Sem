#include <thread>
#include <mutex>
#include <string>

void Func(string name)
{
   
}

int main()
{
    std::thread thread1(Func, "t1");
    std::thread thread2(Func, "t2");
    std::thread thread3(Func, "t3");
    thread1.join();
    thread2.join();
    thread3.join();
    system("pause");
}