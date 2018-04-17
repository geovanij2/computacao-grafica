#ifndef OBJECTS_H
#define OBJECTS_H

#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>
#include "coordinate.hpp"
#include "Transformation.hpp"

typedef std::vector<Coordinate> Coordinates;
enum obj_type { OBJECT,
				POINT,
				LINE,
				POLYGON,
				CURVE,
				BEZIER_CURVE,
				BSPLINE_CURVE };

class Object {
	public:
		Object(const std::string name) : 
			_name(name)
		{}

		virtual ~Object() {};

		std::string get_name() {
			return _name;
		}

		const std::string get_name() const {
			return _name;
		}

		virtual obj_type get_type() const {
			return obj_type::OBJECT;
		}

		virtual std::string get_type_name() const {
			return "Object";
		}

		Coordinates& get_coords() {
			return _coords;
		}

		const Coordinates& get_coords() const {
			return _coords;
		}

		Coordinates& get_normalized_coords() {
			return _normalized_coords;
		}

		const Coordinates& get_normalized_coords() const {
			return _normalized_coords;
		}

		Coordinate& get_coord_at_index(int index) {
			return _coords[index];
		}

		const Coordinate& get_coord_at_index(int index) const {
			return _coords[index];
		}

		Coordinate& get_normalized_coord_at_index(int index) {
			return _normalized_coords[index];
		}

		const Coordinate& get_normalized_coord_at_index(int index) const {
			return _normalized_coords[index];
		}

		Coordinate get_center_coord() {
			int dim = _coords[0].size()-1;
			Coordinate sum(dim);
			for (int i = 0; i < _coords.size(); i++)
				sum += _coords[i];
			for (int i = 0; i < sum.size()-1; i++)
				sum[i] /= _coords.size();
			return sum;
		}

		Coordinate get_normalized_center_coord() {
			int dim = _normalized_coords[0].size()-1;
			Coordinate sum(dim);
			for (int i = 0; i < _normalized_coords.size(); i++)
				sum += _normalized_coords[i];
			for (int i = 0; i < sum.size()-1; i++)
				sum[i] /= _normalized_coords.size();
			return sum;
		}

		bool operator==(const Object& other) {
			return this->_name == other.get_name();
		}

		Object& operator*() {
			return *this;
		}

		void transform_coords(const Transformation& t) {
			Matrix m = t.get_transformation_matrix();
			for (int i = 0; i < _coords.size(); i++) {
				_coords[i].transform(m);
			}
		}

		void set_normalized_coords(const Transformation& t) {
			if (_normalized_coords.size() > 0)
				_normalized_coords.clear();
			Matrix m = t.get_transformation_matrix();
			for (int i = 0; i < _coords.size(); i++) {
				Coordinate normalized_coord = _coords[i];
				normalized_coord.transform(m);
				_normalized_coords.push_back(normalized_coord);
			}
		}

		void set_normalized_coords(const Coordinates& coords) {
			_normalized_coords = coords;
		}

		friend std::ostream& operator<<(std::ostream& os, const Object& obj) {
			os << obj.get_name() << ": [";
			for (auto i = obj._coords.begin(); i != obj._coords.end(); ++i)
				os << *i << ',';
			os << ']';
			os << ", normalizado: [";
			for (auto i = obj._normalized_coords.begin(); i != obj._normalized_coords.end(); ++i)
				os << *i << ',';
			os << ']';
			return os;
		}

		virtual bool isFilled() {return false;}

	protected:
		void add_coordinate(double x, double y) {
			_coords.emplace_back(x,y);
		}

		void add_coordinate(double x , double y, double z) {
			_coords.emplace_back(x,y,z);
		}

		void add_coordinate(const Coordinate& coord) {
			_coords.push_back(coord);
		}

		void add_coordinate(const Coordinates& coords) {
			_coords.insert(_coords.end(), coords.begin(), coords.end());
		}
	private:
		const std::string _name;
		Coordinates _coords;
		Coordinates _normalized_coords;
};

class Point : public Object {

	public:
		Point(std::string name, double x, double y) :
			Object(name)
		{
			this->add_coordinate(x,y);
		}

		Point(std::string name, Coordinate coord) :
			Object(name)
		{
			this->add_coordinate(coord);
		}

		virtual ~Point() {}

		virtual obj_type get_type() const {
			return obj_type::POINT;
		}

		virtual std::string get_type_name() const {
			return "Point";
		}
	protected:
	private:
};

class Line : public Object {
	public:
		Line(std::string name, double xi, double yi, double xf, double yf) :
			Object(name)
		{
			this->add_coordinate(xi,yi);
			this->add_coordinate(xf,yf);
		}

		Line(std::string name, Coordinate initial_coord, Coordinate final_coord) :
			Object(name)
		{
			this->add_coordinate(initial_coord);
			this->add_coordinate(final_coord);
		}

		Line(std::string name, Coordinates coords) :
			Object(name)
		{
			if (coords.size() != 2) {
				throw "vector size must be 2 to construct a line";
			}
			this->add_coordinate(coords);
		}

		~Line() {}

		virtual obj_type get_type() const {
			return obj_type::LINE;
		}

