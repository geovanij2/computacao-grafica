#ifndef OBJECTS_H
#define OBJECTS_H

#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>
#include "coordinate.hpp"
#include "Transformation.hpp"

typedef std::vector<Coordinate> Coordinates;
typedef std::vector<std::vector<Coordinate>> control_matrix;

enum obj_type { OBJECT,
				POINT,
				LINE,
				POLYGON,
				CURVE,
				BEZIER_CURVE,
				BSPLINE_CURVE,
				OBJECT_3D,
				BEZIER_SURFACE,
				BSPLINE_SURFACE };

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

		virtual Coordinate get_center_coord() {
			Coordinate sum(3);
			for (int i = 0; i < _coords.size(); i++)
				sum += _coords[i];
			for (int i = 0; i < sum.size()-1; i++)
				sum[i] /= _coords.size();
			return sum;
		}

		virtual Coordinate get_normalized_center_coord() {
			Coordinate sum(3);
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

		virtual void transform_coords(const Transformation& t) {
			Matrix m = t.get_transformation_matrix();
			for (int i = 0; i < _coords.size(); i++) {
				_coords[i].transform(m);
			}	 	  	 	     	  		  	  	    	      	 	
		}

		virtual void set_normalized_coords(const Transformation& t) {
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

		virtual bool isFilled() { return false; }

		void add_coordinate(const Coordinate& coord) {	 	  	 	     	  		  	  	    	      	 	
			_coords.push_back(coord);
		}

	protected:
		void add_coordinate(double x , double y, double z) {
			_coords.emplace_back(x,y,z);
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
		Point(std::string name, double x, double y, double z) :
			Object(name)
		{
			this->add_coordinate(x,y,z);
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
		Line(std::string name,
			 double xi, double yi, double zi,
			 double xf, double yf, double zf) :
			Object(name)
		{
			this->add_coordinate(xi, yi, zi);
			this->add_coordinate(xf, yf, zf);
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
		BezierCurve(std::string name) :
			Curve(name)
		{}

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

					double z = (-t3 +3*t2 -3*t + 1) * _control_points[i*3+0][2] 
								+ (3*t3 -6*t2 +3*t) * _control_points[i*3+1][2]
								+ (-3*t3 +3*t2) * _control_points[i*3+2][2]
								+ (t3) * _control_points[i*3+3][2];

					add_coordinate(x,y,z);
				}	 	  	 	     	  		  	  	    	      	 	
			}
		}
	protected:
	private:
};

class BsplineCurve : public Curve {
	public:
	    BsplineCurve(std::string name) :
	        Curve(name)
	    {}
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

				double az = -n16 * c1[2] +0.5 * c2[2] -0.5 * c3[2] +n16 * c4[2];
				double bz =  0.5 * c1[2] 	   -c2[2] +0.5 * c3[2];
				double cz = -0.5 * c1[2] 			  +0.5 * c3[2];
				double dz =  n16 * c1[2] +n23 * c2[2] +n16 * c3[2];

				double delta_z  = az * t3 + bz * t2 + cz * t;
				double delta_z3 = az * (6 * t3);
				double delta_z2 = delta_z3 +bz * (2 * t2);

				double vx = dx, vy = dy, vz = dz;
				add_coordinate(vx, vy, vz);
				for (double t = 0.0; t < 1.0; t += _step) {
					double x = vx, y = vy, z = vz;

					x += delta_x;
					delta_x += delta_x2;
					delta_x2 += delta_x3;

					y += delta_y;
					delta_y += delta_y2;
					delta_y2 += delta_y3;

					z += delta_z;
					delta_z += delta_z2;
					delta_z2 += delta_z3;

					add_coordinate(x,y,z);
					vx = x;
					vy = y;
					vz = z;
				}	 	  	 	     	  		  	  	    	      	 	
			}
		}
	protected:
	private:
};

typedef std::vector<Polygon> face_list;

class Object3D : public Object {
	public:
		Object3D(const std::string name) :
			Object(name)
		{}

		Object3D(const std::string name, const face_list& faces) :
			Object(name)
		{
			for (int i = 0; i < faces.size(); ++i) {
			    _faces.push_back(faces[i]);   
			}
		}
		virtual ~Object3D() {};
	
		virtual obj_type get_type() const {
			return obj_type::OBJECT_3D;
		}

