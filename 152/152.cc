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

/*
 * brute-force: permutation of 79 items: 2^79-1 possibilities
 *  - 1/2^2 is mandatory, so: 2^78-1 possibilities
 */

#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

#include <tr1/unordered_map>
using namespace std::tr1;

const unsigned int FLOAT_PRECISION = 15;
const double DELTA = 1/pow(10,FLOAT_PRECISION+1);

#define FLOAT_EQ(x,y) (fabs((x)-(y)) <= DELTA)
#define FLOAT_GT(x,y) ((x)-(y) > DELTA)
#define FLOAT_GE(x,y) (FLOAT_GT((x),(y)) || FLOAT_EQ((x),(y)))
#define FLOAT_LT(x,y) ((y)-(x) > DELTA)
#define FLOAT_LE(x,y) (FLOAT_LT((x),(y)) || FLOAT_EQ((x),(y)))

const unsigned int MAX_VALUE = 45;
const unsigned int CACHE_SIZE = 8 * 1024 * 1024; // * 4 bytes = mem

double inverse_squares[MAX_VALUE+1];
double max_possible_total[MAX_VALUE+1];
//vector<double> cache(CACHE_SIZE);
vector<double> cache;
//vector<double> cache_uniq;
unordered_map<double, unsigned int> cache_hash;

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
	//unsigned int i = 0;
	//cache[i++] = 0;
	cache.push_back (0);
	//i++;

	//for (unsigned int item = 3; item <= MAX_VALUE && i < CACHE_SIZE; item++)
	for (unsigned int item = 3; item <= MAX_VALUE; item++)
	{
		cout << "item " << item << endl;
		//unsigned int last = i-1;
		size_t size = cache.size ();

		//for (unsigned int c = 0; c < size && i < CACHE_SIZE; c++)
		for (unsigned int c = 0; c < size; c++)
		{
			double new_value = inverse_squares[item] + cache[c];

			if (FLOAT_LT(new_value, 0.25))
			{
				if (FLOAT_GE(new_value + max_possible_total[item+1], 0.25))
				{
					cache.push_back (new_value);
					//i++;
				}
			}
			else
			{
				break;
			}
		}

		sort (cache.begin (), cache.end ());
	}

	unsigned int i = 0;
	vector<double>::const_iterator itEnd = cache.end ();
	for (vector<double>::const_iterator it = cache.begin (); it != itEnd; ++it)
	{
		cout << "cache[" << (i++) << "]: " << *it << endl;
		//if (cache_uniq.empty () || ! FLOAT_EQ(cache_uniq.back (), (*it)))
		//{
		//	cache_uniq.push_back (*it);
		//}
	}

	//cout << "cache.size (): " << cache.size () << endl;
	//cout << "cache_uniq.size (): " << cache_uniq.size () << endl;
}

