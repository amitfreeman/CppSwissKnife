#include <iostream>

class Test{
  int i;
  int &r;  //reference needs to be initialized in initilizer list
  const int c; //same for constant
  static int si;
  static const int sci=101; //has to be declared & defined at same time
  
  public:
   
  Test(): i(0), r(i), c(-1)
  {
       std::cout<<"--Default constructor--"<<std::endl;
       std::cout<<"i="<<i<<std::endl;
       //std::cout<<"r="<<r<<std::endl;
       std::cout<<"c="<<c<<std::endl;
       si++;
  }

  Test(int in, int& ref,  int con): i(in), r(ref), c(con)
  {
       std::cout<<"--Parametarized constructor--"<<std::endl;
       //r=ref;  //won't work
      // c=con;  //won't work
      //c=100;  //won't work either
       std::cout<<"i="<<i<<std::endl;
       std::cout<<"r="<<r<<std::endl;
       std::cout<<"c="<<c<<std::endl;
       si++;
  }

 void  printTest();

    static int getsi(){
        return Test::si;
    }

    static int getsci(){
        return Test::sci;
    }
};

void Test::printTest(){
    std::cout<<"printing"<<std::endl<<"i="<<i<<", r="<<r<<", c="<<c<<std::endl;
}

int Test::si=0;

int main(){
    int n=20;
    int& ref=n;
    Test t(10, ref, n);
    n=21;
    t.printTest(); 
    
    Test t1;
    t1.printTest();

    std::cout<<"si="<<Test::getsi()<<std::endl;
    std::cout<<"sci="<<Test::getsci()<<std::endl;
 
    std::cout<<"--- Constant value pointer ---"<<std::endl;
    int rand=10;
    int rand1=11;
    const int *i;  //constant value
    i=&rand;
    std::cout<<"i="<<i<<", *i="<<*i<<std::endl;
    //*i=11;  //illegal
    i=&rand1;  //pointer itself can be changed
    std::cout<<"After change, i="<<i<<", *i="<<*i<<std::endl;

    std::cout<<"--- Constant pointer ---"<<std::endl;
    int* const ip=&rand; //constant pointer, need initilization
    std::cout<<"ip="<<ip<<", *ip="<<*ip<<std::endl;
    //ip=&rand1; //illegal
    *ip=21;
    std::cout<<"After change, ip="<<ip<<", *ip="<<*ip<<std::endl;

   // const int const *ipc=&rand; //double const not allowed

    return 0;
}