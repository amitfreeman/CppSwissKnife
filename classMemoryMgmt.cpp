#include <iostream>

using namespace std;

class Cls{
  
  private:
     int var;

  public:
     Cls(int i=0){
         this->var=i;
     }
     
    int get_var(){
         return var;
     }
     
     
     ~Cls(){
         cout<<"\nDestroying Cls with value ="<<this->var;
     }
    
};


int main()
{
   cout << "Hello World" << endl; 
   
   Cls A(10);  //object A stored in stack memory
   Cls* B=new Cls(20);  //object B stored on heap memory
   
   cout<<"\nA="<<A.get_var()<<endl;
   cout<<"\nB="<<B->get_var()<<endl;
   

   delete B;  //if we don't delete it explicitely memory won't be release at end of program
   cout<<"\nExiting";
   
   return 0;
} //A is automatically destroyed
