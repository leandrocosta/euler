/*
 * Problem 7
 *
 * 28 December 2001
 *
 * By listing the first six prime numbers: 2, 3, 5, 7, 11, and 13, we can see that the 6th prime is 13.
 *
 * What is the 10001st prime number?
 */

/*
 * Sieve of Eratosthenes by ranges
 *
 * RANGE_SIZE = a range size
 *
 * Allocate range[0-RANGE_SIZE];
 *
 * While nth prime number is not found
 * 	Initialize range with numbers from 1 until RANGE_SIZE;
 *
 * 	Runs Eratosthenes for this range until the nth prime number is found:
 * 		- For each prime number, store the pair <number, last_multiple>
 * 		- If range is over:
 * 			- Initialize range with the following numbers;
 * 			- Mark multiple numbers for each prime number from the last_multiple;
 * 			- Continue with Eratosthenes
 *
 *
 * Eratosthenes:
 *
 * Allocate range[0-RANGE_SIZE]
 * Define primes = empty
 *
 * For i in 0-RANGE_SIZE
 * 	Initialize range[i] = i;
 *
 * For i in 2-sqrt(RANGE_SIZE)
 * 	if (range[i] != 0)
 * 		primes.add(i)
 * 		mark all multiples of i (m) from i^2 until RANGE_SIZE (range[m] = 0)
 *
 * For i in sqrt(RANGE_SIZE)-RANGE_SIZE
 * 	if (range[i] != 0)
 * 		primes.add (i)
 *
 * 	Print primes
 *
 *********************************
 * Sieve of Eratosthenes by ranges
 *
 * N: find nth prime number
 * range[RANGE_SIZE]: a vector of RANGE_SIZE numbers
 * range_start: the first number of current range
 * primes: vector of pairs (prime,last_marked_multiple)
 *
 * range_start = 2
 *
 * while primes.size() < N:
 *		// initialize range
 * 		for i from 0 until RANGE_SIZE-1:
 * 			range[i] = range_start+i
 *
 * 		for each pair p from primes:
 * 			if p.second != 0
 * 				m = p.second + p.first
 * 			else
 * 				m = p.first ^ 2
 * 			end if
 *
 * 			for (; m < range_start + RANGE_SIZE-1; m += p.first):
 * 				range[m-range_start] = 0
 * 				p.second = m
 * 			end while
 *		end for
 *
 *		// run eratosthenes while primes.size() < N:
 *		for n from range_start until sqrt(range_start + RANGE_SIZE-1):
 *			if (range[n-range_start] != 0)
 *				p = pair(n, 0)
 *
 *				for (m = n^2; m < range_start + RANGE_SIZE-1; m += n):
 *					range[m-range_start] = 0
 *					p.second = m
 *				end for
 *
 *				primes.add (p)
 *			end if
 *		end for
 *
 *		for n from sqrt(range_start + RANGE_SIZE-1) until range_start + RANGE_SIZE-1:
 *			if (range[n-range_start] != 0)
 *				primes.add (pair(n, 0))
 *			end if
 *		end for
 *
 *
 * end while
 * 				
 *
 *
 */

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <vector>
using namespace std;

vector<unsigned int> get_primes_by_erathostenes(unsigned int max)
{
	vector<unsigned int> primes;

	vector<unsigned int> range(max+1);

	for (size_t i = 0; i < range.size(); i++)
		range[i] = i;

	for (size_t i = 2; i <= trunc(sqrt(max)); i++)
	{
		//cout << "i: " << i << endl;
		if (range[i])
		{
			primes.push_back(i);

			for (size_t m = pow(i, 2); m < range.size(); m += i)
				range[m] = 0;
		}
	}

	//cout << "-" << endl;

	for (size_t i = trunc(sqrt(max))+1; i <= max; i++)
	{
		//cout << "i: " << i << endl;
		if (range[i])
			primes.push_back(i);

	}

	return primes;
}

unsigned int get_nth_prime_by_erathostenes_range(const unsigned int& nth, unsigned int range_len)
{
	vector<unsigned int> range(range_len);
	vector<pair<unsigned int, unsigned int> > primes;
	unsigned int range_start = 2;

	while (primes.size() < nth)
	{
		unsigned int range_end = range_start + range_len-1;

		cout << "running eratosthenes - range_start: " << range_start << ", range_end: " << range_end << ", primes.size(): " << primes.size() << endl;

		// initialize range
		for (size_t i = 0; i < range_len; i++)
			range[i] = range_start+i;

		for(vector<pair<unsigned int, unsigned int> >::iterator it = primes.begin(); it != primes.end(); ++it)
		{
			unsigned int m;

			if (it->second != 0)
				m = it->second + it->first;
			else
				m = pow(it->first, 2);

			for (; m <= range_end; m += it->first)
			{
				if (m >= range_start)
				{
					//cout << "verifying " << it->first << ", marking " << m << endl;
					range[m-range_start] = 0;
					it->second = m;
				}
			}
		}

		// run eratosthenes
		for (size_t n = range_start; primes.size() < nth && n <= trunc(sqrt(range_end)); n++)
		{
			//cout << "checking " << n << endl;
			if (range[n-range_start] != 0)
			{
				pair<unsigned int, unsigned int> p = make_pair<unsigned int, unsigned int>(n, 0);

				for (size_t m = pow(n, 2); m <= range_end; m += n)
				{
					range[m-range_start] = 0;
					p.second = m;
				}

				//cout << "adding prime number: " << p.first << endl;
				primes.push_back (p);
			}
		}

		for (size_t n = round(sqrt(range_end))+1; primes.size() < nth && n <= range_end; n++)
		{
			if (n >= range_start && range[n-range_start] != 0)
			{
				//cout << "  adding prime number: " << n << endl;
				primes.push_back (make_pair<unsigned int, unsigned int>(n, 0));
			}
		}

		range_start += range_len;
	}

	return primes.back().first;
}

int main (int argc, char* argv[])
{
	unsigned int nth;
	unsigned int range_len;

	if (argc > 1)
	{
		nth = atoi(argv[1]);
		range_len = atoi(argv[2]);
	}
	else
	{
		nth = 10001;
		range_len = 10000;
	}

	/*
	vector<unsigned int> primes = get_primes_by_erathostenes(nth);
	cout << "we have " << primes.size () << " prime nambers" << endl;
	cout << "last one: " << *(primes.end()-1) << endl;
	*/

	cout << "prime: " << get_nth_prime_by_erathostenes_range(nth, range_len) << endl;


}
