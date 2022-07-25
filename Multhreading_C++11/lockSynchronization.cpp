#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex mtx;
std::condition_variable cv;
bool ready=false;

int num=1;
int MAX=51;

/* Below is the plain program without any locking mechanism,
   works perfectly fine! */
void print_even(){
   while(num<MAX){
        if(!(num&1)){
            std::cout<<"\nEven num: "<<num;
            num++;
        }
   }
}

void print_odd(){
   while(num<MAX){
        if(num&1){
            std::cout<<"\nOdd num: "<<num;
            num++;
        }
   }
}

/* This program demonstrates synchronization between two
   threads using condition variable - which intern uses mutex 
   with lock_guard */
void printEvenSync(){
   while(num<MAX){
      std::unique_lock<std::mutex> locker(mtx);
      cv.wait(locker, [](){ return (!(num&1)); });
      std::cout<<"\nEven num: "<<num<<std::endl;
      num++;
      locker.unlock();
      //cv.notify_all();
      cv.notify_one();  /* notifies one thread waiting for, for more than one behavior undefined */
   }
}

void printOddSync(){
   while(num<MAX){
      std::unique_lock<std::mutex> locker(mtx);
      cv.wait(locker, [](){ return ((num&1)); });
      std::cout<<"\nOdd num: "<<num<<std::endl;
      num++;
      locker.unlock();
      //cv.notify_all();
      cv.notify_one();
   }
}


int main(){
     /*std::thread t1(print_even);
     std::thread t2(print_odd);

     t1.join();
     t2.join();*/

     std::thread ts1(printEvenSync);
     std::thread ts2(printOddSync);

     ts1.join();
     ts2.join();

     std::cout<<std::endl;
}