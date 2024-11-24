#include <iostream>
#include <conio.h>
#include <ctime>
#include <string>

int main()
{
    clock_t start, end;
    start = clock();
    std::cout << "Press any key to start\n";
    char x = _getch(); 
    std::cout << "Start - " << x << std::endl;

    std::string message = "Живем";
    
    while (_kbhit) 
    {
            char key = _getch(); 
            if (key == 'j') 
            {
                std::cout << "Exiting \n";
                break;
            }
            else
            {
                message = "KEY: ";
                message += key; 
            }
        //}

        end = clock();
        if ((double)(end - start) / CLOCKS_PER_SEC >= 1) // Проверка ли секунда ?
        {
            std::cout << message << std::endl;
            start = clock(); 
        }
    }

    return 0;
}
