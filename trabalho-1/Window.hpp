#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "objects.hpp"

class Window
{
  public:
    Window(double width, double height):
      _lowmin(0,0), _lowmax(width,0), _uppermin(0,height), _uppermax(width, height){}
      virtual ~Window() {} 

       void zoom(double step);
       void moveX(double value);
       void moveY(double value);

  protected:
  private:
    Coordinate _lowmin, _lowmax, _uppermin, _uppermax;
};

/* step > 0  -  Zoom in */
void Window::zoom(double step)
{
  step /= 2;
  Coordinate coord_e(step, step);
  Coordinate coord_d(step, -step);

  _lowmin += coord_e;
  _uppermax -= coord_e;
  _lowmax -= coord_d;
  _uppermin += coord_d;


}

/* Move Window Horizontally */
void Window::moveX(double value)
{
  Coordinate coord(value, 0);
  _lowmin += coord;
  _lowmax += coord;
  _uppermin += coord;
  _uppermax += coord;
}

/* Move Window Vertically */
void Window::moveY(double value)
{
  Coordinate coord(0, value);
  _lowmin += coord;
  _lowmax += coord;
  _uppermin += coord;
  _uppermax += coord;
}

#endif