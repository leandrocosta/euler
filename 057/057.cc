#include <iostream>
using namespace std;

#include "ttmath/ttmath.h"

typedef ttmath::Big<1,10> BigUInt;

size_t count = 0;

void run(BigUInt& num, BigUInt& den)
{
	BigUInt f1 = num;
	BigUInt f2 = den;

	BigUInt temp_den = f2;
	BigUInt temp_num = temp_den + f1;

	BigUInt temp_den2 = temp_num;
	BigUInt temp_num2 = temp_den;

	den = temp_den2;
	num = temp_den2 + temp_num2;

	cout << "num: " << num << ", den: " << den << endl;

	BigUInt i10 = 10;

	BigUInt l_num = ttmath::SkipFraction(ttmath::Log(num, i10));
	BigUInt l_den = ttmath::SkipFraction(ttmath::Log(den, i10));

	if (l_num > l_den)
		count++;
}

int main ()
{
	cout.precision(16);

	/*

	double v = 1.0+1.0/(2.0+1.0/2.0);
	double den = 2.0+1.0/2.0;

	cout << "den: " << den << endl;;

	for (int i = 3; i <= 1000; i++)
	{
		v = 1.0+1.0/v;
		cout << "v: " << v << endl;
	}
	*/

	/*
	 * v = 1 + 1/f
	 * f = (1+1 + f1/f2)
	 */

	/*
	 * 1 + 1/(1+1 + 1/2)
	 */

	/*
	double f1 = 3.0;
	double f2 = 2.0;

	double temp_den = f2;
	double temp_num = 1.0 * temp_den + f1;

	double temp_den2 = temp_num;
	double temp_num2 = temp_den;

	double den = temp_den2;
	double num = temp_den2 + temp_num2;

	cout << "num: " << num << ", den: " << den << endl;
	*/

	/*
	 * next
	 * 1 + 1/(1 + 7/5)
	 */

	/*
	f1 = num;
	f2 = den;

	temp_den = f2;

	temp_num = 1.0 * temp_den + f1;

	temp_den2 = temp_num;
	temp_num2 = temp_den;

	den = temp_den2;
	num = temp_den2 + temp_num2;

	cout << "num: " << num << ", den: " << den << endl;
	*/

	BigUInt num = 3;
	BigUInt den = 2;

	for (size_t i = 2; i <= 1000; i++)
	{
		run (num, den);
	}

	/*
	BigUInt i = 1;

	cout << "i: " << i << endl;
	i *= 20000;
	cout << "i: " << i << endl;
	i *= 20000;
	cout << "i: " << i << endl;
	i *= 20000;
	cout << "i: " << i << endl;
	i *= 20000;
	cout << "i: " << i << endl;
	cout.flush();

	BigUInt i10 = 10;

	BigUInt l = ttmath::Log(i, i10);
	cout << "l: " << ttmath::Round(l) << endl;
	*/

	cout << "count: " << count << endl;
}

