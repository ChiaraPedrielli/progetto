//implementazioni dei metodi
#include "biliardo.hpp"
#include <stdexcept>
#include <cmath>

//c'erano dei nomi che non si possono dare ai namespace
namespace pf{

    //overload operatore == tra due punti
    bool operator==(Point const& lhs, Point const& rhs) {
        return lhs.x == rhs.x && lhs.y == rhs.y;
    }

    //metodi getter per poter accedere alle variabili membro di Ball:
    const Point& Ball::coordba() const { return coordba_; };
    double Ball::d() const { return d_; };

    void Ball::move_to(Point new_point) {
        coordba_ = new_point;
    };

    /*
    double Border::slope(Point const& p1, Point const& p2){
        if (p1.x == p2.x){
            throw std::runtime_error ("La retta è veticale");
        }
        else {
            return double ((p2.y-p1.y)/(p2.x-p1.x));
        }
        
    }
    */

      const CollisionResult Border::next_collision(Ball const& b) {
        double s = std::tan(b.d());
        double x_up = (((r1_) + s*((b.coordba()).x) - (b.coordba()).y) / (s - slopeup_));

        if (x_up > 0) {
            if (x_up <= L_) {
            bool hit = true;
            bool up = true; 
            double y_up = r1_ + (slopeup_)*x_up;
            }

            else {
                //vuol dire che la palla è uscita direttamente senza urtare
                bool hit = false;
            }
        }

        else {
           double x_down = ((-(r1_) + s*((b.coordba()).x) - (b.coordba()).y) / (s + slopeup_));

            if (x_up <= L_) {
            bool hit = true;
            bool up = false; 
            double y_down = -r1_ - (slopeup_)*x_down;
            }

            else {
                //vuol dire che la palla è uscita direttamente senza urtare
                bool hit = false;
            }
        }

     }

    

    void Bounce(Border& r, Ball& b );
}
