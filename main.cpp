//qua ci vuole il corpo della funzione: utile mettere tutto in try catch per gestire le eccezioni

#include <exception>
#include <iostream>
#include <random>
#include "biliardo.hpp"

int main(){
   try{

    pf::Ball ball({0,0},0);
    pf::Border b1(0,0,1);
    pf::Border b2(0,0,1);
    double x,y,r1_,r2_,L_,angle;

    std::cout << "Inserire l'ordinata della pallina e l'angolo di lancio in radianti: i valori consessi sono da -pi/2 a +pi/2";
    std::cin >> x >> y >> angle;
    ball.move_to({x,y});
    ball.set_angle(angle);
    std::cout << "Si definisce ora il bordo superiore del biliardo. \n Inserire in ordine: ordinata estremo sinistro, ascissa estremo destro e ordinata estremo destro";
    std::cin >> r1_ >> r2_ >> L_;
    b1.modify_r1(r1_);
    b1.modify_r2(r2_);
    b1.modify_L(L_);
    std::cout << "Si definisce ora il bordo inferiore del biliardo. \n Inserire in ordine: ordinata estremo sinistro, ascissa estremo destro e ordinata estremo destro";
    std::cin >> r1_ >> r2_ >> L_;
    b2.modify_r1(r1_);
    b2.modify_r2(r2_);
    b2.modify_L(L_);
    std::cout << "Si procede al lancio della pallina";

    /*pf::Ball result = BallSimulation(b1,b2,ball);*/
    std::cout << "La posizione finale della pallina è la seguente: ("<<ball.coordba().x<<","<<ball.coordba().y<<")\n";
    //forse sarebbe bello dire anche quanti rimbalzi ha fatto
   

   




   }catch (std::exception const& e){
    std::cerr << "Caught exception : '"<< e.what() << "'\n";
    return EXIT_FAILURE;
   }catch(...){
    std::cerr << "Caught unknown exception\n";
    return EXIT_FAILURE;
   }
  


}
  