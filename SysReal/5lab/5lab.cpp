#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <random>

int coins = 101;
int rest = coins;
int Bob_coins = 0;
int Tom_coins = 0;



std::mutex m;
std::atomic<bool> Turn; //1-Том, 0-Боб

void coin_sharing(std::string name, int& thief_coins, int& companion_coins)
{
	int even= rest % 2 == 0 ? 0 : 1; //если кол-во монет нечетное тоeven= 1 
	while (true)
	{
		m.lock();
		if ((name == "Том" && Turn) || (name == "Боб" && !Turn))
		{
			if (coins > even && thief_coins <= companion_coins)
			{
				--coins;
				++thief_coins;
				std::cout << "Монеты :" << coins << "\n";
				std::cout << name << " :" << thief_coins << " " << companion_coins <<"\n";

				Turn = !Turn;
			}
			else if (coins == even)
			{
				Turn = !Turn;
				m.unlock();
				break;
			}
		}
		m.unlock();
	}
}


int main()
{
	std::cout<<"Всего : "<< rest<<"\n";
	srand(static_cast<unsigned>(time(0)));
	Turn = rand() % 2 == 0;

	std::thread TomThread(coin_sharing, "Том", std::ref(Tom_coins), std::ref(Bob_coins));
	std::thread BobThread(coin_sharing, "Боб", std::ref(Bob_coins), std::ref(Tom_coins));

	TomThread.join();
	BobThread.join();

	std::cout << std::endl << "У Боба: " << Bob_coins << std::endl;
	std::cout << "у Тома: " << Tom_coins << std::endl;
	std::cout << "Осталось: " << (rest - Bob_coins - Tom_coins);
} 