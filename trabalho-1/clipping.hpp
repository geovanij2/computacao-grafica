#ifndef CLIPPING_HPP
#define CLIPPING_HPP

#include "objects.hpp"

enum class Line_clip_algs { CS, LB };

class Clipping {
	public:
		Clipping(double x_min, double x_max, double y_min, double y_max):
			_x_min(x_min),
			_x_max(x_max),
			_y_min(y_min),
			_y_max(y_max)
		{}

		virtual ~Clipping() {}

		void set_line_clip_alg(const Line_clip_algs alg) {
			_alg = alg;
		};

		bool clip(Object* obj);

	protected:
	private:
		/* Methods */
		bool clip_point(const Coordinate& c);
		bool clip_line(Coordinate& c1, Coordinate& c2);
		bool clip_polygon(Object* obj);

		int compute_coord_rc(const Coordinate& c);
		bool cohen_sutherland_line_clip(Coordinate& c0, Coordinate& c1);
		bool liang_basky_line_clip(Coordinate& c0, Coordinate& c1);

		bool sutherland_hodgman_polygon_clip(Object* obj);
		void clip_left(Coordinates& input, Coordinates& output);
		void clip_right(Coordinates& input, Coordinates& output);
		void clip_top(Coordinates& input, Coordinates& output);
		void clip_bottom(Coordinates& input, Coordinates& output);

		bool clip_curve(Object* obj);

		/* Attributes */
		double _x_min, _x_max, _y_min, _y_max;
		Line_clip_algs _alg = Line_clip_algs::CS;
		enum RC { 
			INSIDE = 0,
			LEFT = 1,
			RIGHT = 2,
			BOTTOM = 4,
			TOP = 8
		};
};

bool Clipping::clip(Object* obj) {
	switch(obj->get_type()) {
		case obj_type::POINT:
			return clip_point(obj->get_normalized_coord_at_index(0));
		case obj_type::LINE:
			return clip_line(obj->get_normalized_coord_at_index(0), obj->get_normalized_coord_at_index(1));
		case obj_type::POLYGON:
			return clip_polygon(obj);
		case obj_type::BSPLINE_CURVE:
		case obj_type::BEZIER_CURVE:
			return clip_curve(obj);
		default:
			return false;
	}
};

bool Clipping::clip_point(const Coordinate& c) {
	return ((c[0] >= _x_min) && (c[0] <= _x_max)
		&& (c[1] >= _y_min) && (c[1] <= _y_max));
};

bool Clipping::clip_line(Coordinate& c0, Coordinate& c1) {
	if (_alg == Line_clip_algs::CS)
		return  cohen_sutherland_line_clip(c0,c1);
	else
		return liang_basky_line_clip(c0,c1);
};

bool Clipping::clip_polygon(Object* obj) {
	return sutherland_hodgman_polygon_clip(obj);
};

int Clipping::compute_coord_rc(const Coordinate& c) {
	int rc = Clipping::RC::INSIDE;

	if (c[0] < _x_min)
		rc |= Clipping::RC::LEFT;
	else if (c[0] > _x_max)
		rc |= Clipping::RC::RIGHT;

	if (c[1] < _y_min)
		rc |= Clipping::RC::BOTTOM;
	else if (c[1] > _y_max)
		rc |= Clipping::RC::TOP;

	return rc;
};

