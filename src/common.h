

#pragma once
#include "hmath/linear_algebra.h"
#include "hmath/geometry.h"
using namespace hmath;
typedef vec<2,float>   Vec2f;
typedef vec<2,int>     Vec2i;
typedef rect_aa<float> Rect;

#include <iostream>
#include <string>
#include <stdint.h>
using namespace std;


class NonCopyable
{
protected:
	NonCopyable () {}
	~NonCopyable () {}  // Protected non-virtual destructor
private: 
	NonCopyable (const NonCopyable &);
	NonCopyable & operator = (const NonCopyable &);
};
