#include <iostream>
#include <typeinfo>

#define flag 0x0000000f

class Example{
    std::string name;
    int num;
};

int main(){

  const std::type_info& tflag=typeid(flag);  
  std::cout<<"typeof(flag): "<<tflag.name()<<", flag: "<<flag<<std::endl;

  auto flagInternal=0x00000001;
  std::cout<<"flagInternal: "<<flagInternal<<std::endl;
  if(flag & flagInternal)
     std::cout<<"in if"<<std::endl;
  else
     std::cout<<"in else"<<std::endl; 

   flagInternal=0x00000020;
  std::cout<<"flagInternal: "<<flagInternal<<std::endl;
  if(flag & flagInternal)
     std::cout<<"in if"<<std::endl;
  else
     std::cout<<"in else"<<std::endl; 

   Example e;
   const std::type_info& tclass=typeid(e);  
   std::cout<<"type of e: "<<tclass.name()<<std::endl;
}