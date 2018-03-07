#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "objects.hpp"

class Window
{
  public:
    Window(double width, double height):
      _lowmin(0,0), _lowmax(height,0), _uppermin(width,0), _uppermax(width, height){}
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
  _lowmin += step;
  _uppermax -= step;
  _lowmax.x -= step;
  _lowmax.y += step;
  _uppermin.x += step;
  _uppermin.y -= step;
}

/* Move Window Horizontally */
void Window::moveX(double value)
{
  _lowmin.x += value;
  _lowmax.x += value;
  _uppermin.x += value;
  _uppermax.x += value;
}

/* Move Window Vertically */
void Window::moveY(double value)
{
  _lowmin.y += value;
  _lowmax.y += value;
  _uppermin.y += value;
  _uppermax.y += value;
}