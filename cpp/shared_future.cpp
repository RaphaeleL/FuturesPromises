#include <exception>
#include <future>
#include <iostream>
#include <thread>
#include <utility>

std::mutex coutMutex;

void add(std::promise<int>&& promise, int a, int b) {
    promise.set_value(a+b);
}

struct Requestor {
    void operator()(std::shared_future<int> shaFut) {
        std::lock_guard<std::mutex> coutGuard(coutMutex);
        std::cout << std::this_thread::get_id() << " " << shaFut.get() << std::endl;
    }
};

int main(){
    
    int a = 6; 
    int b = 3;

    Requestor req;

    std::promise<int> promise_add;
    
    std::shared_future<int> result_add = promise_add.get_future();
   
    std::thread thread_add(add, std::move(promise_add), a, b);
    std::thread shared_thread_1(req, result_add);
    std::thread shared_thread_2(req, result_add);
    std::thread shared_thread_3(req, result_add);
    std::thread shared_thread_4(req, result_add);
    std::thread shared_thread_5(req, result_add);

    shared_thread_1.join();
    shared_thread_2.join();
    shared_thread_3.join();
    shared_thread_4.join();
    shared_thread_5.join();

}
