#pragma once 
 
#include <iostream> 
#include <vector> 
#include <atomic> 
#include <memory> 
#include <limits> 
 
#include "Node.h" 
 
#define min(a,b) (((a) < (b)) ? (a) : (b)) 
 
namespace lf { 
 
    template<typename Position> 
    class LockFreeVersionedStack { 
    public: 
        typedef Node<Position>* NodePtr; 
        typedef std::atomic<Node<Position>*> AtomicNodePtr; 
        typedef std::atomic_uint AtomicVersion; 
 
        LockFreeVersionedStack(size_t readers_num) : subs_num_(readers_num) { 
            stop_flag_.store(false); 
            subscribers_ = new AtomicVersion[readers_num]; 
            for (int i = 0; i < readers_num; i++) { 
                subscribers_[i].store(0); 
            } 
        } 
 
        ~LockFreeVersionedStack() { 
            while (pop()); 
            for (auto& node : trash_) { 
                delete node; 
            } 
            delete[] subscribers_; 
        } 
 
        bool subscribe(const unsigned int& id, NodePtr& stack_ptr) { 
            auto& sub = subscribers_[id]; 
            while (sub.load(std::memory_order_relaxed) == stack_.version.load(std::memory_order_relaxed)) { 
                if (stop_flag_) { 
                    return false; 
                } 
            } 
            auto readed_version = sub.load(); 
            while (!sub.compare_exchange_strong(readed_version, stack_.version)) { 
                if (stop_flag_) { 
                    return false; 
                } 
            } 
            stack_ptr = stack_.head.load(); 
            return true; 
        } 
 
        void unsubscribe(const unsigned int& id) { 
            subscribers_[id].store(0); 
        } 
 
        void push(Position value) { 
            NodePtr new_node = new Node<Position>; 
            new_node->data = std::move(value); 
            new_node->next = stack_.head.load(); 
            new_node->version = stack_.version.load() + 1; 
 
            stack_.head.store(new_node); 
            ++(stack_.version); 
        } 
 
        bool pop() { 
            NodePtr old_node = stack_.head.load(); 
            if (old_node == nullptr) { 
                return false; 
            } 
 
            NodePtr new_first_node = old_node->next; 
            if (new_first_node != nullptr) { 
                new_first_node->version = stack_.version.load() + 1; 
            } 
            stack_.head.store(new_first_node); 
            ++(stack_.version); 
 
            update_trash(old_node); 
            return true; 
        } 
 
        void stop() { 
            stop_flag_.store(true); 
        } 
 
        bool is_stopped() { 
            return stop_flag_.load(); 
        } 
 
        unsigned int last_version() { 
            return stack_.version.load(); 
        } 
 
    private: 
        void update_trash(NodePtr old_node) { 
            trash_.push_back(old_node); 
 
            uint64_t min_version = (std::numeric_limits<uint64_t>::max)(); 
            for (size_t i = 0; i < subs_num_; ++i) { 
                auto version = subscribers_[i].load(); 
                if (version == 0) { 
                    continue; 
                } 
                min_version = min(min_version, version); 
            } 
            for (size_t i = 0; i < trash_.size();) { 
                if (trash_[i]->version < min_version) { 
                    delete trash_[i]; 
                    trash_.erase(trash_.begin() + i); 
                } 
                else { 
                    ++i; 
                } 
            } 
        } 
 
        struct VersionedHead { 
            AtomicNodePtr head; 
            AtomicVersion version; 
        }; 
 
        VersionedHead stack_; 
        AtomicVersion* subscribers_; 
        size_t subs_num_; 
        std::vector<NodePtr> trash_; 
        std::atomic_bool stop_flag_; 
    }; 
}