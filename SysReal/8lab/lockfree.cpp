#include <iostream>  
#include <thread>  
#include <vector>  
#include <atomic>  
#include <memory>  
#include <random>  
#include <cmath>  
 
#include "Node.h"  
#include "Stack.h"  
 
#define min(a,b) (((a) < (b)) ? (a) : (b)) 
 
namespace { 
    struct Position { 
        double x; 
        double y; 
    }; 
 
    const double x_start = 0.0; 
    const double x_end = 4.0; 
    const double step = 0.001; 
    size_t readers_num = 4; 
    lf::LockFreeVersionedStack<Position> stack(readers_num); 
 
    void writter() { 
        double x_current = x_start; 
        for (size_t i = 0; i < 10000; i++) { 
            for (int i = 0; i < 1000; i++) { 
                double y = -(x_current * x_current) + 4 * x_current; 
                stack.push({ x_current, y }); 
                x_current += step; 
            } 
            for (int i = 0; i < 1000; i++) { 
                if (!stack.pop()) { 
                    throw std::runtime_error("can't delete element"); 
                } 
                x_current -= step; 
            } 
            for (int i = 0; i < 500; i++) { 
                double y = -(x_current * x_current) + 4 * x_current; 
                stack.push({ x_current, y }); 
                x_current += step; 
            } 
            for (int i = 0; i < 500; i++) { 
                if (!stack.pop()) { 
                    throw std::runtime_error("can't delete element"); 
                } 
                x_current -= step; 
            } 
        } 
        stack.stop(); 
        std::cout << "All versions" << stack.last_version() << std::endl; 
    } 
 
    class Reader { 
    public: 
        Reader(unsigned int id, lf::LockFreeVersionedStack<Position>* stack) : id_(id), stack_(stack) {} 
 
        void life() { 
            while (!stack_->is_stopped()) { 
                auto data = read(); 
                check(data); 
            } 
        } 
 
        std::vector<Position> read() { 
            lf::LockFreeVersionedStack<Position>::NodePtr data_ptr; 
            if (!stack_->subscribe(id_, data_ptr)) { 
                return {}; 
            } 
 
            std::vector<Position> result; 
            while (data_ptr != nullptr) { 
                result.push_back(data_ptr->data); 
                data_ptr = data_ptr->next; 
            } 
            versions_cnt++; 
            return result;

        } 
 
        void check(std::vector<Position>& data) { 
            double x_prev = x_start; 
            double step = 0.001; 
            double epsilon = 1e-6;
            for (const auto& pos : data) { 
                double expected_y = -(pos.x * pos.x) + 4 * pos.x; 
                if (std::abs(pos.y - expected_y) > epsilon) { 
                    throw std::logic_error("Inconsistent data: Point does not belong to the parabola"); 
                } 
 
                if (std::abs(pos.x - x_prev) > epsilon && std::abs(std::abs(pos.x - x_prev) - step) > epsilon) { 
                    throw std::logic_error("Inconsistent data: Non-linear sequence"); 
                } 
                x_prev = pos.x; 
            } 
        } 
        unsigned int versions_cnt = 0; 
     
    private: 
        int id_; 
        lf::LockFreeVersionedStack<Position>* stack_; 
    }; 
} 
 
int main() { 
    std::vector<Reader> readers; 
    std::vector<std::thread> threads; 
    for (int i = 0; i < readers_num; i++) { 
        readers.emplace_back(i, &stack); 
    } 
    for (int i = 0; i < readers_num; i++) { 
        threads.emplace_back(&Reader::life, &readers[i]); 
    } 
 
    writter(); 
 
    for (size_t i = 0; i < readers_num; i++) { 
        threads[i].join(); 
    } 
 
    for (size_t i = 0; i < readers_num; i++) { 
        std::cout << "readed" << i << " : " << readers[i].versions_cnt << std::endl; 
    } 
}