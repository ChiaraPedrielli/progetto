//implementazioni dei metodi
#include "biliardo.hpp"
#include <stdexcept>
#include <cmath>
#include <iostream>

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

    

    const CollisionResult Border::next_collision(Ball const& b, Border& b1, Border& b2) {
       double s = std::tan(b.d());
        double x_up = (((b1.r1()) + s*((b.coordba()).x) - (b.coordba()).y) / (s - b1.slopeup()));
        

        if (x_up > b.coordba().x) {
            if (x_up <= b1.L()) {
            double y_up = b1.r1() + (b1.slopeup())*x_up;
            pf::Point p {x_up, y_up};
            pf::Ball ball {p,s}; //la s è ancora quella iniziale, va fatto il metodo per cambiare l'angolo

            return CollisionResult{true, Ball({x_up,y_up},s), true};
            }

            else {
                return CollisionResult{false, Ball({b1.L(), s*(b1.L()-(b.coordba()).x)+b.coordba().y},std::atan(s)), false}; //mi ha consigliatpo chat di mettere i valori di hit e upper direttamente qui tanto i loro valori non servono all'interno di questa funzione
            }
        }

        else {
           double x_down = ((-(b2.r1()) + s*((b.coordba()).x) - (b.coordba()).y) / (s + b2.slopeup()));

            if (x_down <= b2.L()) { 
            double y_down = -b2.r1() - (b2.slopeup())*x_down;
            return CollisionResult{true, Ball({x_down,y_down},s), true};
            }

            else {
                //vuol dire che la palla è uscita direttamente senza urtare
                return CollisionResult{false, Ball({b1.L(), s*(b1.L()-b.coordba().x)+b.coordba().y}, std::atan(s)), false};
            }
        }

    }

    const double Border::NewAngle(CollisionResult const& cr){
        int a = (cr.upper) ? 1: -1;
        return std::atan(a*L_/(r2_ - r1_));
        //modificherei ball con i nuovi xi e yi nel metodo simulazione di una particella completa
    }

    double Border::r1() const {return r1_;}
    double Border::r2() const{return r2_;}
    double Border::L() const{return L_;}
    double Border::slopeup() const {return slopeup_;}
    //non so se servono effettivamente
    void Border::modify_r1(double r1){r1_ = r1;}
    void Border::modify_r2(double r2){r2_ = r2;}
    void Border::modify_L(double L){L_ = L;}
    void Border::modify_slopeup(double slopeup){slopeup_ = slopeup;}
     

    

    //void Bounce(Border& r, Ball& b ); non lo avevo visto, credo si aquello che ho scritto sotto

    Result Result::BallSimulation ( Border& b1, Border& b2, Ball& b){
        
       for (int bounce = 0; bounce < 1000000; ++bounce){
        CollisionResult res = pf::Border::next_collision(b,b1,b2);
        if(res.has_hit==false){
         b.move_to(res.hit.coordba());
         return Result(bounce, b); 
        }else{
            if (res.upper==false){
               b.move_to(res.hit.coordba());
               b.set_angle(b2.NewAngle(res));
            }else{
                b.move_to(res.hit.coordba());
                b.set_angle(b1.NewAngle(res));
            }
         
        }
      
      }
      std::cout << "E' stato raggiunto il numero massimo di rimbalzi della pallina: non è possibile determinare la posizione finale di questa traiettoria";
      return Result(1000000,b);
      //flag interno? messaggio d'errore?

   }
}
