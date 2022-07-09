/* This program implements circular buffer concept
 * It is the single array, where first & last index are imagined
 * to be connected. For that two pointers head & tail are used.
 *  head in incremented when adding data, tail is incremented when reading-removing data
 * If older data is overwritten when adding new one if buffer is full.
 */
#include <iostream>
#include <sstream>
#include <mutex>
#include <sys/wait.h>
#include <unistd.h>

/* Functionality provided for circular buffer 
 *  Adding data
    Removing data
    Checking full/empty state/size
 */
class CircularBuffer{
   int* cbuf;
   size_t head;
   size_t tail;
   const size_t max_size;
   bool full;
   std::mutex mtx;

   public:
   
   CircularBuffer(size_t s): cbuf(new int[s]), max_size(s)
   {
      head=0; tail=0; full=false;
   }

   void cbput(int data){
      std::lock_guard<std::mutex> lock(mtx);
      cbuf[head]=data;

      if(++head==max_size)
          head=0;

       if(full){
           if(++tail==max_size)
                tail=0;
       }

      full=(head==tail);
   }

   int cbget(){
      std::lock_guard<std::mutex> lock(mtx);
      int data=cbuf[tail];
      
      if(++tail==max_size)
         tail=0;

      full=false;
      return data;
   }

   int cbsize(){
       int size=max_size;

       if(!full){
            if(head>=tail)
                size=head-tail;
            else
                size=max_size+head-tail;
        }

        //std::cout<<"cbuf.size="<<size<<std::endl;
        return size;
   }

   int cbcapacity(){
       return max_size;
   }

   bool cbempty(){
      return (!full && (head==tail));
   }

   bool cbfull(){
       return full;
   }

   void printCbuf(){
       std::cout<<"------------ cbuf:size="<<this->cbsize()<<"------------"<<std::endl;
       for(int i=0; i<max_size; i++){
           if(head==i)
             std::cout<<"H:";
           if(tail==i)
             std::cout<<"T:";
           std::cout<<cbuf[i]<<"  ";
       }
       std::cout<<"\n------------------------------"<<std::endl;
   }

   ~CircularBuffer(){
       if(cbuf)
          delete[] cbuf;
   }

};

int main(int argc, char* argv[]){ /* sample args "44 5 89 7 28 -1 31 4" */
    CircularBuffer cb(5);
    std::cout<<"\n   AT START..."<<std::endl;
    cb.printCbuf();
    std::cout<<"is it empty? "<<cb.cbempty()<<std::endl;

    if(argc>1){
        std::stringstream ss(argv[1]);
        int input;
        while(ss>>input){
            if(!cb.cbfull()){
               cb.cbput(input);
               cb.printCbuf();
            }
            else{
                pid_t pid=fork();
                if(pid==0){  //in child process get data
                    int data=cb.cbget();
                    std::cout<<"Got:"<<data<<std::endl;
                }
                else{
                    cb.cbput(input); //in main process put data
                    cb.printCbuf();
                }
            }
        }
    }

    std::cout<<"\n   AT FINISH..."<<std::endl;
    cb.printCbuf();
    return 0;
}