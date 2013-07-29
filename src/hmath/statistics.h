

/*
	Higher Math Utility Headers
	http://github.com/dustyco/hmath
	
	Statistics
	See github wiki for usage info
	https://github.com/dustyco/hmath/wiki/Statistics
*/


#pragma once
#include <iterator>
#include <cmath>


namespace hmath {
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ALGORITHMS //////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	// Calculate the mean of a range of values
	template <class InputIterator>
	typename std::iterator_traits<InputIterator>::value_type mean (const InputIterator& begin, const InputIterator& end) {
		typedef typename std::iterator_traits<InputIterator>::value_type value_type;
		unsigned int n = 0;
		value_type sum = 0;
		for (InputIterator it=begin; it!=end; ++it,++n) sum += (*it);
		return sum/n;
	}
	
	// Calculate the variance of a range of values
	template <class InputIterator>
	typename std::iterator_traits<InputIterator>::value_type variance (const InputIterator& begin, const InputIterator& end) {
		typedef typename std::iterator_traits<InputIterator>::value_type value_type;
		unsigned int n = 0;
		value_type sum = 0;
		value_type mu = mean(begin, end);
		for (InputIterator it=begin; it!=end; ++it,++n) {
			value_type deviation = (*it) - mu;
			sum += deviation*deviation;
		}
		return sum/n;
	}
	
	// Calculate the standard deviation of a range of values
	template <class InputIterator>
	typename std::iterator_traits<InputIterator>::value_type standard_deviation (const InputIterator& begin, const InputIterator& end) {
		return std::sqrt(variance(begin, end));
	}
	
}
