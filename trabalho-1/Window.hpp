#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <cmath>
#include "coordinate.hpp"
#include "Transformation.hpp"

enum window_view { PARALLEL, PERSPECTIVE };

class Window {
	public:
		Window(double width, double height):
			_center(width/2, height/2, 0),
			_width(width),
			_heigth(height),
			_t({ {1, 0, 0, 0},
				 {0, 1, 0, 0},
				 {0, 0, 1, 0},
				 {0, 0, 0, 1} })
		{}
		
		virtual ~Window() {}

		double get_width() { return _width; }
		double get_height() { return _heigth; }

		double get_angle_x() { return _angle_x; }
		double get_angle_y() { return _angle_y; }
		double get_angle_z() { return _angle_z; }

		void rotate_x(double degrees) { _angle_x += degrees; }
		void rotate_y(double degrees) { _angle_y += degrees; }
		void rotate_z(double degrees) { _angle_z += degrees; }

		void zoom(double step);

		void moveX(double value);
		void moveY(double value);
		void moveZ(double value);

		void change_view(const window_view view) { _view = view; }
		void set_focal_distance(double d) { _d = d; }

		Coordinate lowmin() const { return Coordinate(-1,-1); }	 	  	 	     	  		  	  	    	      	 	
		Coordinate uppermax() const { return Coordinate(1,1); }
		Coordinate center() const { return _center; }

		Transformation& get_transformation() { return _t; }
		void update_transformation();

	protected:
	private:
		void move(double x, double y, double z) {
			Coordinate c(x,y,z);
			auto t = Transformation::generate_rotation_matrix(_angle_x, _angle_y, _angle_z);
			c.transform(t.get_transformation_matrix());
			_center += c;
		}

		Coordinate _center;
		double _angle_x = 0, _angle_y = 0, _angle_z = 0; // radians
		double _width, _heigth;
		double _d = 1000;
		window_view _view = window_view::PERSPECTIVE;
		Transformation _t;
};

/* step > 0  -  Zoom in */
void Window::zoom(double step) {
	_width -= step;
	_heigth -= step;
}

/* Move Window Horizontally */
void Window::moveX(double value) {
	move(value, 0, 0);
}

/* Move Window Vertically */
void Window::moveY(double value) {
	move(0, value, 0);
}	 	  	 	     	  		  	  	    	      	 	

void Window::moveZ(double value) {
	move(0, 0, value);
}

void Window::update_transformation() {
	_t = Transformation({ {1, 0, 0, 0},
						  {0, 1, 0, 0},
						  {0, 0, 1, 0},
						  {0, 0, 0, 1} });
	switch(_view) {
		case window_view::PERSPECTIVE:
			_t *= Transformation::generate_translation_matrix(-_center[0], -_center[1], -_center[2] + _d);
			_t *= Transformation::generate_rotation_matrix(-_angle_x, -_angle_y, -_angle_z);
			_t *= Transformation::generate_perspective_matrix(_d);
			_t *= Transformation::generate_scaling_matrix(1/(_width/2), 1/(_heigth/2), 4.0/(_width + _heigth));
			break;
		case window_view::PARALLEL:
			_t *= Transformation::generate_translation_matrix(-_center[0], -_center[1], -_center[2]);
			_t *= Transformation::generate_rotation_matrix(-_angle_x, -_angle_y, -_angle_z);
			_t *= Transformation::generate_scaling_matrix(1/(_width/2), 1/(_heigth/2), 4.0/(_width + _heigth));
	}
}

#endif
