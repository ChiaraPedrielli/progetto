#include "biliardo.hpp"
#include <cmath>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <cassert>

namespace pf {

//Getter methods
const Point &Ball::coordba() const { return coordba_; };
double Ball::d() const { return d_; };

void Ball::move_to(const Point& new_point) { coordba_ = new_point; };

void Ball::set_angle(double new_s) {
  d_ = new_s; 
}


CollisionResult Border::next_collision(const Ball &b, const Border &b1, const Border &b2) {
  double s = std::tan(b.d());
  double angle = b.d();
  const double EPS = 1e-9;

  assert(b.coordba().x >= 0.0 && b.coordba().x <= b1.L() && "next_collision: ball.x out of expected range");

  //Denominator = 0 (ball trajectory and the upper border are parallel)
  if (std::fabs(s - b1.slopeup()) < EPS) {
    double x_down = ((b2.r1()) + s * ((b.coordba()).x) - (b.coordba()).y) /
                    (s - b2.slopeup());
    if (x_down <= b2.L() && x_down > b.coordba().x ) {
      double y_down = b2.r1() + (b2.slopeup()) * x_down;
      return CollisionResult{true, Ball({x_down, y_down}, angle), false};
    } else {
      return CollisionResult{
          false,
          Ball({b1.L(), s * (b1.L() - (b.coordba()).x) + b.coordba().y},
               angle),
          false};
    }
  }

  //Denominator = 0 (ball trajectory and the lower border are parallel)
  if (std::fabs(s - b2.slopeup()) < EPS) {
    
    double x_up = (((b1.r1()) + s * ((b.coordba()).x) - (b.coordba()).y) /
                 (s - b1.slopeup()));
    if (x_up > b.coordba().x) {

    if (x_up <= b1.L()) {
      double y_up = b1.r1() + (b1.slopeup()) * x_up;
      return CollisionResult{true, Ball({x_up, y_up}, angle), true};
    } else {
      return CollisionResult{
          false,
          Ball({b1.L(), s * (b1.L() - (b.coordba()).x) + b.coordba().y},
               angle),
          true};
    }
  }
  }


  double x_up = (((b1.r1()) + s * ((b.coordba()).x) - (b.coordba()).y) /
                 (s - b1.slopeup()));

  //Vertical borders management
  if (b1.L() == 0) {
    return CollisionResult{
        false,
        Ball({b1.L(), s * (b1.L() - (b.coordba()).x) + b.coordba().y},
             angle),
        false};
  }

  //IN QUESTO METODO E' STATO SOSTITUITO  std::atan(s) CON ANGLE IN MODO DA NON FARE TAN E ATAN DUE VOLTE. 

    if (x_up > b.coordba().x) {

    if (x_up <= b1.L()) {
      double y_up = b1.r1() + (b1.slopeup()) * x_up;
      return CollisionResult{true, Ball({x_up, y_up}, angle), true};
    } else {
      return CollisionResult{
          false,
          Ball({b1.L(), s * (b1.L() - (b.coordba()).x) + b.coordba().y},
               angle),
          true};
    }
  } else {
    double x_down = ((b2.r1()) + s * ((b.coordba()).x) - (b.coordba()).y) /
                    (s - b2.slopeup());
    if (x_down <= b2.L() && x_down > b.coordba().x ) {
      double y_down = b2.r1() + (b2.slopeup()) * x_down;
      return CollisionResult{true, Ball({x_down, y_down}, angle), false};
    } else {
      return CollisionResult{
          false,
          Ball({b1.L(), s * (b1.L() - (b.coordba()).x) + b.coordba().y},
               angle),
           false};
    }
  }
}

//Returns the new slope of the straight line after the collision
double Border::NewAngle(CollisionResult const &cr, const Border &b1) {

  assert(b1.L() > 0 && "NewAngle: b1.L must be > 0 (initial_checks should have enforced this)");
  
  double mb = (cr.upper)? ((b1.r2()-b1.r1())/b1.L()) : (-(b1.r2()-b1.r1())/b1.L());
  double mp = std::tan(cr.hit.d());

  if (std::abs(1 - mb*mb + 2*mb*mp) < 1e-6) {
    throw std::runtime_error("Uncalculable bounce: division by zero");
  }

  double new_slope = ((2*mb-(1-mb*mb)*mp)/(1-mb*mb+2*mb*mp));
  assert(std::isfinite(new_slope) && "NewAngle: produced non-finite slope");
  return new_slope;
}


double Border::r1() const { return r1_; }
double Border::r2() const { return r2_; }
double Border::L() const { return L_; }
double Border::slopeup() const { return slopeup_; }

void Border::move_border(double r1, double r2, double L) {
  r1_ = r1;
  r2_ = r2;
  L_ = L;
  if (L_ != 0.0) {slopeup_ = (r2_ - r1_) / L_;} 
  else { slopeup_ = 0.0;}
}

void Border::initial_checks(Border const &b1, Border const &b2,
                            Ball const &ball) {
  double b1r1 = b1.r1();
  double b1r2 = b1.r2();
  double b2r1 = b2.r1();
  double b2r2 = b2.r2();
  double b1L = b1.L();
  //double x = ball.coordba().x;        QUESTO PER ORA NON LO USIAMO PERO PUO SERVIRE PER L'ASSERT
  double y = ball.coordba().y;

  if (b1r1 <= y || b2r1 >= y) {
    throw std::runtime_error("Invalid initial setup: The ball must be between the borders. \n");
  }

  if ((b1r2 == 300)) {
    throw std::runtime_error(
        "Invalid initial setup: the borders intersect \n");
  }

  if (b1L == 0 && (b1r2 <= y || b2r2 >= y)) {
    throw std::runtime_error("invalid initial stup: the borders are vertical and obstruct the ball \n");
  }
  
  if (ball.d() < -1.55 || ball.d() > 1.55) {
    throw std::runtime_error(
        "The entered value of the launch angle is not valid for billiard geometry\n");
  }

  if (b1L <= 0 ) {
    throw std::runtime_error(
        "The value of the inserted L parameter is invalid.\n");
  }
  }

void Border::set_r1(double val) { r1_ = val; }
void Border::set_r2(double val) { r2_ = val; }
void Border::set_L(double val) { L_ = val; }
void Border::set_slopeup(double val) { slopeup_ = val; }
 

Result Result::BallSimulation(const Border &b1, const Border &b2,
                              Ball &b) {
  int bounce{0};
  std::vector<Ball> trajectory;
  //double x_old = b.coordba().x;
  //double ball_old_slope = std::tan(b.d());

  

  for (int i = 0; i <= 1000000; i++) {  
   
    
    CollisionResult res = pf::Border::next_collision(b, b1, b2);
    if (res.has_hit == false) {
      b.move_to(res.hit.coordba());
      trajectory.push_back(b);
      return Result(bounce, b, trajectory);
      return Result(bounce, b);
      
    } else {
      
      b.move_to(res.hit.coordba());
      b.set_angle(std::atan(pf::Border::NewAngle(res, b1)));
      trajectory.push_back(b);

      /*if (b.coordba().x < x_old) {
        throw std::runtime_error(
            "Due to the dynamics of the system the ball went back.\n");
          }*/

      if( res.upper == false && !(b2.slopeup()<0 && std::tan(b.d())>b2.slopeup())){

         if (pf::Border::NewAngle(res, b1) < 0 ) {
          ++bounce;
          // std::cout<<"rimbalzi: "<<bounce<<".\n";
        throw std::runtime_error(
            "Due to the dynamics of the system the ball went back after.\n");
          }
      }
     
      if( res.upper == true && !(b1.slopeup()>0 && std::tan(b.d())>b2.slopeup()))
      if (pf::Border::NewAngle(res, b1) > 0) {
        ++bounce;
          // std::cout<<"rimbalzi: "<<bounce<<".\n";
        throw std::runtime_error(
          
            "Due to the dynamics of the system the ball went back.\n");
          }

      ++bounce;
      //ball_old_slope = std::tan(b.d());
    }
  }

  std::cout
      << "The maximum number of bounces of the ball has been reached.\n";
  return Result(1000000, b, trajectory);
  return Result(bounce, b);
}

}
