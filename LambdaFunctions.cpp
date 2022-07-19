#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/* Lambdas are functors
 *  auto greet = []() { cout<<"Hello"; }
 * [] = lambda introducer
 * () = parameter list
 */

int main(){

    int main_int=100;
    
    auto add = [] (int a, int b){
        cout<<"Sum = "<<a+b<<endl;
    };

    add(100, 1);

    /* capture enclosing function variable
     * by reference */
    auto addRef = [&main_int] (int n) {
        main_int+=n;
    };
  
    addRef(11);
    cout<<"main_int: "<<main_int<<endl;

    /* capture enclosing function variable
     * by value */
    auto addVal = [main_int] (int n) {
        return main_int+n;
    };
  
    int ans=addVal(21);
    cout<<"ans: "<<ans<<", main_int: "<<main_int<<endl;

    /* [=] captures all enclosing variables by value */

    /* use inline function in STL */
    vector<int> arr = {1,2,3,4,5,6,7,11,30};
    int even_count = count_if(arr.begin(), arr.end(), [](int num){
          return num%2 == 0;
    });

    cout<<"There are "<<even_count<<" even numbers."<<endl;

    return 0;
}