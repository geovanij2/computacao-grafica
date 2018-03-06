#ifndef OBJECTS_H
#define OBJECTS_H

#include <string>
#include <vector>

class Coordinate {
	public:
 		Coordinate() :
 			x(0),
 			y(0)
 		{}

 		Coordinate(double x, double y) :
 			x(x),
 			y(y)
 		{}

 		virtual ~Coordinate() {};

 		Coordinate& operator+=(const Coordinate& other) {
 			this->x += other.x;
 			this->y += other.y;
 			return *this;
 		}

 		Coordinate& operator-=(const Coordinate& other) {
 			this->x -= other.x;
 			this->y -= other.y;
 			return *this;
 		}

 		bool operator==(const Coordinate& other) {
 			return (this->x == other.x && this->y == other.y);
 		}

 		double x, y;
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
			return "OBJECT";
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

	protected:

	private:
		std::string _name;
		Coordinates _coords;
};

#endif