#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {

    int binary_search(int start, int end, int target, vector<int>& nums){
         if(start < end){
             int mid=(start+end)/2;

             if(nums[mid]==target)
                return mid;
             else if(nums[mid]<target)
                return binary_search(mid+1, end, target, nums);
             else 
                return binary_search(start, mid, target, nums);
         }
         else
           return -1;
    }

public:

    int search(vector<int>& nums, int target) {
        int end=nums.size();

        return binary_search(0, end, target, nums);
    }
};

int main(){

    vector<int> arr={9,6,7,18,-2,100,2003,2022,69,44,101};
    sort(arr.begin(), arr.end());

    for(int &i: arr)
       cout<<" "<<i;

    cout<<endl;
  
    Solution s;
    int num=100;
    int pos=s.search(arr, num);
    if(pos<0) 
       cout<<"Element num: "<<num<<", not found";
    else 
       cout<<"Found element num: "<<num<<", at postion: "<<pos;

     cout<<endl;     

    num=199;
    pos=s.search(arr, num);
    if(pos<0) 
       cout<<"Element num: "<<num<<", not found";
    else 
       cout<<"Found element num: "<<num<<", at postion: "<<pos;

     cout<<endl;     

}