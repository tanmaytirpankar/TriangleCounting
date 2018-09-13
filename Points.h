//
// Created by tanmay on 29/5/18.
//

#ifndef UNTITLED_POINTS_H
#define UNTITLED_POINTS_H


//
// Created by tanmay on 29/5/18.
//
template <class T>
class Points{

public:
    T x,y,z;
    int choice;
    Points()
    {
        this->x=0;
        this->y=0;
        this->z=0;
    }
    Points(T x,T y, T z)
    {
        this->x=x;
        this->y=y;
        this->z=z;
    }
    T getX()
    {
        return x;
    }
    T getY()
    {
        return y;
    }
    T getZ()
    {
        return z;
    }
    bool operator <(Points<T> p2)
    {
        if(choice == 1) {
            if (x < p2.x)
                return true;
            else
                return false;
        }
        else if(choice == 2) {
            if (y < p2.y)
                return true;
            else
                return false;
        }
        else if(choice == 3) {
            if (z < p2.z)
                return true;
            else
                return false;
        }
    }
    bool operator >(Points<T> p2)
    {
//        if(choice == 1){
//            if(x>p2.x)
//                return true;
//            else
//                return false;
//        }
//        else if(choice == 2){
//            if(y>p2.y)
//                return true;
//            else
//                return false;
//        }
//        else if(choice == 3){
//            if(z>p2.z)
//                return true;
//            else
//                return false;
//        }
        if(x>p2.x)
            return true;
        else if(x<p2.x)
            return false;
        else if(y>p2.y)
            return true;
        else
            return false;
    }
};


#endif //UNTITLED_POINTS_H
