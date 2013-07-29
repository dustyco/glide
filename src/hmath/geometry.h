

/*
	Higher Math Utility Headers
	http://github.com/dustyco/hmath
	
	Geometry
	See github wiki for usage info
	https://github.com/dustyco/hmath/wiki/Geometry
*/


#pragma once
#include <list>
#include "linear_algebra.h"


namespace hmath {
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// TYPES ///////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	// ND straight line between two points
	template <int N, class R>
	struct line {
		typedef vec<N,R> vec_type;
		line () {}
		line (const vec_type& a, const vec_type& b) : a(a), b(b) {}
		vec_type a, b;
	};
	
	// 2D circle: position, radius
	template <class R>
	struct circle {
		typedef vec<2,R> vec_type;
		circle () {}
		circle (const vec_type& p, const R& r) : p(p), r(r) {}
		vec_type p;
		R r;
	};
	
	// 2D axis-aligned rectangle, radii from center to perpendicular edges
	template <class R>
	struct rect_aa {
		typedef vec<2,R> vec_type;
		rect_aa () {}
		rect_aa (const vec_type& p, const R& rx, const R& ry) : p(p), rx(rx), ry(ry) {}
		vec_type p;
		R rx, ry;
	};
	
	// 2D rotatable rectangle, radii from center to perpendicular edges
	template <class R>
	struct rect {
		typedef vec<2,R> vec_type;
		rect () {}
		rect (const vec_type& p, const R& rx, const R& ry, const float& theta) : p(p), rx(rx), ry(ry), theta(theta) {}
		vec_type p;
		R rx, ry;
		float theta;
	};
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ALGORITHMS //////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	// 2D: Returns true if a point is on the left side of a line
	// http://www.gamedev.net/topic/542870-determine-which-side-of-a-line-a-point-is/
	// http://stackoverflow.com/questions/3461453/determine-which-side-of-a-line-a-point-lies
	template <class R>
	bool is_left (const vec<2,R>& point, const vec<2,R>& line_a, const vec<2,R>& line_b) {
		return ((line_b.x-line_a.x)*(point.y-line_a.y) - (line_b.y-line_a.y)*(point.x-line_a.x)) < R(0);
	}
	template <class R>
	bool is_left (const vec<2,R>& point, const line<2,R>& line) {
		return is_left(point, line.a, line.b);
	}
	
	// 2D: Finds the standard form (Ax+By=C) coefficients of a line
	// http://community.topcoder.com/tc?module=Static&d1=tutorials&d2=geometry2
	template <class R>
	void line_to_standard_form (const line<2,R>& line, R& A, R& B, R& C) {
		A = line.b.y - line.a.y;
		B = line.a.x - line.b.x;
		C = A*line.a.x + B*line.a.y;
	}
	
	// 2D: Returns the shortest distance from a point to a line
	// https://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line
	template <class R>
	R distance_signed (const vec<2,R>& point, const line<2,R>& line) {
		R A, B, C;
		line_to_standard_form(line, A, B, C);
		return (A*point.x + B*point.y - C) / std::sqrt(A*A + B*B);
	}
	template <class R>
	R distance (const vec<2,R>& point, const line<2,R>& line) {
		return std::abs(distance_signed(point, line));
	}
	
	// 2D: Return the normal vector of a line (rotate it by 90 degrees clockwise, same magnitude)
	// http://stackoverflow.com/questions/1243614/how-do-i-calculate-the-normal-vector-of-a-line-segment
	template <class R> vec<2,R> normal   (const line<2,R>& line) { return normal(line.b-line.a); }
	template <class R> vec<2,R> normal_i (const line<2,R>& line) { return normal_i(line.b-line.a); }
	
	// 2D: Intersection result: Two infinite lines (use test function first to see if the segments are crossing)
	template <class R>
	vec<2,R> intersect (const line<2,R>& a, const line<2,R>& b) {
		R x1_x2 = a.a.x - a.b.x;
		R x3_x4 = b.a.x - b.b.x;
		R y1_y2 = a.a.y - a.b.y;
		R y3_y4 = b.a.y - b.b.y;
		
		R d = (x1_x2)*(y3_y4) - (y1_y2)*(x3_x4);
		R ca = cross(a.a, a.b);
		R cb = cross(b.a, b.b);
		return vec<2,R>(
			(ca*(x3_x4) - (x1_x2)*cb),
			(ca*(y3_y4) - (y1_y2)*cb)
		) / d;
		// ^ Don't forget the determinant
	}
	
	// 2D: Intersection test: Two line segments
	template <class R>
	bool touching (const line<2,R>& a, const line<2,R>& b) {
		return (is_left(a.a, b) != is_left(a.b, b)) && (is_left(b.a, a) != is_left(b.b, a));
	}
	
	// 2D: Intersection test: Concave polygon-point
	template <class R, class Vec2Iterator>
	bool touching (Vec2Iterator& first, Vec2Iterator& last, const vec<2,R>& point) {
		// TODO
		for (Vec2Iterator it=first; it!=last; ++it) {
			
		}
	}
	
	// 2D: Convex polygon-plane intersection
	template <class R, class VecIterator>
	void clip (const line<2,R>& plane, VecIterator first, VecIterator last, std::list<vec<2,R> >& output) {
		// TODO
	}
	
	// 2D: Calculate the signed area and centroid of an irregular, concave, non-overlapping polygon
	// See also: area
	template <class R, class Vec2Iterator>
	void area_and_centroid (const Vec2Iterator& first, const Vec2Iterator& last, R& area, vec<2,R>& centroid ) {
		float x0, y0, x1, y1, a;
		area = 0;
		centroid = vec<2,R>::origin();
		for (Vec2Iterator it=first; it!=last;) {
			x0 = it->x;
			y0 = it->y;
			++it;
			x1 = it->x;
			y1 = it->y;
			a = x0*y1 - x1*y0;
			area += a;
			centroid.x += (x0 + x1)*a;
			centroid.y += (y0 + y1)*a;
		}
		// Last and first pair
		x0 = last->x;
		y0 = last->y;
		x1 = first->x;
		y1 = first->y;
		a = x0*y1 - x1*y0;
		area += a;
		centroid.x += (x0 + x1)*a;
		centroid.y += (y0 + y1)*a;
		// Finalize
		area /= 2;
		centroid /= area*6;
	}
	
	// 2D: Calculate the only the signed area of an irregular, concave, non-overlapping polygon
	// See also: area_and_centroid
	template <class R, class Vec2Iterator>
	void area (const Vec2Iterator& first, const Vec2Iterator& last, R& area) {
		float x0, y0, x1, y1;
		area = 0;
		for (Vec2Iterator it=first; it!=last;) {
			x0 = it->x;
			y0 = it->y;
			++it;
			x1 = it->x;
			y1 = it->y;
			area += x0*y1 - x1*y0;
		}
		// Last and first pair
		x0 = last->x;
		y0 = last->y;
		x1 = first->x;
		y1 = first->y;
		area += x0*y1 - x1*y0;
		// Finalize
		area /= 2;
	}
	
	template <class R>
	bool touching (const circle<R>& a, const circle<R>& b) {
		return (a.p-b.p).length() < a.r+b.r;
	}
	
	template <class R>
	bool touching (const rect_aa<R>& a, const rect_aa<R>& b) {
		return (a.p.x+a.rx)>(b.p.x-b.rx) && (a.p.x-a.rx)<(b.p.x+b.rx)
			&& (a.p.y+a.ry)>(b.p.y-b.ry) && (a.p.y-a.ry)<(b.p.y+b.ry);
	}
	
}
