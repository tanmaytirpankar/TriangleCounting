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
class RadixSort {
    vector<Points<T>> arr, arr1;
    vector<Points<T>> count;
    unsigned long num_threads;
    unsigned long n;
    int word_size;
    int lvls;
public:
    RadixSort(vector<Points<T>> *inparr, unsigned long n, unsigned long num, int word_size) {
        this->n = n;
        this->num_threads = num;
        this->word_size = word_size;
        vector<Points<T>> count(0);
        arr = arr1 = *inparr;
        lvls = (sizeof(arr[0].getX()) * 8) / word_size + 1;
//        print();
//        cout<<endl;
//        print1();
        /*cout<<"Enter Points";
        arr=new Points<T>[n];
        for (int i = 0; i < n; i++) {
            unsigned int x,y,z;
            cin>>x>>y>>z;
            arr[i]=Points<T>(x,y,z);
        }*/
    }

    void print() {
        for (int i = 0; i < n; i++) {
            int num_length = sizeof(arr[0].getX()) * 8;
            T x,y,z;
            cout << "(" << arr[i].getX() << "," << arr[i].getY() << "," << arr[i].getZ() << "),";
            for (int j = 0; j < num_length; j++) {
                x = arr[i].getX();
                x = x >> num_length - j - 1;
                cout << x % 2;
            }
            cout << ",";
            for (int j = 0; j < num_length; j++) {
                y = arr[i].getY();
                y = y >> num_length - j - 1;
                cout << y % 2;
            }
            cout << ",";
            for (int j = 0; j < num_length; j++) {
                z = arr[i].getZ();
                z = z >> num_length - j - 1;
                cout << z % 2;
            }
            cout << endl;
        }
    }

    void print1() {
        for (int i = 0; i < n; i++) {
            cout << "(" << arr[i].getX() << "," << arr[i].getY() << "," << arr[i].getZ() << "," << i <<"),";
        }
        cout << endl;
    }