		virtual std::string get_type_name() const {
			return "3D Object";
		}

		face_list& get_face_list() {
			return this->_faces;
		}

		void insert_faces(const face_list& faces) {	 	  	 	     	  		  	  	    	      	 	
			for (int i = 0; i < faces.size(); ++i) {
			    _faces.push_back(faces[i]);   
			}
		}

		virtual Coordinate get_center_coord() {
			Coordinate sum(3);
			int n = 0;
			for (auto &face : _faces) {
				for (auto &coord : face.get_coords()) {
					sum += coord;
					
				}
				n += face.get_coords().size();
				
			}
			
			sum[0] /= n;
			sum[1] /= n;
			sum[2] /= n;
			return sum;
		}

		virtual Coordinate get_normalized_center_coord() {
			Coordinate sum(3);
			int n = 0;
			for (auto &face : _faces) {
				for (auto &coord : face.get_normalized_coords())
					sum += coord;
				n += face.get_normalized_coords().size();
			}
			sum[0] /= n;
			sum[1] /= n;
			sum[2] /= n;
			return sum;
		}

		virtual void transform_coords(const Transformation& t) {
			Matrix m = t.get_transformation_matrix();

			for (auto &face : _faces) {	 	  	 	     	  		  	  	    	      	 	
				for (auto &coord : face.get_coords()) {
					coord.transform(m);
				}
			}
		}

		virtual void set_normalized_coords(const Transformation& t) {
			Matrix m = t.get_transformation_matrix();

			for (auto &face : _faces) {
				auto &coords = face.get_normalized_coords();
				if (coords.size() > 0)
					coords.clear();
				for (auto coord : face.get_coords()) {
				    coord.transform(m);
					coords.push_back(coord);
				}
			}
		}
	protected:
	private:
		face_list _faces;
};

typedef std::vector<Curve> curve_list;

class Surface : public Object
{
    public:
        Surface(const std::string& name) :
            Object(name) {}

        Surface(const std::string& name, int maxLines, int maxCols) :
            Object(name)
			{
				m_maxLines = maxLines;
				m_maxCols = maxCols;
			}

		Surface(const std::string& name, const Coordinates& coords) :
            Object(name) 
		{}

        virtual void generate_surface(const Coordinates& cpCoords){};

        Coordinates& get_control_points(){ return m_controlPoints; }

        virtual void transform_coords(const Transformation& t) {
			Matrix m = t.get_transformation_matrix();

			for (auto &curve : m_curveList) {
				for (auto &coord : curve.get_coords()) {
					coord.transform(m);
				}
			}
		}

		virtual void set_normalized_coords(const Transformation& t) {
			Matrix m = t.get_transformation_matrix();

			for (auto &curve : m_curveList) {
				auto &coords = curve.get_normalized_coords();
				if (coords.size() > 0)
					coords.clear();
				for (auto coord : curve.get_coords()) {
				    coord.transform(m);
					coords.push_back(coord);
				}
			}
		}

		virtual Coordinate get_center_coord() {
			Coordinate sum(3);
			int n = 0;
			for (auto &curve : m_curveList) {
				for (auto &coord : curve.get_coords()) {
					sum += coord;	
				}
				n += curve.get_coords().size();		
			}
			
			sum[0] /= n;
			sum[1] /= n;
			sum[2] /= n;
			return sum;
		}

		virtual Coordinate get_normalized_center_coord() {
			Coordinate sum(3);
			int n = 0;
			for (auto &curve : m_curveList) {
				for (auto &coord : curve.get_normalized_coords())
					sum += coord;
				n += curve.get_normalized_coords().size();
			}
			sum[0] /= n;
			sum[1] /= n;
			sum[2] /= n;
			return sum;
		}

		curve_list& get_curve_list() {
			return m_curveList;
		}

        int getMaxLines(){ return m_maxLines; }
        int getMaxCols(){ return m_maxCols; }

    protected:
        void setControlPoints(const Coordinates& coords) {
			m_controlPoints.insert(m_controlPoints.end(), coords.begin(), coords.end());
		}

    protected:
            //Guarda os pontos de controle da surface
            // para serem usados na hora de salvar a surface no .obj
            Coordinates m_controlPoints;
            float m_step = 0.05; //Passo usado na bleding function

            int m_maxLines = 4, m_maxCols = 4;
            curve_list m_curveList;
};

