/*
 * Tn = Tn-1 + n
 *
 * Pn = Pn-1 + 3*n - 2
 *
 * Hn = Hn-1 + 4*n - 3
 *
 *
 * Algorithm:
 *
 * T = T285
 * P = P165
 * H = H143
 *
 * t = 285
 * p = 165
 * h = 143
 *
 * do
 * 	i++
 * 	p++
 * 	h++
 *
 * 	T = Ti
 * 
 * 	while (P < T)
 * 		P = Pp++
 * 	while (H < T)
 * 		H = Hh++
 *
 * 	if (T == P == H)
 * 		print T
 * 		exit
 */

#include <iostream>
using namespace std;

unsigned long long calcT(size_t i)
{
	if (i == 1)
		return 1;
	else
		return calcT(i-1) + i;
}

unsigned long long calcP(size_t i)
{
	if (i == 1)
		return 1;
	else
		return calcP(i-1) + 3*i - 2;
}

unsigned long long calcH(size_t i)
{
	if (i == 1)
		return 1;
	else
		return calcH(i-1) + 4*i - 3;
}

int main()
{
	size_t t = 285;
	size_t p = 165;
	size_t h = 143;

	unsigned long long T = calcT(t);
	unsigned long long P = calcP(p);
	unsigned long long H = calcH(h);

	cout << "T: " << T << ", P: " << T << ", H: " << H << endl;

	do
	{
		T = T + (++t);

		while(P < T)
			P = P + 3*(++p) - 2;

		while(H < T)
			H = H + 4*(++h) - 3;
	} while (T != P || T != H);

	cout << T << endl;

	return 0;
}
