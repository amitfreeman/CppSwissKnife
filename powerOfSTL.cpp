/* This file contains demo for STL
 * vector, set and map, their usage and key features
 * and one demo to find point in set of range , to demonstrate power of STL
 */

#include <bits/stdc++.h>  //all header files
//#include <iostream>

using namespace std;

bool rev_func(int x, int y){
    return x > y;
}

void vectorDemo();
void setDemo();
void mapDemo();
void powerOfStl(); //problem sovling

int main()
{
   cout << "Hello World" << endl; 
   //C++ STL
   cout<<"--- 1. VECTOR DEMO ----" <<endl;
   vectorDemo();
   
   cout<<"\n\n--- 2. SET DEMO ----" <<endl;
   setDemo();
  
   cout<<"\n\n--- 3. MAP DEMO ----" <<endl;
   mapDemo();
   
   cout<<"\n\n--- 3. power of STL ----" <<endl;
   cout<<"given the set of pair-range like {2,9} {10,30} {30,400} {401, 450}" <<endl;
   cout<<"find if input num belongs in any of range" <<endl;
   powerOfStl();
   
   return 0;
}

void vectorDemo(){
   vector<int> v={11, 2, 23, 4}; //declare & define vector
   cout<< "v[1] : " << v[1] << endl; 
  //print using short form
  cout<<"printing v using short form" << endl;
  for(int i: v){
      cout<< i << ", ";
  }
   // algorithms
   cout<<"\nsorting in ascending" << endl;
   sort(v.begin(), v.end());
   
   bool present = binary_search(v.begin(), v.end(), 23);
   cout<< "23 is present? : " << present << endl;
   present = binary_search(v.begin(), v.end(), 100);
   cout<< "100 is present? : " << present << endl;
   
   v.push_back(100);
   v.push_back(100);
   v.push_back(100);
   v.push_back(100);
   v.push_back(123);
   // 2,4,11,23,100,100,100,100,123
   
   vector<int>::iterator it = lower_bound(v.begin(), v.end(), 100);
   auto it2 = upper_bound(v.begin(), v.end(), 100);
   
   cout<< "it : " << *it << ", " << "it2 : " << *it2 <<endl;
   cout<< "diff in pos it2-it1 : " << it2 - it << endl; 
   
   auto it3 = lower_bound(v.begin(), v.end(), 200); // <= 200
   cout<< "upper bound 200 reaches end? " ;
   cout<< ((it3 == v.end())?"true":"false" ) << endl;

   //sort vector in reverse
   cout<<"\nsorting in reverse" << endl;
   sort(v.begin(), v.end(), rev_func);
   
   //print using iterator
   cout<<"printing v using iterator" << endl;
   for(it3=v.begin(); it3<v.end(); it3++){
       cout<< *it3 << ", " ;
   }

   cout<<"\niterate using reference and modify" << endl;
   for(int &i: v){
      cout<< ++i << ", ";
   }
   
   cout<<"\n pop element out of vector" << endl;
   v.pop_back();
   for(int i: v){
       cout<< i << ", ";
   }
   
}

void setDemo(){
    set<int> s={2,3,1};
    s.insert(3);
    s.insert(-5);
    
    cout<<"set is by default in ascending order, and has uniq values"<<endl;
    for(int i : s){
        cout<< i << " ";
    }
    cout<< endl;
    
    cout<< "find 1 -> " ;
    auto it = s.find(1);
    if(it==s.end())
      cout<<"not present";
    else
       cout<<"present";
       
     cout<<endl;
    cout<<"uppder_bound, lower_bound"<<endl;
    it = s.upper_bound(1);
    if(it!=s.end())
        cout<< "upper bound 1 *it = "<< *it << endl;

    it = s.upper_bound(0);
    if(it!=s.end())
        cout<< "upper bound 0 *it = "<< *it << endl;

    it = s.upper_bound(10);
    if(it!=s.end())
        cout<< "*it = "<< *it;
    else
        cout<<"can't find upper_bound for 10" <<  endl;
        
    s.insert(6);
    s.insert(7);
    cout<< "remove/erase from set"<<endl;
    s.erase(3);
    s.erase(s.begin(), s.find(2));
    for(int i : s){
        cout<< i << " ";
    }
    
}

void mapDemo(){
   map<int, int> m;
   m[1]=100;
   m[2]=200;
   m[444]=400;
   m[3]=300;
   m.insert(pair<int, int> (9,99));
   
   cout<<"printing map"<<endl;
   cout<<"key : \tvalue"<<endl;
   for(auto it=m.begin(); it!=m.end(); it++){
       cout<<it->first <<":\t"<< it->second <<endl;
   }
   cout<<endl<< "m[2] : "<< m[2];
   m.erase(2);
   cout<<endl<< "m[2] : "<< m[2];
   
   cout<<"\n\nprinting occurence of chars in string - bazzinga"<<endl;
   map<char,int> mc;
   string str="bazzinga";
   for(char c: str){
       mc[c]++;
   }
    
   cout<<"key : \tvalue"<<endl;
   for(auto it=mc.begin(); it!=mc.end(); it++){
       cout<<it->first <<":\t"<< it->second <<endl;
   }
   
   //stringstream - stream of strings
   cout<<"\nusing stringstream to count frequency of strings in stream"<<endl;
   map<string, int> ms;
   str="how much wood would a woodchuck chuck if a woodchuck could chuck wood";
   stringstream ss(str);
   /*operator << — add a string to the stringstream object.
    operator >> — read something from the stringstream object*/
   for(string word; ss >> word ;){
       ms[word]++;
   }
   
   cout<<"key : \tvalue"<<endl;
   for(auto it=ms.begin(); it!=ms.end(); it++){
       cout<<it->first <<":\t"<< it->second <<endl;
   }
}


void  powerOfStl(){
    set< pair<int,int> > s;
    s.insert( {401, 450} );
    s.insert( {20, 30} );
    s.insert( {2, 9} );
    s.insert( {30, 400} );
    int point;
    cout<<"Enter num : "<<endl;
    //cin>> point;
    point=45;
    cout<<"num = "<< point << endl;
    
    auto it=s.upper_bound( {point, INT_MAX} );
    it--;
    pair<int, int> current = *it;
    
    if(current.first <= point && point <= current.second){
        cout<<"yes, its present in "<< current.first<<","<<current.second <<endl;
    }
    else{
        cout<<"not present"<<endl;
    }
    
    point=451;
    cout<<"num = "<< point << endl;
    it=s.upper_bound({point, INT_MAX});
    if( (--it)->first <= point && point <= (it)->second ){
        cout<<"yes, its present in"<< it->first << ", " << it->second <<endl;
    }
    else{
         cout<<"not present"<<endl;
    }
}
