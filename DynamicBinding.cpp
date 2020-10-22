#include <iostream>

using namespace std;

/* This program demonstrates dynamic binding in cpp
    using virtual function */

class base
{
    public: 
    virtual void print () 
    { cout<< "print base class" <<endl; } 
   
    void show () 
    { cout<< "show base class" <<endl; } 
};

class derived:public base
{ 
public: 
    void print () //print () is already virtual function in derived class, we could also declared as virtual void print () explicitly 
    { cout<< "print derived class" <<endl; } 
   
    void show () 
    { cout<< "show derived class" <<endl; } 
}; 

int main()
{
   cout << "Hello World" << endl; 
   base *bptr;
   derived d;
   bptr=&d;
   
   //binding virtual function at runtime
   bptr->print();
   
   //non virtual function
  bptr->show();
   
   return 0;
}
