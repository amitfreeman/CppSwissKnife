#include <iostream>
 
/* Create interface in cpp using virtual function */
 
using namespace std;
 
// Base class
class Shape {
   public:
      // pure virtual function providing interface framework.
      virtual int getArea() = 0;
      /*int getArea(){
          cout<<"Shape getArea()"<<endl;
       } ;*/  
       //without virtual, no runtime linking, function defined in class for respective object will be called

      void setWidth(int w) {
         width = w;
      }
   
      void setHeight(int h) {
         height = h;
      }

      virtual ~Shape(){  }
   
   protected:
      int width;
      int height;
};
 
// Derived classes
class Rectangle: public Shape {
   public:
      int getArea() { 
         return (width * height); 
      }

      virtual void funcR(){
         cout<<"virtual in Derived class Rectangle!"<<endl;
      }
};

class Exmperiment: public Rectangle{
   public:
      void funcR(){
        cout<<"In Experiment class!"<<endl;
     }
};

class Triangle: public Shape {
   public:
      int getArea() { 
         return (width * height)/2; 
      }
};
 
int main(void) {
    Shape *shape;
   Rectangle Rect;
   Triangle  Tri;
   shape=&Rect;
 
   /*Rect.setWidth(5);
   Rect.setHeight(7);*/
   shape->setWidth(5);
   shape->setHeight(7);
   
   // Print the area of the object.
   cout << "Total Rectangle area: " << shape->getArea() << endl;

   Tri.setWidth(5);
   Tri.setHeight(7);
   
   // Print the area of the object.
   cout << "Total Triangle area: " << Tri.getArea() << endl; 

   /* static cast */
   int i=10;
   float f=12.22;
   i=static_cast<int>(f);  // like i=(int)f;
   cout<<"i:"<<i<<endl;
   void *v=static_cast<int*>(&i);

   /* Dynamic cast */
   Shape *shape1;
   Rectangle *square=new Rectangle();
   Triangle  *triangle;

   /* shape points to Rectangle */
   shape1 = dynamic_cast<Shape*>(square);
   if(shape1)
      cout<<"success dynamic cast 1"<<endl;
   else
      cout<<"failure dynamic cast 1"<<endl;

   /* base class contains Rectangle object,
      conversion to Triangle object pointer not correct */
   triangle = dynamic_cast<Triangle*>(shape1);
   if(triangle)
      cout<<"success dynamic cast 2"<<endl;
   else
      cout<<"failure dynamic cast 2"<<endl;

   /* make it point to Triangle */
   shape1=dynamic_cast<Shape*>(&Tri);

   /* base class now contains Triangle object,
      conversion successful */
   triangle = dynamic_cast<Triangle*>(shape1); 
   if(triangle)
      cout<<"success dynamic cast 3"<<endl;
   else
      cout<<"failure dynamic cast 3"<<endl;

   return 0;
}
