#include <future>
#include <iostream>
#include <thread>
#include <utility>

void add(std::promise<int>&& promise, int a, int b) {
    promise.set_value(a+b);
}

void sub(std::promise<int>&& promise, int a, int b) {
    promise.set_value(a-b);
}

int main(){

    int a = 6;
    int b = 3;
    
    std::promise<int> promise_add; 
    std::promise<int> promise_sub; 

    std::future<int> result_add = promise_add.get_future(); 
    std::future<int> result_sub = promise_sub.get_future(); 
    
    std::thread thread_add(add, std::move(promise_add), a, b);
    std::thread thread_sub(sub, std::move(promise_sub), a, b);
    
    std::cout << a << " + " << b << " = " << result_add.get() << std::endl;
    std::cout << a << " - " << b << " = " << result_sub.get() << std::endl;

    thread_add.join();
    thread_sub.join();

}
