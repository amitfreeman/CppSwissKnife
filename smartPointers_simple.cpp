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

int main()
{
   cout << "Hello World - smart pointers" << endl; 
   
   /* normal integer ptr */
   int *ip=new int(11);
   cout<<"*ip="<<*ip<<endl;

   /* smart integer ptr */
   unique_ptr<int> iu(new int(1001));
   cout<<"*iu="<<*iu<<endl;
   
   /* class A */
   A *a=new A("jaanJaye");
   a->printA();
   
   //shared_ptr<A> as(new A("parShaanNaJaye"));
   shared_ptr<A> as=make_shared<A>("parShaanNaJaye");
   as->printA();
   
   //unique_ptr<A> au(new A());;
   unique_ptr<A> au=make_unique<A>();
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