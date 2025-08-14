// qua vengono dichiaarte le classi/struct con dento i rispettivi metodi,
// anche i metodi qui sono solo dichiarati, la loro implementazione va in .cpp
// va tutto in un NAMESPACE

#ifndef PF_BILIARDO_HPP
#define PF_BILIARDO_HPP

namespace pf {

struct Point
{ // per le coordinate
  double x;
  double y;
};



// overload dell'operatore == se può servire per confrontare due punti
bool operator==(Point const&, Point const&);

//froward declaration per evitare problemi di ordine di dichiarazione
class Ball;
class Border;
struct CollisionResult;

class Ball
{
 private:
  Point coordba_{0., 0.};
  double d_{0.}; // angolo di lancio

 public:
  Ball(Point p, double t) : coordba_{p} , d_{t} {} // costruttore, forse si può inserire gia qui una exception se l'angolo o le coordinate non sono valide

  // metodi getter per poter accedere alle variabili membro:
  const Point& coordba() const;
  double d() const;

  // metodi per aggiornare la posizione e la pendenza della retta dopo ogni urto
  void move_to(Point new_point); //quindi poi una volta che calcoliamo la x dell'urto calcoliamo anche la y e poi convertiamo in un Point

  void Bounce(Border& r, Ball& b);
  void set_angle(double new_s);
  
  
 
};

struct CollisionResult {
    bool has_hit;
    Ball hit;
    bool upper;
};



class Border
{
 private:
  double r1_{0.}; // coordinate estremi rette
  double r2_{0.};
  double L_{0.};
  double slopeup_{0.};

  public:
  Border (double R1, double R2, double lenght) : r1_{R1}, r2_{R2}, L_{lenght}, slopeup_{(r2_ - r1_)/L_} {}
  
  //metodi getter
  double r1() const;
  double r2() const;
  double L() const;
  double slopeup() const;
  void move_border (double r1, double r2, double L, Ball ball);
  static void check_borders (Border const b1, Border const b2, Ball const ball);
  

  static const CollisionResult next_collision( Ball const& b, Border& b1, Border& b2);
  double NewAngle(CollisionResult const& cr);

  

 

};

struct Result {
  double bounces;
  Ball result;

  Result(int bo, const Ball& ba) : bounces(bo), result(ba) {}


  static Result BallSimulation ( Border& b1, Border& b2, Ball& b);
};



} // namespace pf

#endif