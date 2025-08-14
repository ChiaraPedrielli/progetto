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
    double x,y,r1,r2,L,angle;

    std::cout << "Inserire l'ordinata della pallina e l'angolo di lancio in radianti: i valori concessi sono da -1.55 a +1,55\n";
    std::cin >> y >> angle;
    ball.move_to({x,y});
    ball.set_angle(angle);
    
    
    std::cout << "Si definisce ora il bordo superiore del biliardo. \n Inserire in ordine: ordinata estremo sinistro, ascissa estremo destro e ordinata estremo destro\n";
    std::cin >> r1 >> L >> r2; 
    b1.move_border(r1, r2, L, ball);

    std::cout << "Si definisce ora il bordo inferiore del biliardo. \n Inserire in ordine: ordinata estremo sinistro, ascissa estremo destro e ordinata estremo destro\n";
    std::cin >> r1 >> r2 >> L;
    b2.move_border(r1,r2,L, ball);
    pf::Border::check_borders(b1,b2,ball);
    
    std::cout << "Si procede al lancio della pallina\n";

    pf::Result end = pf::Result::BallSimulation(b1,b2,ball);
    std::cout << "La posizione finale della pallina è la seguente: ("<<end.result.coordba().x<<","<<end.result.coordba().y<<") e sono stati eseguiti "<<end.bounces<<" rimbalzi\n";
    //forse sarebbe bello dire anche quanti rimbalzi ha fatto
   

   




   }catch (std::exception const& e){
    std::cerr << "Eccezione catturata : '"<< e.what() << "'\n";
    return EXIT_FAILURE;
   }catch(...){
    std::cerr << "Caught unknown exception\n";
    return EXIT_FAILURE;
   }
  


}
  