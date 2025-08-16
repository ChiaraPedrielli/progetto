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

    

    const CollisionResult Border::next_collision(Ball & b, Border& b1, Border& b2) {
       double s = std::tan(b.d());
       double window_length = 1500;
        
       
       double x_up = (((b1.r1()) + s*((b.coordba()).x) - (b.coordba()).y) / (s - b1.slopeup()));
       

       //gestione brodi verticali
       if(b1.L()==0 && b1.r2()==0){
         return CollisionResult{false, Ball({window_length, s*(window_length-(b.coordba()).x)+b.coordba().y},std::atan(s)), false};
       }

       //gestione palla e bordo paralleli
       if(s == b1.slopeup()){
        return CollisionResult{false, Ball({window_length, s*(window_length-(b.coordba()).x)+b.coordba().y},std::atan(s)), false};
       }




       if(x_up >= 0){
           

           if(x_up < b.coordba().x){
            throw std::runtime_error("La pallina torna indietro e viene ignorata dal programma");
           }

          if (x_up <= b1.L()){
             double y_up = b1.r1() + (b1.slopeup())*x_up;

             return CollisionResult {true, Ball({x_up,y_up},std::atan(s)), true};
          }else{
             return CollisionResult{false, Ball({window_length, s*(window_length-(b.coordba()).x)+b.coordba().y},std::atan(s)), false};
       
          }
       }else{
          double x_down = ((b2.r1()) + s*((b.coordba()).x) - (b.coordba()).y) / (s - b2.slopeup());
          if(x_down <= b2.L()){
            double y_down = b2.r1() + (b2.slopeup())*x_down;
            return CollisionResult {true, Ball({x_down,y_down},std::atan(s)), false};
          }else{
            return CollisionResult{false, Ball({window_length, s*(window_length-(b.coordba()).x)+b.coordba().y},std::atan(s)), false};
          }
       }
       

       

    }

    double Border::NewAngle(CollisionResult const& cr, Border b1){
        double a = (cr.upper) ? -1: 1;
        return std::atan(a*b1.L()/(b1.r2() - b1.r1()));
        //modificherei ball con i nuovi xi e yi nel metodo simulazione di una particella completa
    }

    double Border::r1() const {return r1_;}
    double Border::r2() const{return r2_;}
    double Border::L() const{return L_;}
    double Border::slopeup() const {return slopeup_;}
    //non so se servono effettivamente

    void Border::move_border(double r1, double r2, double L){
         //forse si può fare in maniera più intelligente usando la slope
        r1_ = r1;
        r2_ = r2;
        L_ = L;
    }

    void Border::initial_checks(Border const b1, Border const b2, Ball const ball){
        double b1r1 = b1.r1();
        double b1r2 = b1.r2();
        double b2r1 = b2.r1();
        double b2r2 = b2.r2();
        double b1L = b1.L();
        //double x = ball.coordba().x;
        double y = ball.coordba().y;


        

        //la palla non è tra i due bordi
        if (b1r1 <= y || b2r1 >= y) {
            throw std::runtime_error("La palla deve trovarsi tra i due bordi: è necessario cambiare le coordinate dei bordi.\n");
        }

        //i bordi si incrociano
        if ((b1r1-300)*(b1r2-300) <= 0){
            throw std::runtime_error("I bordi del biliardo si incrociano: è impossibile realizzare il lancio. \n Le rodinate dei due estremi devono essere entrambe positive o entrambe negative.\n");
        }

        //i bordi sono verticali e coprono la pallina
        if(b1L == 0 && (b1r2 <= y || b2r2 >= y)){
            throw std::runtime_error("Configurazione dei bordi non valida, non è possibile effettuare il lancio.\n");
        }


        //angolo di tiro
        if( ball.d() < -1.55 || ball.d() > 1.55 ){
         throw std::runtime_error("Errore: valore dell'angolo di lancio non valido.\n");
        }
    }

    
    
     

    

    //void Bounce(Border& r, Ball& b ); non lo avevo visto, credo si aquello che ho scritto sotto

    Result Result::BallSimulation ( Border& b1, Border& b2, Ball& b){
        
        double bounce;

      for(bounce = 0; bounce <= 1000000; bounce++){
        if (bounce >= 1 && cos(b.d())<0){
            throw std::runtime_error("Per la dinamica del sistema la pallina è tornata indietro.\n");
        }




        CollisionResult res = pf::Border::next_collision(b,b1,b2);
        if(res.has_hit==false){
         b.move_to(res.hit.coordba());
         return Result(bounce, b); 
        }else{
            
            b.move_to(res.hit.coordba());
            b.set_angle(pf::Border::NewAngle(res,b1));
           
         
        }

      } 

        std::cout << "E' stato raggiunto il numero massimo di rimbalzi della pallina: non è possibile determinare la posizione finale di questa traiettoria\n";
        return Result(1000000,b);
      
    }
  
      

}

