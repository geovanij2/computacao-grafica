#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

#include "objects.cpp"

typedef std::vector<Coordinate> Matrix;

class Transformation {
	public:
		Transformation(int n) {
			if (n < 2 || n > 3)
				throw std::out_of_range("Transformations are only possible in 2 or 3 dimensions");
			this->_size = n;
		};

		~Transformation() {};

		Matrix& get_transformation_matrix() {
			return this->_m;
		};

		const Matrix& get_transformation_matrix() const {
			return this->_m;
		};

		void generate_translation_matrix(Coordinate coord) {
			for (int i = 0; i < this->_size; ++i)
				this->_m.push_back(Coordinate(this->_size));

			this->_m.push_back(coord);

			for (int i = 0; i < this->_size; ++i)
				this->_m[i][i] = 1;
		};

		friend std::ostream& operator<<(std::ostream& os, const Transformation& t) {
    		const auto &m = t.get_transformation_matrix();
    		for(int i=0; i < m.size(); i++)
    		    os << m[i] << ",\n";
            os << std::endl;
    		return os;
		};
	protected:
	private:
		Matrix _m;
		int _size;
};

#endif