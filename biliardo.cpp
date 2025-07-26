//implementazioni dei metodi
#include "biliardo.hpp"
#include <stdexcept>

//c'erano dei nomi che non si possono dare ai namespace
namespace pf{
    double Border::slope(Point const& p1, Point const& p2){
        if (p1.x == p2.x){
            throw std::runtime_error ("La retta è veticale");
        }else{return double ((p2.y-p1.y)/(p2.x-p1.x));}
        
    }

    void Bounce(Border& r, Ball& b );
}
