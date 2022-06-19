/*============================================================================
 * Name        : ThreadMutex.cpp
 * Author      : Amit Paropkari
 * Version     :
 * Description : Thread mutex demo
 *============================================================================*/

#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

int amount=100;
int counter=0;
mutex m;  //declare mutex variable

void addMoney(){
	/* simple mutex lock, when called, it will either lock
	 * or wait till it can lock, blocking
	 */
	m.lock();    //lock critical section
	++amount;
	m.unlock();   //unlock critical section

	/* try lock example, when called it will try to lock
	 * return true if it can, otherwise return false and let
	 * program continue, non-blocking
	 */
	for(int i=0; i<10; i++){
		if(m.try_lock()){
			++counter;
			cout<<"\nThread id: "<<this_thread::get_id()<<" incremented counter now"<<endl;
			this_thread::sleep_for( chrono::milliseconds(20) );
			m.unlock();
		}
		else{
            cout<<"\nThread id: "<<this_thread::get_id()<<" Can't lock, doing something else..."<<endl;
			this_thread::sleep_for( chrono::milliseconds(10) );
		}
	}
}

int main() {
	cout << "--- Mutex Threading demo ----" << endl;

	/* create threads */
	thread t1(addMoney);
	thread t2(addMoney);

    /* join acts as wait
	   main thread waits till calling 
	   thread is finished, & joins main thread */
	if(t1.joinable())
	  t1.join();
	if(t1.joinable())  
	  t2.join();

	cout<<"\nAfter threads amount: "<<amount<<endl;
	cout<<"After threads count: "<<counter<<endl;
	return 0;
}
