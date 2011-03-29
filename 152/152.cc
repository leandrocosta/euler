/*
 * Problem 152
 *
 * There are several ways to write the number 1/2 as a sum of inverse squares using distinct integers.
 *
 * For instance, the numbers {2,3,4,5,7,12,15,20,28,35} can be used:
 *
 * 1   1 + 1 + 1 + 1 + 1 + 1  + 1  + 1  + 1  + 1
 * - = -   -   -   -   -   -    -    -    -    -
 * 2   2²  3²  4²  5²  7²  12²  15²  20²  28²  35²
 *
 *
 * In fact, only using integers between 2 and 45 inclusive, there are exactly three ways to do it, the remaining two
 * being: {2,3,4,6,7,9,10,20,28,35,36,45} and {2,3,4,6,7,9,12,15,28,30,35,36,45}.
 *
 * How many ways are there to write the number 1/2 as a sum of inverse squares using distinct integers between 2 and 80 inclusive?
 */


#include <climits>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <tr1/unordered_map>
using namespace std;
using namespace std::tr1;

typedef vector<unsigned int> TermsList;

const unsigned int FLOAT_PRECISION = 15;
const double DELTA = 1/pow(10,FLOAT_PRECISION+1);

#define FLOAT_EQ(x,y) (fabs(x-y) <= DELTA)
#define FLOAT_GT(x,y) (x-y > DELTA)
#define FLOAT_GE(x,y) (x-y >= DELTA)
#define FLOAT_LT(x,y) (y-x > DELTA)
#define FLOAT_LE(x,y) (y-x >= DELTA)

const unsigned int MAX_VALUE = 36;
const unsigned int CACHE_ITEM_BITS = 0; //22; //20; //10; //3;
const unsigned int CACHE_SIZE = 0; //4*1024*1024 - 1; //1024; //8;

double inverse_squares[MAX_VALUE+1];
double max_possible_total[MAX_VALUE+1];

vector<double> cache(CACHE_SIZE);

void init_inverse_squares ()
{
	for (unsigned int i = 2; i <= MAX_VALUE; i++)
	{
		inverse_squares[i] = 1/pow(i, 2);
		cout << "inverse_squares[" << i << "]: " << inverse_squares[i] << endl;
	}
}

void init_max_possible_total ()
{
	max_possible_total[MAX_VALUE] = inverse_squares[MAX_VALUE];

	for (unsigned int i = MAX_VALUE-1; i >= 2; i--)
	max_possible_total[i] = inverse_squares[i] + max_possible_total[i+1];
}

void init_cache ()
{
	for (unsigned int i = 1; i <= CACHE_SIZE; i++)
	{
		double value = 0;

		for (unsigned int b = 0; b < CACHE_ITEM_BITS; b++)
		{
			if ((i >> b) & 0x1)
			value += inverse_squares[MAX_VALUE-b];
		}

		cache[i-1] = value;
		//cout << "cache[" << i << "]: " << value << endl;
	}

	sort(cache.begin (), cache.end ());
	/*
	for (unsigned int i = 0; i < CACHE_SIZE; i++)
	{
		cout << "cache[" << i << "]: " << cache[i] << endl;
	}
	*/
}

void print_solution (TermsList& solution)
{
	cout << "2";
	for (TermsList::const_iterator it = solution.begin (); it != solution.end (); ++it)
	{
		cout << ", " << static_cast<unsigned int>(*it);
	}

	cout << endl;
}

