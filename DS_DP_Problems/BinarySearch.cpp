#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
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

    int search(vector<int>& nums, int target) {
        int end=nums.size();

        return binary_search(0, end, target, nums);
    }
};