#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "biliardo.hpp"
#include "doctest.h"

//test da fare: 
// 1) che le rette non siano verticali
// 2) che la palla sia tra le due rette e non fuori

TEST_CASE("Testing BallSimulation"){
    

    

}

TEST_CASE("Testing move_borders"){

    pf::Ball ball({0,0},0);
    pf::Border b1(0,0,1);
    pf::Border b2(0,0,1);
    pf::Result end(0,ball);


   SUBCASE("Vertical border blocks the ball"){
        ball.move_to({0, 30});
        CHECK_THROWS(b1.move_border(90, 20, 0, ball)); //r1, r2, L
    }

    SUBCASE ("Vertical wall doesn't block the ball"){
        ball.move_to({0,30});
        ball.set_angle(0);
        b1.move_border(80, 60, 0, ball);
        b2.move_border(5, 28, 0, ball);
        end = pf::Result::BallSimulation(b1,b2,ball);
        CHECK(end.result.coordba().x == 1500);
        CHECK(end.result.coordba().y == 30);
    }
}

TEST_CASE("Testing set_angles"){

    pf::Ball ball({0,0},0);
    pf::Border b1(0,0,1);
    pf::Border b2(0,0,1);
    pf::Result end(0,ball);

    CHECK_THROWS(ball.set_angle(1.57));


   
}

TEST_CASE("Testing check_borders"){

    pf::Ball ball({0,0},0);
    pf::Border b1(0,0,1);
    pf::Border b2(0,0,1);
    pf::Result end(0,ball);

    SUBCASE("Switched borders"){}


   
}
