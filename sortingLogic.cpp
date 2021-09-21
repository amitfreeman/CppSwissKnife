/* This C++ program implements insertion, merge & quick sort
 * algorithm logic on simple integer array & compares
 * their performances.
 * Input - size of array user wants, it generates array of that size with random numbers
 */
#include <iostream>
#include <chrono>

using namespace std;

void printArray(int* arr, const int size);
void copyArray(int *oarr, int *iarr, const int size);

void insertionSort(int *arr, const int size);
void mergeSort(int *arr, int left, int right);
void quickSortL(int arr[],  int left, int right);  /* taking pivot as last element */

int main()
{
    cout<<"-- Begin ---"<<endl;

    int size=0;
    cout<<"Q) What size of array you want?"<<endl;
    cin>>size;

    int arr[size];
    srand((unsigned)time(0));

    for(int i=0; i<size; i++)
        arr[i]=(rand()%size+1);
    
    cout<<"--- Initial array is: ----"<<endl;
    printArray(arr, size);

    int isArr[size];
    copyArray(isArr, arr, size);
    /*cout<<"copied array:"<<endl;
    printArray(isArr, size);*/
    cout<<"\n---- Starting insertion sort ----"<<endl;
    auto t_start = chrono::high_resolution_clock::now();
    insertionSort(isArr, size);
    auto t_end = chrono::high_resolution_clock::now();
    cout<<"After insertion sort:"<<endl;
    printArray(isArr, size);
    double elapsed_time_ms = chrono::duration<double, std::milli>(t_end-t_start).count();
    cout<<"$$ TIME taken by insertion sore = "<<elapsed_time_ms<<" milli sec $$"<<endl;

    int msArr[size];
    copyArray(msArr, arr, size);
    cout<<"\n---- Starting merge sort ----"<<endl;
    t_start = chrono::high_resolution_clock::now();
    mergeSort(msArr, 0, size-1);
    t_end = chrono::high_resolution_clock::now();
    cout<<"After merge sort:"<<endl;
    printArray(msArr, size);
    elapsed_time_ms = chrono::duration<double, std::milli>(t_end-t_start).count();
    cout<<"$$ TIME taken by merge sore = "<<elapsed_time_ms<<" milli sec $$"<<endl;

    int qArr[size];
    copyArray(qArr, arr, size);
    cout<<"\n---- Starting quick sort (pivot as last element) ----"<<endl;
    t_start = chrono::high_resolution_clock::now();
    quickSortL(qArr, 0, size-1);
    t_end = chrono::high_resolution_clock::now();
    cout<<"After quick sort:"<<endl;
    printArray(qArr, size);
    elapsed_time_ms = chrono::duration<double, std::milli>(t_end-t_start).count();
    cout<<"$$ TIME taken by quick sore = "<<elapsed_time_ms<<" milli sec $$"<<endl;

    int qArr1[size];
    copyArray(qArr1, arr, size);
    cout<<"\n---- Starting quick sort (pivot as first element) ----"<<endl;
    t_start = chrono::high_resolution_clock::now();
    quickSortL(qArr1, 0, size-1);
    t_end = chrono::high_resolution_clock::now();
    cout<<"After quick sort:"<<endl;
    printArray(qArr1, size);
    elapsed_time_ms = chrono::duration<double, std::milli>(t_end-t_start).count();
    cout<<"$$ TIME taken by quick sore = "<<elapsed_time_ms<<" milli sec $$"<<endl;

    cout<<endl;
    return 0;
}

/* generic function to print array */
void printArray(int* arr, const int size){
    for(int idx=0; idx<size; idx++)
        cout<<" "<<arr[idx];

    cout<<endl;
}

/* generic function to copy array */
void copyArray(int *oarr, int *iarr, const int size){
    for(int idx=0; idx<size; idx++)
        oarr[idx]=iarr[idx];
}

/* Insertion sort definition:
 * iterate array from 1 to size -> i
 * current ith element = key, compare with predessor
 * if its less than previous element, shift by one postion each till previous element
 * swap with lesser number position
 */