void check_cache (double total, TermsList& solution)
{
	//cout << "check_cache..." << endl;
	double d = 0.25 - total;

	unsigned int min = 0;
	unsigned int max = CACHE_SIZE-1;
	unsigned int mid = -1;


	/*
	if (solution.size () == 8 && solution[0] == 3 && solution[1] == 4 && solution[2] == 5 && solution[3] == 7 && solution[4] == 12 && solution[5] == 15 && solution[6] == 20 && solution[7] == 28)
	{
		//cout << "trying" << endl;
	}
	*/

	//if (d - cache[min] >= 0.00000000000000001 && cache[max] - d >= 0.00000000000000001)
	if (CACHE_SIZE > 0 && FLOAT_GE(d, cache[min]) && FLOAT_LE(d, cache[max]))
	{

		/*
		if (solution.size () == 7 && solution[0] == 3 && solution[1] == 4 && solution[2] == 6 && solution[3] == 7 && solution[4] == 9 && solution[5] == 10 && solution[6] == 20)
		{
			cout << "try (partial: " << total << ") ";
			print_solution(solution);
		}
		*/

		do
		{
			//cout << "min: " << min << ", max: " << max << ", mid: " << mid << endl;
			mid = min + (max - min)/2;

			if (FLOAT_GT(d, cache[mid]))
			{

				/*
				if (solution.size () == 7 && solution[0] == 3 && solution[1] == 4 && solution[2] == 6 && solution[3] == 7 && solution[4] == 9 && solution[5] == 10 && solution[6] == 20)
				{
					cout << "d: " << d << " > cache[mid]:" << cache[mid] << ", min = mid+1: " << min << " = " << mid << "+1" << endl;
				}
				*/
				min = mid+1;
			}
			else
			{
				/*
				if (solution.size () == 7 && solution[0] == 3 && solution[1] == 4 && solution[2] == 6 && solution[3] == 7 && solution[4] == 9 && solution[5] == 10 && solution[6] == 20)
				{
					cout << "d: " << d << " <= cache[mid]:" << cache[mid] << ", max = mid-1: " << max << " = " << mid << "-1" << endl;
				}
				*/
				max = mid-1;
			}
		} while ((! FLOAT_EQ(cache[mid], d)) && min <= max);

		/*
		if ((solution.size () == 1 && solution[0] == 3)
		|| (solution.size () == 2 && solution[0] == 3 && solution[1] == 4)
		|| (solution.size () == 3 && solution[0] == 3 && solution[1] == 4 && solution[2] == 5)
		|| (solution.size () == 4 && solution[0] == 3 && solution[1] == 4 && solution[2] == 5 && solution[3] == 7)
		|| (solution.size () == 5 && solution[0] == 3 && solution[1] == 4 && solution[2] == 5 && solution[3] == 7 && solution[4] == 12)
		|| (solution.size () == 6 && solution[0] == 3 && solution[1] == 4 && solution[2] == 5 && solution[3] == 7 && solution[4] == 12 && solution[5] == 15)
		|| (solution.size () == 7 && solution[0] == 3 && solution[1] == 4 && solution[2] == 5 && solution[3] == 7 && solution[4] == 12 && solution[5] == 15 && solution[6] == 20)
		|| (solution.size () == 8 && solution[0] == 3 && solution[1] == 4 && solution[2] == 5 && solution[3] == 7 && solution[4] == 12 && solution[5] == 15 && solution[6] == 20 && solution[7] == 28))
		{
			cout << "try (partial: " << total << ") ";
			print_solution(solution);
			cout << "min: " << min << ", max: " << max << ", mid: " << mid << endl;
			cout << "d: " << d << ", cache[mid]: " << cache[mid] << endl;
		}
		*/

	   /*
		if (solution.size () == 7 && solution[0] == 3 && solution[1] == 4 && solution[2] == 6 && solution[3] == 7 && solution[4] == 9 && solution[5] == 10 && solution[6] == 20)
		{
			cout << "end of search" << endl;
			cout << "min: " << min << ", max: " << max << ", mid: " << mid << endl;
			cout << "d: " << d << ", cache[mid]: " << cache[mid] << ", cache[mid+1]: " << cache[mid+1] << endl;
		}
		*/

	   while (FLOAT_EQ(cache[mid-1], d))
	   {
		   mid--;
	   }

	   while (FLOAT_EQ (total + cache[mid], 0.25))
	   {
		   cout << "partial solution: ";
		   print_solution(solution);
		   mid++;
	   }
   }

	//cout << "check cache end" << endl;
}

void run (double total, unsigned int i, TermsList& solution)
{
	//cout << "running..." << endl;

	total += inverse_squares[i];
	solution.push_back (i);

	/*
	if ((solution.size () == 1 && solution[0] == 3)
	|| (solution.size () == 2 && solution[0] == 3 && solution[1] == 4)
	|| (solution.size () == 3 && solution[0] == 3 && solution[1] == 4 && solution[2] == 5)
	|| (solution.size () == 4 && solution[0] == 3 && solution[1] == 4 && solution[2] == 5 && solution[3] == 7)
	|| (solution.size () == 5 && solution[0] == 3 && solution[1] == 4 && solution[2] == 5 && solution[3] == 7 && solution[4] == 12)
	|| (solution.size () == 6 && solution[0] == 3 && solution[1] == 4 && solution[2] == 5 && solution[3] == 7 && solution[4] == 12 && solution[5] == 15)
	|| (solution.size () == 7 && solution[0] == 3 && solution[1] == 4 && solution[2] == 5 && solution[3] == 7 && solution[4] == 12 && solution[5] == 15 && solution[6] == 20)
	|| (solution.size () == 8 && solution[0] == 3 && solution[1] == 4 && solution[2] == 5 && solution[3] == 7 && solution[4] == 12 && solution[5] == 15 && solution[6] == 20 && solution[7] == 28))
	{
		cout << "try (partial: " << total << ") ";
		print_solution(solution);
	}
	*/

	if (FLOAT_EQ(total, 0.25))
	{
		cout << "solution: ";
		print_solution(solution);
	}
	else if (total < 0.25)
	{
		if (i < MAX_VALUE - CACHE_ITEM_BITS)
		{
			for (++i; i <= MAX_VALUE - CACHE_ITEM_BITS; i++)
			{
				if ((total + max_possible_total[i]) >= 0.25)
				{
					run (total, i, solution);
				}
			}

		   check_cache (total, solution);
		}
		else
		{
		   check_cache (total, solution);
		}
	}

	solution.pop_back ();
}

int main ()
{
	//      cout << "r: " << (1/pow(2,2) + 1/pow(3,2) + 1/pow(4,2) + 1/pow(5,2) + 1/pow(7,2) + 1/pow(12,2) + 1/pow(15,2) + 1/pow(20,2) + 1/pow(28,2) + 1/pow(35,2)) << endl;

	cout.precision(FLOAT_PRECISION);

	init_inverse_squares ();
	init_max_possible_total ();
	init_cache ();

	double total = 0;
	TermsList solution;

	for (unsigned int i = 3; i <= MAX_VALUE; i++)
	{
		if (max_possible_total[i] >= 0.25)
		{
			//cout << "i: " << static_cast<unsigned int>(i) << endl;
			run (total, i, solution);
		}
	}
}


