#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "coordinate.hpp"
#include "Transformation.hpp"

class Window {
	public:
		Window(double width, double height):
			_lowmin(0,0),
			_lowmax(width,0),
			_uppermin(0,height),
			_uppermax(width, height),
			_center(width/2, height/2),
			_angle(0),
			_width_scale(1/(width/2)),
			_heigth_scale(1/(height/2)),
			_t({ {1, 0, 0},
				 {0, 1, 0},
				 {0, 0, 1} })
		{}
		
		virtual ~Window() {} 

		void zoom(double step);
		void moveX(double value);
		void moveY(double value);

		Coordinate lowmin() const { return Coordinate(-1,-1); }
		Coordinate uppermax() const { return Coordinate(1,1); }
		Coordinate center() const { return _center; }
		double angle() const { return _angle; }
		
		void printteste() {
			printf("%f\n", _lowmin[0]);
		}

		Transformation& get_transformation() { return _t; }
		void update_transformation();

	protected:
	private:
		Coordinate _lowmin, _lowmax, _uppermin, _uppermax;
		Coordinate _center;
		double _angle; // radians
		double _width_scale, _heigth_scale;
		Transformation _t;
};

/* step > 0  -  Zoom in */
void Window::zoom(double step) {
	step /= 2;
	Coordinate coord_e(step, step);
	Coordinate coord_d(step, -step);

	_lowmin += coord_e;
	_uppermax -= coord_e;
	_lowmax -= coord_d;
	_uppermin += coord_d;

	_width_scale -= step;
	_heigth_scale -= step;
}

/* Move Window Horizontally */
void Window::moveX(double value) {
	Coordinate coord(value, 0);

	_lowmin += coord;
	_lowmax += coord;
	_uppermin += coord;
	_uppermax += coord;

	_center += coord;
}

/* Move Window Vertically */
void Window::moveY(double value) {
	Coordinate coord(0, value);

	_lowmin += coord;
	_lowmax += coord;
	_uppermin += coord;
	_uppermax += coord;

	_center += coord;
}

void Window::update_transformation() {
	_t = Transformation({ {1, 0, 0},
						  {0, 1, 0},
						  {0, 0, 1} });
	_t *= Transformation::generate_2D_translation_matrix(_center[0], _center[1]);
	_t *= Transformation::generate_2D_rotation_matrix(_angle);
	_t *= Transformation::generate_2D_scaling_matrix(_width_scale, _heigth_scale);
}

#endif