#include <iostream>

class Test{
  int i;
  int &r;  //reference needs to be initialized in initilizer list
  const int c; //same for constant
public:

  Test(): i(0), r(r), c(-1)
  {
       std::cout<<"--Default constructor--"<<std::endl;
       std::cout<<"i="<<i<<std::endl;
       std::cout<<"r="<<r<<std::endl;
       std::cout<<"c="<<c<<std::endl;
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
  }

 void  printTest();
};

void Test::printTest(){
    std::cout<<"printing"<<std::endl<<"i="<<i<<", r="<<r<<", c="<<c<<std::endl;
}

int main(){
    int n=20;
    int& ref=n;
    Test t(10, ref, n);
    n=21;
    t.printTest(); 
    
    Test t1;
    t1.printTest();
    return 0;
}