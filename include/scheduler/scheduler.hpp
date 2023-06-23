#pragma once

#include <iostream>
#include <thread>
#include <vector>
#include <unordered_map>
#include <atomic>
#include "concurrent_queue.hpp"

class RunTimeScheduler{
private:

    struct WorkerArgs{
        bool no_args;
        int id;
        std::function<void()> func;
    };

    bool compare_func(void* w1, void* w2);

    std::atomic<bool> end;
    int n_threads;
    int n_tasks;
    std::vector<std::thread> _threads;
    std::unordered_map<int, std::thread* > thread_map;

    ConcurrentQueue<WorkerArgs* > q;

public:
    RunTimeScheduler(int n_threads);
    ~RunTimeScheduler();
    void worker_func(int id);

    template <typename _runnable>
    void launch_task(_runnable f);

    template <typename _callable, class... args>
    void launch_task(_callable f,args... var_args);   
};

template <typename _runnable>
void RunTimeScheduler::launch_task(_runnable f){
    WorkerArgs* w = new WorkerArgs;
    w->no_args = true;
    w->func = (std::forward<_runnable> (f));
    w->id = this->n_tasks;
    this->n_tasks++;
    this->q.add((w));
}

template <typename _callable, class... args>
void RunTimeScheduler::launch_task(_callable f, args... var_args){
    WorkerArgs* w = new WorkerArgs;
    w->no_args = false;
    //bind the function with its arguments so that we don't have to store the args for later. Now w->func holds f(var_args)
    w->func = (std::bind(std::forward<_callable>(f), std::forward<args> (var_args)...));
    w->id = this->n_tasks;
    this->n_tasks++;
    this->q.add((w));
}
