//
// Created by tanmay on 29/5/18.
//

#ifndef UNTITLED_RADIXSORT_H
#define UNTITLED_RADIXSORT_H
//
// Created by tanmay tirpankar on 29/5/18.
//

#include "Points.h"
#include <vector>
#include <iostream>
#include <omp.h>
#include <time.h>
#include <fstream>
#include <string>
#include <chrono>
#include <math.h>
#include <bitset>
#include <bits/stdc++.h>
#include <algorithm>
using namespace std;
template <class T>
class RadixSort{
    vector <Points<T>> arr,arr1;
    int num_threads;
    unsigned int n;
    int word_size;
    int lvls;
public:
    RadixSort(unsigned int n,int num,int word_size)
    {
        this->n=n;
        this->num_threads=num;
        this->word_size=word_size;
        ifstream in;
        string line;
        omp_set_num_threads(num_threads);
        unsigned long int x,y,z;
        arr=vector<Points<T>>(n);
        arr1=vector<Points<T>>(n);
//        string filename = "/home/tanmaypoke/Theory-16-25-81-B1k1.tsv";
////        cout<<"Enter filename with absolute path: ";
////        cin>>filename;
//        in.open(filename);
//        if(in.is_open())
//        {}
//        else
//            cout<<"File couldn't be opened."<<endl;
//        for (int i = 0; i < n; i++) {
//            getline( in, line,'\t');
//            x=strtoul(line.c_str(),NULL,0);
//            getline( in, line,'\t');
//            y=strtoul(line.c_str(),NULL,0);
//            getline( in, line,'\n');
//            z=strtoul(line.c_str(),NULL,0);
//            arr1[i]=arr[i]=Points<T>(x,y,z);
////            print1();
//        }
//        in.close();
        lvls=2*((sizeof(arr[0].getX())*8)/word_size+1);
        srand(2);

#pragma omp parallel for
        for (int i = 0; i < n; i++) {
            x=rand()%1000;
//            x=x<<32;
//            x=x|rand();
            y=rand()%1000;
//            y=y<<32;
//            y=y|rand();
            z=rand()%1000;
//            z=z<<32;
//            z=z|rand();
            arr1[i]=arr[i]=Points<T>(x,y,z);
        }
//        print();
//        cout<<endl;
        print1();
        cout<<endl;
        /*cout<<"Enter Points";
        arr=new Points<T>[n];
        for (int i = 0; i < n; i++) {
            unsigned int x,y,z;
            cin>>x>>y>>z;
            arr[i]=Points<T>(x,y,z);
        }*/
    }

