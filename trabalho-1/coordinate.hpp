#ifndef COORDINATE_HPP
#define COORDINATE_HPP

#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>

typedef std::vector<std::vector<double>> Matrix;

class Coordinate: public std::vector<double> {
	public:
 		Coordinate(int n=3) {
 			for (int i = 0; i < 3; i++)
 				this->push_back(0);
 			this->push_back(1);
 		}

 		Coordinate(double x, double y, double z = 0) {
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
 					sum += this->at(j) * m[j][i];
 				}
 				res.push_back(sum);
 			} 	  	 	     	  		  	  	    	      	 	
 			for (int i = 0; i < res.size(); ++i) {
 				// bringing back to w = 1;
 				this->at(i) = res[i]/res[3];
 			}
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

#endif
