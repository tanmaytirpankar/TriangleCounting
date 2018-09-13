#include <iostream>
#include "RadixSort.h"
#include <fstream>
#include <string>
#include <ctime>
#include <cmath>
#include <chrono>
using namespace std;
int main() {
    unsigned int n;
    int num,buck;
    cout<<"Enter the number of points:";
    cin>>n;
    cout<<"Number of points "<<n<<endl;
    cout<<"Enter the number of threads:";
    cin>>num;
    cout<<"Enter the size of bucketing number";
    cin>>buck;
    /*ofstream fs;
    string filename = "exampleOutput5.csv";
    fs.open(filename);
    for (int counter = 0; counter <  n; counter++)
    {

        fs << (rand()%(2^31)) << "," << (rand()%(2^31)) << "," << (rand()%(2^31)) << endl;
    }
    fs.close();*/
//    omp_set_max_active_levels(3);
//    cout<<omp_get_thread_limit()<<endl;
    omp_set_nested(1);
    RadixSort<unsigned long > *obj=new RadixSort<unsigned long>(n,num,buck);
    //obj->printinp();
    //clock_t t1,t2;
    //t1=clock();
    chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();
    obj->sorting();
    chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>( t2 - t1 ).count();
    cout <<endl <<"The time taken is "<<duration <<" microseconds"<<endl;
//    obj->transfer();
    obj->check();
    obj->check1();

//    obj->print();
    return 0;
}