#ifndef OBJECTS_H
#define OBJECTS_H

#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>
#include "Transformation.hpp"

class Coordinate: public std::vector<double> {
	public:
 		Coordinate(int n) {
 			if (n < 2 || n > 3)
 				throw std::out_of_range("Coodinate must be 2 or 3 dimensional");
 			for (int i = 0; i < n; i++)
 				this->push_back(0);
 			this->push_back(1);
 		}

 		Coordinate(double x, double y) {
 			this->push_back(x);
 			this->push_back(y);
 			this->push_back(1);
 		}

 		Coordinate(double x, double y, double z) {
 			this->push_back(x);
 			this->push_back(y);
 			this->push_back(z);
 			this->push_back(1);
 		}

 		void transform(Matrix& m) {
 			if (m.size() != this->size()) {
 				throw "dimensional error";
 			}
 			std::vector<double> res;
 			for (int i = 0; i < this->size(); ++i) {
 				double sum = 0;
 				for (int j = 0; j < m.size(); ++j) {
 					sum += this->at(i) * m[j][i];
 				}
 				res.push_back(sum);
 			}
 			for (int i = 0; i < res.size(); ++i)
 				this->at(i) = res[i];
 		}

 		Coordinate& operator+=(const Coordinate& other) {
 			if (this->size() != other.size())
 				throw std::out_of_range("Different dimensions coodinates");
            for (int i = 0; i < this->size()-1; ++i)
                this->at(i) = this->at(i) + other[i];
            return *this;
 		};
 		
 		friend Coordinate operator+(Coordinate lhs, const Coordinate& rhs) {
 			lhs += rhs;
 			return lhs;
 		};

 		Coordinate& operator-=(const Coordinate& other) {
 			if (this->size() != other.size())
 				throw std::out_of_range("Different dimensions coodinates");
            for (int i = 0; i < this->size()-1; ++i)
                this->at(i) = this->at(i) - other[i];
            return *this;
 		};
 		
 		friend Coordinate operator-(Coordinate lhs, const Coordinate& rhs) {
 			lhs -= rhs;
 			return lhs;
 		};

		bool operator==(const Coordinate& other) {
			if (this->size() != other.size())
				return false;
			for (int i = 0; i < this->size(); ++i) {
				if (this->at(i) != other[i])
					return false;
			}
			return true;
		};

		friend std::ostream& operator<<(std::ostream& os, const Coordinate& coord) {
			os << '[';
			int i;
			for (i = 0; i < coord.size()-1; ++i)
				os << coord[i] << ',';
			os << coord[i] << ']';
			return os;
		};

 		virtual ~Coordinate() {};
 	protected:
 	private:
};

typedef std::vector<Coordinate> Coordinates;
enum obj_type { OBJECT, POINT, LINE, POLYGON };

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

		Coordinate& get_coord_at_index(int index) {
			return _coords[index];
		}

		const Coordinate& get_coord_at_index(int index) const {
			return _coords[index];
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

		friend std::ostream& operator<<(std::ostream& os, const Object& obj) {
			os << obj.get_name() << ": [";
			for (auto i = obj._coords.begin(); i != obj._coords.end(); ++i)
				os << *i << ',';
			os << ']';
			return os;
		}

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
		Polygon(std::string name, Coordinates coords) :
			Object(name)
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
	protected:
	private:
};

#endif