//http://www.cad[2]ju.edu.cn/home/zhx/GM/005/00-bcs2.pdf
class BezierSurface : public Surface
{
    public:
        BezierSurface(const std::string& name) :
            Surface(name)
		{}
		BezierSurface(const std::string& name, int maxLines, int maxCols, const Coordinates& coords) :
            Surface(name,maxLines,maxCols)
		{
			generateSurface(coords);
		}
        BezierSurface(const std::string& name, const Coordinates& coords) :
            Surface(name)
		{
			generateSurface(coords);
		}

        virtual obj_type get_type() const { return obj_type::BEZIER_SURFACE; }
		virtual std::string get_type_name() const { return "Bezier Surface"; }

		void generateSurface(const Coordinates& cpCoords) {
			if(m_controlPoints.size() != 0)
				return;

			setControlPoints(cpCoords);
			const auto& coords = m_controlPoints;

			int tmp3xMaxLines = 3*m_maxLines,
				tmp3xMaxCols = 3*m_maxCols;
			for(int nLine = 0; (m_maxLines != 4 && nLine <= tmp3xMaxLines) ||
					(nLine < tmp3xMaxLines); nLine += tmp3xMaxCols){
				for(int nCol = 0; nCol < m_maxCols-1; nCol += 3){

					for(float s = 0.0; s <= 1.0; s += m_step){
						double s2 = s * s;
						double s3 = s2 * s;

						BezierCurve curve("curve"+std::to_string(s));
						for(float t = 0.0; t <= 1.0; t += m_step){
							double t2 = t * t;
							double t3 = t2 * t;

							curve.add_coordinate(blendingFunction(s,s2,s3,t,t2,t3,nLine,nCol,coords));
						}
						m_curveList.push_back(curve);
					}

					for(float t = 0.0; t <= 1.0; t += m_step){
						double t2 = t * t;
						double t3 = t2 * t;

						BezierCurve curve("curve"+std::to_string(t));
						for(float s = 0.0; s <= 1.0; s += m_step){
							double s2 = s * s;
							double s3 = s2 * s;

							curve.add_coordinate(blendingFunction(s,s2,s3,t,t2,t3,nLine,nCol,coords));
						}
						m_curveList.push_back(curve);
					}
				}
			}
		}
        Coordinate blendingFunction(float s, double s2, double s3, float t, double t2, double t3, int nLine, int nCol, const Coordinates& coords) {
			double x = (-((-s3 +3*s2 -3*s +1)*coords[0+nLine+nCol][0] +(3*s3 -6*s2 +3*s)*coords[m_maxCols+nLine+nCol][0]   +(-3*s3 +3*s2)*coords[m_maxCols*2+nLine+nCol][0]   +(s3)*coords[m_maxCols*3+nLine+nCol][0]) +3*((-s3 +3*s2 -3*s +1)*coords[1+nLine+nCol][0] +(3*s3 -6*s2 +3*s)*coords[m_maxCols+1+nLine+nCol][0] +(-3*s3 +3*s2)*coords[m_maxCols*2+1+nLine+nCol][0] +(s3)*coords[m_maxCols*3+1+nLine+nCol][0]) -3*((-s3 +3*s2 -3*s +1)*coords[2+nLine+nCol][0] +(3*s3 -6*s2 +3*s)*coords[m_maxCols+2+nLine+nCol][0] +(-3*s3 +3*s2)*coords[m_maxCols*2+2+nLine+nCol][0] +(s3)*coords[m_maxCols*3+2+nLine+nCol][0]) +((-s3 +3*s2 -3*s +1)*coords[3+nLine+nCol][0] +(3*s3 -6*s2 +3*s)*coords[m_maxCols+3+nLine+nCol][0] +(-3*s3 +3*s2)*coords[m_maxCols*2+3+nLine+nCol][0] +(s3)*coords[m_maxCols*3+3+nLine+nCol][0]))*t3
						+(3*((-s3 +3*s2 -3*s +1)*coords[0+nLine+nCol][0] +(3*s3 -6*s2 +3*s)*coords[m_maxCols+nLine+nCol][0]   +(-3*s3 +3*s2)*coords[m_maxCols*2+nLine+nCol][0]   +(s3)*coords[m_maxCols*3+nLine+nCol][0]) -6*((-s3 +3*s2 -3*s +1)*coords[1+nLine+nCol][0] +(3*s3 -6*s2 +3*s)*coords[m_maxCols+1+nLine+nCol][0] +(-3*s3 +3*s2)*coords[m_maxCols*2+1+nLine+nCol][0] +(s3)*coords[m_maxCols*3+1+nLine+nCol][0]) +3*((-s3 +3*s2 -3*s +1)*coords[2+nLine+nCol][0] +(3*s3 -6*s2 +3*s)*coords[m_maxCols+2+nLine+nCol][0] +(-3*s3 +3*s2)*coords[m_maxCols*2+2+nLine+nCol][0] +(s3)*coords[m_maxCols*3+2+nLine+nCol][0]))*t2
						+(-3*((-s3 +3*s2 -3*s +1)*coords[0+nLine+nCol][0] +(3*s3 -6*s2 +3*s)*coords[m_maxCols+nLine+nCol][0]   +(-3*s3 +3*s2)*coords[m_maxCols*2+nLine+nCol][0]   +(s3)*coords[m_maxCols*3+nLine+nCol][0]) +3*((-s3 +3*s2 -3*s +1)*coords[1+nLine+nCol][0] +(3*s3 -6*s2 +3*s)*coords[m_maxCols+1+nLine+nCol][0] +(-3*s3 +3*s2)*coords[m_maxCols*2+1+nLine+nCol][0] +(s3)*coords[m_maxCols*3+1+nLine+nCol][0]))*t
						+(((-s3 +3*s2 -3*s +1)*coords[0+nLine+nCol][0] +(3*s3 -6*s2 +3*s)*coords[m_maxCols+nLine+nCol][0]   +(-3*s3 +3*s2)*coords[m_maxCols*2+nLine+nCol][0]   +(s3)*coords[m_maxCols*3+nLine+nCol][0]));

			double y = (-((-s3 +3*s2 -3*s +1)*coords[0+nLine+nCol][1] +(3*s3 -6*s2 +3*s)*coords[m_maxCols+nLine+nCol][1]   +(-3*s3 +3*s2)*coords[m_maxCols*2+nLine+nCol][1]   +(s3)*coords[m_maxCols*3+nLine+nCol][1]) +3*((-s3 +3*s2 -3*s +1)*coords[1+nLine+nCol][1] +(3*s3 -6*s2 +3*s)*coords[m_maxCols+1+nLine+nCol][1] +(-3*s3 +3*s2)*coords[m_maxCols*2+1+nLine+nCol][1] +(s3)*coords[m_maxCols*3+1+nLine+nCol][1]) -3*((-s3 +3*s2 -3*s +1)*coords[2+nLine+nCol][1] +(3*s3 -6*s2 +3*s)*coords[m_maxCols+2+nLine+nCol][1] +(-3*s3 +3*s2)*coords[m_maxCols*2+2+nLine+nCol][1] +(s3)*coords[m_maxCols*3+2+nLine+nCol][1]) +((-s3 +3*s2 -3*s +1)*coords[3+nLine+nCol][1] +(3*s3 -6*s2 +3*s)*coords[m_maxCols+3+nLine+nCol][1] +(-3*s3 +3*s2)*coords[m_maxCols*2+3+nLine+nCol][1] +(s3)*coords[m_maxCols*3+3+nLine+nCol][1]))*t3
						+(3*((-s3 +3*s2 -3*s +1)*coords[0+nLine+nCol][1] +(3*s3 -6*s2 +3*s)*coords[m_maxCols+nLine+nCol][1]   +(-3*s3 +3*s2)*coords[m_maxCols*2+nLine+nCol][1]   +(s3)*coords[m_maxCols*3+nLine+nCol][1]) -6*((-s3 +3*s2 -3*s +1)*coords[1+nLine+nCol][1] +(3*s3 -6*s2 +3*s)*coords[m_maxCols+1+nLine+nCol][1] +(-3*s3 +3*s2)*coords[m_maxCols*2+1+nLine+nCol][1] +(s3)*coords[m_maxCols*3+1+nLine+nCol][1]) +3*((-s3 +3*s2 -3*s +1)*coords[2+nLine+nCol][1] +(3*s3 -6*s2 +3*s)*coords[m_maxCols+2+nLine+nCol][1] +(-3*s3 +3*s2)*coords[m_maxCols*2+2+nLine+nCol][1] +(s3)*coords[m_maxCols*3+2+nLine+nCol][1]))*t2
						+(-3*((-s3 +3*s2 -3*s +1)*coords[0+nLine+nCol][1] +(3*s3 -6*s2 +3*s)*coords[m_maxCols+nLine+nCol][1]   +(-3*s3 +3*s2)*coords[m_maxCols*2+nLine+nCol][1]   +(s3)*coords[m_maxCols*3+nLine+nCol][1]) +3*((-s3 +3*s2 -3*s +1)*coords[1+nLine+nCol][1] +(3*s3 -6*s2 +3*s)*coords[m_maxCols+1+nLine+nCol][1] +(-3*s3 +3*s2)*coords[m_maxCols*2+1+nLine+nCol][1] +(s3)*coords[m_maxCols*3+1+nLine+nCol][1]))*t
						+(((-s3 +3*s2 -3*s +1)*coords[0+nLine+nCol][1] +(3*s3 -6*s2 +3*s)*coords[m_maxCols+nLine+nCol][1]   +(-3*s3 +3*s2)*coords[m_maxCols*2+nLine+nCol][1]   +(s3)*coords[m_maxCols*3+nLine+nCol][1]));
						
			double z = (-((-s3 +3*s2 -3*s +1)*coords[0+nLine+nCol][2] +(3*s3 -6*s2 +3*s)*coords[m_maxCols+nLine+nCol][2]   +(-3*s3 +3*s2)*coords[m_maxCols*2+nLine+nCol][2]   +(s3)*coords[m_maxCols*3+nLine+nCol][2]) +3*((-s3 +3*s2 -3*s +1)*coords[1+nLine+nCol][2] +(3*s3 -6*s2 +3*s)*coords[m_maxCols+1+nLine+nCol][2] +(-3*s3 +3*s2)*coords[m_maxCols*2+1+nLine+nCol][2] +(s3)*coords[m_maxCols*3+1+nLine+nCol][2]) -3*((-s3 +3*s2 -3*s +1)*coords[2+nLine+nCol][2] +(3*s3 -6*s2 +3*s)*coords[m_maxCols+2+nLine+nCol][2] +(-3*s3 +3*s2)*coords[m_maxCols*2+2+nLine+nCol][2] +(s3)*coords[m_maxCols*3+2+nLine+nCol][2]) +((-s3 +3*s2 -3*s +1)*coords[3+nLine+nCol][2] +(3*s3 -6*s2 +3*s)*coords[m_maxCols+3+nLine+nCol][2] +(-3*s3 +3*s2)*coords[m_maxCols*2+3+nLine+nCol][2] +(s3)*coords[m_maxCols*3+3+nLine+nCol][2]))*t3
						+(3*((-s3 +3*s2 -3*s +1)*coords[0+nLine+nCol][2] +(3*s3 -6*s2 +3*s)*coords[m_maxCols+nLine+nCol][2]   +(-3*s3 +3*s2)*coords[m_maxCols*2+nLine+nCol][2]   +(s3)*coords[m_maxCols*3+nLine+nCol][2]) -6*((-s3 +3*s2 -3*s +1)*coords[1+nLine+nCol][2] +(3*s3 -6*s2 +3*s)*coords[m_maxCols+1+nLine+nCol][2] +(-3*s3 +3*s2)*coords[m_maxCols*2+1+nLine+nCol][2] +(s3)*coords[m_maxCols*3+1+nLine+nCol][2]) +3*((-s3 +3*s2 -3*s +1)*coords[2+nLine+nCol][2] +(3*s3 -6*s2 +3*s)*coords[m_maxCols+2+nLine+nCol][2] +(-3*s3 +3*s2)*coords[m_maxCols*2+2+nLine+nCol][2] +(s3)*coords[m_maxCols*3+2+nLine+nCol][2]))*t2
						+(-3*((-s3 +3*s2 -3*s +1)*coords[0+nLine+nCol][2] +(3*s3 -6*s2 +3*s)*coords[m_maxCols+nLine+nCol][2]   +(-3*s3 +3*s2)*coords[m_maxCols*2+nLine+nCol][2]   +(s3)*coords[m_maxCols*3+nLine+nCol][2]) +3*((-s3 +3*s2 -3*s +1)*coords[1+nLine+nCol][2] +(3*s3 -6*s2 +3*s)*coords[m_maxCols+1+nLine+nCol][2] +(-3*s3 +3*s2)*coords[m_maxCols*2+1+nLine+nCol][2] +(s3)*coords[m_maxCols*3+1+nLine+nCol][2]))*t
						+(((-s3 +3*s2 -3*s +1)*coords[0+nLine+nCol][2] +(3*s3 -6*s2 +3*s)*coords[m_maxCols+nLine+nCol][2]   +(-3*s3 +3*s2)*coords[m_maxCols*2+nLine+nCol][2]   +(s3)*coords[m_maxCols*3+nLine+nCol][2]));

			return Coordinate(x,y,z);
		}
};

