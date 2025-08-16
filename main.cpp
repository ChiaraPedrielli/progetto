//qua ci vuole il corpo della funzione: utile mettere tutto in try catch per gestire le eccezioni

#include <exception>
#include <iostream>
#include <random>
#include "biliardo.hpp"
#include <SFML/Graphics.hpp>


int main(){
   try{

    sf::RenderWindow window(sf::VideoMode(800,600), "Biliardo triangolare");
    sf::CircleShape circle(50.0);
    circle.setPosition(sf::Vector2f(0,300));
    circle.setFillColor(sf::Color::Yellow);

    
       
       pf::Ball ball({0,0},0);
       pf::Border b1(0,0,1);
       pf::Border b2(0,0,1);
       double y,r1,r2,L,angle;

       std::cout << "La linea centrale corrisponde all'asse x, l'asse y si trova sul lato sinistro della finestra.\n Inserire l'ordinata della pallina e l'angolo di lancio in radianti: i valori concessi sono da -1.55 a +1,55\n";
       std::cin >> y >> angle;
       ball.move_to({0, y + 300});
       ball.set_angle(angle);
       circle.setPosition(sf::Vector2f(0,y + 300));
    
    
       std::cout << "Si definiscono ora le coordinate di un bordo, l'altro brodo verrà costruito tramite una simmetria rispetto all'asse x. Inserire, nell'ordine: ordinata dell'estremo sinistro, ascissa dell'estremo destro e ordinata dell'estremo destro";
       std::cin >> r1 >> L >> r2; 
        if(r1 >= 0){
          b1.move_border(r1 + 300, r2 + 300, L);
          b2.move_border(300 - r1, 300 - r2, L);
        } else {
          b2.move_border(r1 + 300, r2 + 300, L);
          b1.move_border(300 - r1, 300 - r2, L);
        }
        pf::Border::initial_checks(b1,b2,ball);
       std::cout << "premere il tasto SPAZIO per lanciare la palla.\n";
        
     while (window.isOpen()){
        sf::Event event;
        
        while (window.pollEvent(event)){
           
           if(event.type == sf::Event::Closed)
           window.close();

           if(event.type == sf::Event::KeyPressed){
             if(event.key.code == sf::Keyboard::Key::Space) {
                pf::Result end = pf::Result::BallSimulation(b1,b2,ball);
                double x_end = end.result.coordba().x - 300;
                double y_end = end.result.coordba().y - 300;
                double d_end = end.result.d();
                std::cout << "La posizione finale della pallina è la seguente: ("<<x_end<<","<<y_end<<") con angolo: "<<d_end<<" radianti.\n Sono stati eseguiti "<<end.bounces<<" rimbalzi\n";

               }
           }
        }

        window.clear();
        window.display();

      }

    
    
   

   




   }catch (std::exception const& e){
    std::cerr << "Eccezione catturata : '"<< e.what() << "'\n";
    return EXIT_FAILURE;
   }catch(...){
    std::cerr << "Caught unknown exception\n";
    return EXIT_FAILURE;
   }
  


}
  