		virtual std::string get_type_name() const {
			return "Line";
		}
	protected:
	private:
};

class Polygon : public Object {
	public:
		Polygon(std::string name, Coordinates coords, bool fill) :
			Object(name),
			_filled(fill)
		{
			if (coords.size() < 3) {
				throw "Polygon must have at least 3 coordinates";
			}
			this->add_coordinate(coords);
		}

		virtual ~Polygon() {}

		virtual obj_type get_type() const {
			return obj_type::POLYGON;
		}

		virtual std::string getTypeName() const {
			return "Polygon";
		}

		virtual bool isFilled() {return _filled;}
	protected:
	private:
		bool _filled;
};

class Curve : public Object {
	public:
		Curve(std::string& name) :
			Object(name)
		{}

		virtual ~Curve() {}

		virtual obj_type get_type() const {
			return obj_type::CURVE;
		}

		virtual std::string getTypeName() const {
			return "Curve";
		}

		Coordinates& get_control_points() {
			return _control_points;
		}

		virtual void generate_curve() {}
	protected:

		void set_control_points(const Coordinates& coords) {
			_control_points.insert(_control_points.end(), coords.begin(), coords.end());
		};

		Coordinates _control_points;
		double _step = 0.02;

};

class BezierCurve : public Curve {
	public:
		BezierCurve(std::string name, const Coordinates& coords) :
			Curve(name)
		{
			set_control_points(coords);
			generate_curve();
		}

		virtual ~BezierCurve() {}

		virtual obj_type get_type() const {
			return obj_type::BEZIER_CURVE;
		}

		virtual std::string getTypeName() const {
			return "Bezier Curve";
		}

		virtual void generate_curve() {

			int num_curves = ((_control_points.size() - 4)/3) + 1;

			for (int i = 0; i < num_curves; i++) {
				for (double t = 0; t < 1; t += _step) {
					double t2 = t * t;
					double t3 = t2 * t;

					double x = (-t3 +3*t2 -3*t + 1) * _control_points[i*3+0][0] 
								+ (3*t3 -6*t2 +3*t) * _control_points[i*3+1][0]
								+ (-3*t3 +3*t2) * _control_points[i*3+2][0]
								+ (t3) * _control_points[i*3+3][0];

					double y = (-t3 +3*t2 -3*t + 1) * _control_points[i*3+0][1] 
								+ (3*t3 -6*t2 +3*t) * _control_points[i*3+1][1]
								+ (-3*t3 +3*t2) * _control_points[i*3+2][1]
								+ (t3) * _control_points[i*3+3][1];

					add_coordinate(x,y);
				}
			}
		}
	protected:
	private:
};

class BsplineCurve : public Curve {
	public:
		BsplineCurve(std::string name, const Coordinates& coords) :
			Curve(name)
		{
			set_control_points(coords);
			generate_curve();
		}

		virtual ~BsplineCurve() {}

		virtual obj_type get_type() const {
			return obj_type::BSPLINE_CURVE;
		}

		virtual std::string getTypeName() const {
			return "B-spline Curve";
		}

		virtual void generate_curve() {
			int num_curves = _control_points.size() - 3;

			double t = _step;
			double t2 = t * _step;
			double t3 = t2 * _step;

			double n16 = 1.0/6.0;
			double n23 = 2.0/3.0;


			for (int i = 0; i < num_curves; i++) {
				auto c1 = _control_points[i];
				auto c2 = _control_points[i+1];
				auto c3 = _control_points[i+2];
				auto c4 = _control_points[i+3];

				double ax = -n16 * c1[0] +0.5 * c2[0] -0.5 * c3[0] +n16 * c4[0];
				double bx =  0.5 * c1[0] 	   -c2[0] +0.5 * c3[0];
				double cx = -0.5 * c1[0] 			  +0.5 * c3[0];
				double dx =  n16 * c1[0] +n23 * c2[0] +n16 * c3[0];

				double delta_x  = ax * t3 + bx * t2 + cx * t;
				double delta_x3 = ax * (6 * t3);
				double delta_x2 = delta_x3 +bx * (2 * t2);

				double ay = -n16 * c1[1] +0.5 * c2[1] -0.5 * c3[1] +n16 * c4[1];
				double by =  0.5 * c1[1] 	   -c2[1] +0.5 * c3[1];
				double cy = -0.5 * c1[1] 			  +0.5 * c3[1];
				double dy =  n16 * c1[1] +n23 * c2[1] +n16 * c3[1];

				double delta_y  = ay * t3 + by * t2 + cy * t;
				double delta_y3 = ay * (6 * t3);
				double delta_y2 = delta_y3 +by * (2 * t2);

				double vx = dx, vy = dy;
				add_coordinate(vx, vy);
				for (double t = 0.0; t < 1.0; t += _step) {
					double x = vx, y = vy;

					x += delta_x;
					delta_x += delta_x2;
					delta_x2 += delta_x3;

					y += delta_y;
					delta_y += delta_y2;
					delta_y2 += delta_y3;

					add_coordinate(x,y);
					vx = x;
					vy = y;
				}
			}
		}
	protected:
	private:
};

#endif
