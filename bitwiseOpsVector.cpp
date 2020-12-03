#include <iostream>
#include <vector>

using namespace std;

int main()
{
   cout << "Hello World" << endl; 
   
   vector<int> vArr= {0,1,2,3,4,5,6,7,8,9};
   
   /* odd/even, fasted way is num & 1,
      0 - even, 1 - odd
      if(0) = false false
   */
   cout<<"Finding odd and even in vector" << endl;
   for(int i: vArr){
       if(i & 1){ 
          cout<< i << ":is odd, " ;
       }
       else{
          cout<< i << ":is even, " ;
       }   
   }
   
    cout << "\nmultiply by 2, by bitwise <<" << endl;
       /* multiply by 2 is - 2 << 1 */
    for(auto it=vArr.begin(); it!=vArr.end(); it++){
        *it=*it<<1;
        cout<< *it << " "; 
    }
    
     cout << "\ndivide by 2, by bitwise <<" << endl;
    /* multiply by 2 is - 2 >> 1 */
    for(auto it=vArr.begin(); it!=vArr.end(); it++){
        *it=*it>>1;
        cout<< *it << " "; 
    }
   
   return 0;
}