    void print()
    {
        for (int i = 0; i < n; i++) {
            int num_length = sizeof(arr[0].getX()) * 8;
            T x = arr[i].getX();
            T y = arr[i].getY();
            T z = arr[i].getZ();
            cout<<"("<<arr[i].getX()<<","<<arr[i].getY()<<","<<arr[i].getZ()<<"),";
            for (int j = 0; j < num_length; j++) {
                x=arr[i].getX();
                x=x>>num_length-j-1;
                cout<<x%2;
            }
            cout<<",";
            for (int j = 0; j < num_length; j++) {
                y=arr[i].getY();
                y=y>>num_length-j-1;
                cout<<y%2;
            }
            cout<<",";
            for (int j = 0; j < num_length; j++) {
                z=arr[i].getZ();
                z=z>>num_length-j-1;
                cout<<z%2;
            }
            cout<<endl;
        }
    }
    void print1()
    {
        for (int i = 0; i < n; i++) {
            cout << "(" << arr1[i].getX() << "," << arr1[i].getY() << "," << arr1[i].getZ() << "),";
        }
    }
    void Sort(unsigned long first, unsigned long last, int level)
    {
        int num_elements=last-first+1;
        if(num_elements<=20 || level<1)
        {
            if(level % 2 == 1)
            {
                sort(&arr1[first], &arr1[last + 1]);
                for (int i = first; i <=last ; i++) {
                    arr[i] = arr1[i];
                }
            } else
            {
                sort(&arr[first], &arr[last + 1]);
                for (int i = first; i <=last ; i++) {
                    arr1[i] = arr[i];
                }
            }
            return ;
        }
        int buckets=int(pow(2,word_size));
        int lvl_for_calculation;
        int choice=0;
        if (level > lvls/2)
            lvl_for_calculation = level - (lvls / 2);
        else
            lvl_for_calculation = level;
        vector<vector<unsigned int>> count(num_threads);
        vector<vector<unsigned int>> position(num_threads);
        for (int l = 0; l < num_threads; l++) {
            count[l] = vector<unsigned int>(buckets);
            position[l]=vector<unsigned int>(buckets);
        }
//        if(num_elements<=1 || level<1)
//        {
//            return ;
//        }

        int position1[buckets];
        //vector<T> temp(num_elements);
        chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();
#pragma omp parallel
        for (int j = 0; j < buckets; j++) {
            position1[j]=0;
            count[omp_get_thread_num()][j]=0;
            position[omp_get_thread_num()][j]=0;
        }
        chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>( t2 - t1 ).count();

        //cout <<"The time taken for initialization is "<<duration <<" microseconds"<<endl;
        t1 = chrono::high_resolution_clock::now();
        int remainder= (sizeof(arr[0].getX())*8)%word_size;
        int shift=((lvl_for_calculation-1)*word_size)-(word_size-remainder);
        int mask=int(pow(2,word_size)-1);
//        cout<<"first: "<<first<<" last: "<<last<<" sizeof(count): "<<count.size()<<" sizeof(count[0])"<<count[0].size()<<endl;
        if(level%2==1 && level > lvls/2)
            choice = 1;
        else if(level%2==1 && level <= lvls/2)
            choice = 2;
        else if(level%2==0 && level > lvls/2)
            choice = 3;
        else if(level%2==0 && level <= lvls/2)
            choice = 4;
        switch(choice)
        {
            case 1:
#pragma omp parallel for
                for (int i = first; i <= last; i++)
                {
                    T x;
                    //temp[i-first]=arr[i];
                        x=arr1[i].getX();
                    if(lvl_for_calculation>1)
                        x=x>>shift;
                    x=x&mask;
                    count[omp_get_thread_num()][x]++;
                }
                break;
            case 2:
#pragma omp parallel for
                for (int i = first; i <= last; i++)
                {
                    T x;
                    x=arr1[i].getY();
                    if(lvl_for_calculation>1)
                        x=x>>shift;
                    x=x&mask;
                    count[omp_get_thread_num()][x]++;
                }
                break;
            case 3:
#pragma omp parallel for
                for (int i = first; i <= last; i++)
                {
                    T x;
                    x=arr[i].getX();
                    if(lvl_for_calculation>1)
                        x=x>>shift;
                    x=x&mask;
                    count[omp_get_thread_num()][x]++;
                }
                break;
            case 4:
#pragma omp parallel for
                for (int i = first; i <= last; i++)
                {
                    T x;
                    x=arr[i].getY();
                    if(lvl_for_calculation>1)
                        x=x>>shift;
                    x=x&mask;
                    count[omp_get_thread_num()][x]++;
                }
                break;
            default:
                cout << "Invalid choice" << endl;
                break;
        }
        //cout<<endl<<"Shift is "<<shift<<endl;
        t2 = chrono::high_resolution_clock::now();
        duration = chrono::duration_cast<chrono::microseconds>( t2 - t1 ).count();

        //cout<<"The time taken for calculating counts is "<<duration <<" microseconds"<<endl;
//        cout<<"Values in count:"<<endl;
//        for(int i = 0; i < num_threads; i++){
//            for(int j = 0; j < buckets; j++){
//                cout<<count[i][j]<<" ";
//            }
//            cout<<endl;
//        }
        t1 = chrono::high_resolution_clock::now();
//        position1[0]=position[0][0]=first;
//        int prev_buc=0;
//        for (int buc = 0; buc < 8; buc++) {
//            for (int tid = 0; tid < num_threads; tid++) {
//                if (tid==0)
//                    position1[buc+1]=position[tid][buc]=prev_buc;
//                else
//                    position[tid][buc]=position[tid-1][buc]+count[tid-1][buc];
//            }
//            prev_buc=position[num_threads-1][buc]+count[num_threads-1][buc];
//        }

        position1[0]=position[0][0]=first;
        for (int buc = 0; buc < buckets; buc++) {
            for (int tid = 1; tid < num_threads; tid++) {
                if(tid==0&&buc==0)
                {}
                else
                {
                    position[tid][buc]=position[tid-1][buc]+count[tid-1][buc];
                }
            }
            if(buc<buckets-1)
                position1[buc+1]=position[0][buc+1]=position[num_threads-1][buc]+count[num_threads-1][buc];
        }

//        cout<<"Positions in position at depth "<<level<<" is:"<<endl;
//        for(int i = 0; i < num_threads; i++){
//            for(int j = 0; j < buckets; j++){
//                cout<<position[i][j]<<" ";
//            }
//            cout<<endl;
//        }

        switch(choice) {
            case 1:
#pragma omp parallel for
                for (int i = first; i <= last; i++) {
                    T x;
                        x = arr1[i].getX();
                    if (lvl_for_calculation > 1)
                        x = x >> shift;
                    x = x & mask;
                    arr[position[omp_get_thread_num()][x]] = arr1[i];
                    position[omp_get_thread_num()][x]++;
                }
                break;
            case 2:
#pragma omp parallel for
                for (int i = first; i <= last; i++) {
                    T x;
                    x = arr1[i].getY();
                    if (lvl_for_calculation > 1)
                        x = x >> shift;
                    x = x & mask;
                    arr[position[omp_get_thread_num()][x]] = arr1[i];
                    position[omp_get_thread_num()][x]++;
                }
                break;
            case 3:
#pragma omp parallel for
                for (int i = first; i <= last; i++) {
                    T x;
                    x = arr[i].getX();
                    if (lvl_for_calculation > 1)
                        x = x >> shift;
                    x = x & mask;
                    arr1[position[omp_get_thread_num()][x]] = arr[i];
                    position[omp_get_thread_num()][x]++;
                }
                break;
            case 4:
#pragma omp parallel for
                for (int i = first; i <= last; i++) {
                    T x;
                    x = arr[i].getY();
                    if (lvl_for_calculation > 1)
                        x = x >> shift;
                    x = x & mask;
                    arr1[position[omp_get_thread_num()][x]] = arr[i];
                    position[omp_get_thread_num()][x]++;
                }
                break;
            default:
                cout << "Invalid choice." << endl;
        }
//        temp.clear();
//        temp.shrink_to_fit();
        t2 = chrono::high_resolution_clock::now();
        duration = chrono::duration_cast<chrono::microseconds>( t2 - t1 ).count();

        //cout<<"The time taken for final sorting is "<<duration <<" microseconds"<<endl;

        //cout<<"Recursion number "<<level<<endl;
        //print();
#pragma omp parallel for
        for (int i = 0; i < buckets; i++)  {
            int begin=position1[i];
            int ending;
            if(i!=buckets-1)
                ending=position1[i+1]-1;
            else
                ending=last;
            Sort(begin,ending,level-1);
        }
    }
    void sorting()
    {
        //cout<<lvls;
        //vector<Points<T>> arr1(n);
        this->Sort(0,n-1,lvls);
        print1();
    }
    void transfer()
    {
        if(check())
        {
            cout<<endl<<"Sorted list is in arr."<<endl;
        } else if(check1())
        {
            cout<<endl<<"Sorted list is in arr1."<<endl;
            #pragma omp parallel for
            for (int i = 0; i < arr1.size(); i++) {
                arr[i]=arr1[i];
            }
        }
    }
    bool check()
    {
        for (int i = 0; i < arr.size()-1; i++) {
            if (arr[i] > arr[i + 1]) {
                cout << "arr Sorted wrongly\n";
                return false;
            }
        }
        cout << "arr sorted correctly\n";
        return true;
    }
    bool check1()
    {
        for (int i = 0; i < arr1.size()-1; i++) {
            if (arr1[i] > arr1[i + 1]) {
                cout << "arr1 Sorted wrongly\n";
                return false;
            }
        }
        cout << "arr1 sorted correctly\n";
        return true;
    }
};

#endif //UNTITLED_RADIXSORT_H

/*
 * Algorithm Sort(pointslist,start,end,level)
 * base case: If number of elements in pointslist (end-start+1) is less than or equal to 1,
 *            If  level<=1
 *                  return .point
 *maintain a count array for number of elements in different buckets (8 buckets here) and initialize them to 0
 * for each element in pointslist
 *        Copy element into a temporary array of size = number of elements in pointslist/bucket
 *        Right shift x,y,z by level-1
 *        x,y,z&1
 *        y<<1
 *        z<<2
 *        bucketnum=x+y+z
 *        Increment count of corresponding bucket by 1.
 * Create a position list by accumulatively adding counts in count array
 * Maintain a copy of this position array which will be used later for passing the start and end positions to next iteration
 * for each element in templist
 *        Right shift x,y,z by level-1
 *        x,y,z&1
 *        y<<1
 *        z<<2
 *        bucketnum=x+y+z
 *        Place this element at positionlist[position[bucketnum]]
 *        Increment the position value of the bucket by 1.
 * for each i=0 to 7 do
 *        start=position[i]
 *        end=position[i+1]-1
 *        Sort(pointslist, start,end,level-1)
 *
 */