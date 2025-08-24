// qua ci vuole il corpo della funzione: utile mettere tutto in try catch per
// gestire le eccezioni

#include "biliardo.hpp"
#include "biliardo_statistica.hpp"
#include <SFML/Graphics.hpp>
#include <exception>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>
#include <array>

int main() {
  try {
    std::cout << "Vuoi eseguire la simulazione grafica o statistica? (digita 'grafica' o 'stat') \n";
    std::string mode_choice;
    std::cin >> mode_choice;

    if (mode_choice == "stat") {
      int N;
      double mu_y0;
      double sigma_y0;
      double mu_th0;
      double sigma_th0;
      double r1;
      double r2;
      double L;

      std::cout << "Numero di simulazioni: ";
      std::cin >> N;
      std::cout << "Media y_0: ";
      std::cin >> mu_y0;
      std::cout << "Deviazione standard y_0: ";
      std::cin >> sigma_y0;
      if(sigma_y0 <= 0){throw std::runtime_error("La deviazione standard deve essere maggiore di zero ");}
      std::cout << "Media theta_0 (radianti): ";
      std::cin >> mu_th0;
      std::cout << "Deviazione standard theta_0: ";
      std::cin >> sigma_th0;
       if(sigma_th0 <= 0){throw std::runtime_error("La deviazione standard deve essere maggiore di zero ");}
      std::cout << "Ordinata estremo sinistro del bordo: ";
      std::cin >> r1;
      std::cout << "ordinata estremo destro del bordo: ";
      std::cin >> r2;
      std::cout << "Ascissa estremo destro del bordo: ";
      std::cin >> L;

      pf::Border b1( r1, L, r2);
      pf::Border b2( -r1, L, -r2);

      StatsResult res = simulate_stats(
        N,
        mu_y0,
        sigma_y0,
        mu_th0,
        sigma_th0,
        b1,
        b2,
        L
      );

      std::cout << "\n Risultati Statistici \n";
      std::cout << "Media y_f: " << res.mean_yf << "\n";
      std::cout << "Deviazione standard y_f: " << res.stdev_yf << "\n";
      std::cout << "Coefficiente di simmetria y_f: " << res.coeff_simm_yf << "\n";
      std::cout << "Coefficiente di appiattimento y_f: " << res.coeff_app_yf << "\n";
      std::cout << "Media theta_f: " << res.mean_thf << "\n";
      std::cout << "Deviazione standard theta_f: " << res.stdev_thf << "\n";
      std::cout << "Coeffieciente di simmetria theta_f: " << res.coeff_simm_thf << "\n";
      std::cout << "Coefficiente di appiattimento theta_f: " << res.coeff_app_thf << "\n";

      return 0;
    }

    // preparazione cose che servono per sfml
    sf::RenderWindow window(sf::VideoMode(800, 600), "Biliardo triangolare",
                            sf::Style::Default);
    // window.setVerticalSyncEnabled(true);
    // window.setFramerateLimit(60);
    //  computer non supporta VerticalSync, quindi può farmi delle storie quando
    //  facciamo aprtire la palla, lo uso per vedere se la grafica funzione,
    //  all'esame dobbiamo cancellarlo
    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
      std::cerr << "Errore: impossibile trovare il font";
      return -1;
    }
    sf::Clock clock;
    sf::Time update = sf::seconds(0.016f);
    int step = 0;
    bool finished = false;
    std::string answer;
    //std::vector<std::string> answers;

    // palla grafica
    sf::CircleShape circle;
    circle.setRadius(7.f);
    circle.setOrigin(7.f, 7.f);
    circle.setPosition(sf::Vector2f(0.f, 300.f));
    circle.setFillColor(sf::Color::Yellow);

    // bordi
    std::array<sf::Vertex, 2> border1 = {sf::Vertex{sf::Vector2f(0.f, 250.f)},
                          sf::Vertex{sf::Vector2f(800.f, 250.f)}};
    border1[0].color = sf::Color::Green;
    border1[1].color = sf::Color::Green;

    std::array<sf::Vertex, 2> border2 = {sf::Vertex{sf::Vector2f(0.f, 350.f)},
                          sf::Vertex{sf::Vector2f(800.f, 350.f)}};
    border2[0].color = sf::Color::Green;
    border2[1].color = sf::Color::Green;

    // creazione oggetti programma
    pf::Ball ball({0, 0}, 0);
    pf::Border b1(0, 0, 1);
    pf::Border b2(0, 0, 1);
    double y, r1, r2, l;

    // linea centrale
    std::array<sf::Vertex, 2> middle_line = {sf::Vertex{sf::Vector2f(0.f, 300.f)},
                              sf::Vertex{sf::Vector2f(800.f, 300.f)}};
    middle_line[0].color = sf::Color::White;
    middle_line[1].color = sf::Color::White;

    std::array<sf::Vertex, 2> arrow_up = {sf::Vertex{sf::Vector2f(792, 292.f)},
                           sf::Vertex{sf::Vector2f(800.f, 300.f)}};
    middle_line[0].color = sf::Color::White;
    middle_line[1].color = sf::Color::White;

    std::array<sf::Vertex, 2> arrow_down = {sf::Vertex{sf::Vector2f(792, 308.f)},
                             sf::Vertex{sf::Vector2f(800.f, 300.f)}};
    middle_line[0].color = sf::Color::White;
    middle_line[1].color = sf::Color::White;

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
        "Inserire l'ordinata della pallina:\n \n si consideri che il range della finestra grafica e' di [-300,300]\n",
        "Inserire l'angolo di lancio della pallina in radianti:\n Valori concessi [-1.55, 1.55]",
        "Inserire l'ordinata dell'estremo sinistro del bordo:\n si consideri che il range della finestra grafica e' di [-300,300]\n eventuali "
        "coordinate negative saranno assegnate al bordo inferiore.\n",
        "Inserire l'ascissa dell'estremo destro del bordo:\n si consideri che il range della finestra grafica e' di [0,800]\n ",
        "Inserire l'ordinata dell'estremo destro del bordo:\n si consideri che il range della finestra grafica e' di [-300,300]\n eventuali "
        "coordinate negative saranno assegnate al bordo inferiore.\n",
        "premere il tasto SPAZIO per lanciare la palla.\n"};

    // funzione lmbda
    auto updateQuestion = [&](int s) {
      if (s >= 0 && static_cast<size_t>(s) < questionText.size())
        questions.setString(questionText[s]);
      else
        questions.setString("");
    };

    updateQuestion(step);
    questions.setCharacterSize(15);
    questions.setPosition(50.f, 50.f);
    std::string userInput;

    while (window.isOpen()) {
      sf::Event event;
      sf::Text result_text;

      if (clock.getElapsedTime() >= update) {
        window.clear();
        window.draw(circle);
        window.draw(middle_line.data(), middle_line.size(),
                    sf::PrimitiveType::Lines);
        window.draw(arrow_up.data(), arrow_up.size(), sf::PrimitiveType::Lines);
        window.draw(arrow_down.data(), arrow_down.size(),
                    sf::PrimitiveType::Lines);
        window.draw(border1.data(), border1.size(), sf::PrimitiveType::Lines);
        window.draw(border2.data(), border2.size(), sf::PrimitiveType::Lines);
        window.draw(questions);
        window.draw(response);
        window.display();
        clock.restart();
      }

      while (window.pollEvent(event)) {

        // il programmas i chiude se viene chiusa la finestra
        if (event.type == sf::Event::Closed) {
          window.close();
          std::cerr
              << "Programma terminato: la finestra grafica è stata chiusa.\n";
          return -1;
        }

        if (!finished && event.type == sf::Event::TextEntered) {
          if (event.text.unicode == 8 && !userInput.empty()) {
            // Backspace
            userInput.erase(userInput.size() - 1, 1);
          } else if (event.text.unicode == 13) {
            // Invio: salva risposta e passa alla domanda successiva
            answer = userInput;
            //answers.push_back(answer);
            userInput.clear();
            /*std::ostringstream oss;
            for (size_t i = 0; i < answers.size(); ++i) {
            oss << questionText[i] << " " << answers[i] << "\n";
            }
            
            response.setString(oss.str());*/
            step++;

            if (step == 1) {
              y = std::stod(answer);
              ball.move_to({0, y + 300});

              circle.setPosition({0.f, static_cast<float>(300.f - y)});
            }

            if (step == 2) {
              double d = std::stod(answer);
              ball.set_angle(d);
            }

            if (step == 3) {
              r1 = std::stod(answer);

              if (r1 >= 0) {
                b1.set_r1(300 + r1);
                b2.set_r1(300 - r1);
                border1[0].position.y = static_cast<float>(300.f - r1);
                border2[0].position.y = static_cast<float>(300.f + r1);
              } else {
                b2.set_r1(300 + r1);
                b1.set_r1(300 - r1);
                border2[0].position.y = static_cast<float>(300.f - r1);
                border1[0].position.y = static_cast<float>(300.f + r1);
              }
            }

            if (step == 4) {
              l = std::stod(answer);
              b1.set_L(l);
              b2.set_L(l);
              border2[1].position.x = static_cast<float>(l);
              border1[1].position.x = static_cast<float>(l);
            }

            if (step == 5) {
              r2 = std::stod(answer);
              if (r2 >= 0) {
                b1.set_r2(300 + r2);
                b2.set_r2(300 - r2);
                border1[1].position.y = static_cast<float>(300.f - r2);
                border2[1].position.y = static_cast<float>(300.f + r2);
                

              } else {
                b2.set_r2(300 + r2);
                b1.set_r2(300 - r2);
                border2[1].position.x = static_cast<float>(300.f - r2);
                border1[1].position.x = static_cast<float>(300.f + r2);
                
              }

              b1.set_slopeup((b1.r2()-b1.r1())/b1.L());
              b2.set_slopeup((b2.r2()-b2.r1())/b2.L());
            }

          } else if (event.text.unicode >= 32) { // caratteri stampabili
            userInput += static_cast<char>(event.text.unicode);
          }

          

          inputText.setString(userInput);
          updateQuestion(step);
        }

        if (event.type == sf::Event::KeyPressed &&
            event.key.code == sf::Keyboard::Key::Space && step == 5) {
          // updateQuestion(step);
          pf::Border::initial_checks(b1, b2, ball);
          finished = true;
          
          double y = ball.coordba().y;
          double x = ball.coordba().x;

          pf::Result end = pf::Result::BallSimulation(b1, b2, ball);
          
          

          double x_end = end.result.coordba().x;
          double y_end = end.result.coordba().y - 300;
          double d_end = end.result.d();

          for (size_t position = 0; position < end.trajectory.size(); ++position) {
            circle.move(
                static_cast<float>(end.trajectory[position].coordba().x-x ),
                static_cast<float>(
                    -(end.trajectory[position].coordba().y-y)));
            x = end.trajectory[position].coordba().x;
            y = end.trajectory[position].coordba().y;
            window.clear();
            window.draw(circle);
            window.draw(middle_line.data(), middle_line.size(),
                        sf::PrimitiveType::Lines);
            window.draw(arrow_up.data(), arrow_up.size(),
                        sf::PrimitiveType::Lines);
            window.draw(arrow_down.data(), arrow_down.size(),
                        sf::PrimitiveType::Lines);
            window.draw(border1.data(), border1.size(),
                        sf::PrimitiveType::Lines);
            window.draw(border2.data(), border2.size(),
                        sf::PrimitiveType::Lines);

            window.draw(response);
            window.display();
            sf::sleep(sf::milliseconds(64));
          }

            std::ostringstream oss;
          oss << "La posizione finale della pallina e' la seguente: (" << x_end
              << "," << y_end << ") con angolo: " << d_end
              << " radianti.\n Sono stati eseguiti " << end.bounces
              << " rimbalzi\n";
              response.setCharacterSize(15);
          response.setFillColor(sf::Color::Green);
          response.setPosition(30, 550);
          response.setString(oss.str());

          
        }

        /**/
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