bool Clipping::cohen_sutherland_line_clip(Coordinate& c0, Coordinate& c1) {
	if (c0 == c1)
		return clip_point(c0);

	int rc0 = compute_coord_rc(c0);
	int rc1 = compute_coord_rc(c1);

	while (true) {
		if (!(rc0 | rc1))
			return true;
		else if (rc0 & rc1)
			return false;
		else {
			double x, y;

			int rc = rc0 ? rc0 : rc1;
			double slope = (c1[1] - c0[1])/(c1[0] - c0[0]);
			if (rc & Clipping::RC::TOP) {
				x = c0[0] + (1/slope) * (_y_max - c0[1]);
				y = _y_max;
			} else if (rc & Clipping::RC::BOTTOM) {
				x = c0[0] + (1/slope) * (_y_min - c0[1]);
				y = _y_min;
			} else if (rc & Clipping::RC::RIGHT) {
				y = c0[1] + slope * (_x_max - c0[0]);
				x = _x_max;
			} else if (rc & Clipping::RC::LEFT) {
				y = c0[1] + slope * (_x_min - c0[0]);
				x = _x_min;
			}

			if (rc == rc0) {
				c0[0] = x;
				c0[1] = y;
				rc0 = compute_coord_rc(c0);
			} else {
				c1[0] = x;
				c1[1] = y;
				rc1 = compute_coord_rc(c1);
			}
		}
	}
};

bool Clipping::liang_basky_line_clip(Coordinate& c0, Coordinate& c1) {
	if (c0 == c1)
		return clip_point(c0);

	auto delta = c1 - c0;
	double p, q, r;
	double u0 = 0.0, u1 = 1.0;

	for (int pos = 0; pos < 4; pos++) {
		switch(pos) {
			case 0:
				p = -delta[0];
				q = c0[0] - _x_min;
				break;
			case 1:
				p = delta[0];
				q = _x_max - c0[0];
				break;
			case 2:
				p = -delta[1];
				q = c0[1] - _y_min;
				break;
			case 3:
				p = delta[1];
				q = _y_max - c0[1];
				break;
			default:
				break;
		}

		if (p == 0 && q < 0)
			return false;

		r = q/p;
		if (p < 0) {
			if (r > u0)
				u0 = r;
		} else if (p > 0) {
			if (r < u1)
				u1 = r;
		}
	}

	if (u0 > u1)
		return false;

	if (u1 < 1) {
		c1[0] = c0[0] + u1 * delta[0];
		c1[1] = c0[1] + u1 * delta[1];
	}
	if (u0 > 0) {
		c0[0] = c0[0] + u0 * delta[0];
		c0[1] = c0[1] + u0 * delta[1];
	}
	return true;
};

bool Clipping::sutherland_hodgman_polygon_clip(Object* obj) {
	auto input = obj->get_normalized_coords();
	Coordinates tmp;
	Coordinates output;

	clip_left(input, tmp);
	clip_right(tmp, output);
	clip_top(output, tmp);
	clip_bottom(tmp, output);

	if (output.size() == 0)
		return false;

	obj->set_normalized_coords(output);
	return true;
};

void Clipping::clip_left(Coordinates& input, Coordinates& output) {
	if (output.size() > 0)
		output.clear();
	if (input.size() == 0)
		return;

	double clip_x = _x_min;
	input.push_back(input[0]);
	for (unsigned int i = 0; i < input.size()-1; i++) {
		Coordinate c0 = input[i];
		Coordinate c1 = input[i+1];

		//Caso 1: out -> out
		if (c0[0] < clip_x && c1[0] < clip_x) {}
		//Case 2: in -> in
		if (c0[0] >= clip_x && c1[0] >= clip_x)
			output.push_back(c1);

		double x = clip_x;
		double m = (c1[1]-c0[1])/(c1[0]-c0[0]);
		double y = m * (x - c0[0]) + c0[1];

		//Caso 3: in -> out
		if (c0[0] >= clip_x && c1[0] < clip_x)
			output.emplace_back(x,y);
		//Caso 4: out -> in
		if (c0[0] < clip_x && c1[0] >= clip_x) {
			output.emplace_back(x,y);
			output.push_back(c1);
		}
	}
};

