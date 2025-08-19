#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "biliardo.hpp"
#include "doctest.h"

// test da fare:
//  1) che le rette non siano verticali
//  2) che la palla sia tra le due rette e non fuori
TEST_CASE("Testing BallSimulation") {
  pf::Ball ball({0, 0}, 0);
  pf::Border b1(0, 0, 1);
  pf::Border b2(0, 0, 1);
  pf::Result end(0, ball);

  SUBCASE("Throw") {
    ball.move_to({0, 300});
    ball.set_angle(0.785398);
    b1.move_border(550, 550, 800);
    b2.move_border(150, 150, 800);
    end = pf::Result::BallSimulation(b1, b2, ball);
    CHECK(end.bounces == 2);
  }

  SUBCASE("Ball comes back") {
    ball.move_to({0, 300});
    ball.set_angle(1.54);
    b1.move_border(600, 305, 40);
    b2.move_border(0, 295, 40);
    CHECK_THROWS(pf::Result::BallSimulation(b1, b2, ball));
  }

  //la palla entra correttamente, con un angolo di 30 gradi ma poi a un certo punto torna indietro
  SUBCASE("Ball enters cooreclty but then comes back") {
    ball.move_to({0, 310});
    ball.set_angle(0.524);
    b1.move_border(320, 308, 50);
    b2.move_border(280, 292, 50);
    CHECK_THROWS(pf::Result::BallSimulation(b1, b2, ball));
  }


  //la palla entra correttamente ed esce subito senza toccare i bordi
  SUBCASE("Ball comes out without touching the borders") {
    ball.move_to({0, 310});
    ball.set_angle(0.524);
    b1.move_border(400, 385, 40);
    b2.move_border(200, 215, 40);
    pf::Result res = pf::Result::BallSimulation(b1, b2, ball);
    CHECK(res.result.coordba().y == doctest::Approx(333.12).epsilon(0.05));
    CHECK(res.result.d() == doctest::Approx(0.524).epsilon(0.05));
    CHECK(res.bounces == doctest::Approx(0.).epsilon(0.05));
  }


  //la palla entra correttamente ed esce dopo 2 rimbalzi, il primo con il bordo inferiore
  SUBCASE("Ball comes out after two bounces, the first with the lower border") {
    ball.move_to({0, 340});
    ball.set_angle(0.785);
    b1.move_border(400, 385, 50);
    b2.move_border(200, 215, 50);
    pf::Result res = pf::Result::BallSimulation(b1, b2, ball);
    CHECK(res.result.coordba().y == doctest::Approx(-307.889).epsilon(0.05));
    CHECK(res.bounces == doctest::Approx(2.).epsilon(0.05));
    CHECK(res.result.d()== doctest::Approx(-1.507).epsilon(0.05));
  }


  //la palla entra correttamente ed esce dopo un rimbalzo
  SUBCASE("Ball comes out after one bounce") {
    ball.move_to({0, 330});
    ball.set_angle(-1.100);
    b1.move_border(380, 370, 98);
    b2.move_border(220, 230, 98);
    pf::Result res = pf::Result::BallSimulation(b1, b2, ball);
    CHECK(res.result.coordba().y == doctest::Approx(367.59).epsilon(0.05));
    CHECK(res.bounces == doctest::Approx(2.).epsilon(0.05));
    CHECK(res.result.d()== doctest::Approx(-1.3679).epsilon(0.05));
  }



}

TEST_CASE("Testing initial_checks") {

  pf::Ball ball({0, 0}, 0);
  pf::Border b1(0, 0, 1);
  pf::Border b2(0, 0, 1);
  pf::Result end(0, ball);

  //ANGOLO CHE ANDREBBE ALL'INDIETRO (MAGGIORE DI 90 GRADI)
  SUBCASE("Not accepetd angle") {
    ball.set_angle(1.57);

    CHECK_THROWS(pf::Border::initial_checks(b1, b2, ball));
  }

  SUBCASE("Ball outside borders") {
    ball.move_to({0, 578});
    b1.move_border(480, 580, 1409);
    b2.move_border(120, 20, 1409);
    CHECK_THROWS(pf::Border::initial_checks(b1, b2, ball));
  }

  SUBCASE("Crossed borders") {
    ball.move_to({0, 347});
    b1.move_border(560, 241, 1400);
    b2.move_border(40, 359, 1400);
    CHECK_THROWS(pf::Border::initial_checks(b1, b2, ball));
  }

  SUBCASE("Vertical borders obstruct ball") {
    ball.move_to({0, 460});
    b1.move_border(570, 305, 0);
    b2.move_border(30, 295, 0);
    CHECK_THROWS(pf::Border::initial_checks(b1, b2, ball));
  }
}
