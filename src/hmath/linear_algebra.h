

/*
	Higher Math Utility Headers
	http://github.com/dustyco/hmath
	
	Linear Algebra
	See github wiki for usage info
	https://github.com/dustyco/hmath/wiki/Linear-Algebra
	
	Changes:
		2013-06-06
			Fixed vec *= vec not returning anything
		2013-06-05
			Switched vector constant functions (origin(), x_unit(), ...) to static const variables (ORIGIN, X_UNIT, ...)
		2013-06-04
			Added lots of matrix stuff
		2013-03-07
			Switched vector-scalar division overloads to use std::numeric_limits<R>::is_iec559 instead
			Added vector bracket operator
*/


#pragma once
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <limits>
#include <cmath>


namespace hmath {
	
	// For static type comparison
	template <class T, class U>
	struct is_same { static const bool value = false; };
	template <class T>
	struct is_same<T, T> { static const bool value = true; };
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// TYPES ///////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	// Totally generic
	template <int N, class R>
	struct vec {
		R a[N];
		
		static const vec ORIGIN;
		
		vec () {}
		vec (const R input[N]) { std::copy(input, input+N, a); }
		R&       operator[] (unsigned int i)       { return a[i]; }
		const R& operator[] (unsigned int i) const { return a[i]; }
	};
	
	template <class R>
	struct vec<2,R> {
		union {
			R a[2];
			struct {
				R x;
				R y;
			};
		};
		
		static const vec ORIGIN;
		static const vec X_UNIT;
		static const vec Y_UNIT;
		
		vec () {};
		vec (R x, R y) : x(x), y(y) {}
		vec (const R input[2]) { std::copy(input, input+2, a); }
		R&       operator[] (unsigned int i)       { return a[i]; }
		const R& operator[] (unsigned int i) const { return a[i]; }
	};
	
	template <class R>
	struct vec<3,R> {
		union {
			R a[3];
			struct {
				R x;
				R y;
				R z;
			};
		};
		
		static const vec ORIGIN;
		static const vec X_UNIT;
		static const vec Y_UNIT;
		static const vec Z_UNIT;
		
		vec () {};
		vec (R x, R y, R z) : x(x), y(y), z(z) {}
		vec (const R input[3]) { std::copy(input, input+3, a); }
		R&       operator[] (unsigned int i)       { return a[i]; }
		const R& operator[] (unsigned int i) const { return a[i]; }
	};
	
	template <class R>
	struct vec<4,R> {
		union {
			R a[4];
			struct {
				R x;
				R y;
				R z;
				R w;
			};
		};
		
		static const vec ORIGIN;
		static const vec X_UNIT;
		static const vec Y_UNIT;
		static const vec Z_UNIT;
		static const vec W_UNIT;
		
		vec () {};
		vec (R x, R y, R z, R w) : x(x), y(y), z(z), w(w) {}
		vec (const R input[4]) { std::copy(input, input+4, a); }
		R&       operator[] (unsigned int i)       { return a[i]; }
		const R& operator[] (unsigned int i) const { return a[i]; }
	};
	
	namespace internal {
		
		template <int N, class R>
		vec<N,R> create_origin () {
			vec<N,R> v;
			for (int i=0; i<N; i++) v[i] = 0;
			return v;
		}
		
	}
	
	// Vector static const member variable definitions ////////////////////////////////////
	template <int N, class R> const vec<N,R> vec<N,R>::ORIGIN = internal::create_origin<N,R>();
	template <class R> const vec<2,R> vec<2,R>::ORIGIN = internal::create_origin<2,R>();
	template <class R> const vec<3,R> vec<3,R>::ORIGIN = internal::create_origin<3,R>();
	template <class R> const vec<4,R> vec<4,R>::ORIGIN = internal::create_origin<4,R>();
	
	template <class R> const vec<2,R> vec<2,R>::X_UNIT = vec<2,R>(1, 0);
	template <class R> const vec<2,R> vec<2,R>::Y_UNIT = vec<2,R>(0, 1);
	template <class R> const vec<3,R> vec<3,R>::X_UNIT = vec<3,R>(1, 0, 0);
	template <class R> const vec<3,R> vec<3,R>::Y_UNIT = vec<3,R>(0, 1, 0);
	template <class R> const vec<3,R> vec<3,R>::Z_UNIT = vec<3,R>(0, 0, 1);
	template <class R> const vec<4,R> vec<4,R>::X_UNIT = vec<4,R>(1, 0, 0, 0);
	template <class R> const vec<4,R> vec<4,R>::Y_UNIT = vec<4,R>(0, 1, 0, 0);
	template <class R> const vec<4,R> vec<4,R>::Z_UNIT = vec<4,R>(0, 0, 1, 0);
	template <class R> const vec<4,R> vec<4,R>::W_UNIT = vec<4,R>(0, 0, 0, 1);
	
