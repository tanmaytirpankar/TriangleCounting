#include <iostream>
#include "RadixSort.h"
#include <fstream>
#include <string>
#include <ctime>
#include <cmath>
#include <chrono>
using namespace std;
int main() {
    unsigned long n=0,num;
    int buck,lvls;
    unsigned long x,y,z;
    ifstream in;
    string line;
    cout<<"Enter the number of points:";
    cin>>n;
    cout<<"Number of points "<<n<<endl;
    cout<<"Enter the number of threads:";
    cin>>num;
    cout<<"Enter the size of bucketing number";
    cin>>buck;
    omp_set_num_threads(num);
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
    vector<Points<unsigned long>> inputarr(n);
    string filename = "/home/tanmaypoke/Theory-16-25-81-B1k1.tsv";
//        cout<<"Enter filename with absolute path: ";
//        cin>>filename;
    in.open(filename);
    if(in.is_open())
    {}
    else
        cout<<"File couldn't be opened."<<endl;
    for (int i = 0; i < n; i++) {
        getline( in, line,'\t');
        x=strtoul(line.c_str(), nullptr,0);
        getline( in, line,'\t');
        y=strtoul(line.c_str(), nullptr,0);
        getline( in, line,'\n');
        z=strtoul(line.c_str(), nullptr,0);
        inputarr[i]=Points<unsigned long>(x,y,z);
    }
    in.close();
//    srand(2);
//#pragma omp parallel for
//    for (int i = 0; i < n; i++) {
//        x=rand()%100;
//            x=x<<32;
//            x=x|rand();
//        y=rand()%100;
//            y=y<<32;
//            y=y|rand();
//        z=rand()%100;
//            z=z<<32;
//            z=z|rand();
//        inputarr[i]=Points<unsigned long>(x,y,z);
//    }
    lvls = (sizeof(inputarr[0].getX()) * 8) / buck + 1;
    omp_set_nested(1);
    RadixSort<unsigned long > obj=RadixSort<unsigned long>(&inputarr,n,num,buck);

    chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();
    obj.print1();
    obj.Sort(0, n - 1, 1, lvls);
    obj.print1();
    vector<Points<unsigned long>> count = obj.degree_count(1);
    for (unsigned long i = 0; i < count.size()-1; i++) {
        if(count[i].getY()>1) {
            obj.Sort(count[i].getZ(), count[i + 1].getZ() - 1, 2, lvls);
        }
    }
    if(count[count.size()-1].getY()>1)
        obj.Sort(count[count.size()-1].getZ(), n - 1, 2, lvls);
    obj.print1();
    RadixSort<unsigned long > obj1=RadixSort<unsigned long>(&count,count.size(),num,buck);
    cout<<"Count:"<< lvls<<","<<count.size()<<endl;
    obj1.Sort(0,count.size()-1,2,lvls);
    obj1.print1();
    obj.newmap(obj1.getArr());
    obj.print1();
    

    chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>( t2 - t1 ).count();
    cout <<endl <<"The time taken is "<<duration <<" microseconds"<<endl;
//    obj->transfer();
    obj.check();
    obj.check1();

//    obj->print();
    return 0;
}