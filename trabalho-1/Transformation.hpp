#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

#include "coordinate.hpp"
#include <cmath>

#define PI 3.1415926535897932384626433832795

class Transformation {
	public:
		Transformation(const Matrix m):
			_m(m) {};

		~Transformation() {};

		Matrix& get_transformation_matrix() {
			return this->_m;
		};

		const Matrix& get_transformation_matrix() const {
			return this->_m;
		};

		static Transformation generate_2D_translation_matrix(double x, double y) {
			Matrix m = { {1, 0, 0},
						 {0, 1, 0},
						 {x, y, 1} };
			return Transformation(m);
		};

		static Transformation generate_2D_scaling_around_obj_center_matrix(double sx, double sy, const Coordinate& obj_center) {
			return generate_2D_translation_matrix(-obj_center[0],-obj_center[1]) * generate_2D_scaling_matrix(sx,sy) * generate_2D_translation_matrix(obj_center[0],obj_center[1]);
		};

		static Transformation generate_2D_rotation_around_point(double degrees, const Coordinate& coord) {
			return generate_2D_translation_matrix(-coord[0], -coord[1]) * generate_2D_rotation_matrix(degrees) * generate_2D_translation_matrix(coord[0], coord[1]);
		};

		Transformation& operator*=(const Transformation& other) {
			Matrix m1 = this->_m;
			const Matrix &m2 = other.get_transformation_matrix();
			int size = m1.size();
			if (size != m2.size())
				throw "Transformations have different dimensions";
			for (int i = 0; i < size; i++) {
				for (int j = 0; j < size; j++) {
					_m[i][j] = 0;
					for (int k = 0; k < size; k++) {
						_m[i][j] += m1[i][k] * m2[k][j];
					}
				}
			}
			return *this;
		}

		friend Transformation operator*(Transformation t1, const Transformation& t2) {
			t1 *= t2;
			return t1;
		}

		friend std::ostream& operator<<(std::ostream& os, const Transformation& t) {
    		const auto &m = t.get_transformation_matrix();
    		int i, j;
    		for (i = 0; i < m.size(); ++i) {
    			os << '[';
    			for (j = 0; j < m[i].size()-1; ++j) {
    				os << m[i][j] << ',';
    			}
    			os << m[i][j] << "]\n";
    		}
    		return os;
		};
	protected:
	private:
		Matrix _m;

		static double to_radians(double degrees) {
			return (PI/180) * degrees;
		};

		static Transformation generate_2D_scaling_matrix(double sx, double sy) {
			Matrix m = { {sx, 0, 0},
						 {0, sy, 0},
						 {0, 0, 1} };
			return Transformation(m);
		};

		static Transformation generate_2D_rotation_matrix(double degrees) {
			double rad = to_radians(degrees);

			Matrix m = { {cos(rad), -sin(rad), 0},
						 {sin(rad), cos(rad), 0},
						 {0, 0, 1} };
			return Transformation(m);
		}
};

#endif