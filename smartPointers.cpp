#include <iostream>
#include <memory>

using namespace std;

class A{
  private:
     string s;
  public:
      A(){
          cout<<"In constructor A"<<endl;
          s="bazzingaa";
      }
      
     A(string str){
          cout<<"In parametarized constructor A"<<endl;
          s=str;
      }
      
      ~A(){
          cout<<"destructor of A: for s="<<s<<endl;
      }
      
      void printA(){
          cout<<"s is = "<<s<<endl;
      }
};

int* func(){
    int *lp=new int(11);
    //delete lp;  //gives core dump
    return lp;   //strangly working without return statement
}

A* funcA(){
    A* la=new A("jaanJaye");  //raw pointers
    return la;
}

shared_ptr<A> funcAS(){
    shared_ptr<A> la=make_shared<A>("parShaanNaJaye");
    return la;
}

unique_ptr<A> funcAU(){
    //unique_ptr<A> la(new A("thisIsUnique"));  
    unique_ptr<A> la=make_unique<A>("thisIsUnique");   //make_unique not in C++11
    return la;
}

int main()
{
   cout << "Hello World - smart pointers" << endl; 
   
   /* normal integer ptr */
   int *ip;
   ip=func();
   cout<<"*ip="<<*ip<<endl;

   /* smart integer ptr */
   unique_ptr<int> iu(new int(1001));
   cout<<"*iu="<<*iu<<endl;
   
   /* class A */
   A *a=funcA();
   a->printA();
   
   shared_ptr<A> as=funcAS();
   as->printA();
   
   unique_ptr<A> au=funcAU();
   au->printA();
   
   //unique_ptr<A> auc=au; //not allowed
   shared_ptr<A> asc=as;
   cout<<"\nasc shared with as"<<endl;
   asc->printA();
   cout<<"  -- shared as.use_count="<<as.use_count()<<endl;
   
   cout<<"\ndeleting pointers"<<endl;
   delete a;
   delete ip;
  /* delete as;
   delete au;
   delete iu; */  /* no need, automatically handled */
   
   cout<<"FINISH"<<endl;
   return 0;
}