void init_cache_pair ()
{
	vector<pair<double, unsigned int> > cache;
	cache.push_back(make_pair<double, unsigned int>(0.0, 1));

	for (unsigned int item = 3; item <= MAX_VALUE && FLOAT_GE(cache.back ().first + max_possible_total[item], 0.25); item++)
	{
		cout << "item " << item << ": " << inverse_squares[item] << endl;

		vector<pair<double, unsigned int> > cache_temp;

		//{
			// get begin for process
			int mid = 0;

			if (cache.size () > 1)
			{
				int min = 0;
				int max = cache.size()-1;

				do
				{
					mid = min + (max - min)/2;

					//cout << "try mid: " << mid << endl;
					if (FLOAT_LT(cache[mid].first + max_possible_total[item], 0.25))
					{
						//cout << "min = mid+1 - " << (cache[mid].first + max_possible_total[item]) << " LT " << (0.25) << endl;
						min = mid+1;
					}
					else
					{
						//cout << "max = mid-1" << endl;
						max = mid-1;
					}
				} while (! FLOAT_EQ(cache[mid].first + max_possible_total[item], 0.25) && min <= max);

				if (mid > 0)
					mid--;

				cout << "cache.size (): " << cache.size () << ", max: " << max_possible_total[item] << ", init: " << mid <<", end: " << (cache.size ()-1) << endl;
			}
		//}

		//for (vector<pair<double, unsigned int> >::const_iterator it_cache = cache.begin (); it_cache != cache.end (); ++it_cache)
		for (unsigned int i = mid; i < cache.size (); i++)
		{
			//double new_value = inverse_squares[item] + it_cache->first;
			double new_value = inverse_squares[item] + cache[i].first;

			if (FLOAT_LT(new_value, 0.25))
			{
				if (item < MAX_VALUE && FLOAT_GE(new_value + max_possible_total[item+1], 0.25))
				{
					//cache_temp.push_back (make_pair<double, unsigned int> (new_value, it_cache->second));
					cache_temp.push_back (make_pair<double, unsigned int> (new_value, cache[i].second));
				}
			} else if (FLOAT_EQ(new_value, 0.25))
			{
				cout << "found!" << endl;
			}
			else
				break;
		}

		if (! cache_temp.empty ())
		{
			cout << "merging caches..." << endl;
			cout.flush ();
			// merge caches
			//sort (cache_temp.begin (), cache_temp.end ());

			unsigned int i_cache = 0;
			unsigned int i_cache_temp = 0;
			unsigned int cache_size = cache.size ();
			unsigned int cache_temp_size = cache_temp.size ();

			while (i_cache < cache_size && i_cache_temp < cache_temp_size)
			{
				if (FLOAT_EQ (cache[i_cache].first, cache_temp[i_cache_temp].first))
				{
					cache[i_cache].second += cache_temp[i_cache_temp].second;
					i_cache++;
					i_cache_temp++;
				} else if (FLOAT_LT(cache[i_cache].first, cache_temp[i_cache_temp].first))
				{
					i_cache++;
				} else
				{
					cache.push_back (cache_temp[i_cache_temp]);
					i_cache_temp++;
				}
			}

			while (i_cache_temp < cache_temp_size)
				cache.push_back(cache_temp[i_cache_temp++]);

			cout << "sorting cache..." << endl;
			cout.flush ();
			sort (cache.begin (), cache.end ());
		}
	}

	//vector<pair<double, unsigned int> >::const_iterator it_cache_end = cache.end ();
	//for (vector<pair<double, unsigned int> >::const_iterator it_cache = cache.begin (); it_cache != it_cache_end; ++it_cache)
	//{
	//	cout << "first: " << it_cache->first << ", second: " << it_cache->second << endl;
	//}
	cout << "cache.size (): " << cache.size () << endl;
}

void init_cache_hash ()
{
	cache_hash[0.0] = 1;
	unsigned int i = 1;

	for (unsigned int item = 3; item <= MAX_VALUE && i < CACHE_SIZE; item++)
	{
		cout << "item " << item << endl;
		vector<double> new_values;

		unordered_map<double, unsigned int>::const_iterator itEnd = cache_hash.end ();
		for (unordered_map<double, unsigned int>::const_iterator it = cache_hash.begin (); it != itEnd; ++it)
		{
			double key = it->first;
			double count = it->second;

			double new_value = inverse_squares[item] + key;

			if (FLOAT_LT(new_value, 0.25) && FLOAT_GE(new_value + max_possible_total[item+1], 0.25))
			{
				new_values.push_back (new_value);
			}
		}

		vector<double>::const_iterator itVecEnd = new_values.end ();
		for (vector<double>::const_iterator itVec = new_values.begin (); itVec != itVecEnd; ++itVec)
		{
			if (cache_hash.find (*itVec) != cache_hash.end ())
			{
				cache_hash[*itVec]++;
			} else
			{
				cache_hash[*itVec] = 1;
				i++;
			}
		}
	}

	unordered_map<double, unsigned int>::const_iterator itEnd = cache_hash.end ();
	for (unordered_map<double, unsigned int>::const_iterator it = cache_hash.begin (); it != itEnd; ++it)
	{
		double key = it->first;
		double count = it->second;

		cout << key << ", " << count << endl;
	}

	cout << "cache_hash.size (): " << cache_hash.size () << endl;
}

int main ()
{
	cout.precision(FLOAT_PRECISION);
	init_inverse_squares ();
	init_max_possible_total ();
	//init_cache ();
	//init_cache_hash ();
	init_cache_pair ();
}