void insertionSort(int *arr, const int size){
    int i,j, key; 

    for(i=1; i<size; i++){
        j=i-1;
        key=arr[i];

        while(j>=0 && arr[j]>key){
            arr[j+1]=arr[j];
            j--; 
        }
        arr[j+1]=key;
    }
}

/* Subroutine for mergeSort:
 * create left & right array
 * merge into original array starting left index
 * place elemt by comparing & assuming arrays are sorted
 */
void merge(int *arr,  int left,  int mid,  int right){
    int leftSize=mid-left+1, rightSize=right-mid;
    int leftArr[leftSize], rightArr[rightSize];
   
    for(int i=0; i<leftSize; i++)
        leftArr[i]=arr[i+left];
    
    for(int i=0; i<rightSize; i++)
        rightArr[i]=arr[i+mid+1];
 
    int lidx=0, ridx=0, mergeidx=left;

    /* compare & merge */
    while( lidx<leftSize && ridx<rightSize){
       if(leftArr[lidx] < rightArr[ridx]){
          arr[mergeidx]=leftArr[lidx];
          lidx++;
       }
       else{
          arr[mergeidx]=rightArr[ridx];
          ridx++;
       }
       mergeidx++;
    }

    /* merge remaining elements from left array if any */
    while(lidx<leftSize){
        arr[mergeidx]=leftArr[lidx];
        lidx++; mergeidx++;
    }
    /* merge remaining elements from right array if any */
    while(ridx<rightSize){
        arr[mergeidx]=rightArr[ridx];
        ridx++; mergeidx++;
    }
}

/* Merge sort definition:
 * find midddle, create partitionL in two for array
 * call itselft for each half recursively 
 */
void mergeSort(int *arr,  int left,  int right){
    if(left >= right)  /* base condition */
       return;
     
    int mid= left + (right-left) / 2;
    //cout<<"left: "<<left<<", mid: "<<mid<<", right: "<<right<<endl;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid+1, right);
    merge(arr, left, mid, right);
}

/* swap routine */
void swap(int *i, int *j){
    *i = *i^*j;
    *j = *i^*j;
    *i = *i^*j;  /* somehow sometimes this logic nullifies array memory for same position swap
                    its fixed in partitionL routine to swap only for different position */
  /* int t=*j;
   *j=*i;
   *i=t;*/
}

/* sub routine of quick sort
 * assume highest element pivot
 * compare elements with pivot, & if less then swap positions
 */
int partitionL(int arr[], int left, int right){
    int pivot=arr[right];
    int i=left-1;

    for(int j=left; j<right; j++){
        if(arr[j]<pivot){
            i++;
            
            if(i!=j)
              swap(&arr[i], &arr[j]);
        }
    }
    if(i+1 != right)
      swap(&arr[i+1], &arr[right]);

    return i+1;
}

/* quickSortL definition:
 * partitionL based on pivot
 * recursively call itself for before & after pivot partitionL
 */
void quickSortL(int arr[],  int left, int right){
    if(left < right){
        int pi=partitionL(arr, left, right);
        //cout<<"pi="<<pi<<endl;
        quickSortL(arr, left, pi-1);
        quickSortL(arr, pi+1, right);
    }
}

/* subroutine for quicksort, first element as pivot */
int partitionR(int arr[], int left, int right){
    int pivot=arr[left];
    int i=right+1;

    for(int j=right; j>left; j--){
        if(arr[j]>pivot){
            i--;
            
            if(i!=j)
              swap(&arr[i], &arr[j]);
        }
    }
    if(i-1 != left)
      swap(&arr[i-1], &arr[right]);

    return i+1;
}

/* quickSort with first element as pivot */
void quickSortR(int arr[],  int left, int right){
    if(left < right){
        int pi=partitionR(arr, left, right);
        //cout<<"pi="<<pi<<endl;
        quickSortR(arr, left, pi-1);
        quickSortR(arr, pi+1, right);
    }
}