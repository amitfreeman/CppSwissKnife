#include <iostream>
#include <stdlib.h>

using namespace std;

class Cls{

   private:
      int var;
      static int stat; //count only when dynamically allocated by operator overloading

   public:
      Cls(int i=0){
         this->var=i;
         //stat++;
      }

      int get_var(){
         return var;
      }

      void* operator new(size_t sz){   //overload new operator
         cout<<"\nin overloaded new with size="<<sz;
         stat++;
         void *p= ::new Cls();
         return p;
      }

      void operator delete(void* p) //overload delete operator
      {
         cout<< "\nOverloading delete operator " << endl;
         stat--;
         free(p);
      }

      static int get_stat(){
         return stat;
      }

      ~Cls(){
         cout<<"\nDestroying Cls with value ="<<this->var;
      }
};

int Cls::stat=0;  //initialize static var, otherwise no memory/value

int main()
{
   cout << "Hello World" << endl;

   Cls A(10);  //object A stored in stack memory
   Cls* B=new Cls(20);  //object B stored on heap memory
   Cls* C=new Cls(30);  //object B stored on heap memory

   cout<<"\nA="<<A.get_var()<<endl;
   cout<<"\nB="<<B->get_var()<<endl;

   //static var
   cout<<"\nstat="<<Cls::get_stat()<<endl;

   delete B;  //if we don't delete it explicitely memory won't be release at end of program
   delete C;  //if we don't delete it explicitely memory won't be release at end of program
   cout<<"\nExiting";

   cout<<"\nstat now="<<Cls::get_stat()<<endl;
   return 0;
} //A is automatically destroyed
