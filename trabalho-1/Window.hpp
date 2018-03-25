#ifndef WINDOW_HPP
#define WINDOW_HPP


#include <cmath>
#include "coordinate.hpp"
#include "Transformation.hpp"

class Window {
	public:
		Window(double width, double height):
			_center(width/2, height/2),
			_angle(0),
			_width(width),
			_heigth(height),
			_t({ {1, 0, 0},
				 {0, 1, 0},
				 {0, 0, 1} })
		{}
		
		virtual ~Window() {} 

		void zoom(double step);
		void moveX(double value);
		void moveY(double value);
		void rotate(double degrees);

		Coordinate lowmin() const { return Coordinate(-1,-1); }
		Coordinate uppermax() const { return Coordinate(1,1); }
		Coordinate center() const { return _center; }
		double angle() const { return _angle; }

		Transformation& get_transformation() { return _t; }
		void update_transformation();

	protected:
	private:
		Coordinate _center;
		double _angle; // radians
		double _width, _heigth;
		Transformation _t;
};

/* step > 0  -  Zoom in */
void Window::zoom(double step) {
	_width -= step;
	_heigth -= step;
}

/* Move Window Horizontally */
void Window::moveX(double value) {
	double teta = Transformation::to_radians(_angle);
	Coordinate coord(cos(teta)*value, -sin(teta)*value);

	_center += coord;
}

/* Move Window Vertically */
void Window::moveY(double value) {
	double teta = Transformation::to_radians(_angle);
	Coordinate coord(sin(teta)*value, cos(teta)*value);

	_center += coord;
}

void Window::rotate(double degrees) {
	_angle += degrees;
}

void Window::update_transformation() {
	_t = Transformation({ {1, 0, 0},
						  {0, 1, 0},
						  {0, 0, 1} });
	_t *= Transformation::generate_2D_translation_matrix(-_center[0], -_center[1]);
	_t *= Transformation::generate_2D_rotation_matrix(-_angle);
	_t *= Transformation::generate_2D_scaling_matrix(1/(_width/2), 1/(_heigth/2));
}

#endif