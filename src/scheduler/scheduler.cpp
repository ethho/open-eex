#include "scheduler.hpp"

RunTimeScheduler::RunTimeScheduler(int n_threads){
    int i;
    this->n_threads = n_threads;
    this->n_tasks = 0;

    //atomically set end to false
    end = false;

    for(i=0;i<this->n_threads;i++){
        this->_threads.push_back(std::thread(&RunTimeScheduler::worker_func,this,i));
    }
    return;
}

RunTimeScheduler::~RunTimeScheduler(){
    int i;
    //atomically set end to true
    end = true;

    //now wait for all tasks to be finished, so that we can call join and do not put threads to sleep unnecessarily
    while(!this->q.is_empty()){
        std::this_thread::yield();
    }

    //join all the threads
    for(i=0;i<this->n_threads;i++){
        this->_threads[i].join();
    }
    return;
}

void RunTimeScheduler::worker_func(int id){
    while(true){
        if(this->end == true && this->q.is_empty()){
            break;
        }else{
            WorkerArgs* w;
            if(!this->q.is_empty()){
                w = static_cast<WorkerArgs* > (this->q.extract_top());
                if(w != nullptr){
                    std::printf("Thread %d took task!\n", id);
                    ((w->func))();            
                }
            }
        }
    }
}