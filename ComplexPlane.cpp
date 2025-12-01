#include <algorithm>
#include <complex>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <iomanip>
#include <sstream>
#include "ComplexPlane.h"


using namespace sf;
using namespace std;




ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight)
  : m_pixel_size{pixelWidth,pixelHeight}, 


  m_aspectRatio(static_cast<float>(pixelHeight) / static_cast<float>(pixelWidth)),
  m_plane_center{0,0},
  m_plane_size{BASE_WIDTH, BASE_HEIGHT * m_aspectRatio},
  m_zoomCount{0},
  m_state{State::CALCULATING}

{


  m_vArray.setPrimitiveType(Points);
  m_vArray.resize(pixelWidth * pixelHeight);




}




void ComplexPlane::draw(RenderTarget& target, RenderStates states) const {




  target.draw(m_vArray);




}




void ComplexPlane::updateRender() {




  if (m_state == State::CALCULATING) {
       int pixelWidth = m_pixel_size.x;
       int pixelHeight = m_pixel_size.y;


      for (int i = 0; i < pixelHeight; i++){
          for(int j = 0; j < pixelWidth; j++){
              
               m_vArray[j + i * pixelWidth].position = {(float)j,(float)i};


               Vector2f complexCoord = mapPixelToCoords(Vector2i(j,i));


               size_t iterationCount = countIterations(complexCoord);


               Uint8 r, g, b;


               iterationsToRGB(iterationCount, r, g, b);


               m_vArray[j + i * pixelWidth].color = {r,g,b};




          }
      }
      m_state = State::DISPLAYING;
  }
}




void ComplexPlane::zoomIn(){
   m_zoomCount++;




  float x_size = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
  float y_size = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);




  m_plane_size = {x_size, y_size};




  m_state = State::CALCULATING;




}




void ComplexPlane::zoomOut(){




   m_zoomCount--;




  float x_size = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
  float y_size = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);




  m_plane_size = {x_size, y_size};




  m_state = State::CALCULATING;




}




void ComplexPlane::setCenter(Vector2i mousePixel){
   Vector2f center_coord = mapPixelToCoords(mousePixel);
  m_plane_center = center_coord;




  m_state = State::CALCULATING;
}




void ComplexPlane::setMouseLocation(Vector2i mousPixel){




  Vector2f mouse_coord = mapPixelToCoords(mousPixel);
  m_mouseLocation = mouse_coord;




}




void ComplexPlane::loadText(Text& text){




  stringstream ss;




  ss << fixed << setprecision(5);
  ss << "Mandelbrot Set" << endl;
  ss << "Center: (" << m_plane_center.x << ", " << m_plane_center.y << ")" << endl;
  ss << "Cursor: (" << m_mouseLocation.x << ", " << m_mouseLocation.y << ")" << endl;
  ss << "Left-click to Zoom in" << endl;
  ss << "Right-click to Zoom out" << endl;




  text.setString(ss.str());
}




size_t ComplexPlane::countIterations(Vector2f coord) {
  complex<double> c(coord.x, coord.y);
 complex<double> z(0.0, 0.0);


 size_t i = 0;


 while(abs(z) < 2.0 && i < MAX_ITER) {


   z = z * z + c;
   i++;


 }
 return i;
}


void ComplexPlane::iterationsToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b) {


   if (count >= MAX_ITER) {
      
       r = 0;
       g = 0;
       b = 0;


       return;
   }


   float normCount = (float)count / (float)MAX_ITER;


   if (normCount < 0.2){ //Purple/Blue
     
       r = (Uint8)(min(1.0, normCount * 5.0) * 255);
       g = 0;
       b = 255;
   }


   else if (normCount < 0.4) { //Blue/Turquoise
      
       r = 255;
       g = (Uint8)(min(1.0, (normCount - 0.2) * 5.0) * 255);
       b = (Uint8)(max(0.0, 1.0 - (normCount - 0.2) * 5.0) * 255);
   }


   else if (normCount < 0.6) { //Green/Yellow


       r = (Uint8)(max(0.0, 1.0 - (normCount - 0.4) * 5.0) * 255);
       g = 255;
       b = 0;
   }


   else if (normCount < 0.8) { //Yellow/Orange


       r = 255;
       g = (Uint8)(max(0.0, 1.0 - (normCount - 0.6) * 5.0) * 255);
       b = 0;
   }


   else {


       r = 255;
       g = 0;
       b = (Uint8)(min(1.0, (normCount - 0.8) * 5.0) * 255);
   }


}


Vector2f ComplexPlane::mapPixelToCoords(Vector2i mousePixel) {


   float x_min = 0.0;
   float x_max = m_pixel_size.x;
  
   float r_min = m_plane_center.x - (m_plane_size.x / 2.0);
   float r_max = m_plane_center.x + (m_plane_size.x / 2.0);


   float r = (((float)mousePixel.x - x_min) / (x_max - x_min)) * (r_max - r_min) + r_min;




   float y_min = 0.0;
   float y_max = m_pixel_size.y;
  
   float i_min = m_plane_center.y - (m_plane_size.y / 2.0);
   float i_max = m_plane_center.y + (m_plane_size.y / 2.0);


   float ratio = ((float)mousePixel.y - y_min) / (y_max - y_min);
  
   float i = (1.0 - ratio) * (i_max - i_min) + i_min;


   return Vector2f(r, i);
}
