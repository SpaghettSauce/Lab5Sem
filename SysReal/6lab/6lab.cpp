#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

const int dish1 = 3000;
const int GLUTTONY = 100;
const int EFFICIENCY_FACTOR = 1;
const int MAX_NUGGETS_PER_EATER = 10000;

class NuggetKitchen {
private:
    std::mutex kitchen_mutex;
    std::mutex readiness_mutex;

    std::condition_variable kitchen;

    int plate1{ dish1 };
    int plate2{ dish1 };
    int plate3{ dish1 };
    int plate1_consumed{ 0 };
    int plate2_consumed{ 0 };
    int plate3_consumed{ 0 };

    bool chef_active{ true };
    bool chef_ready{ false };

    int active_fattys{ 3 };

    bool fatty1_ready{ false };
    bool fatty2_ready{ false };
    bool fatty3_ready{ false };

public:
    void chefAction(std::chrono::time_point<std::chrono::high_resolution_clock> start_time) {
        while (chef_active) {
            {
                std::unique_lock<std::mutex> lock(kitchen_mutex);
                kitchen.wait(lock, [this] { return chef_ready || !chef_active; });
                if (!chef_active) return;

                plate1 += EFFICIENCY_FACTOR;
                plate2 += EFFICIENCY_FACTOR;
                plate3 += EFFICIENCY_FACTOR;
                std::cout << "Кук сделал нагетов " << EFFICIENCY_FACTOR << "\n";

                {
                    std::lock_guard<std::mutex> flag_lock(readiness_mutex);
                    fatty1_ready = false;
                    fatty2_ready = false;
                    fatty3_ready = false;
                    chef_ready = false;
                }
            }

            kitchen.notify_all();

            if (active_fattys == 0) {
                std::cout << "Fatty's взорвались, L кук\n";
                chef_active = false;
                return;
            }

            auto current_time = std::chrono::high_resolution_clock::now();
            auto elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time).count();
            if (elapsed_seconds > 5) {
                std::cout << "Прошло 5 дней\n";
                chef_active = false;
                kitchen.notify_all();
                return;
            }
        }
    }

    void fattyAction(int fatty_id) {
        while (chef_active) {
            int consumption = GLUTTONY;
            {
                std::unique_lock<std::mutex> lock(kitchen_mutex);
                kitchen.wait(lock, [this] { return (plate1 > 0 || plate2 > 0 || plate3 > 0) || !chef_active; });
                if (!chef_active) return;

                if (fatty_id == 1 && !fatty1_ready) {
                    consumption = std::min(GLUTTONY, plate1);
                    plate1 -= consumption;
                    plate1_consumed += consumption;
                    fatty1_ready = true;
                    std::cout << "1 fatty съел наггетов: " << consumption << "\n";
                } else if (fatty_id == 2 && !fatty2_ready) {
                    consumption = std::min(GLUTTONY, plate2);
                    plate2 -= consumption;
                    plate2_consumed += consumption;
                    fatty2_ready = true;
                    std::cout << "2 fatty съел наггетов: " << consumption << "\n";
                } else if (fatty_id == 3 && !fatty3_ready) {
                    consumption = std::min(GLUTTONY, plate3);
                    plate3 -= consumption;
                    plate3_consumed += consumption;
                    fatty3_ready = true;
                    std::cout << "3 fatty съел наггетов: " << consumption << "\n";
                }

                {
                    std::lock_guard<std::mutex> flag_lock(readiness_mutex);
                    if (fatty1_ready && fatty2_ready && fatty3_ready) {
                        chef_ready = true;
                        kitchen.notify_all();
                    }
                }

                if (plate1 <= 0 || plate2 <= 0 || plate3 <= 0) {
                    std::cout<<"Пустая тарелка \n";
                    chef_active = false;
                    return;
                }

                if (plate1_consumed > MAX_NUGGETS_PER_EATER) {
                    std::cout << "1 лопнул \n";
                    active_fattys--;
                    plate1_consumed = 0;
                    return;
                } else if (plate2_consumed > MAX_NUGGETS_PER_EATER) {
                    std::cout << "2 лопунл\n";
                    active_fattys--;
                    plate2_consumed = 0;
                    return;
                } else if (plate3_consumed > MAX_NUGGETS_PER_EATER) {
                    std::cout << "3 лопнул\n";
                    active_fattys--;
                    plate3_consumed = 0;
                    return;
                }
                std::this_thread::yield();
            }
        }
    }
};

int main() {
    NuggetKitchen kitchen;
    auto start_time = std::chrono::high_resolution_clock::now();
    std::thread chef_thread(&NuggetKitchen::chefAction, &kitchen, start_time);

    std::thread fatty1_thread(&NuggetKitchen::fattyAction, &kitchen, 1);
    std::thread fatty2_thread(&NuggetKitchen::fattyAction, &kitchen, 2);
    std::thread fatty3_thread(&NuggetKitchen::fattyAction, &kitchen, 3);

    chef_thread.join();
    fatty1_thread.join();
    fatty2_thread.join();
    fatty3_thread.join();
}
