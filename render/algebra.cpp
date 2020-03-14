#include <cmath>
#include <complex>
#include <iostream>

#include "algebra.h"

namespace Algebra
{
	int filterRealNumbers(int numComplexValues, const complex inArray[], double outArray[])
	{
		int numRealValues = 0;
		for (int i=0; i < numComplexValues; ++i)
		{
			if (fabs(inArray[i].imag()) < TOLERANCE)
				outArray[numRealValues++] = inArray[i].real();
		}
		return numRealValues;
	}

	int solveQuadraticEquation(complex a, complex b, complex c, complex roots[2])
	{
		if (IsZero(a))
		{
			if (IsZero(b))
			{
				return 0;
			}
			else
			{
				roots[0] = -c / b;
				return 1;
			}
		}
		else
		{
			const complex radicand = b*b - 4.0*a*c;
			if (IsZero(radicand))
			{
				roots[0] = -b / (2.0 * a);
				return 1;
			}
			else
			{
				const complex r = sqrt(radicand);
				const complex d = 2.0 * a;

				roots[0] = (-b + r) / d;
				roots[1] = (-b - r) / d;
				return 2;
			}
		}
	}
}