void Clipping::clip_right(Coordinates& input, Coordinates& output) {
	if (output.size() > 0)
		output.clear();
	if (input.size() == 0)
		return;

	double clip_x = _x_max;
	input.push_back(input[0]);
	for (unsigned int i = 0; i < input.size()-1; i++) {
		Coordinate c0 = input[i];
		Coordinate c1 = input[i+1];

		//Caso 1: out -> out
		if (c0[0] >= clip_x && c1[0] >= clip_x) {}
		//Caso 2: in -> in
		if (c0[0] < clip_x && c1[0] < clip_x)
			output.push_back(c1);

		double x = clip_x;
		double m = (c1[1]-c0[1])/(c1[0]-c0[0]);
		double y = m * (x-c0[0]) + c0[1];

		//Caso 3: in -> out
		if (c0[0] < clip_x && c1[0] >= clip_x)
			output.emplace_back(x,y);
		//Caso 4: out -> in
		if (c0[0] >= clip_x && c1[0] < clip_x) {
			output.emplace_back(x,y);
			output.push_back(c1);
		}
	}
};

void Clipping::clip_top(Coordinates& input, Coordinates& output) {
	if(output.size() > 0)
		output.clear();
	if(input.size() == 0)
		return;

	double clip_y = _y_max;
	input.push_back(input[0]);
	for (unsigned int i = 0; i < input.size()-1; i++) {
		Coordinate c0 = input[i];
		Coordinate c1 = input[i+1];

		//Caso 1: out -> out
		if(c0[1] > clip_y && c1[1] > clip_y) {}

		//Caso 2: in -> in
		if(c0[1] <= clip_y && c1[1] <= clip_y)
			output.push_back(c1);

		double y = clip_y;
		double m = (c1[0]-c0[0])/(c1[1]-c0[1]);
		double x = m * (y-c0[1]) + c0[0];

		//Caso 3: in -> out
		if(c0[1] <= clip_y && c1[1] > clip_y)
			output.emplace_back(x,y);

		//Caso 4: out -> in
		if(c0[1] > clip_y && c1[1] <= clip_y){
			output.emplace_back(x,y);
			output.push_back(c1);
		}
	}
};

void Clipping::clip_bottom(Coordinates& input, Coordinates& output) {
	if(output.size() > 0)
		output.clear();
	if(input.size() == 0)
		return;

	double clip_y = _y_min;
	input.push_back(input[0]);
	for(unsigned int i = 0; i < input.size()-1; i++) {
		Coordinate c0 = input[i];
		Coordinate c1 = input[i+1];

		//Caso 1: out -> out
		if(c0[1] < clip_y && c1[1] < clip_y) {}

		//Caso 2: in -> in
		if(c0[1] >= clip_y && c1[1] >= clip_y)
			output.push_back(c1);

		double y = clip_y;
		double m = (c1[0]-c0[0])/(c1[1]-c0[1]);
		double x = m * (y-c0[1]) + c0[0];

		//Caso 3: in -> out
		if(c0[1] >= clip_y && c1[1] < clip_y)
			output.emplace_back(x,y);

		//Caso 4: out -> in
		if(c0[1] < clip_y && c1[1] >= clip_y){
			output.emplace_back(x,y);
			output.push_back(c1);
		}
	}
};

bool Clipping::clip_curve(Object* obj) {
	Coordinates& coords = obj->get_normalized_coords();
	Coordinates new_curve;
	bool prev_inside = true;
	Coordinate prev(2);

	for (int i = 0; i < coords.size(); i++) {
		if (clip_point(coords[i])) {
			if (!prev_inside) {
				clip_line(prev, coords[i]);
				new_curve.push_back(prev);
			}
			new_curve.push_back(coords[i]);
			prev_inside = true;
		} else {
			if (prev_inside && new_curve.size() != 0) {
				clip_line(prev, coords[i]);
				new_curve.push_back(coords[i]);
			}
			prev_inside = false;
		}
		prev = coords[i];
	}
	if (new_curve.size() == 0)
		return false;

	obj->set_normalized_coords(new_curve);
	return true;
};

#endif // CLIPPING_HPP
