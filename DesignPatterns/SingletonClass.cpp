#include <iostream>

using namespace std;

class Singleton {
   static Singleton *instance;
   int data;
 
   // Private constructor so that no objects can be created.
   Singleton() {
      cout<<"Singleton private constructor called!"<<endl;
      data = 0;
   }

   public:
   static Singleton *getInstance() {
      if (!instance)
      instance = new Singleton;
      return instance;
   }

   int getData() {
      return this -> data;
   }

   void setData(int data) {
      this -> data = data;
   }
};

//Initialize pointer to zero so that it can be initialized in first call to getInstance
Singleton *Singleton::instance = nullptr;

int main(){
   cout<<"--- Main start ---"<<endl;
   Singleton *s = s->getInstance();
   cout <<"s Data = "<< s->getData() << endl;
   s->setData(100);
   cout <<"s Data after set = "<< s->getData() << endl;

   Singleton *s1= s->getInstance();
   cout<<"s1 Data = "<< s1->getData() << endl;

   return 0;
}