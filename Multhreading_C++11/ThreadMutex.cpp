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

int amount=0;
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
	for(int i=0; i<100; i++){
		if(m.try_lock()){
			++counter;
			cout<<"\nI incremented counter now"<<endl;
			m.unlock();
		}
		else{
            cout<<"\nI Can't lock, doing something else..."<<endl;
		}
	}
}

int main() {
	cout << "--- Mutex Threading demo ----" << endl;

	/* create threads */
	thread t1(addMoney);
	thread t2(addMoney);

	t1.join();
	t2.join();

	cout<<"\nAfter threads amount: "<<amount<<endl;
	cout<<"After threads count: "<<counter<<endl;
	return 0;
}
