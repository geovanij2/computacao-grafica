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

	void drawPoint(Object objeto, cairo_t* cr);
    void drawLine(Object objeto, cairo_t* cr);
    void drawPolygon(Object objeto, cairo_t* cr);

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

Coordinates Viewport::transformOneCoordinates(const Coordinates& coords) const {
	Coordinates transformed_vector;
	for (int i = 0; i < coords.size(); ++i)
		transformed_vector.push_back(transformOneCoordinate(coords[i]));
	return transformed_vector;
}

void Viewport::drawPoint(Object objeto, cairo_t* cr){
    Coordinate coord = transformOneCoordinate(objeto.get_coord_at_index(0));
    //prepareContext();
    cairo_move_to(cr, coord.x, coord.y);
    cairo_arc(cr, coord.x, coord.y, 1.0, 0.0, (2*G_PI) );
    cairo_fill(cr);
}

void Viewport::drawLine(Object objeto, cairo_t* cr) {
	Coordinates transformed_vector = transformOneCoordinates(objeto.get_coords());
	cairo_move_to(cr, transformed_vector[0].x, transformed_vector[0].y);
	cairo_line_to(cr, transformed_vector[1].x, transformed_vector[1].y);
	cairo_stroke(cr);
}
void Viewport::drawPolygon(Object obj,cairo_t* cr) {
	Coordinates transformed_vector = transformOneCoordinates(obj.get_coords());
	cairo_move_to(cr, transformed_vector[0].x, transformed_vector[0].y);
	for (int i = 1; i < transformed_vector.size(); ++i)
		cairo_line_to(cr, transformed_vector[i].x, transformed_vector[i].y);
	cairo_line_to(cr, transformed_vector[0].x, transformed_vector[0].y);
	cairo_stroke(cr);
}

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