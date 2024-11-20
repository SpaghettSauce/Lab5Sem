Программы живут не только линейно - до завершения алгоритма, но и отвечая на события: нажатие кнопок в интерфейсе; события ОС или сигналы устройств. В их жизненном цикле участвуют и события, и потоки.
```
#include <iostream>
#include <conio.h>
#include <stdio.h>

int main()
{
    clock_t start, end;
    start = clock();
    std::cout << "Press anykey to start\n";
    char x = _getch();
    std::cout << "start - " << x << std::endl;

    while (!_kbhit())  // буфер не пустой?
    {
        end = clock();
        if ((double)(end - start) / CLOCKS_PER_SEC >= 1) //прошла  ли секунда?
        {
            std::cout << "the program is still alive!\n";
            start = clock();
        }
    }
}
```

Каждую секунду программа выводит текст. Модифицируйте её так, чтобы по нажатию кнопки изменялся выводимый текст. А по нажатию определённой был выход из программы. О методе _kbhit  - чтения из буффера ввода:
https://docs.microsoft.com/ru-ru/cpp/c-runtime-library/reference/kbhit?view=vs-2017

О методе _getch: 
https://docs.microsoft.com/ru-ru/cpp/c-runtime-library/reference/getch-getwch?view=msvc-160

