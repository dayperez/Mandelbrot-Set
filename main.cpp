#include <cstdlib>
#include <ctime>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <vector>
#include "ComplexPlane.h"


using namespace sf;
using namespace std;




int main()
{




  int screenWidth = VideoMode::getDesktopMode().width / 2;
  int screenHeight = VideoMode::getDesktopMode().height /2;


  VideoMode vm(screenWidth, screenHeight);
 
  RenderWindow window(vm, "Mandelbrot Set", Style::Default);
   ComplexPlane complexPlane(screenWidth, screenHeight);
 
  Font font;
      if (!font.loadFromFile("arial.ttf")) {
          cout << "Error loading font.." << endl;
      }
   Text text;
  text.setFont(font);
  text.setCharacterSize(14);
  text.setFillColor(Color::White);
  text.setPosition(20.f, 20.f);
  text.setString(" Welcome...");


  while(window.isOpen()){


   Event event;
   while (window.pollEvent(event)){
        if (event.type == Event::Closed)
           {
                   window.close();
           }


           if (event.type == Event::MouseButtonPressed)
           {
               Vector2i mousePixel = Mouse::getPosition(window);


               if (event.mouseButton.button == Mouse::Right) {
                   complexPlane.zoomOut();
                   complexPlane.setCenter(mousePixel);
               }
               else if (event.mouseButton.button == Mouse::Left){
                   complexPlane.zoomIn();
                   complexPlane.setCenter(mousePixel);
               }
           }
           if (event.type == Event::MouseMoved){
               Vector2i mousePixel = Mouse::getPosition(window);
               complexPlane.setMouseLocation(mousePixel);
           }
   }


   if (Keyboard::isKeyPressed(Keyboard::Escape)){
       window.close();
   }


   complexPlane.updateRender();


   complexPlane.loadText(text);


   window.clear();


   window.draw(complexPlane);


   window.draw(text);


   window.display();


  }


  return 0;




}


