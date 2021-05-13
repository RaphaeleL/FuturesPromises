#include <chrono>
#include <future>
#include <iostream>
#include <thread>

void blocking() {
    std::async(std::launch::async,[]{
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "Blocking> Thread 1" << std::endl;
    });
    
    std::async(std::launch::async,[]{
        std::this_thread::sleep_for(std::chrono::seconds(1));  
        std::cout << "Blocking> Thread 2" << std::endl;
    });
  
    std::cout << "Blocking> Main Thread" << std::endl;  
}

void not_blocking() {
    auto first = std::async(std::launch::async, [] {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "Not Blocking> Thread 1" << std::endl;
    });

    auto second = std::async(std::launch::async, [] {
        std::this_thread::sleep_for(std::chrono::seconds(1));  
        std::cout << "Not Blocking> Thread 2" << std::endl; 
    });

    std::cout << "Not Blocking> Thread Main" << std::endl;
}

int main() {
    blocking();
    not_blocking();
}