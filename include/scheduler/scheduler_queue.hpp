#pragma once 

#include <queue>
#include <functional>
#include <mutex>
#include <iostream>

//Coarse grained lock implementation
template <typename T>
class ConcurrentQueue{
private:

    std::queue<T> q;
    std::mutex m;

public:
    bool is_empty(); //check if PQ is empty
    void add(T elt); //add elt to PQ
    int size();
    T extract_top(); //extract elt at the top of PQ - along with rearrangement
    T view_top(); //view elt at the top of PQ without re-inserting
};

//all implementations are fairly basic, with a lock and a function call followed by unlock
template <typename T>
bool ConcurrentQueue<T>::is_empty(){
    bool ans;
    this->m.lock();
    ans = this->q.empty();
    this->m.unlock();
    return ans;
}

template <typename T>
void ConcurrentQueue<T>::add(T elt){
    this->m.lock();
    this->q.push(elt);
    this->m.unlock();
}

template <typename T>
T ConcurrentQueue<T>::extract_top(){
    T elt;
    this->m.lock();
    if(this->q.empty()){
        return nullptr;
    }
    elt = this->q.front();
    this->q.pop();
    this->m.unlock();
    return elt;
}

template <typename T>
T ConcurrentQueue<T>::view_top(){
    T elt;
    this->m.lock();
    elt = this->q.front();
    this->m.unlock();
    return elt;
}

template <typename T>
int ConcurrentQueue<T>::size(){
    int size;
    this->m.lock();
    m = this->q.size();
    this->m.unlock();
    return size;
}