// implementazioni dei metodi
#include "biliardo.hpp"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <stdexcept>

// c'erano dei nomi che non si possono dare ai namespace
namespace pf {

// overload operatore == tra due punti
bool operator==(Point const &lhs, Point const &rhs) {
  return lhs.x == rhs.x && lhs.y == rhs.y;
}

// metodi getter per poter accedere alle variabili membro di Ball:
const Point &Ball::coordba() const { return coordba_; };
double Ball::d() const { return d_; };

void Ball::move_to(Point new_point) { coordba_ = new_point; };

void Ball::set_angle(double new_s) {

  d_ = new_s; // ho messo double perchè il metodo NewSlope che calcola il nuovo
              // angolo restituisce un double, se vediamo che è meglio un altro
              // tipo lo modifichiamo
}

const CollisionResult Border::next_collision(Ball &b, Border &b1, Border &b2) {
  double s = std::tan(b.d());
  double angle = b.d();

  double x_up = (((b1.r1()) + s * ((b.coordba()).x) - (b.coordba()).y) /
                 (s - b1.slopeup()));

  // gestione brodi verticali
  if (b1.L() == 0) {
    return CollisionResult{
        false,
        //Ball({b1.L(), s * (b1.L() - (b.coordba()).x) + b.coordba().y},
        //     std::atan(s)),
        Ball({b1.L(), s * (b1.L() - (b.coordba()).x) + b.coordba().y},
             angle),
        false};
  }

  // gestione palla e bordo paralleli (FORSE QUESTA VA TOLTA)
  if (s == b1.slopeup()) {
    return CollisionResult{
        false,
        Ball({b1.L(), s * (b1.L() - (b.coordba()).x) + b.coordba().y},
             /*std::atan(s)*/angle),
        false};
  }

  if (x_up > b.coordba().x) {

    if (x_up <= b1.L()) {
      double y_up = b1.r1() + (b1.slopeup()) * x_up;

      return CollisionResult{true, Ball({x_up, y_up}, /*std::atan(s)*/angle), true};
    } else {
      return CollisionResult{
          false,
          Ball({b1.L(), s * (b1.L() - (b.coordba()).x) + b.coordba().y},
               /*std::atan(s)*/angle),
          true};
    }
  } else {
    double x_down = ((b2.r1()) + s * ((b.coordba()).x) - (b.coordba()).y) /
                    (s - b2.slopeup());
    if (x_down <= b2.L() && x_down > b.coordba().x) {
      double y_down = b2.r1() + (b2.slopeup()) * x_down;
      return CollisionResult{true, Ball({x_down, y_down}, /*std::atan(s)*/angle), false};
    } else {
      return CollisionResult{
          false,
          Ball({b1.L(), s * (b1.L() - (b.coordba()).x) + b.coordba().y},
               /*std::atan(s)*/angle),
          false};
    }
  }
}

//IL METODO NEW ANGLE ORA E' MODIFICATO: NON RESITUISCE PIU L'ANGOLO MA LA SLOPE,
//SE POI CI SERVE L'ANGOLO FACCIAMO L'ARCOTANGENTE DELLA SLOPE DOPO AVER CHIAMATO IL METODO
//(IN BALL SIMULATION ORA E' COSI)
double Border::NewAngle(CollisionResult const &cr, Border &b1) {
  
  double mb = (cr.upper)? ((b1.r2()-b1.r1())/b1.L()) : (-(b1.r2()-b1.r1())/b1.L());
  double mp = std::tan(cr.hit.d());
  double new_slope = ((2*mb-(1-mb*mb)*mp)/(1-mb*mb+2*mb*mp));
  /*if (std::abs(1 - mb*mb + 2*mb*mp) < 1e-6)
    throw std::runtime_error("Rimbalzo non calcolabile: divisione per zero"); LO PROPONE CHAT, può capitare che si annulli se tipo mb = -1 e mp = 1*/
  return new_slope;
  // modificherei ball con i nuovi xi e yi nel metodo simulazione di una
  // particella completa
}

double Border::r1() const { return r1_; }
double Border::r2() const { return r2_; }
double Border::L() const { return L_; }
double Border::slopeup() const { return slopeup_; }
// non so se servono effettivamente

void Border::move_border(double r1, double r2, double L) {
  // forse si può fare in maniera più intelligente usando la slope
  r1_ = r1;
  r2_ = r2;
  L_ = L;
  slopeup_ = (r2_ - r1_) / L_;
}

void Border::initial_checks(Border const &b1, Border const &b2,
                            Ball const &ball) {
  double b1r1 = b1.r1();
  double b1r2 = b1.r2();
  double b2r1 = b2.r1();
  double b2r2 = b2.r2();
  double b1L = b1.L();
  // double x = ball.coordba().x;
  double y = ball.coordba().y;

  // la palla non è tra i due bordi
  if (b1r1 <= y || b2r1 >= y) {
    throw std::runtime_error("La palla deve trovarsi tra i due bordi: è "
                             "necessario cambiare le coordinate dei bordi.\n");
  }

  // i bordi si incrociano R2 GIACE ESATTAMENTE SULL'ASSE X
  if ((b1r2 == 300)) {
    throw std::runtime_error(
        "I bordi del biliardo si incrociano: è impossibile realizzare il "
        "lancio. \n");
  }

  // i bordi sono verticali e coprono la pallina
  if (b1L == 0 && (b1r2 <= y || b2r2 >= y)) {
    throw std::runtime_error("Configurazione dei bordi non valida, non è "
                             "possibile effettuare il lancio.\n");
  }

  // angolo di tiro
  if (ball.d() < -1.55 || ball.d() > 1.55) {
    throw std::runtime_error(
        "Errore: valore dell'angolo di lancio non valido.\n");
  }
}

void Border::set_r1(double val) { r1_ = val; }
void Border::set_r2(double val) { r2_ = val; }
void Border::set_L(double val) { L_ = val; }
void Border::set_slopeup(double val) { slopeup_ = val; }


/*Result Result::BallSimulation(Border &b1, Border &b2, Ball &b) {
  int bounce{0};

  for (int i = 0; i <= 1000000; i++) {

    CollisionResult res = pf::Border::next_collision(b, b1, b2);
    if (res.has_hit == false) {
      b.move_to(res.hit.coordba());
      return Result(bounce, b);

    } else {
      b.move_to(res.hit.coordba());
      b.set_angle(std::atan(pf::Border::NewAngle(res, b1)));

      if (pf::Border::NewAngle(res, b1) < 0 && res.upper == false) {
        throw std::runtime_error(
            "Per la dinamica del sistema la pallina è tornata indietro.\n");
          }
      
      if (pf::Border::NewAngle(res, b1) > 0 && res.upper == true) {
        throw std::runtime_error(
            "Per la dinamica del sistema la pallina è tornata indietro.\n");
          }

      ++bounce;
    }

  }

  std::cout
      << "E' stato raggiunto il numero massimo di rimbalzi della pallina.\n";
  return Result(1000000, b);
}*/

Result Result::BallSimulation(Border &b1, Border &b2,
                              Ball &b) {
  int bounce{0};
  std::vector<Ball> trajectory;

  for (int i = 0; i <= 1000000; i++) {
  
    

    CollisionResult res = pf::Border::next_collision(b, b1, b2);
    if (res.has_hit == false) {
      b.move_to(res.hit.coordba());
      trajectory.push_back(b);
      return Result(bounce, b, trajectory);
      
    } else {
      b.move_to(res.hit.coordba());
      b.set_angle(std::atan(pf::Border::NewAngle(res, b1)));
      trajectory.push_back(b);

      if (pf::Border::NewAngle(res, b1) < 0 && res.upper == false) {
        throw std::runtime_error(
            "Per la dinamica del sistema la pallina è tornata indietro.\n");
          }
      
      if (pf::Border::NewAngle(res, b1) > 0 && res.upper == true) {
        throw std::runtime_error(
            "Per la dinamica del sistema la pallina è tornata indietro.\n");
          }

      ++bounce;
    }
  }

  std::cout
      << "E' stato raggiunto il numero massimo di rimbalzi della pallina.\n";
  return Result(1000000, b, trajectory);
}

} // namespace pf
