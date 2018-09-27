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
    T x,y;
    Points()
    {
        this->x=0;
        this->y=0;
    }
    Points(T x,T y)
    {
        this->x=x;
        this->y=y;
    }
    void setX(T val)
    {
        x = val;
    }
    T getX()
    {
        return x;
    }
    T getY()
    {
        return y;
    }
    bool operator <(Points<T> p2)
    {
        if (x < p2.x)
            return true;
        else if(x > p2.x)
            return false;
        else if(y < p2.y)
            return true;
        else
            return false;
    }//(5,7),(2,9),(9,3),(7,9),(4,3),(7,0)
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
