#include <cstdlib>
#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

const unsigned int FLOAT_PRECISION = 15;
const double DELTA = 1/pow(10,FLOAT_PRECISION+1);

#define FLOAT_EQ(x,y) (fabs((x)-(y)) <= DELTA)
#define FLOAT_GT(x,y) ((x)-(y) > DELTA)
#define FLOAT_GE(x,y) (FLOAT_GT((x),(y)) || FLOAT_EQ((x),(y)))
#define FLOAT_LT(x,y) ((y)-(x) > DELTA)
#define FLOAT_LE(x,y) (FLOAT_LT((x),(y)) || FLOAT_EQ((x),(y)))

double* inverse_squares;
double* max_possible_total;
vector<pair<double, unsigned short> > cache_pair;
vector<double> cache;

void init_inverse_squares (unsigned int max_value)
{
	inverse_squares = new double[max_value+1];

	for (unsigned int i = 2; i <= max_value; i++)
	{
		inverse_squares[i] = 1/pow(i, 2);
		//cout << "inverse_squares[" << i << "]: " << inverse_squares[i] << endl;
	}
}

void init_max_possible_total (unsigned int max_value)
{
	max_possible_total = new double[max_value+1];

	max_possible_total[max_value] = inverse_squares[max_value];

	for (unsigned int i = max_value-1; i >= 2; i--)
		max_possible_total[i] = inverse_squares[i] + max_possible_total[i+1];
}

void init_cache_pair (unsigned int max_value, unsigned int items_cache)
{
	cache_pair.push_back(make_pair<double, unsigned short>(0.0, 1));

	for (unsigned int item = max_value; item > max_value-items_cache; item--)
	{
		cout << "processing item " << item << endl;
		vector<pair<double, unsigned short> > cache_temp;

		for (unsigned int i = 0; i < cache_pair.size (); i++)
		{
			double new_value = inverse_squares[item] + cache_pair[i].first;
			cache_temp.push_back (make_pair<double, unsigned short> (new_value, cache_pair[i].second));
		}

		cout << "merging caches - cache_pair.size ():" << cache_pair.size () << ", cache_temp.size (): " << cache_temp.size () << endl;
		cout.flush ();

		unsigned int i_cache = 0;
		unsigned int i_cache_temp = 0;
		unsigned int cache_size = cache_pair.size ();
		unsigned int cache_temp_size = cache_temp.size ();

		while (i_cache < cache_size && i_cache_temp < cache_temp_size)
		{
			if (FLOAT_EQ (cache_pair[i_cache].first, cache_temp[i_cache_temp].first))
			{
				//cout << "found duplicate!" << endl;
				cache_pair[i_cache].second += cache_temp[i_cache_temp].second;
				i_cache++;
				i_cache_temp++;
			} else if (FLOAT_LT(cache_pair[i_cache].first, cache_temp[i_cache_temp].first))
			{
				i_cache++;
			} else
			{
				cache_pair.push_back (cache_temp[i_cache_temp]);
				i_cache_temp++;
			}
		}

		while (i_cache_temp < cache_temp_size)
			cache_pair.push_back(cache_temp[i_cache_temp++]);

		cout << "sorting cache - cache_pair.size ():" << cache_pair.size () << endl;
		cout.flush ();
		sort (cache_pair.begin (), cache_pair.end ());
	}

	unsigned int items = 0;
	unsigned int max_item = 0;
	vector<pair<double, unsigned short> >::const_iterator it_cache_end = cache_pair.end ();
	for (vector<pair<double, unsigned short> >::const_iterator it_cache = cache_pair.begin (); it_cache != it_cache_end; ++it_cache)
	{
		//cout << "first: " << it_cache->first << ", second: " << it_cache->second << endl;
		items += it_cache->second;
		if (it_cache->second > max_item)
			max_item = it_cache->second;
	}
	cout << "cache_pair.size (): " << cache_pair.size () << endl;
	cout << "items: " << items << endl;
	cout << "max_item: " << max_item << endl;
}

void init_cache (unsigned int max_value, unsigned int items_cache)
{
	cache.push_back(0.0);

	for (unsigned int item = max_value; item > max_value-items_cache; item--)
	{
		cout << "processing item " << item << endl;
		vector<double> cache_temp;

		for (unsigned int i = 0; i < cache.size (); i++)
		{
			double new_value = inverse_squares[item] + cache[i];
			cache_temp.push_back (new_value);
		}

		cout << "merging caches - cache.size ():" << cache.size () << ", cache_temp.size (): " << cache_temp.size () << endl;
		cout.flush ();

		cache.insert (cache.end (), cache_temp.begin (), cache_temp.end ());
		/*
		unsigned int i_cache = 0;
		unsigned int i_cache_temp = 0;
		unsigned int cache_size = cache.size ();
		unsigned int cache_temp_size = cache_temp.size ();

		while (i_cache < cache_size && i_cache_temp < cache_temp_size)
		{
			if (FLOAT_EQ (cache[i_cache].first, cache_temp[i_cache_temp].first))
			{
				//cout << "found duplicate!" << endl;
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
		*/

		cout << "sorting cache - cache.size ():" << cache.size () << endl;
		cout.flush ();
		sort (cache.begin (), cache.end ());
	}

	cout << "cache.size (): " << cache.size () << endl;
}

const bool is_solution (const double& total)
{
	//return (total == 0.5);
	return FLOAT_EQ(total, 0.5);
}

const bool promising (const unsigned int& item, const double& value)
{
	//return (total + inverse_squares[item] <= 0.5 && total + inverse_squares[item] + max_possible_total[item+1] >= 0.5);
	return (FLOAT_LT(value, 0.5) && FLOAT_GE(value + max_possible_total[item+1], 0.5));
}

void check_item(const unsigned int item, const double total, const unsigned int& max_value, vector<unsigned int>& solution, const unsigned int& items_cache)
{
	double value = total + inverse_squares[item];

	if (is_solution (value))
	{
		cout << "solution: " << solution[0];

		for (unsigned int i = 1; i < solution.size (); i++)
			cout << ", " << solution[i];

		cout << ", " << item << endl;
	} else if (item < max_value && promising (item, value))
	{
		solution.push_back (item);
		for (unsigned int i = item + 1; i <= max_value-items_cache; i++)
			check_item (i, value, max_value, solution, items_cache);
	
		// TODO: check if we have solutions in cache

		solution.pop_back ();
	}
	/*
	else
	{
		//cout << "backtracking on " << item << endl;
	}
	*/
}

void run_backtracking (unsigned int& max_value, const unsigned int& items_cache)
{
	/*
	stack<unsigned int> stack_items;

	stack_items.push_back (2);

	while (! stack_items.empty ())
	{
	}
	*/

   double total = 0.0;
   vector<unsigned int> solution;

   for (unsigned int i = 2; i <= max_value-items_cache; i++)
   {
	   check_item(i, total, max_value, solution, items_cache);
   }
	// TODO: check if we have solutions in cache
}

int main (int argc, char* argv[])
{
	unsigned int max_value = atoi(argv[1]);
	unsigned int items_cache = atoi(argv[2]);

	cout.precision(FLOAT_PRECISION);

	init_inverse_squares (max_value);
	init_max_possible_total (max_value);

	/*
	if (items_cache)
		init_cache (max_value, items_cache);
	*/

	run_backtracking (max_value, items_cache);
}
