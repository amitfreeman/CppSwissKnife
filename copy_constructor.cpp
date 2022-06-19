#include <iostream>
#include <string>
#include <cstring>

using namespace std;

class String
{

private:
  char *m_buffer;
  unsigned int m_size;

public:
  String(const char *str)
  {
    cout << "Constructor" << endl;
    m_size = strlen(str);
    m_buffer = new char[m_size + 1];
    memcpy(m_buffer, str, m_size);
    m_buffer[m_size] = 0;
  }

  //String(const String& copy) = delete;  /* to not allow copying of object */

  /* copy constructor */
  String(const String &copy)
  {
    if (copy.m_buffer != nullptr)
    {
      cout << "copy String!!" << copy.m_buffer << endl;
      m_size = copy.m_size;
      m_buffer = new char[m_size];
      memcpy(m_buffer, copy.m_buffer, m_size + 1);
    }
  }

  /* assignment operator */
  void operator=(const String &str)
  {
    if (this != &str)
    {
      cout << "Assignment operator called by : " << this->m_buffer << endl;
      m_size = str.m_size;
      m_buffer = new char[m_size];
      memcpy(m_buffer, str.m_buffer, m_size + 1);
    }
    // return *this;  /* if return type is String& */
  }

  /* move constructor: it uses rvalue reference denoted by '&&'
     takes temporary variable, & 'assigns' it's memory location
     to permanent variable */
  String(String &&copy) noexcept
  {
    cout << "move String!! " << copy.m_buffer << endl;
    m_size = copy.m_size;
    m_buffer = copy.m_buffer;

    /* destroy temporary var */
    copy.m_buffer = nullptr;
    copy.m_size = 0;
  }

  /* move assignment operator */
  void operator=(String &&copy) noexcept
  {
    if (this != &copy)
    {
      cout << "move String using =!! " << copy.m_buffer << endl;
      delete[] this->m_buffer;
      m_size = copy.m_size;
      m_buffer = copy.m_buffer;

      /* destroy temporary var */
      copy.m_buffer = nullptr;
      copy.m_size = 0;
    }
    //return *this;   /* if return type is String& &/
  }

  /* index operator overload */
  char &operator[](unsigned int index)
  {
    return m_buffer[index];
  }

  ~String()
  {
    cout << "Destruct: ";
    if (m_buffer)
    {
      cout << this->m_buffer;
      delete[] m_buffer;
    }
    cout << endl;
  }

  friend ostream &operator<<(ostream &stream, const String &str);
};

ostream &operator<<(ostream &stream, const String &str)
{
  stream << str.m_buffer;
  return stream;
}

// For move semantics
class Entity
{
private:
  String m_Name;

public:
  Entity(const String &name)
      : m_Name(name)
  {
  }

  Entity(String &&name)
      : m_Name(std::move(name))
  { //direct constructor to use move, is same as rvalue type cast (String&&)
    cout << "Moved Entity: " << m_Name << endl;
  }

  void PrintEntity()
  {
    cout << "Entity is : " << m_Name << endl;
  }
};

int main()
{

  try
  {
    String string = "Van Gough"; //constructor called
    String second = string;      //copy constructor called

    /* change char in second string */
    second[1] = 'e';

    cout << string << endl;
    cout << second << endl;

    String third = "third";
    cout << third << endl;
    third = string; //assignment operator called
    third = third;  //same object assignment will not be executed
    cout << third << endl;

    cout << "---Move semantics demo---" << endl;
    Entity ent("Amit"); //here temporary string "Amit" is passed in place of object
    ent.PrintEntity();

    String fourth = "wow!";
    String dest = std::move(fourth); //move constructor called
    //cout<<"fourth: "<<fourth<<endl;
    cout << "dest: " << dest << endl;

    String fifth = "Cat says meow!";
    dest = std::move(fifth); //move assignment called
    cout << "dest: " << dest << endl;
  }
  catch (exception e)
  {
    cout << e.what() << endl;
  }

  cout << "---FINISH---" << endl;
  return 0;
}