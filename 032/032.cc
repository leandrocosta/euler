#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

size_t sum = 0;
vector<size_t> solutions;

void try_nums(size_t x, size_t y)
{
	if (ceil(log10(x)) + ceil(log10(y)) + ceil(log10(x*y)) != 9)
		return;

	//cout << "x: " << x << ", y: " << y << ", x*y: " << (x*y) << endl;

	vector<size_t> values(10, 0);

	//cout << "x: " << x << ", log10(" << x << "): " << trunc(log10(x)) << endl;

	for (size_t i = 0; i <= trunc(log10(x)); i++)
	{
		size_t digit = ((size_t) x/(size_t) trunc(pow(10,i)))%10;
		values[digit]++;
	}

	for (size_t i = 0; i <= trunc(log10(y)); i++)
	{
		size_t digit = ((size_t) y/(size_t) trunc(pow(10,i)))%10;
		values[digit]++;
	}

	for (size_t i = 0; i <= trunc(log10(x*y)); i++)
	{
		size_t digit = ((size_t) (x*y)/(size_t) trunc(pow(10,i)))%10;
		values[digit]++;
	}

	for (size_t i = 1; i < values.size(); i++)
	{
		if (values[i] != 1)
		{
			//cout << "values[" << i << "]: " << values[i] << endl;
			return;
		}
	}

	cout << "x: " << x << ", y: " << y << ", x*y: " << (x*y) << endl;

	if (find(solutions.begin(), solutions.end(), x*y) == solutions.end())
		solutions.push_back(x*y);
}

int main ()
{
	/*
	cout << "100 * 100 = " << (100 * 100) << endl;
	cout << "99 * 100 = " << (99 * 100) << endl;
	cout << "999 / 99 = " << (999 / 99) << endl;
	cout << "9999 / 99 = " << (9999 / 99) << endl;

	cout << "11 * 99 = " << (11*99) << endl;
	cout << "101 * 99 = " << (101*99) << endl;
	*/

	for (size_t x = 1; x <= 98; x++)
	{
		for (size_t y = x+1; y <= 9876; y++)
		{
			try_nums(x, y);
		}
	}

	//try_nums(39, 186);

	
	for (size_t i = 0; i < solutions.size(); i++)
		sum += solutions[i];

	cout << "sum: " << sum << endl;
}
