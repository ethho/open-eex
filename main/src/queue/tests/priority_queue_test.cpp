#include "catch_amalgamated.hpp"
#include "priority_queue.hpp"
#include "../priority_queue.cpp" //yes I know this is highly ill-advised but without this the compiler has no access to an implementation of ConcurrentPriorityQueue<int> member functions
                                 //because that was never called in priority_queue.cpp

bool cmp(float a, float b){
    return a >= b;
}

int main(int argc, char** argv){
    ConcurrentPriorityQueue<float> Q;
    Q.init(cmp);
    Q.add(5.0);
    Q.add(8.0);
    std::cout << Q.view_top() << std::endl;
    float elt = Q.extract_top();
    std::cout << Q.extract_top() << std::endl;
    std::cout << Q.is_empty() << std::endl;
    return 0;
}