// qua ci vuole il corpo della funzione: utile mettere tutto in try catch per
// gestire le eccezioni

#include "biliardo.hpp"
#include <SFML/Graphics.hpp>
#include <exception>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

int main() {
  try {

    // preparazione cose che servono per sfml
    sf::RenderWindow window(sf::VideoMode(800, 600), "Biliardo triangolare",
                            sf::Style::Default);
    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
      std::cerr << "Errore: impossibile trovare il font";
      return -1;
    }
    sf::CircleShape circle(50.0);
    circle.setPosition(sf::Vector2f(0, 300));
    circle.setFillColor(sf::Color::Yellow);
    sf::Clock clock;
    sf::Time update = sf::seconds(0.016f);
    int step = 0;
    bool finished = false;
    std::string answer;

    // creazione oggetti programma
    pf::Ball ball({0, 0}, 0);
    pf::Border b1(0, 0, 1);
    pf::Border b2(0, 0, 1);
    double y,d, r1, r2, l, angle;

    // testi da visualizzare
    // domanda
    sf::Text questions("", font, 32);
    questions.setFillColor(sf::Color::White);
    questions.setPosition(50.f, 50.f);

    // risposta utente (input)
    sf::Text inputText("", font, 32);
    inputText.setFillColor(sf::Color::White);
    inputText.setPosition(50.f, 120.f);

    // risposta programma (risultato), frose si può riutilizzare question
    sf::Text response("", font, 32);
    response.setFillColor(sf::Color::Green);
    response.setPosition(50.f, 200.f);

    // dati che vogliamo raccogliere e domande che vogliamo fare

    std::vector<std::string> questionText = {
        "Inserire l'ordinata della pallina.\n",
        "Inserire l'angolo di lancio della pallina.\n",
        "Inserire l'ordinata dell'estremo sinistro del bordo.\n",
        "Inserire l'ascissa dell'estremo destro del bordo.\n",
        "Inserire l'ordinata dell'estremo destro del bordo.\n"};

    // funzione lmbda
    auto updateQuestion = [&](int s) {
      if (s >= 0 && s < questionText.size())
        questions.setString(questionText[s]);
      else
        questions.setString("");
    };

    updateQuestion(step);
    std::string userInput;

    while (window.isOpen()) {
      sf::Event event;

      if (clock.getElapsedTime() >= update) {
        window.clear();
        window.draw(circle);
         window.draw(questions); 
        window.display();
        clock.restart();
      }

      /*
      if (std::cin.peek() !=
          EOF) { // serve per evitare che scrivendo solos td::cin questo
                 // ostacoli l'aggiornamento della finestra grafica
        std::cout
            << "La linea centrale corrisponde all'asse x, l'asse y si "
               "trova sul lato sinistro della finestra.\n Inserire "
               "l'ordinata della pallina e l'angolo di lancio in radianti: "
               "i valori concessi sono da -1.55 a +1,55\n";
        // std::cin >> y >> angle;
        ball.move_to({0, y + 300});
        ball.set_angle(angle);
        circle.setPosition(sf::Vector2f(0, y + 300));

        std::cout
            << "Si definiscono ora le coordinate di un bordo, l'altro bordo "
               "verrà costruito tramite una simmetria rispetto all'asse x. "
               "Inserire, nell'ordine: ordinata dell'estremo sinistro, ascissa "
               "dell'estremo destro e ordinata dell'estremo destro.\n";

        // std::cin >> r1 >> L >> r2;
        if (r1 >= 0) {
          b1.move_border(r1 + 300, r2 + 300, L);
          b2.move_border(300 - r1, 300 - r2, L);
        } else {
          b2.move_border(r1 + 300, r2 + 300, L);
          b1.move_border(300 - r1, 300 - r2, L);
        }
      }
      pf::Border::initial_checks(b1, b2, ball);
      std::cout << "premere il tasto SPAZIO per lanciare la palla.\n";
      */
      while (window.pollEvent(event)) {

        if (event.type == sf::Event::Closed)
          window.close();

        if (!finished && event.type == sf::Event::TextEntered) {
          if (event.text.unicode == 8 && !userInput.empty()) {
            // Backspace
            userInput.erase(userInput.size() - 1, 1);
          } else if (event.text.unicode == 13) {
            // Invio: salva risposta e passa alla domanda successiva
            answer = userInput;
            userInput.clear();
            step++;

            if (step == 0) {
              y = std::stod(answer);
            }

            if (step == 1) {
              d = std::stod(answer);
            }

            if (step == 2) {
              r1 = std::stod(answer);
            }

            if (step == 3) {
              l = std::stod(answer);
            }

            if (step == 4) {
              r2 = std::stod(answer);
            }

            if (step >= questionText.size()) {
              finished = true;

              // aggiorna la palla
              ball.move_to({0, y + 300});
              ball.set_angle(angle);
              if (r1 >= 0) {
                b1.move_border(r1 + 300, r2 + 300, l);
                b2.move_border(300 - r1, 300 - r2, l);
              } else {
                b2.move_border(r1 + 300, r2 + 300, l);
                b1.move_border(300 - r1, 300 - r2, l);
              }

              pf::Border::initial_checks(b1, b2, ball);
              std::cout << "premere il tasto SPAZIO per lanciare la palla.\n";

              /*std::ostringstream oss;
              for (size_t i = 0; i < answers.size(); ++i) {
                oss << questions[i] << " " << answers[i] << "\n";
              }
              response.setString(oss.str());*/
            } else {
              updateQuestion(step);
            }
          } else if (event.text.unicode >= 32) { // caratteri stampabili
            userInput += static_cast<char> (event.text.unicode);
          }

          inputText.setString(userInput);

          if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Key::Space) {
              pf::Result end = pf::Result::BallSimulation(b1, b2, ball);
              double x_end = end.result.coordba().x - 300;
              double y_end = end.result.coordba().y - 300;
              double d_end = end.result.d();
              std::cout << "La posizione finale della pallina è la seguente: ("
                        << x_end << "," << y_end << ") con angolo: " << d_end
                        << " radianti.\n Sono stati eseguiti " << end.bounces
                        << " rimbalzi\n";
            }
          }
        }
      }
    }
  } catch (std::exception const &e) {
    std::cerr << "Eccezione catturata : '" << e.what() << "'\n";
    return EXIT_FAILURE;
  } catch (...) {
    std::cerr << "Caught unknown exception\n";
    return EXIT_FAILURE;
  }
}

