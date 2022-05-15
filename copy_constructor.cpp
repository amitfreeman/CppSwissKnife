#include<iostream>
#include<string>
#include<cstring>

using namespace std;

class String{

  private:
  char* m_buffer;
  unsigned int m_size;

  public:
  String(const char* str){
      m_size=strlen(str);
      m_buffer=new char[m_size+1];
      memcpy(m_buffer, str, m_size);
      m_buffer[m_size]=0;
  }

   //String(const String& copy) = delete;  /* to not allow copying of object */

  /* copy constructor */
  String(const String& copy){
       cout<<"copy String!!"<<copy.m_buffer<<endl;
       m_size=copy.m_size;
       m_buffer=new char[m_size];
       memcpy(m_buffer, copy.m_buffer, m_size+1);
  }

  void operator=(String& str){
     cout<<"Assignment operator called by : "<<this->m_buffer<<endl;
     m_size=str.m_size;
     m_buffer=new char[m_size];
     memcpy(m_buffer, str.m_buffer, m_size+1);
  }

  char& operator [] (unsigned int index) {
      return m_buffer[index];
  }

  ~String(){
      cout<<"Destruct: "<<this->m_buffer<<endl;
      if(m_buffer)
         delete[] m_buffer;
  }

  friend ostream& operator<< (ostream& stream, const String& str);

};

ostream& operator<< (ostream& stream, const String& str){
    stream<< str.m_buffer;
    return stream;
}

int main(){

  try{
    String string="Van Gough";
    String second = string;

    /* change char in second string */
    second[1]='e';

    cout<< string <<endl;
    cout<< second <<endl;

    String third="third";
    cout<< third <<endl;
    third=string;
    cout<< third <<endl;
   }
   catch(exception e){
       cout<<e.what()<<endl;
   }
    
    cout<<"---FINISH---"<<endl;
    return 0;
}