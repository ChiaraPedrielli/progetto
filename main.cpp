//qua ci vuole il corpo della funzione: utile mettere tutto in try catch per gestire le eccezioni

#include <exception>
#include <iostream>
#include <random>
#include "biliardo.hpp"
/*
int main(){
   try{

   1) bisogna chiedere in input: coordinate palla e angolo di lancio
   estremi delle rette (prima la retta superiore e poi la retta inferiore)
   2) si calcolano le slope delle rette
   3) si chiede se si vuole lanciare, si-> parte il metodo bounce; no-> allora PERCHE' USI IL PRGORAMMA?
   (sarebbe bello mettere un bottone per il lancio)
   4)si ritornano le coordinate finali, bisogna gestire i casi particoalre in cui torna
   
   */
   /*
   pf::Ball ball;
   pf::Border r1; //bordo superiore
   pf::Border r2; //bordo inferiore

   ball.coordba.x = 0;
   r1.coordbo1.x = 0;
   r2.coordbo1.x = 0;
   std::string answer;

   std::cout << "Inserire l'ordinata della palla \n";
   std::cin >> ball.coordba.y;
   std::cout << "La palla partirà dal punto ("<<ball.coordba.x<<","<<ball.coordba.y<<").\n Inserire l'angolo di lancio (il numero sarà inteso in radainti, i valori validi sono da 1.507 a -1.507 che corrispondono a più e meno pi greco mezzi)\n";
   std::cin >> ball.d;
   std::cout << "Inserire le coordinate degli estremi del bordo superiore in questo ordine: ordinata estremo sinistro, ascissa estremo destro, ordinata estremo destro \n";
   std::cin >> r1.coordbo1.y >> r1.coordbo2.x>> r1.coordbo2.y;
   std::cout << "Il bordo superiore del biliardo è la retta passante per i punti: ("<<r1.coordbo1.x<<","<<r1.coordbo1.y<<") e ("<<r1.coordbo2.x<<","<<r1.coordbo2.y<<") \n";
   std::cout << "Inserire le coordinate degli estremi del bordo inferiore in questo ordine: ordinata estremo sinistro, ascissa estremo destro, ordinata estremo destro \n";
   std::cin >> r2.coordbo1.y >> r2.coordbo2.x>> r2.coordbo2.y;
   std::cout << "Il bordo inferiore del biliardo è la retta passante per i punti: ("<<r2.coordbo1.x<<","<<r2.coordbo1.y<<") e ("<<r2.coordbo2.x<<","<<r2.coordbo2.y<<") \n";
   std::cout << "Se si desidera lanciare la palla premere un qualsaisi taso tranne 'n', se non si vuole lanciare la palla premere 'n' \n"; //sarebbe bello graficamente mettere un bottone per lanciare, così con la domanda non ha molto senso forse
   std::cin >> std::answer;
   if (std::answer == "n"|| std::answer == "N"){
    std::cout << "programma terminato";
    exit(0);}else{      //a Gaicomini va bene uscire dal programma così?

    } 


   }catch (std::exception const& e){
    std::cerr << "Caught exception : '"<< e.what() << "'\n";
    return EXIT_FAILURE;
   }catch(...){
    std::cerr << "Caught unknown exception\n";
    return EXIT_FAILURE;
   }
  


}
   */