#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <chrono>

const int dish1 = 3000;
const int GLUTTONY = 1;
const int EFFICIENCY_FACTOR = 1;
const int MAX_NUGGETS_PER_EATE = 10000;

class NuggetKitchen {
private:
    std::mutex kitchen_mutex;
    std::mutex readiness_mutex;

    std::condition_variable kitchen_cv;

    std::atomic<int> plate1{ dish1 };
    std::atomic<int> plate2{ dish1 };
    std::atomic<int> plate3{ dish1 };
    std::atomic<int> plate1_consumed{ 0 };
    std::atomic<int> plate2_consumed{ 0 };
    std::atomic<int> plate3_consumed{ 0 };

    std::atomic<bool> chef_active{ true };
    std::atomic<bool> chef_ready{ false };

    std::atomic<int> active_fattys{ 3 };

    std::atomic<bool> fatty1_ready{ false };
    std::atomic<bool> fatty2_ready{ false };
    std::atomic<bool> fatty3_ready{ false };

public:
    void chefAction(std::chrono::time_point<std::chrono::high_resolution_clock> start_time) {
        while (chef_active) {
            {
                std::unique_lock<std::mutex> lock(kitchen_mutex);
                kitchen_cv.wait(lock, [this] { return chef_ready || !chef_active; });
                if (!chef_active) return;
                plate1 += EFFICIENCY_FACTOR;
                plate2 += EFFICIENCY_FACTOR;
                plate3 += EFFICIENCY_FACTOR;
                std::cout << "Chef prepared " << EFFICIENCY_FACTOR << " nuggets per plate.\n";
                std::lock_guard<std::mutex> flag_lock(readiness_mutex);
                fatty1_ready = false;
                fatty2_ready = false;
                fatty3_ready = false;
                chef_ready = false;

            }

            kitchen_cv.notify_all();

            if (active_fattys == 0) {
                std::cout << "All fattys are out. Chef didn’t get paid.\n";
                chef_active = false;
                return;
            }

            auto current_time = std::chrono::high_resolution_clock::now();
            auto elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time).count();
            if (elapsed_seconds > 5) {
                std::cout << "Chef quit the job!\n";
                chef_active = false;
                kitchen_cv.notify_all();
                return;
            }
        }
    }

    void fattyAction(int fatty_id) {
        while (chef_active) {
            int consumption = GLUTTONY;
            {
                std::unique_lock<std::mutex> lock(kitchen_mutex);
                kitchen_cv.wait(lock, [this] { return plate1.load() > 0 || plate2.load() > 0 || plate3.load() > 0 || !chef_active; });
                if (!chef_active) return;
                if (fatty_id == 1 && fatty1_ready == 0) {
                    consumption = std::min(GLUTTONY, plate1.load());
                    plate1.fetch_sub(consumption);
                    plate1_consumed.fetch_add(consumption);
                    fatty1_ready = true;
                    std::cout << "Jim ate " << consumption << " nuggets.\n";
                }
                else if (fatty_id == 2 && fatty2_ready == false) {
                    consumption = std::min(GLUTTONY, plate2.load());
                    plate2.fetch_sub(consumption);
                    plate2_consumed.fetch_add(consumption);
                    fatty2_ready = true;
                    std::cout << "Bob ate " << consumption << " nuggets.\n";
                    
                }
                else if (fatty_id == 3 && fatty3_ready == false) {
                    consumption = std::min(GLUTTONY, plate3.load());
                    plate3.fetch_sub(consumption);
                    plate3_consumed.fetch_add(consumption);
                    fatty3_ready = true;
                    std::cout << "Maxim ate " << consumption << " nuggets.\n";

                }

                std::lock_guard<std::mutex> flag_lock(readiness_mutex);
                if (fatty1_ready && fatty2_ready && fatty3_ready) {
                chef_ready = true;
                kitchen_cv.notify_all();
                }

                if (plate1 == 0 || plate2 == 0 || plate3 == 0) {
                    if (fatty_id == 1) {
                        std::cout << "Jim found empty plates. Chef is fired!\n";
                    }
                    if (fatty_id == 2) {
                        std::cout << "Bob found empty plates. Chef is fired!\n";
                    }
                    if (fatty_id == 3) {
                        std::cout << "Maxim found empty plates. Chef is fired!\n";
                    }
                    chef_active = false;
                    return;
                }

                if (plate1_consumed.load() > MAX_NUGGETS_PER_EATE) {
                    std::cout << "Jim overate and is out!\n";
                    active_fattys.fetch_sub(1);
                    plate1_consumed.store(0);
                    return;
                }
                else if (plate2_consumed.load() > MAX_NUGGETS_PER_EATE) {
                    std::cout << "Bob overate and is out!\n";
                    active_fattys.fetch_sub(1);
                    plate2_consumed.store(0);
                    return;
                }
                else if (plate3_consumed.load() > MAX_NUGGETS_PER_EATE) {
                    std::cout << "Maxim overate and is out!\n";
                    active_fattys.fetch_sub(1);
                    plate3_consumed.store(0);
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