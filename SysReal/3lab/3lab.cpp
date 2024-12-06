#include <iostream>
#include <conio.h>
#include <ctime>
#include <string>

int main()
{
    clock_t start, end;
    start = clock();
    std::string message = "Живём";

    while (true)
    {
        end = clock();
        if ((double)(end - start) / CLOCKS_PER_SEC >= 1) 
        {
            std::cout << message << std::endl;
            start = clock(); 
        }

        if (_kbhit()) 
        {
            char input = _getch(); 

            if (input == 'q') 
            {
                std::cout << "Exiting program...\n";
                break;
            }
            else
            {
                message =  input; 
                std::cout << "Key: " << message << std::endl;
            }
        }
    }

    return 0;
}
