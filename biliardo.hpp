//qua vengono dichiaarte le classi/struct con dento i rispettivi metodi, anche i metodi qui sono solo dichiarati,
//  la loro implementazione va in .cpp
//va tutto in un NAMESPACE

#ifndef PF_BILIARDO_HPP
#define PF_BILIARDO_HPP

namespace pf{

struct Point{ //per le coordinate
    double x;
    double y;
};

struct Border{
    Point coordbo1; //coordinate estremi rette
    Point coordbo2;

    double slope(Point const& p1, Point const& p2);

};

struct Ball{
    Point coordba;
    double d; //angolo di lancio

    void Bounce(Border& r, Ball& b );
};






}//fine namespace pf




#endif