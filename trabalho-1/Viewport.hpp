#ifndef VIEWPORT_HPP
#define VIEWPORT_HPP

#include "Window.hpp"
#include "objects.hpp"
#include "lista-encadeada.hpp"
#include "elemento.hpp"

class Viewport
{
  public:
  	Viewport(double width, double height):
            _width(width), _height(height), _window(new Window(width,height)){}

    virtual ~Viewport() {}
    void zoom(double step);
    void moveX(double value);
    void moveY(double value);
    void drawDisplayFile(cairo_t* cr);

    Coordinate transformOneCoordinate(const Coordinate& c) const;
    Coordinates transformOneCoordinates(const Coordinates& coords) const;
  protected:
  private:
    Window* _window;
  	double _width, _height;
    ListaEnc<Object*> *_objetos;

	void drawPoint(Object* objeto, cairo_t* cr);
    void drawLine(Object* objeto, cairo_t* cr);
    void drawPolygon(Object* objeto, cairo_t* cr);

};

void Viewport::zoom(double step){
  _window->zoom(step);
}
void Viewport::moveX(double step){
  _window->moveX(step);
}
void Viewport::moveY(double step){
  _window->moveY(step);
}

Coordinate Viewport::transformOneCoordinate(const Coordinate& coord) const {
    const Coordinate lowmin = _window->lowmin();
    const Coordinate uppermax = _window->uppermax();

    double xvp = ((coord.x-lowmin.x)/(uppermax.x-lowmin.x))*_width;
    double yvp = (1-((coord.y-lowmin.y)/(uppermax.y-lowmin.y)))*_height;
    printf("(%f,%f) virou (%f,%f)\n", coord.x, coord.y, xvp, yvp);
    return Coordinate(xvp,yvp);
}

void Viewport::drawPoint(Object* objeto, cairo_t* cr){
    //Coordinate coord = transformCoordinate(obj->getCoord(0));
Coordinates Viewport::transformOneCoordinates(const Coordinates& coords) const {
	Coordinates transformed_vector;
	for (int i = 0; i < coords.size(); ++i)
		transformed_vector.push_back(transformOneCoordinate(coords[i]));
	return transformed_vector;
}

    //prepareContext();
    //cairo_move_to(_cairo, coord.x, coord.y);
    //cairo_arc(_cairo, coord.x, coord.y, 1.0, 0.0, (2*PI) );
    //cairo_fill(_cairo);
}

void Viewport::drawLine(Object* obj, cairo_t* cr){}
void Viewport::drawPolygon(Object* obj,cairo_t* cr){}

void Viewport::drawDisplayFile(cairo_t* cr){
  cairo_move_to(cr, 25, 18);
  cairo_line_to(cr, 525, 18);
  cairo_line_to(cr, 525, 517);
  cairo_line_to(cr, 25, 517);
  cairo_line_to(cr, 25, 18);
  cairo_stroke(cr);

  //percorrer o displayfile enviando os objetos para o respectivo draw

}

#endif