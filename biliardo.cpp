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

    void Ball::set_angle(double new_s){
        d_ = new_s; //ho messo double perchè il metodo NewSlope che calcola il nuovo angolo restituisce un double, se vediamo che è meglio un altro tipo lo modifichiamo
    }

    

    const CollisionResult Border::next_collision(Ball const& b) {
       double s = std::tan(b.d()); //dobbiamo scrivere cosa fare se tan=0?
        double x_up = (((r1_) + s*((b.coordba()).x) - (b.coordba()).y) / (s - slopeup_));

        if (x_up > 0) {
            if (x_up <= L_) {
            double y_up = r1_ + (slopeup_)*x_up;
            pf::Point p {x_up, y_up};
            pf::Ball ball {p,s}; //la s è ancora quella iniziale, va fatto il metodo per cambiare l'angolo

            return CollisionResult{true, Ball({x_up,y_up},s), true};
            }

            else {
                return CollisionResult{false, Ball({L_, s*(L_-(b.coordba()).x)+(b.coordba()).y},std::atan(s)), false}; //mi ha consigliatpo chat di mettere i valori di hit e upper direttamente qui tanto i loro valori non servono all'interno di questa funzione
            }
        }

        else {
           double x_down = ((-(r1_) + s*((b.coordba()).x) - (b.coordba()).y) / (s + slopeup_));

            if (x_down <= L_) { 
            double y_down = -r1_ - (slopeup_)*x_down;
            return CollisionResult{true, Ball({x_down,y_down},s), true};
            }

            else {
                //vuol dire che la palla è uscita direttamente senza urtare
                return CollisionResult{false, Ball({L_, s*(L_-(b.coordba()).x)+(b.coordba()).y}, std::atan(s)), false};
            }
        }

    }

    const double Border::NewAngle(CollisionResult const& cr, Ball const& b){
        if (!cr.hit) return b.d_;
        int a = (cr.upper) ? 1: -1;
        return std::atan(a*L_/(r2_ - r1_));
        //modificherei ball con i nuovi xi e yi nel metodo simulazione di una particella completa
    }

    double Border::r1() const {return r1_;};
    double Border::r2() const {return r2_;};
    double Border::L() const {return L_;};
    double Border::slopeup() const {return slopeup_;};

    

    //void Bounce(Border& r, Ball& b ); non lo avevo visto, credo si aquello che ho scritto sotto

    Ball BallSimualtion (const Border& b1, const Border& b2, Ball& b){
     for (int bounce = 0; bounce < 1000000; ++bounce){
      CollisionResult res =next_collision(b);
      if(!res.hit){
         b.move_to(res.ball.coordba_);
        return b; //dovrei aver messo nel metodo Collision il calcolo della retta della pallina che esce senza rimbalzare
      }else{
        if(!res.upper){
           b.move_to(res.ball.coordba_);
           b.set_angle(NewAngle (b2)); //ricordiamoci che b2 è quello sotto
        }else{
          b.move_to(res.ball.coordba_);
          b.set_angle(NewAngle (b1));
        }
         
      }
      
     }
   }
}
