#ifndef VIEWPORT_HPP
#define VIEWPORT_HPP

#include "Window.hpp"
#include "objects.hpp"
#include "ListaEnc.hpp"
#include "Elemento.hpp"

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
    void addObject(Object* obj) { _objetos.adiciona(obj); };

    Coordinate transformOneCoordinate(const Coordinate& c) const;
    Coordinates transformOneCoordinates(const Coordinates& coords) const;
  protected:
  private:
    Window* _window;
  	double _width, _height;
    ListaEnc<Object*> _objetos;

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

    double xvp = ((coord[0]-lowmin[0])/(uppermax[0]-lowmin[0]))*_width;
    double yvp = (1-((coord[1]-lowmin[1])/(uppermax[1]-lowmin[1])))*_height;
    return Coordinate(xvp,yvp);
}

Coordinates Viewport::transformOneCoordinates(const Coordinates& coords) const {
	Coordinates transformed_vector;
	for (int i = 0; i < coords.size(); ++i)
		transformed_vector.push_back(transformOneCoordinate(coords[i]));
	return transformed_vector;
}

void Viewport::drawPoint(Object* objeto, cairo_t* cr){
    Coordinate coord = transformOneCoordinate(objeto->get_coord_at_index(0));
    //prepareContext();
    cairo_move_to(cr, coord[0], coord[1]);
    cairo_arc(cr, coord[0], coord[1], 1.0, 0.0, (2*G_PI) );
    cairo_fill(cr);
}

void Viewport::drawLine(Object* objeto, cairo_t* cr) {
	Coordinates transformed_vector = transformOneCoordinates(objeto->get_coords());
	cairo_move_to(cr, transformed_vector[0][0], transformed_vector[0][1]);
	cairo_line_to(cr, transformed_vector[1][0], transformed_vector[1][1]);
	cairo_stroke(cr);
}
void Viewport::drawPolygon(Object* obj,cairo_t* cr) {
	Coordinates transformed_vector = transformOneCoordinates(obj->get_coords());
	cairo_move_to(cr, transformed_vector[0][0], transformed_vector[0][1]);
	for (int i = 1; i < transformed_vector.size(); ++i)
		cairo_line_to(cr, transformed_vector[i][0], transformed_vector[i][1]);
	cairo_line_to(cr, transformed_vector[0][0], transformed_vector[0][1]);
	cairo_stroke(cr);
}

void Viewport::drawDisplayFile(cairo_t* cr) {
  //percorrer o displayfile enviando os objetos para o respectivo draw
  for (int i = 0; i < _objetos.tamanho(); ++i) {
    auto teste = _objetos.retornaDaPosicao(i);
    switch(teste->get_type()) {
      case obj_type::OBJECT:
        break;
      case obj_type::POINT:
        drawPoint(teste, cr);
        break;
      case obj_type::LINE:
        drawLine(teste, cr);
        break;
      case obj_type::POLYGON:
        drawPolygon(teste, cr);
        break;
    }
  }
}

#endif