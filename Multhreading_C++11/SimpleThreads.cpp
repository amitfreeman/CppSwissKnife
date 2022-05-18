/*============================================================================
 * Name        : MultithreadingCpp.cpp
 * Author      : Amit Paropkari
 * Version     :
 * Copyright   : Your copyright notice
 * Description : Simple thread creation program and time measurement if executions
 *============================================================================*/

#include <iostream>
#include <thread>
#include <chrono>
#include <algorithm>

using namespace std;
using namespace std::chrono;
typedef unsigned long long ull;

ull oddSum=0;
ull evenSum=0;

class findSum{

public:
	void findOddSum(const ull* start, ull* end){
		cout<<"In findOddSum"<<endl;
		for(ull i=*start; i<=*end; ++i){
			if(i & 1) {
				oddSum+=i;
			}
		}

	}

	void findEvenSum(const ull* start, ull* end){
		cout<<"In findEvenSum"<<endl;
		for(ull i=*start; i<=*end; ++i){
			if(! (i & 1) ) {
				evenSum+=i;
			}
		}
	}
};

int main() {
	cout << "Multithreading" << endl; // prints !!!Hello World!!!

	ull start=0, end=20000000000;

	//high resolution clock
    auto startTime = high_resolution_clock::now();
    findSum obj;

	/* plain sequential call */
	/*obj.findOddSum(&start, &end);
	obj.findEvenSum(&start, &end);*/

	/* 87762 mili seconds without thread */

    /* using threading */
    /* call simple stand alone functions*/
    /*thread t1(findOddSum, &start, &end);
    thread t2(findEvenSum, &start, &end);*/

    /* call using class objects functions
     *    for public method - pass object reference
     *    for static method - pass only method, object not needed
     * */
    
    thread t1(&findSum::findOddSum, &obj, &start, &end);
    thread t2(&findSum::findEvenSum, &obj, &start, &end);

    /* this acts as wait
	   main thread waits till calling thread is finished */
    if(t1.joinable())
      t1.join();
    if(t2.joinable())
      t2.join();

    /* 91795 mseconds with thread? */

	auto stopTime = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stopTime - startTime);

	cout<<"Even Sum: "<<evenSum<<endl;
	cout<<"Odd Sum: "<<oddSum<<endl;
	cout<<"time took - "<<duration.count()/1000 <<" mili seconds"<<endl;

	return 0;
}
