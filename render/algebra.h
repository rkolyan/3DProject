#ifndef ALGEBRA_H
#define ALGEBRA_H

#include "constants.h"

#include <complex>

namespace Algebra
{
	typedef std::complex<double> complex;


	inline bool IsZero(complex x)
	{
		return (fabs(x.real()) < TOLERANCE) && (fabs(x.imag()) < TOLERANCE);
	}

	int filterRealNumbers(int numComplexValues, const complex inArray[], double outArray[]);
	int solveQuadraticEquation(complex a, complex b, complex c, complex roots[2]);

	inline int solveQuadraticEquation(double a, double b, double c, double roots[2])
	{
		complex croots[2];
		const int numComplexRoots = solveQuadraticEquation(complex(a), complex(b), complex(c), croots);
		return filterRealNumbers(numComplexRoots, croots, roots);
	}

    inline double radiansFromDegrees(double degrees)
    {
        return degrees * (PI / 180.0);
    }
}

#endif //ALGEBRA_H
