#include "priority_queue.hpp"

template <typename T>
void ConcurrentPriorityQueue<T>::init(cmp_func cmp){
    this->compare_func = cmp;
}

//all implementations are fairly basic, with a lock and a function call followed by unlock
template <typename T>
bool ConcurrentPriorityQueue<T>::is_empty(){
    bool ans;
    this->m.lock();
    ans = this->pq.empty();
    this->m.unlock();
    return ans;
}

template <typename T>
void ConcurrentPriorityQueue<T>::add(T elt){
    this->m.lock();
    this->pq.push(elt);
    this->m.unlock();
}

template <typename T>
T ConcurrentPriorityQueue<T>::extract_top(){
    T elt;
    this->m.lock();
    elt = this->pq.top();
    this->pq.pop();
    this->m.unlock();
    return elt;
}

template <typename T>
T ConcurrentPriorityQueue<T>::view_top(){
    T elt;
    this->m.lock();
    elt = this->pq.top();
    this->m.unlock();
    return elt;
}