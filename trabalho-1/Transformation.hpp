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

		static Transformation generate_translation_matrix(double x, double y, double z) {
			Matrix m = { {1, 0, 0, 0},
						 {0, 1, 0, 0},
						 {0, 0, 1, 0},
						 {x, y, z, 1} };
			return Transformation(m);
		};

		static Transformation generate_scaling_around_obj_center_matrix(double sx, double sy, double sz, const Coordinate& obj_center) {
			return generate_translation_matrix(-obj_center[0],-obj_center[1],-obj_center[2])
				* generate_scaling_matrix(sx,sy,sz)
				* generate_translation_matrix(obj_center[0],obj_center[1],obj_center[2]);
		};

		static Transformation generate_full_rotation(double angle_x, double angle_y, double angle_z, double angle_a, const Coordinate& p, const Coordinate& v) {	 	  	 	     	  		  	  	    	      	 	
			return generate_rx(angle_x) * generate_ry(angle_y) * generate_rz(angle_z) * generate_ra(angle_a, p, v);
		};

		static double to_radians(double degrees) {
			return (PI/180) * degrees;
		};

		static Transformation generate_scaling_matrix(double sx, double sy, double sz) {
			Matrix m = { {sx, 0, 0, 0},
						 {0, sy, 0, 0},
						 {0, 0, sz, 0},
						 {0, 0,  0, 1} };
			return Transformation(m);
		};

		static Transformation generate_rx(double angle_x, bool isRad=false) {
			double rad = isRad ? angle_x : -to_radians(angle_x);

			Matrix m = { {1,  	 0,			0,	  0},
						 {0,  cos(rad), sin(rad), 0},
						 {0, -sin(rad), cos(rad), 0},
						 {0,  	 0,			0,	  1} };

			return Transformation(m);
		}

		static Transformation generate_ry(double angle_y, bool isRad=false) {
			double rad = isRad ? angle_y : -to_radians(angle_y);

			Matrix m = { {cos(rad), 0, -sin(rad), 0},
						 {	  0,	1,		0,	  0},
						 {sin(rad), 0,  cos(rad), 0},
						 {	  0,	0,		0,	  1} };

			return Transformation(m);
		}	 	  	 	     	  		  	  	    	      	 	

		static Transformation generate_rz(double angle_z, bool isRad=false) {
			double rad = isRad ? angle_z : -to_radians(angle_z);

			Matrix m = { { cos(rad), sin(rad), 0, 0},
						 {-sin(rad), cos(rad), 0, 0},
						 {	  0,		0,	   1, 0},
						 {	  0,		0,	   0, 1} };

			return Transformation(m);
		}

		static Transformation generate_ra(double angle_a, const Coordinate& p, const Coordinate& v) {
    		// known vectors
    		if (v[0] == 0 && v[1] == 0 && v[2] == 0) {
    			Matrix m = { {1, 0, 0, 0},
    						 {0, 1, 0, 0},
    						 {0, 0, 1, 0},
    						 {0, 0, 0, 1} };
    			return Transformation(m);
    		}
    		//eixo y
    		else if (v[0] == 0 && v[2] == 0) {
    			return generate_translation_matrix(-p[0], -p[1], -p[2])
    				* generate_ry(angle_a, false)
    				* generate_translation_matrix(p[0], p[1], p[2]);
    		//eixo z
    		} else if (v[0] == 0 && v[1] == 0) {
    			return generate_translation_matrix(-p[0], -p[1], -p[2])
    				* generate_rz(angle_a, false)
    				* generate_translation_matrix(p[0], p[1], p[2]);
    		//eixo x
    		} else if (v[1] == 0 && v[2] == 0) {
    			return generate_translation_matrix(-p[0], -p[1], -p[2])
    				* generate_rx(angle_a, false)
    				* generate_translation_matrix(p[0], p[1], p[2]);
    		}
    		double d = sqrt(v[0] * v[0] + v[2] * v[2]);
    		double beta = atan(v[0]/v[2]);
    		double alpha = atan(v[1]/d);

			return generate_translation_matrix(-p[0],-p[1],-p[2])
				* generate_ry(-beta, true)
				* generate_rx(alpha, true)
				* generate_rz(angle_a)
				* generate_rx(-alpha, true)
				* generate_ry(beta, true)
				* generate_translation_matrix(p[0],p[1],p[2]);
		}
		

		static Transformation generate_rotation_matrix(double angle_x, double angle_y, double angle_z) {
			return generate_rx(angle_x) * generate_ry(angle_y) * generate_rz(angle_z);
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
};

#endif
	 	  	 	     	  		  	  	    	      	 	
