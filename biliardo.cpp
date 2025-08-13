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

    

    const CollisionResult Border::next_collision(Ball const& b) {
       double s = std::tan(b.d()); //dobbiamo scrivere cosa fare se tan=0?
        double x_up = (((r1_) + s*((b.coordba()).x) - (b.coordba()).y) / (s - slopeup_));

        if (x_up > 0) {
            if (x_up <= L_) {
            bool hit = true;
            bool up = true; 
            double y_up = r1_ + (slopeup_)*x_up;

            return CollisionResult{true, ball, true};
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

    double Border::r1() const {return r1_;};
    double Border::r2() const {return r2_;};
    double Border::L() const {return L_;};
    double Border::slopeup() const {return slopeup_;};

    

    void Bounce(Border& r, Ball& b );
}
