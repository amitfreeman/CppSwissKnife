/* This file is to demonstrate
   simple File in/out handling */

#include <iostream>
#include <fstream>  /* festream is base class for ifstream & ofstream */
#include <string>

using namespace std;
int MAX_SIZE=2000;

int main(){
    char str[MAX_SIZE];
    cout<<"--File Handling--"<<endl;

    fstream file;
    file.open("../FilesArea/sample_file.txt", ios::app | ios::out);

    if(!file)
       cout<<"couldn't open file"<<endl;

   cout<<"Say what you want here - "<<endl;
   cin.getline(str, MAX_SIZE, '\n');
   file<<str;
   file<<endl;

   file.clear();
   file.seekg(0, ios::beg);  /* move pointer to the start of file, as of now not working */
   string read;
   cout<<"\ncontents of file now is -"<<endl;
   while(!file.eof()){
       file>>read;
       cout<<read<<endl;
   }

   file.close();

   /* opening file again in input mode */
   ifstream ifile;
   ifile.open("../FilesArea/sample_file.txt");
   cout<<"\ncontents of file HERE is -"<<endl;
   while(!ifile.eof()){
       getline(ifile, read);
       cout<<read<<endl;
   }

   ifile.close();

   return 0;
}