    void Sort(unsigned long first, unsigned long last, int choice1, int level) {
        if (level == lvls) {
            for (unsigned long i = first; i <= last; i++) {
                arr[i].choice = arr1[i].choice = choice1;
            }
        }
        unsigned long num_elements = last - first + 1;

        if (num_elements <= 20 || level < 1) {

            if (level % 2 == 1 && num_elements > 0) {
                sort(&arr1[first], &arr1[last + 1]);
                for (unsigned long i = first; i <= last; i++) {
                    arr[i] = arr1[i];
                }
            } else if(num_elements > 0){
                sort(&arr[first], &arr[last + 1]);
                for (unsigned long i = first; i <= last; i++) {
                    arr1[i] = arr[i];
                }
            }
            return;
        }
        int buckets = int(pow(2, word_size));
        int choice = 0;
        vector<vector<unsigned long>> count(num_threads);
        vector<vector<unsigned long>> position(num_threads);
        for (int l = 0; l < num_threads; l++) {
            count[l] = vector<unsigned long>(buckets);
            position[l] = vector<unsigned long>(buckets);
        }
//        if(num_elements<=1 || level<1)
//        {
//            return ;
//        }

        unsigned long position1[buckets];
        //vector<T> temp(num_elements);
        chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();
#pragma omp parallel
        for (int j = 0; j < buckets; j++) {
            position1[j] = 0;
            count[omp_get_thread_num()][j] = 0;
            position[omp_get_thread_num()][j] = 0;
        }
        chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();

        //cout <<"The time taken for initialization is "<<duration <<" microseconds"<<endl;
        t1 = chrono::high_resolution_clock::now();
        int remainder = (sizeof(arr[0].getX()) * 8) % word_size;
        int shift = ((level - 1) * word_size) - (word_size - remainder);
        int mask = int(pow(2, word_size) - 1);
//        cout<<"first: "<<first<<" last: "<<last<<" sizeof(count): "<<count.size()<<" sizeof(count[0])"<<count[0].size()<<endl;
        if (level % 2 == 1) {
            if (choice1 == 1)
                choice = 1;
            else if (choice1 == 2)
                choice = 2;
            else if (choice1 == 3)
                choice = 3;
        } else if (level % 2 == 0) {
            if (choice1 == 1)
                choice = 4;
            else if (choice1 == 2)
                choice = 5;
            else if (choice1 == 3)
                choice = 6;
        }


        switch (choice) {
            case 1:
#pragma omp parallel for
                for (unsigned long i = first; i <= last; i++) {
                    T x;
                    x = arr1[i].getX();
                    if (level > 1)
                        x = x >> shift;
                    x = x & mask;
                    count[omp_get_thread_num()][x]++;
                }
                break;
            case 2:
#pragma omp parallel for
                for (unsigned long i = first; i <= last; i++) {
                    T x;
                    x = arr1[i].getY();
                    if (level > 1)
                        x = x >> shift;
                    x = x & mask;
                    count[omp_get_thread_num()][x]++;
                }
                break;
            case 3:
#pragma omp parallel for
                for (unsigned long i = first; i <= last; i++) {
                    T x;
                    x = arr1[i].getZ();
                    if (level > 1)
                        x = x >> shift;
                    x = x & mask;
                    count[omp_get_thread_num()][x]++;
                }
                break;
            case 4:
#pragma omp parallel for
                for (unsigned long i = first; i <= last; i++) {
                    T x;
                    x = arr[i].getX();
                    if (level > 1)
                        x = x >> shift;
                    x = x & mask;
                    count[omp_get_thread_num()][x]++;
                }
                break;
            case 5:
#pragma omp parallel for
                for (unsigned long i = first; i <= last; i++) {
                    T x;
                    x = arr[i].getY();
                    if (level > 1)
                        x = x >> shift;
                    x = x & mask;
                    count[omp_get_thread_num()][x]++;
                }
                break;
            case 6:
#pragma omp parallel for
                for (unsigned long i = first; i <= last; i++) {
                    T x;
                    x = arr[i].getZ();
                    if (level > 1)
                        x = x >> shift;
                    x = x & mask;
                    count[omp_get_thread_num()][x]++;
                }
                break;
            default:
                cout << "Invalid choice" << endl;
                break;
        }
        //cout<<endl<<"Shift is "<<shift<<endl;
        t2 = chrono::high_resolution_clock::now();
        duration = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();

        //cout<<"The time taken for calculating counts is "<<duration <<" microseconds"<<endl;
//        if(level > 0){
//        cout<<"Values in count:"<<endl;
//        for(int i = 0; i < num_threads; i++){
//            for(int j = 0; j < buckets; j++){
//                cout<<count[i][j]<<" ";
//            }
//            cout<<endl;
//        }}
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

        position1[0] = position[0][0] = first;
        for (int buc = 0; buc < buckets; buc++) {
            for (int tid = 1; tid < num_threads; tid++) {
                if (tid == 0 && buc == 0) {}
                else {
                    position[tid][buc] = position[tid - 1][buc] + count[tid - 1][buc];
                }
            }
            if (buc < buckets - 1)
                position1[buc + 1] = position[0][buc + 1] =
                        position[num_threads - 1][buc] + count[num_threads - 1][buc];
        }
//        if(level > 0){
//        cout<<"Positions in position at depth "<<level<<" is:"<<endl;
//        for(int i = 0; i < num_threads; i++){
//            for(int j = 0; j < buckets; j++){
//                cout<<position[i][j]<<" ";
//            }
//            cout<<endl;
//        }
//            for (int k = 0; k < buckets; ++k) {
//                cout<<position1[k]<<" ";
//            }
//            cout<<endl;}

        switch (choice) {
            case 1:
#pragma omp parallel for
                for (unsigned long i = first; i <= last; i++) {
                    T x;
                    x = arr1[i].getX();
                    if (level > 1)
                        x = x >> shift;
                    x = x & mask;
                    arr[position[omp_get_thread_num()][x]] = arr1[i];
                    position[omp_get_thread_num()][x]++;
                }
                break;
            case 2:
#pragma omp parallel for
                for (unsigned long i = first; i <= last; i++) {
                    T x;
                    x = arr1[i].getY();
                    if (level > 1)
                        x = x >> shift;
                    x = x & mask;
                    arr[position[omp_get_thread_num()][x]] = arr1[i];
                    position[omp_get_thread_num()][x]++;
                }
                break;
            case 3:
#pragma omp parallel for
                for (unsigned long i = first; i <= last; i++) {
                    T x;
                    x = arr1[i].getZ();
                    if (level > 1)
                        x = x >> shift;
                    x = x & mask;
                    arr[position[omp_get_thread_num()][x]] = arr1[i];
                    position[omp_get_thread_num()][x]++;
                }
                break;
            case 4:
#pragma omp parallel for
                for (unsigned long i = first; i <= last; i++) {
                    T x;
                    x = arr[i].getX();
                    if (level > 1)
                        x = x >> shift;
                    x = x & mask;
                    arr1[position[omp_get_thread_num()][x]] = arr[i];
                    position[omp_get_thread_num()][x]++;
                }
                break;
            case 5:
#pragma omp parallel for
                for (unsigned long i = first; i <= last; i++) {
                    T x;
                    x = arr[i].getY();
                    if (level > 1)
                        x = x >> shift;
                    x = x & mask;
                    arr1[position[omp_get_thread_num()][x]] = arr[i];
                    position[omp_get_thread_num()][x]++;
                }
                break;
            case 6:
#pragma omp parallel for
                for (unsigned long i = first; i <= last; i++) {
                    T x;
                    x = arr[i].getZ();
                    if (level > 1)
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
        duration = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();

        //cout<<"The time taken for final sorting is "<<duration <<" microseconds"<<endl;

        //cout<<"Recursion number "<<level<<endl;
        //print();
#pragma omp parallel for
        for (unsigned long i = 0; i < buckets; i++) {
            unsigned long begin = position1[i];
            unsigned long ending;
            if (i != buckets - 1)
                ending = position1[i + 1] - 1;
            else
                ending = last;
            Sort(begin, ending, choice1, level - 1);
        }
    }

    void transfer() {
        if (check()) {
            cout << endl << "Sorted list is in arr." << endl;
        } else if (check1()) {
            cout << endl << "Sorted list is in arr1." << endl;
#pragma omp parallel for
            for (int i = 0; i < arr1.size(); i++) {
                arr[i] = arr1[i];
            }
        }
    }

    bool check() {
        for (int i = 0; i < arr.size() - 1; i++) {
            if (arr[i] > arr[i + 1]) {
                cout << "arr Sorted wrongly\n";
                cout << i<<endl;
                return false;
            }
        }
        cout << "arr sorted correctly\n";
        return true;
    }

    bool check1() {
        for (int i = 0; i < arr1.size() - 1; i++) {
            if (arr1[i] > arr1[i + 1]) {
                cout << "arr1 Sorted wrongly\n";
                cout << i << endl;
                return false;
            }
        }
        cout << "arr1 sorted correctly\n";
        return true;
    }

    vector<Points<T>> degree_count(int choice) {
        T x = arr[0].getX();
        Points<T> temp(x, 0, 0);
        count.push_back(temp);
        unsigned long distinct_num_count = 1;
        switch (choice) {
            case 1:
                for (unsigned long i = 0; i < n; i++) {
                    if (arr[i].getX() != x) {
                        x = arr[i].getX();
                        Points<T> temp(x, 1, 0);
                        count.push_back(temp);
                        distinct_num_count++;
                    } else
                        count[distinct_num_count - 1].y++;
                }
                break;
            case 2:
                for (unsigned long i = 0; i < n; i++) {
                    if (arr[i].getY() != x) {
                        x = arr[i].getY();
                        Points<T> temp(x, 1, 0);
                        count.push_back(temp);
                        distinct_num_count++;
                    } else
                        count[distinct_num_count - 1].y++;
                }
                break;
            case 3:
                for (unsigned long i = 0; i < n; i++) {
                    if (arr[i].getZ() != x) {
                        x = arr[i].getZ();
                        Points<T> temp(x, 1, 0);
                        count.push_back(temp);
                        distinct_num_count++;
                    } else
                        count[distinct_num_count - 1].y++;
                }
                break;
            default:
                cout << "Invalid choice.\n";
        }
        for (unsigned long i = 1; i < distinct_num_count; i++) {
            count[i].z = count[i-1].z + count[i-1].y;
        }
        for (unsigned long j = 0; j < distinct_num_count; j++) {
            cout << "(" << count[j].getX() << "," << count[j].getY() << "," << count[j].getZ() << "),";
        }
        cout << endl;
        return count;
    }
    vector<Points<T>> getArr()
    {
        return arr;
    }
    void newmap(vector<Points<T>> orderedCount)
    {
        unsigned long k=0;
        unsigned long x = arr[0].getX();
        for (int i = 0; i < n; i++) {
            if(arr[i].getX() == x){
                arr[i].setX(orderedCount[k].getX());
            } else{
                k++;
                x = arr[i].getX();
                arr[i].setX(orderedCount[k].getX());
            }
        }
    }
};

#endif //UNTITLED_RADIXSORT_H

