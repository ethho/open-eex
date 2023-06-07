#pragma once 

#include <queue>
#include <functional>
#include <mutex>
#include <iostream>

//Coarse grained lock implementation
template <typename T>
class ConcurrentPriorityQueue{
private:
    //pointer to the compare function, typedef is only in the scope of the class
    typedef bool (*cmp_func) (T,T);

    std::priority_queue<T> pq;
    std::mutex m;
    cmp_func compare_func;

public:
    void init(cmp_func cmp = nullptr); //sets up the compare function
    bool is_empty(); //check if PQ is empty
    void add(T elt); //add elt to PQ
    T extract_top(); //extract elt at the top of PQ - along with rearrangement
    T view_top(); //view elt at the top of PQ without re-inserting
};