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
        if(x<p2.x)
            return true;
        else if(x>p2.x)
            return false;
        else if(y<p2.y)
            return true;
        else
            return false;
    }
    bool operator >(Points<T> p2)
    {
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