	// Vector addition ////////////////////////////////////////////////////////////////////
	template <int N, class R>
	vec<N,R> operator +  (const vec<N,R>& lhs, const vec<N,R>& rhs) {
		vec<N,R> v;
		for (int i=0; i<N; i++) v[i] = lhs[i] + rhs[i];
		return v;
	}
	template <int N, class R>
	vec<N,R> operator -  (const vec<N,R>& lhs, const vec<N,R>& rhs) {
		vec<N,R> v;
		for (int i=0; i<N; i++) v[i] = lhs[i] - rhs[i];
		return v;
	}
	template <int N, class R>
	vec<N,R> operator += (vec<N,R>& lhs, const vec<N,R>& rhs) {
		for (int i=0; i<N; i++) lhs[i] += rhs[i];
		return lhs;
	}
	template <int N, class R>
	vec<N,R> operator -= (vec<N,R>& lhs, const vec<N,R>& rhs) {
		for (int i=0; i<N; i++) lhs[i] -= rhs[i];
		return lhs;
	}
	
	// Vector-vector multiplication /////////////////////////////////////////////////////////////////
	template <int N, class R>
	vec<N,R> operator *  (const vec<N,R>& lhs, const vec<N,R>& rhs) {
		vec<N,R> v;
		for (int i=0; i<N; i++) v[i] = lhs[i] * rhs[i];
		return v;
	}
	template <int N, class R>
	vec<N,R> operator /  (const vec<N,R>& lhs, const vec<N,R>& rhs) {
		vec<N,R> v;
		for (int i=0; i<N; i++) v[i] = lhs[i] / rhs[i];
		return v;
	}
	template <int N, class R>
	vec<N,R> operator *= (vec<N,R>& lhs, const vec<N,R>& rhs) {
		for (int i=0; i<N; i++) lhs[i] *= rhs[i];
		return lhs;
	}
	template <int N, class R>
	vec<N,R> operator /= (vec<N,R>& lhs, const vec<N,R>& rhs) {
		for (int i=0; i<N; i++) lhs[i] /= rhs[i];
		return lhs;
	}
	
	// Vector-scalar multiplication /////////////////////////////////////////////////////////////////
	template <int N, class R>
	vec<N,R> operator *  (const vec<N,R>& lhs, const R& rhs) {
		vec<N,R> v;
		for (int i=0; i<N; i++) v[i] = lhs[i] * rhs;
		return v;
	}
	template <int N, class R>
	vec<N,R> operator /  (const vec<N,R>& lhs, const R& rhs) {
		vec<N,R> v;
		if (std::numeric_limits<R>::is_iec559 && N>1) {
			// Cache reciprocol to avoid multiple divisions
			R irhs = R(1)/rhs;
			for (int i=0; i<N; i++) v[i] = lhs[i]*irhs;
		} else {
			// Otherwise divide normally (integers or custom types)
			for (int i=0; i<N; i++) v[i] = lhs[i] / rhs;
		}
		return v;
	}
	template <int N, class R>
	vec<N,R> operator *= (vec<N,R>& lhs, const R& rhs) {
		for (int i=0; i<N; i++) lhs[i] *= rhs;
		return lhs;
	}
	template <int N, class R>
	vec<N,R> operator /= (vec<N,R>& lhs, const R& rhs) {
		if (std::numeric_limits<R>::is_iec559 && N>1) {
			// Cache reciprocol to avoid multiple divisions
			R irhs = R(1)/rhs;
			for (int i=0; i<N; i++) lhs[i] = lhs[i]*irhs;
		} else {
			// Otherwise divide normally (integers or custom types)
			for (int i=0; i<N; i++) lhs[i] /= rhs;
		}
		return lhs;
	}
	
