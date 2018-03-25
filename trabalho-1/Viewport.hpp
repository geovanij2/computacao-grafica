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
            _width(width),
            _height(height),
            _window(new Window(width,height))
    { 
      normalize_all_objs();
    }

    virtual ~Viewport() {}
    void zoom(double step);
    void moveX(double value);
    void moveY(double value);
    void rotate_window(double degrees);
    void drawDisplayFile(cairo_t* cr);
    void addObject(Object* obj) { _objetos.adiciona(obj); normalize_obj(obj); };
    Object* getObject(int index) { _objetos.retornaDaPosicao(index);};
    Coordinate transformOneCoordinate(const Coordinate& c) const;
    Coordinates transformOneCoordinates(const Coordinates& coords) const;
    void normalize_obj(Object* obj);
  protected:
  private:
    Window* _window;
    double _width, _height;
    ListaEnc<Object*> _objetos;

    void normalize_all_objs();

	  void drawPoint(Object* objeto, cairo_t* cr);
    void drawLine(Object* objeto, cairo_t* cr);
    void drawPolygon(Object* objeto, cairo_t* cr);

};

void Viewport::zoom(double step){
  _window->zoom(step);
  normalize_all_objs();
}
void Viewport::moveX(double step){
  _window->moveX(step);
  normalize_all_objs();
}
void Viewport::moveY(double step){
  _window->moveY(step);
  normalize_all_objs();
}

void Viewport::rotate_window(double degrees) {
  _window->rotate(degrees);
  normalize_all_objs();
}

void Viewport::normalize_obj(Object* obj) {
  Transformation t = _window->get_transformation();
  obj->set_normalized_coords(t);
}

void Viewport::normalize_all_objs() {
  _window->update_transformation();
  auto t = _window->get_transformation();

  for (int i = 0; i < _objetos.tamanho(); i++) {
    Object* obj = _objetos.retornaDaPosicao(i);
    obj->set_normalized_coords(t);
  }
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
    Coordinate coord = transformOneCoordinate(objeto->get_normalized_coord_at_index(0));
    //prepareContext();
    cairo_move_to(cr, coord[0], coord[1]);
    cairo_arc(cr, coord[0], coord[1], 1.0, 0.0, (2*G_PI) );
    cairo_fill(cr);
}

void Viewport::drawLine(Object* objeto, cairo_t* cr) {
	Coordinates transformed_vector = transformOneCoordinates(objeto->get_normalized_coords());
	cairo_move_to(cr, transformed_vector[0][0], transformed_vector[0][1]);
	cairo_line_to(cr, transformed_vector[1][0], transformed_vector[1][1]);
	cairo_stroke(cr);
}
void Viewport::drawPolygon(Object* obj,cairo_t* cr) {
	Coordinates transformed_vector = transformOneCoordinates(obj->get_normalized_coords());
	cairo_move_to(cr, transformed_vector[0][0], transformed_vector[0][1]);
	for (int i = 1; i < transformed_vector.size(); ++i)
		cairo_line_to(cr, transformed_vector[i][0], transformed_vector[i][1]);
	cairo_line_to(cr, transformed_vector[0][0], transformed_vector[0][1]);
	cairo_stroke(cr);
}

void Viewport::drawDisplayFile(cairo_t* cr) {
  //percorrer o displayfile enviando os objetos para o respectivo draw
  for (int i = 0; i < _objetos.tamanho(); ++i) {
    auto obj = _objetos.retornaDaPosicao(i);
    switch(obj->get_type()) {
      case obj_type::OBJECT:
        break;
      case obj_type::POINT:
        drawPoint(obj, cr);
        break;
      case obj_type::LINE:
        drawLine(obj, cr);
        break;
      case obj_type::POLYGON:
        drawPolygon(obj, cr);
        break;
    }
  }
}

#endif