#include <iostream>
#include "RadixSort.h"
#include <fstream>
#include <string>
#include <ctime>
#include <cmath>
#include <chrono>
using namespace std;
int main() {
    unsigned long n=0;
    unsigned int buck,num;
    int lvls;
    unsigned long x,y;
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
    string filename = "/home/tanmaypoke/Theory-16-25-81-Bk1.tsv";
//        cout<<"Enter filename with absolute path: ";
//        cin>>filename;

    in.open(filename);
    if(in.is_open())
    {}
    else {
        cout << "File couldn't be opened." << endl;
        return 0;
    }

//    for (int i = 0; i < n; i++) {
//        getline( in, line,'\t');
//        x=strtoul(line.c_str(), nullptr,0);
//        getline( in, line,'\t');
//        y=strtoul(line.c_str(), nullptr,0);
//        inputarr[i]=Points<unsigned long>(x,y);
//    }

    getline( in, line,'\t');
    while(!in.eof()) {
        x=strtoul(line.c_str(), nullptr,0);
        getline( in, line,'\t');
        y=strtoul(line.c_str(), nullptr,0);
        Points<unsigned long> temp(x,y);
        inputarr.push_back(temp);
        n++;
        getline( in, line,'\n');
        getline( in, line,'\t');
    }
    cout<<"Number of points "<<n<<endl;
    in.close();

//    srand(2);
//#pragma omp parallel for
//    for (int i = 0; i < n; i++) {
//        x=rand();
////            x=x<<32;
////            x=x|rand();
//        y=rand();
////            y=y<<32;
////            y=y|rand();
//        inputarr[i]=Points<unsigned long>(x,y);
//    }
    lvls = (sizeof(inputarr[0].getX()) * 16) / buck + 1;
    omp_set_nested(1);
    RadixSort<unsigned long > obj=RadixSort<unsigned long>(n,num,buck,&inputarr);
//    cout << "Symmetry: " << obj.symmetrycheck() << endl;
    chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();
//    obj.print();
    cout << "Original list:\n";
//    obj.print1();
    obj.Sort(0, n - 1, lvls);
    cout << "Sorted original list:\n";
//    obj.print1();
    cout << "Count list:\n";
    vector<Points<unsigned long>> count = obj.degree_count();
    RadixSort<unsigned long > obj1=RadixSort<unsigned long>(count.size(),num,buck,&count);
//    cout<<"Count:"<< lvls<<","<<count.size()<<endl;
    obj1.Sort(0,count.size()-1,lvls);
    obj.writeToFile(1);
    cout << "Sorted count list:\n";
    obj1.print1();
    obj.newmap(obj1.getArr());
    cout << "Replaced u list:\n";
//    obj.print1();
    obj.writeToFile(2);
    obj.swap();
    obj.transfer();
    obj.Sort(0, n - 1, lvls);
    cout << "Sorted list on v,u:\n";
//    obj.print1();
    obj.writeToFile(3);
    obj.newmap(obj1.getArr());
    obj.writeToFile(4);
    cout << "Final list:\n";
//    obj.print1();
    chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>( t2 - t1 ).count();
    cout <<endl <<"The time taken is "<<duration <<" microseconds"<<endl;
    obj.transfer();
    obj.Sort(0, n - 1, lvls);
    obj.check();
    obj.check1();
    obj1.check();
    obj1.check1();
    obj.writeToFile(5);

    //Code for matching final sets of degrees. The set of degree counts in original list and set of degree counts in final list should be the same.
//    vector<Points<unsigned long>> finalcount1 = obj1.degree_count();
//    vector<Points<unsigned long>> count1 = obj.degree_count();
//    RadixSort<unsigned long > obj2=RadixSort<unsigned long>(count1.size(),num,buck,&count1);
//    obj2.writeToFile(6);
//    obj2.Sort(0,count1.size()-1,lvls);
//    obj2.writeToFile(7);
//    vector<Points<unsigned long>> finalcount2 = obj2.degree_count();
    //Code for verifying symmetry, i.e. if (u,v) exists then (v,u) exists too.
    cout << "Symmetry: " << obj.symmetrycheck() << endl;

//    obj->print();
    return 0;
}