	// Comparisons //////////////////////////////////////////////////////////////////////////////
	template <int N, class R> bool operator <  (const vec<N,R>& lhs, const vec<N,R>& rhs) { return magsquared(lhs) <  magsquared(rhs); }
	template <int N, class R> bool operator <= (const vec<N,R>& lhs, const vec<N,R>& rhs) { return magsquared(lhs) <= magsquared(rhs); }
	template <int N, class R> bool operator >  (const vec<N,R>& lhs, const vec<N,R>& rhs) { return magsquared(lhs) >  magsquared(rhs); }
	template <int N, class R> bool operator >= (const vec<N,R>& lhs, const vec<N,R>& rhs) { return magsquared(lhs) >= magsquared(rhs); }
	template <int N, class R> bool operator == (const vec<N,R>& lhs, const vec<N,R>& rhs) {
		for (int i=0; i<N; i++) if (lhs[i]!=rhs[i]) return false;
		return true;
	}
	template <int N, class R> bool operator != (const vec<N,R>& lhs, const vec<N,R>& rhs) {
		for (int i=0; i<N; i++) if (lhs[i]!=rhs[i]) return true;
		return false;
	}
	
	// Stream insertion /////////////////////////////////////////////////////////////////////////
	template <int N, class R>
	std::ostream& operator<< (std::ostream& os, const vec<N,R>& v) {
		os << '(' << v[0];
		for (int i=1; i<N; i++)
			os << ", " << v[i];
		os << ')';
		return os;
	}
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ALGORITHMS //////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	template <int N, class R>         R length_squared (const vec<N,R>& v) {
		R sum = 0;
		for (int i=0; i<N; i++) sum += v[i]*v[i];
		return sum;
	}
	template <int N, class R>        R  length     (const vec<N,R>& v) { return sqrt(length_squared(v)); }
	template <int N, class R> vec<N,R>  unit       (const vec<N,R>& v) { return v / length(v); }
	template <int N, class R>     void  normalize  (vec<N,R>& v)       { v /= length(v); }
	template <int N, class R>        R  dot        (const vec<N,R>& v1, const vec<N,R>& v2) {
		R sum = 0;
		for (int i=0; i<N; i++) sum += v1[i]*v2[i];
		return sum;
	}
	// 2D: Return the normal vector of a vector (rotate it by 90 degrees clockwise, same magnitude)
	template <class R>        vec<2,R>  normal     (const vec<2,R>& v) { return vec<2,R>(v.y, -v.x); }
	template <class R>        vec<2,R>  normal_i   (const vec<2,R>& v) { return vec<2,R>(-v.y, v.x); }
	
	// 2D only
	template <int N, class R>    float  angle      (const vec<N,R>& v1, const vec<N,R>& v2) {
		return std::acos(dot(unit(v1), unit(v2)));
	}
	// 2D only
	template <class R>           float  angle_aa   (const vec<2,R>& v) {
		if (v.x>0) {
			// QI or QIV
			if (v.y>0) {
				// QI
				return std::atan(float(v.y)/float(v.x));
			} else if (v.y<0) {
				// QIV
				return std::atan(float(v.x)/std::fabs(float(v.y))) + 4.71238898f;
			} else if (v.y==0) {
				// West
				return 0.0f;
			}
		} else if (v.x<0) {
			// QII or QIII
			if (v.y>0) {
				// QII
				return std::atan(std::fabs(float(v.x))/float(v.y)) +  1.57079633f;
			} else if (v.y<0) {
				// QIII
				return std::atan(float(v.x)/std::fabs(float(v.y))) + 4.71238898f;
			} else if (v.y==0) {
				// West, pi
				return 3.14159265f;
			}
		} else if (v.x==0) {
			// On y axis
			if (v.y>0) {
				// North, pi/2
				return 1.57079633f;
			} else if (v.y<0) {
				// South, 3*pi/2
				return 4.71238898f;
			}
		}
		// Origin
		return 0.0f;
	}
	// 3D: Return the cross product of 2 vectors
	template <class R>        vec<3,R>  cross      (const vec<3,R>& v1, const vec<3,R>& v2) {
		return vec<3,R>(
			v1.y*v2.z - v1.z*v2.y,
			v1.z*v2.x - v1.x*v2.z,
			v1.x*v2.y - v1.y*v2.x
		);
	}
	// 2D: A specialization of the 3D version which assumes
	// the input vectors have no Z component (2D) and returns
	// only the Z component of the output which has no X or Y components (scalar)
	template <class R>               R  cross      (const vec<2,R>& v1, const vec<2,R>& v2) {
		return v1.x*v2.y - v1.y*v2.x;
	}
	
	
	
	// Row-major NxN matrix
	template <int N, class R>
	struct mat {
		static mat identity ();
		R&       operator[] (unsigned int i)       { return a[i]; }
		const R& operator[] (unsigned int i) const { return a[i]; }
	
		R a[N*N];
	};
	template <class R>
	struct mat<2,R> {
		mat () {}
		mat (R theta);
		R&       operator[] (unsigned int i)       { return a[i]; }
		const R& operator[] (unsigned int i) const { return a[i]; }
	
		R a[4];
	};
	template <class R>
	struct mat<3,R> {
		mat () {}
		mat (R pitch, R yaw, R roll);
		R&       operator[] (unsigned int i)       { return a[i]; }
		const R& operator[] (unsigned int i) const { return a[i]; }
	
		R a[9];
	};
	
	// Construct 2D rotation matrix (angle in radians)
	template <class R>
	mat<2,R>::mat (R theta) {
		R cos_theta = cos(theta);
		R sin_theta = sin(theta);
		a[0] = cos_theta; a[1] = -sin_theta;
		a[2] = sin_theta; a[3] =  cos_theta;
	}
	
	// Construct 3D rotation matrix (angles in radians)
	template <class R>
	mat<3,R>::mat (R pitch, R yaw, R roll) {

		R cos_p = cos(pitch);
		R sin_p = sin(pitch);

		R cos_y = cos(yaw);
		R sin_y = sin(yaw);

		R cos_r = cos(roll);
		R sin_r = sin(roll);

		mat<3,R> Rx;
		Rx[0] =  1;			Rx[2] =  0;			Rx[3] =  0;
		Rx[3] =  0;			Rx[4] =  cos_p;		Rx[5] = -sin_p;
		Rx[6] =  0;			Rx[7] =  sin_p;		Rx[8] =  cos_p;

		mat<3,R> Ry;
		Ry[0] =  cos_y;		Ry[1] = 0;			Ry[2] =  sin_y;
		Ry[3] =  0;			Ry[4] = 1;			Ry[5] =  0;
		Ry[6] = -sin_y;		Ry[7] = 0;			Ry[8] =  cos_y;

		mat<3,R> Rz;
		Rz[0] =  cos_r;		Rz[1] = -sin_r;		Rz[2] =  0;
		Rz[3] =  sin_r;		Rz[4] =  cos_r;		Rz[5] =  0;
		Rz[6] =  0;			Rz[7] =  0;			Rz[8] =  1;

		*this = Rx*Ry*Rz;
	}

	// Matrix-vector multiplication /////////////////////////////////////////////////////////////////
	template <int N, class R>
	vec<N,R> operator *  (const mat<N,R>& lhs, const vec<N,R>& rhs) {
		vec<N,R> v;
		for (int row=0; row<N; row++) {
			v[row] = 0;
			for (int col=0; col<N; col++) {
				v[row] += lhs[row*N + col] * rhs[col];
			}
		}
		return v;
	}

	// Matrix-matrix multiplication /////////////////////////////////////////////////////////////////
	template <int N, class R>
	mat<N,R> operator *  (const mat<N,R>& lhs, const mat<N,R>& rhs) {
		mat<N,R> ret;
		for (int row=0; row!=N; row++) 
		for (int col=0; col!=N; col++) {
			R temp = 0;
			for (int i=0; i!=N; ++i) {
				temp += lhs[row*N + i]*rhs[col + i*N];
			}
			ret[row*N + col] = temp;
		}
		return ret;
	}

	// Stream insertion /////////////////////////////////////////////////////////////////////////
	/*
		[ 1, 0 ]
		[ 0, 1 ]
	*/
	template <int N, class R>
	std::ostream& operator<< (std::ostream& os, const mat<N,R>& m) {
		
		for (int row=0; row!=N; row++) {
			os << "[ " << std::fixed << std::setw(5) << std::setprecision(2) << m[row*N];
			for (int col=1; col!=N; col++) {
				os << ", " << std::fixed << std::setw(5) << std::setprecision(2) << m[row*N + col];
			}
			os << std::fixed << std::setw(3) << std::setprecision(2) << "]" << std::endl;
		}
		return os;
	}
}
