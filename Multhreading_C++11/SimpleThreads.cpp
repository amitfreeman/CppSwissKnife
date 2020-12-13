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

	ull start=0, end=1900000000;

	//high resolution clock
    auto startTime = high_resolution_clock::now();

	/*findOddSum(&start, &end);
	findEvenSum(&start, &end);*/

    /* call simple stand alone functions*/
    /*thread t1(findOddSum, &start, &end);
    thread t2(findEvenSum, &start, &end);*/

    /* call using class objects functions
     *    for public method - pass object reference
     *    for static method - pass only method, object not needed
     * */
    findSum obj;
    thread t1(&findSum::findOddSum, &obj, &start, &end);
    thread t2(&findSum::findEvenSum, &obj, &start, &end);

    if(t1.joinable())
      t1.join();
    if(t2.joinable())
      t2.join();

	auto stopTime = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stopTime - startTime);

	cout<<"Even Sum: "<<evenSum<<endl;
	cout<<"Odd Sum: "<<oddSum<<endl;
	cout<<"time took - "<<duration.count()/1000000 <<" seconds"<<endl;

	return 0;
}
