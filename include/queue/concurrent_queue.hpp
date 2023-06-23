#pragma once 

#include <queue>
#include <functional>
#include <mutex>
#include <iostream>

//Coarse grained lock implementation
template <typename T>
class ConcurrentQueue{
/**
 * @brief This class implements a regular queue (not a priority queue) for the use of the scheduler. I could have used the priority queue but I didn't feel like
*/
private:
    //pointer to the compare function, typedef is only in the scope of the class
    typedef bool (*cmp_func) (T,T);

    std::queue<T> pq;
    std::mutex m;
    cmp_func compare_func;

public:
    void init(cmp_func cmp = nullptr); //sets up the compare function
    bool is_empty(); //check if PQ is empty
    void add(T elt); //add elt to PQ
    int size();
    T extract_top(); //extract elt at the top of PQ - along with rearrangement
    T view_top(); //view elt at the top of PQ without re-inserting
};

template <typename T>
void ConcurrentQueue<T>::init(cmp_func cmp){
    this->compare_func = cmp;
}

//all implementations are fairly basic, with a lock and a function call followed by unlock
template <typename T>
bool ConcurrentQueue<T>::is_empty(){
    bool ans;
    this->m.lock();
    ans = this->pq.empty();
    this->m.unlock();
    return ans;
}

template <typename T>
void ConcurrentQueue<T>::add(T elt){
    this->m.lock();
    this->pq.push(elt);
    this->m.unlock();
}

template <typename T>
T ConcurrentQueue<T>::extract_top(){
    T elt;
    this->m.lock();
    elt = this->pq.front();
    this->pq.pop();
    this->m.unlock();
    return elt;
}

template <typename T>
T ConcurrentQueue<T>::view_top(){
    T elt;
    this->m.lock();
    elt = this->pq.front();
    this->m.unlock();
    return elt;
}

template <typename T>
int ConcurrentQueue<T>::size(){
    int size;
    this->m.lock();
    m = this->pq.size();
    this->m.unlock();
    return size;
}