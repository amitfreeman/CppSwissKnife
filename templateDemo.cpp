#include <iostream>

using namespace std;

/* This program is to show usage of template
   for function, class, multiple variable 
   to achieve polymorphism using generics */

/* Template generic function */
template <typename T>
//void myMax(T x, T y)
T myMax(T x, T y)
{
    return (x>y)?x:y;
    /*if(x>y)
        cout<< x << " is greater";
    else
         cout<< y << " is greater"; */
}

/* Template class example */
template <typename T>
class Array { 
private: 
    T* ptr; 
    int size; 
  
public: 
    Array(T arr[], int s){
     ptr = new T[s]; 
     size = s; 
     for (int i = 0; i < size; i++) 
         ptr[i] = arr[i]; 
    }; 
    
    void print(){
        for (int i = 0; i < size; i++) 
        cout << " " << *(ptr + i); 
        cout << endl; 
    }; 
}; 
  
/* Template passing multiple arguments */
template<class T, class U, class Z>
class A{
    T x;
    U y;
    Z z;
    
  public:
     A(T ix, U iy)
     {
         cout<< "A's constructor called" <<endl;
         this->x=ix;
         this->y=iy;
     }
     
    void printElements(){
         cout << "A has x=" << x << " and y="<< y << endl;
     }
};

int main()
{
   cout << "Hello World" << endl; 
   
   /* Template function */
 // myMax<int> (3, 7);  //for print purpise
   cout << myMax<int> (3, 7)<< endl;
   cout << myMax<double> (3.0, 7.0)<< endl;
   cout << myMax<char> ('e', 'g')<< endl;
   
   /* template class */
   cout<< "Template class"<< endl;
    int arr[5] = { 1, 2, 3, 4, 5 }; 
    Array<int> a(arr, 5); 
    a.print();
    
    /* Template multiple arguments */
    A<char, char> aClass('a','b');
    A<int, double> bClass(3, 7.0);
    
    aClass.printElements();
    bClass.printElements();
   
   return 0;
}