class BSplineSurface : public Surface
{
    public:
        BSplineSurface(const std::string& name) :
            Surface(name) {}
        BSplineSurface(const std::string& name, int maxLines, int maxCols, const Coordinates& coords) :
            Surface(name,maxLines,maxCols) { generateSurface(coords); }
		BSplineSurface(const std::string& name, const Coordinates& coords) :
            Surface(name) { generateSurface(coords); }

        virtual obj_type get_type() const { return obj_type::BSPLINE_SURFACE; }
		virtual std::string get_type_name() const { return "B-Spline Surface"; }

		void generateSurface(const Coordinates& cpCoords) {
			if(m_controlPoints.size() != 0)
				return;

			setControlPoints(cpCoords);
			const auto& coords = m_controlPoints;

			double n16 = 1.0/6.0;
			double n23 = 2.0/3.0;

			for(int nLine = 0; (m_maxLines != 4 && nLine <= m_maxLines) ||
						(nLine < m_maxLines); nLine += m_maxCols){
				for(int nCol = 0; nCol <= m_maxCols-4; nCol += 1){

					for(float s = 0.0; s <= 1.0; s += m_step){
						double s2 = s * s;
						double s3 = s2 * s;

						BsplineCurve curve("curve"+std::to_string(s));
						for(float t = 0.0; t <= 1.0; t += m_step){
							double t2 = t * t;
							double t3 = t2 * t;

							curve.add_coordinate(blendingFunction(s,s2,s3,t,t2,t3,n16,n23,nLine,nCol,coords));
						}
						m_curveList.push_back(curve);
					}

					for(float t = 0.0; t <= 1.0; t += m_step){
						double t2 = t * t;
						double t3 = t2 * t;

						BsplineCurve curve("curve"+std::to_string(t));
						for(float s = 0.0; s <= 1.0; s += m_step){
							double s2 = s * s;
							double s3 = s2 * s;

							curve.add_coordinate(blendingFunction(s,s2,s3,t,t2,t3,n16,n23,nLine,nCol,coords));
						}
						m_curveList.push_back(curve);
					}
				}
			}
		}
		Coordinate blendingFunction(float s, double s2, double s3, float t, double t2, double t3, double n16, double n23, int nLine, int nCol, const Coordinates& coords) {
			double x = (-n16*((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[0+nLine+nCol][0] +(0.5*s3 -s2 +n23)*coords[m_maxCols+nLine+nCol][0]   +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+nLine+nCol][0]   +(n16*s3)*coords[m_maxCols*3+nLine+nCol][0]) +0.5*((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[1+nLine+nCol][0] +(0.5*s3 -s2 +n23)*coords[m_maxCols+1+nLine+nCol][0] +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+1+nLine+nCol][0] +(n16*s3)*coords[m_maxCols*3+1+nLine+nCol][0]) -0.5*((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[2+nLine+nCol][0] +(0.5*s3 -s2 +n23)*coords[m_maxCols+2+nLine+nCol][0] +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+2+nLine+nCol][0] +(n16*s3)*coords[m_maxCols*3+2+nLine+nCol][0]) +n16*((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[3+nLine+nCol][0] +(0.5*s3 -s2 +n23)*coords[m_maxCols+3+nLine+nCol][0] +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+3+nLine+nCol][0] +(n16*s3)*coords[m_maxCols*3+3+nLine+nCol][0]))*t3
						+(0.5*((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[0+nLine+nCol][0] +(0.5*s3 -s2 +n23)*coords[m_maxCols+nLine+nCol][0]   +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+nLine+nCol][0]   +(n16*s3)*coords[m_maxCols*3+nLine+nCol][0]) -((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[1+nLine+nCol][0] +(0.5*s3 -s2 +n23)*coords[m_maxCols+1+nLine+nCol][0] +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+1+nLine+nCol][0] +(n16*s3)*coords[m_maxCols*3+1+nLine+nCol][0]) +0.5*((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[2+nLine+nCol][0] +(0.5*s3 -s2 +n23)*coords[m_maxCols+2+nLine+nCol][0] +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+2+nLine+nCol][0] +(n16*s3)*coords[m_maxCols*3+2+nLine+nCol][0]))*t2
						+(-0.5*((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[0+nLine+nCol][0] +(0.5*s3 -s2 +n23)*coords[m_maxCols+nLine+nCol][0]   +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+nLine+nCol][0]   +(n16*s3)*coords[m_maxCols*3+nLine+nCol][0]) +0.5*((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[2+nLine+nCol][0] +(0.5*s3 -s2 +n23)*coords[m_maxCols+2+nLine+nCol][0] +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+2+nLine+nCol][0] +(n16*s3)*coords[m_maxCols*3+2+nLine+nCol][0]))*t
						+(n16*((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[0+nLine+nCol][0] +(0.5*s3 -s2 +n23)*coords[m_maxCols+nLine+nCol][0]   +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+nLine+nCol][0]   +(n16*s3)*coords[m_maxCols*3+nLine+nCol][0]) +n23*((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[1+nLine+nCol][0] +(0.5*s3 -s2 +n23)*coords[m_maxCols+1+nLine+nCol][0] +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+1+nLine+nCol][0] +(n16*s3)*coords[m_maxCols*3+1+nLine+nCol][0]) +n16*((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[2+nLine+nCol][0] +(0.5*s3 -s2 +n23)*coords[m_maxCols+2+nLine+nCol][0] +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+2+nLine+nCol][0] +(n16*s3)*coords[m_maxCols*3+2+nLine+nCol][0]));
						
			double y = (-n16*((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[0+nLine+nCol][1] +(0.5*s3 -s2 +n23)*coords[m_maxCols+nLine+nCol][1]   +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+nLine+nCol][1]   +(n16*s3)*coords[m_maxCols*3+nLine+nCol][1]) +0.5*((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[1+nLine+nCol][1] +(0.5*s3 -s2 +n23)*coords[m_maxCols+1+nLine+nCol][1] +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+1+nLine+nCol][1] +(n16*s3)*coords[m_maxCols*3+1+nLine+nCol][1]) -0.5*((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[2+nLine+nCol][1] +(0.5*s3 -s2 +n23)*coords[m_maxCols+2+nLine+nCol][1] +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+2+nLine+nCol][1] +(n16*s3)*coords[m_maxCols*3+2+nLine+nCol][1]) +n16*((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[3+nLine+nCol][1] +(0.5*s3 -s2 +n23)*coords[m_maxCols+3+nLine+nCol][1] +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+3+nLine+nCol][1] +(n16*s3)*coords[m_maxCols*3+3+nLine+nCol][1]))*t3
						+(0.5*((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[0+nLine+nCol][1] +(0.5*s3 -s2 +n23)*coords[m_maxCols+nLine+nCol][1]   +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+nLine+nCol][1]   +(n16*s3)*coords[m_maxCols*3+nLine+nCol][1]) -((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[1+nLine+nCol][1] +(0.5*s3 -s2 +n23)*coords[m_maxCols+1+nLine+nCol][1] +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+1+nLine+nCol][1] +(n16*s3)*coords[m_maxCols*3+1+nLine+nCol][1]) +0.5*((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[2+nLine+nCol][1] +(0.5*s3 -s2 +n23)*coords[m_maxCols+2+nLine+nCol][1] +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+2+nLine+nCol][1] +(n16*s3)*coords[m_maxCols*3+2+nLine+nCol][1]))*t2
						+(-0.5*((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[0+nLine+nCol][1] +(0.5*s3 -s2 +n23)*coords[m_maxCols+nLine+nCol][1]   +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+nLine+nCol][1]   +(n16*s3)*coords[m_maxCols*3+nLine+nCol][1]) +0.5*((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[2+nLine+nCol][1] +(0.5*s3 -s2 +n23)*coords[m_maxCols+2+nLine+nCol][1] +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+2+nLine+nCol][1] +(n16*s3)*coords[m_maxCols*3+2+nLine+nCol][1]))*t
						+(n16*((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[0+nLine+nCol][1] +(0.5*s3 -s2 +n23)*coords[m_maxCols+nLine+nCol][1]   +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+nLine+nCol][1]   +(n16*s3)*coords[m_maxCols*3+nLine+nCol][1]) +n23*((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[1+nLine+nCol][1] +(0.5*s3 -s2 +n23)*coords[m_maxCols+1+nLine+nCol][1] +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+1+nLine+nCol][1] +(n16*s3)*coords[m_maxCols*3+1+nLine+nCol][1]) +n16*((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[2+nLine+nCol][1] +(0.5*s3 -s2 +n23)*coords[m_maxCols+2+nLine+nCol][1] +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+2+nLine+nCol][1] +(n16*s3)*coords[m_maxCols*3+2+nLine+nCol][1]));
						
			double z = (-n16*((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[0+nLine+nCol][2] +(0.5*s3 -s2 +n23)*coords[m_maxCols+nLine+nCol][2]   +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+nLine+nCol][2]   +(n16*s3)*coords[m_maxCols*3+nLine+nCol][2]) +0.5*((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[1+nLine+nCol][2] +(0.5*s3 -s2 +n23)*coords[m_maxCols+1+nLine+nCol][2] +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+1+nLine+nCol][2] +(n16*s3)*coords[m_maxCols*3+1+nLine+nCol][2]) -0.5*((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[2+nLine+nCol][2] +(0.5*s3 -s2 +n23)*coords[m_maxCols+2+nLine+nCol][2] +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+2+nLine+nCol][2] +(n16*s3)*coords[m_maxCols*3+2+nLine+nCol][2]) +n16*((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[3+nLine+nCol][2] +(0.5*s3 -s2 +n23)*coords[m_maxCols+3+nLine+nCol][2] +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+3+nLine+nCol][2] +(n16*s3)*coords[m_maxCols*3+3+nLine+nCol][2]))*t3
						+(0.5*((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[0+nLine+nCol][2] +(0.5*s3 -s2 +n23)*coords[m_maxCols+nLine+nCol][2]   +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+nLine+nCol][2]   +(n16*s3)*coords[m_maxCols*3+nLine+nCol][2]) -((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[1+nLine+nCol][2] +(0.5*s3 -s2 +n23)*coords[m_maxCols+1+nLine+nCol][2] +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+1+nLine+nCol][2] +(n16*s3)*coords[m_maxCols*3+1+nLine+nCol][2]) +0.5*((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[2+nLine+nCol][2] +(0.5*s3 -s2 +n23)*coords[m_maxCols+2+nLine+nCol][2] +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+2+nLine+nCol][2] +(n16*s3)*coords[m_maxCols*3+2+nLine+nCol][2]))*t2
						+(-0.5*((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[0+nLine+nCol][2] +(0.5*s3 -s2 +n23)*coords[m_maxCols+nLine+nCol][2]   +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+nLine+nCol][2]   +(n16*s3)*coords[m_maxCols*3+nLine+nCol][2]) +0.5*((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[2+nLine+nCol][2] +(0.5*s3 -s2 +n23)*coords[m_maxCols+2+nLine+nCol][2] +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+2+nLine+nCol][2] +(n16*s3)*coords[m_maxCols*3+2+nLine+nCol][2]))*t
						+(n16*((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[0+nLine+nCol][2] +(0.5*s3 -s2 +n23)*coords[m_maxCols+nLine+nCol][2]   +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+nLine+nCol][2]   +(n16*s3)*coords[m_maxCols*3+nLine+nCol][2]) +n23*((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[1+nLine+nCol][2] +(0.5*s3 -s2 +n23)*coords[m_maxCols+1+nLine+nCol][2] +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+1+nLine+nCol][2] +(n16*s3)*coords[m_maxCols*3+1+nLine+nCol][2]) +n16*((-n16*s3 +0.5*s2 -0.5*s +n16)*coords[2+nLine+nCol][2] +(0.5*s3 -s2 +n23)*coords[m_maxCols+2+nLine+nCol][2] +(-0.5*s3 +0.5*s2 +0.5*s +n16)*coords[m_maxCols*2+2+nLine+nCol][2] +(n16*s3)*coords[m_maxCols*3+2+nLine+nCol][2]));

			return Coordinate(x,y,z);
		}
};

#endif	 	  	 	     	  		  	  	    	      	 	
