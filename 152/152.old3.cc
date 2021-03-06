#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>
using namespace std;

const unsigned int FLOAT_PRECISION = 15;
const double DELTA = 1/pow(10,FLOAT_PRECISION+1);

const unsigned int MEM_SIZE_IN_BYTES = 512 * 1024 * 1024;
const unsigned int MEM_ITEMS = MEM_SIZE_IN_BYTES / sizeof(double);

#define FLOAT_EQ(x,y) (fabs((x)-(y)) <= DELTA)
#define FLOAT_GT(x,y) ((x)-(y) > DELTA)
#define FLOAT_GE(x,y) (FLOAT_GT((x),(y)) || FLOAT_EQ((x),(y)))
#define FLOAT_LT(x,y) ((y)-(x) > DELTA)
#define FLOAT_LE(x,y) (FLOAT_LT((x),(y)) || FLOAT_EQ((x),(y)))

double* inverse_squares;
double* max_possible_total;
vector<pair<double, unsigned short> > cache_pair;
vector<double> cache;
vector<double> small_ones;
unsigned int first_small_one;

vector<string> files;

size_t items_in_cache;
size_t solutions = 0;

void init_inverse_squares (const unsigned int& max_value)
{
	inverse_squares = new double[max_value+1];

	for (unsigned int i = 2; i <= max_value; i++)
	{
		inverse_squares[i] = 1/pow(i, 2);
		//cout << "inverse_squares[" << i << "]: " << inverse_squares[i] << endl;
	}
}

void init_max_possible_total (const unsigned int& max_value)
{
	max_possible_total = new double[max_value+1];

	max_possible_total[max_value] = inverse_squares[max_value];

	for (unsigned int i = max_value-1; i >= 2; i--)
		max_possible_total[i] = inverse_squares[i] + max_possible_total[i+1];
}

void init_cache_pair (const unsigned int& max_value, const unsigned int& items_cache)
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

/*
 * works with 2^27 items in memory
 */
void init_cache (const unsigned int& max_value, const unsigned int& items_cache)
{
	cache.push_back(0.0);

	for (unsigned int item = max_value; item > max_value-items_cache; item--)
	{
		cout << "processing item " << item << endl;
		//vector<double> cache_temp;

		size_t size = cache.size();

		for (unsigned int i = 0; i < size; i++)
		{
			double new_value = inverse_squares[item] + cache[i];
			//cache_temp.push_back (new_value);
			cache.push_back (new_value);
		}
	}

	cout << "sorting cache - cache.size ():" << cache.size () << endl;
	cout.flush ();
	sort (cache.begin (), cache.end ());

	cout << "cache.size (): " << cache.size () << endl;
}

const bool still_have_data (vector<ifstream*>& fstreams)
{
	for (vector<ifstream*>::iterator it = fstreams.begin (); it != fstreams.end (); ++it)
	{
		if (! (*it)->eof ())
			return true;
	}

	return false;
}

void sort_data ()
{
	ofstream output(".cache.dat", ios_base::binary);

	cout << "sorting data..." << endl;
	vector<ifstream*> fstreams;

	//cout << "opening files" << endl;
	for (vector<string>::const_iterator it = files.begin (); it != files.end (); ++it)
	{
		fstreams.push_back (new ifstream(it->c_str (), ios_base::binary));

		fstreams.back()->seekg(0, ios::end);
		cout << "file " << it->c_str () << ", size: " << fstreams.back()->tellg();
		fstreams.back()->seekg(0, ios::beg);

		double value;
		fstreams.back()->read(reinterpret_cast<char*>(&value), sizeof(double));
		cout << ", first element: " << value << endl;

		fstreams.back()->seekg(0, ios::beg);
	}

	//cout << "getting the very first ones" << endl;
	cout << "merging..." << endl;
	cout.flush();

	vector<double> firsts;

	for (vector<ifstream*>::iterator it = fstreams.begin (); it != fstreams.end (); ++it)
	{
		double value;
		(*it)->read(reinterpret_cast<char*>(&value), sizeof(double));
		//cout << "firsts.push_back(" << value << ")" << endl;
		firsts.push_back (value);
	}

	do
	{
		unsigned int i = 0;
		while (i < firsts.size () && FLOAT_EQ(firsts[i], 0))
			i++;
			
		if (! FLOAT_EQ(firsts[i], 0))
		{
			unsigned int less = i;

			for (++i; i < firsts.size (); i++)
			{
				if (FLOAT_GT(firsts[i], 0) && FLOAT_LT(firsts[i], firsts[less]))
				less = i;
			}

			//cout << firsts[less] << " from " << less << endl;
			output.write(reinterpret_cast<char*>(&(firsts[less])), sizeof(double));

			if (fstreams[less]->eof ())
			{
				//cout << "closed!" << endl;
				firsts[less] = 0;
			}
			else
			{
				fstreams[less]->read(reinterpret_cast<char*>(&(firsts[less])), sizeof(double));
			}
		} else {
			cout << "should'n happen!" << endl;
			exit(-1);
		}
	} while (still_have_data(fstreams));

	//cout << "closing files" << endl;

	for (vector<ifstream*>::iterator it = fstreams.begin (); it != fstreams.end (); ++it)
	{
		(*it)->close();
		delete *it;
	}

	output.close();
}

void write_cache_to_file (const vector<double>& cache, const unsigned int& size, const unsigned int& file_idx)
{
	cout.flush();

	stringstream filename;
	filename << ".cache";
	filename << file_idx;
	filename << ".dat";

	cout << "writing to file " << filename.str() << " - size: " << size << "..." << endl;

	ofstream file(filename.str().c_str(), ios_base::binary);
	for (unsigned int x = 0; x < size; x++)
	{
		//cout << "writing " << setw(2) << x << ": " << cache[x] << endl;
		file.write(reinterpret_cast<const char*>(&(cache[x])), sizeof(double));
	}
	file.close();

	files.push_back(filename.str());

	//cout << "writing finished" << endl;
	//cout.flush();
}

void generate_data (const unsigned int& max_value, const unsigned int& items_cache)
{
	cout << "generating data - " << ((1LL << items_cache) / MEM_ITEMS) << " file(s)..." << endl;
	cout.flush();

	vector<double> cache_external (MEM_ITEMS);
	unsigned int index = 0;
	unsigned int file_idx = 0;

	for (unsigned long long i = 1; i < (1LL << (items_cache)); i++)
	{
		double value = 0.0;

		for (unsigned int bit = 0; bit < items_cache; bit++)
		{
			if ((i >> bit) & 0x1)
				value += inverse_squares[max_value-bit];
		}

		cache_external[index++] = value;

		if (index == MEM_ITEMS)
		{
			cout << "sorting file " << file_idx << "..." << endl;
			cout.flush();
			sort (cache_external.begin (), cache_external.end ());

			//cout << "writing to file..." << endl;
			write_cache_to_file (cache_external, index, file_idx++);
			index = 0;
		}
	}

	if (index > 0)
	{
		cout << "sorting file " << file_idx << "..." << endl;
		sort (cache_external.begin (), cache_external.begin () + index);

		//cout << "writing to file..." << endl;
		write_cache_to_file (cache_external, index, file_idx);
	}
}

void init_cache_external (const unsigned int& max_value, const unsigned int& items_cache)
{
	generate_data (max_value, items_cache);
	sort_data ();
}

void init_small_ones (const unsigned int& max_value)
{
	double v = inverse_squares[max_value] + inverse_squares[max_value-1];

	for (unsigned int i = max_value; i > 0 && FLOAT_LT(inverse_squares[i], v); i--)
		first_small_one = i;

	cout << "first_small_one: " << first_small_one << endl;
}

const bool is_solution (const double& total)
{
	//return (total == 0.5);
	return FLOAT_EQ(total, 0.5);
}

const bool promising (const unsigned int& item, const double& value, const unsigned int& max_value)
{
	//return (total + inverse_squares[item] <= 0.5 && total + inverse_squares[item] + max_possible_total[item+1] >= 0.5);
	//return (FLOAT_LT(value, 0.5) && FLOAT_GE(value + max_possible_total[item+1], 0.5));
	return (FLOAT_LE(value+inverse_squares[max_value], 0.5) && FLOAT_GE(value + max_possible_total[item+1], 0.5));
}

void check_cache(const double& total, ifstream& cache_file, const vector<unsigned int>& solution)
{
	int min = 0;
	int max = items_in_cache - 1;
	int mid;

	double v_search = 0.5 - total;
	double v;

	//cout << "checking cache - items_in_cache: " << items_in_cache << ", v_search: " << v_search << endl;
	//cache_file.seekg(0, ios::end);

	if (cache_file.eof ())
	{
		cache_file.close ();
		cache_file.clear ();
   		cache_file.open(".cache.dat", ios_base::binary);
	}
	else
		cache_file.seekg(0, ios::beg);

	do
	{
		mid = min + (max - min)/2;

		cache_file.seekg(mid*sizeof(double), ios::beg);
		cache_file.read(reinterpret_cast<char*>(&v), sizeof(double));
		//cache_file.seekg(mid*sizeof(double), ios::beg);

		//cout << "trying min: " << min << ", mid: " << mid << ", max: " << max << ", v: " << v << ", tellg: " << cache_file.tellg() << endl;

		if (FLOAT_LT(v, v_search))
		{   
			min = mid+1;
		}   
		else
		{   
			max = mid-1;
		}
	} while (!FLOAT_EQ(v, v_search) && min <= max);

	while (mid > 0 && FLOAT_EQ(v, v_search))
	{
		cache_file.seekg((--mid)*sizeof(double), ios::beg);
		cache_file.read(reinterpret_cast<char*>(&v), sizeof(double));
	}

	if (FLOAT_LT(v, v_search))
	{
		cache_file.seekg((++mid)*sizeof(double), ios::beg);
		cache_file.read(reinterpret_cast<char*>(&v), sizeof(double));
	}

	while (FLOAT_EQ(total + v, 0.5))
	{
		if (solution.size() > 0)
		{
			cout << "solution: " << solution[0];

			for (unsigned int i = 1; i < solution.size (); i++)
				cout << ", " << solution[i];

			cout << ", ... - total: " << total << ", v: " << v << endl;
		}
		else
		{
			cout << "!!!SOLUTION!!!" << endl;;
		}

		cout.flush();
		cache_file.read(reinterpret_cast<char*>(&v), sizeof(double));
	}

	//cout << "checking cache end" << endl;
}

void check_item(const unsigned int& item, const double& total_old, const unsigned int& max_value, vector<unsigned int>& solution, const unsigned int& items_cache, ifstream& cache_file)
{
	double total_new = total_old + inverse_squares[item];

	//if (solution.size () < 9)
	if (total_new <= 0.5)
	{
		cout << "check item ";

		for (vector<unsigned int>::const_iterator it = solution.begin (); it != solution.end(); ++it)
			cout << *it << ", ";
		cout << item << ", total: " << total_new << endl;
	}

	if (is_solution (total_new))
	{
		cout << "solution: " << solution[0];

		for (unsigned int i = 1; i < solution.size (); i++)
			cout << ", " << solution[i];

		cout << ", " << item << endl;
		cout.flush();
	} else if (item < max_value && promising (item, total_new, max_value))
	{
		//cout << "promising" << endl;
		solution.push_back (item);
		for (unsigned int i = item + 1; i <= max_value-items_cache; i++)
			check_item (i, total_new, max_value, solution, items_cache, cache_file);
	
		check_cache(total_new, cache_file, solution);
		//cout << "tellg: " << cache_file.tellg() << endl;

		solution.pop_back ();
	}
	else
	{
		//cout << "backtracking on " << item << endl;
	}
}

void run_backtracking (unsigned int& max_value, const unsigned int& items_cache)
{
	cout << "running backtracking" << endl;
	/*
	stack<unsigned int> stack_items;

	stack_items.push_back (2);

	while (! stack_items.empty ())
	{
	}
	*/

   double total = 0.0;
   vector<unsigned int> solution;
   ifstream cache_file(".cache.dat", ios_base::binary);

   cache_file.seekg(0, ios::end);
   items_in_cache = cache_file.tellg()/sizeof(double);
   cache_file.seekg(0, ios::beg);

   for (unsigned int i = 2; i <= max_value-items_cache; i++)
   {
	   check_item(i, total, max_value, solution, items_cache, cache_file);
   }

   check_cache(total, cache_file, solution);
}

void run_breadthsearch (unsigned int& max_value, const unsigned int& items_cache)
{
	cout << "running breadthsearch" << endl;

	vector<double> data;
	data.push_back (0);

	for (unsigned int item = 3; item <= max_value-items_cache; item++)
	{
		cout << "checking item " << item << endl;

		size_t size = data.size ();

		for (unsigned int c = 0; c < size; c++)
		{
			double new_value = inverse_squares[item] + data[c];

			if (FLOAT_LT(new_value+inverse_squares[first_small_one], 0.25))
			//if (FLOAT_LT(new_value+inverse_squares[max_value], 0.25))
			//if (FLOAT_LT(new_value, 0.25))
			{
				if (FLOAT_GE(new_value + max_possible_total[item+1], 0.25))
					data.push_back(new_value);
			}
			else if (FLOAT_EQ(new_value, 0.25))
			{
				cout << "!!!SOLUTION!!!" << endl;
				break;
			}
			else {
				for (unsigned int i = (item < first_small_one ? first_small_one:item+1); i <= max_value; i++)
				{
					if (FLOAT_EQ(new_value + inverse_squares[i], 0.25))
					{
						cout << "!!!SOLUTION!!! (small_ones) - new_value: " << new_value << ", small_one: " << i << endl;
						break;
					}
					/*
					   else
					   {
					   if (item == 28)
					   {
					   cout << "checking new_value: " << new_value << ", small_one: " << *it << endl;
					   }
					   }
					   */
				}

				if (FLOAT_GE(new_value+inverse_squares[max_value], 0.25))
				{
					break;
				}

					/*
				if (FLOAT_EQ(new_value, 0.25)
						|| FLOAT_EQ(new_value+inverse_squares[max_value], 0.25)
						|| FLOAT_EQ(new_value+inverse_squares[max_value-1], 0.25)
						|| FLOAT_EQ(new_value+inverse_squares[max_value-2], 0.25)
						|| FLOAT_EQ(new_value+inverse_squares[max_value-3], 0.25)
						|| FLOAT_EQ(new_value+inverse_squares[max_value-4], 0.25)
						|| FLOAT_EQ(new_value+inverse_squares[max_value-5], 0.25)
						|| FLOAT_EQ(new_value+inverse_squares[max_value-6], 0.25)
						|| FLOAT_EQ(new_value+inverse_squares[max_value-7], 0.25)
						|| FLOAT_EQ(new_value+inverse_squares[max_value-8], 0.25)
						|| FLOAT_EQ(new_value+inverse_squares[max_value-9], 0.25)
						|| FLOAT_EQ(new_value+inverse_squares[max_value-10], 0.25))
				//if (FLOAT_EQ(new_value, 0.25) || FLOAT_EQ(new_value+inverse_squares[max_value], 0.25))
				//if (FLOAT_EQ(new_value, 0.25))
					cout << "!!!SOLUTION!!!" << endl;
					*/

				//break;
			}
		}

		sort (data.begin (), data.end ());
		//cout << "data.size(): " << data.size() << "/" << pow(2, item-2) << ", total - last: " << data.back() << ", last-1: " << *(data.end()-2) << ", middle: " << data[data.size()/2] <<", first: " << data.front() << ", max_possible_total[" << (item+1) << "]: " << max_possible_total[item+1] << endl;

		vector<double>::iterator it = data.begin();
		/*
		while (FLOAT_LE(*it + max_possible_total[item+1], 0.25))
		{
			if (FLOAT_EQ(*it + max_possible_total[item+1], 0.25))
				cout << "!!!SOLUTION!!!" << endl;

			++it;
		}
		*/
		if (it != data.end() && item < max_value)
		{
			while (it != data.end() && FLOAT_LE(*it + max_possible_total[item+1], 0.25))
			{
				if (FLOAT_EQ(*it + max_possible_total[item+1], 0.25))
					cout << "!!!SOLUTION!!!" << endl;

				++it;
			}

			data.erase(data.begin(), it);
		}
		cout << "data.size(): " << data.size() << "/" << pow(2, item-2) << ", first: " << data.front() << ", last: " << data.back() << endl;
	}

	if (items_cache)
	{
		vector<unsigned int> solution;
		ifstream cache_file(".cache.dat", ios_base::binary);

		cache_file.seekg(0, ios::end);
		items_in_cache = cache_file.tellg()/sizeof(double);
		cache_file.seekg(0, ios::beg);

		cout << "checking cache..." << endl;
		cout.flush();

		for (vector<double>::iterator it = data.begin(); it != data.end(); ++it)
		{
			check_cache(0.25 + *it, cache_file, solution);
		}
	}
}

void try1(int argc, char* argv[])
{
	unsigned int max_value = atoi(argv[1]);
	unsigned int items_cache = atoi(argv[2]);

	cout.precision(FLOAT_PRECISION);

	init_inverse_squares (max_value);
	init_max_possible_total (max_value);

	if (items_cache)
		init_cache_external (max_value, items_cache);

	init_small_ones(max_value);

	/*
	if (items_cache)
		init_cache (max_value, items_cache);
		//init_cache_pair (max_value, items_cache);
		*/

	//run_backtracking (max_value, items_cache);
	run_breadthsearch (max_value, items_cache);
}

void generate_small_ones(size_t& max_value, unsigned int item_from, const double max_item)
{
	cout << "generating small ones - item_from: " << item_from << ", max_item: " << max_item << endl;
	cout.flush();

	small_ones.clear();
	small_ones.push_back(0);

	for (size_t i = max_value; i >= item_from; i--)
	{
		cout << "small ones checking item " << i << ", small_ones.size(): " << small_ones.size() << endl;

		size_t size = small_ones.size();

		for (size_t c = 0; c < size; c++)
		{
			double new_value = inverse_squares[i] + small_ones[c];

			if (new_value <= max_item)
				small_ones.push_back(new_value);
			else
				break;
		}

		if (small_ones.size() > size)
			sort (small_ones.begin(), small_ones.end());
	}
	cout << "small_ones.size(): " << small_ones.size() << endl;
}

void generate_big_ones(size_t& max_value, unsigned int item_from, const double min_item)
{
	cout << "generating big ones - item_from: " << item_from << ", min_item: " << min_item << ", max_possible: " << max_possible_total[item_from] << endl;
	cout.flush();

	small_ones.clear();
	small_ones.push_back(max_possible_total[item_from]);

	for (size_t i = item_from; i <= max_value; i++)
	{
		cout << "big ones checking item " << i << ", small_ones.size(): " << small_ones.size() << endl;

		size_t size = small_ones.size();

		for (size_t c = size-1; c >= 0; c--)
		{
			double new_value = small_ones[c] - inverse_squares[i];

			if (new_value >= min_item)
			{
				if (new_value <= 0.25)
					small_ones.push_back(new_value);
			}
			else
				break;
		}

		if (small_ones.size() > size)
			sort (small_ones.begin(), small_ones.end());
	}
	cout << "small_ones.size(): " << small_ones.size() << endl;
}


void run_breadthsearch_step (unsigned int& max_value)
{
	cout << "running breadthsearch_step" << endl;

	vector<double> data;
	data.push_back (0);

	for (unsigned int item = 3; item <= max_value; item++)
	{
		cout << "checking item " << item << endl;

		size_t size = data.size ();

		for (unsigned int c = 0; c < size; c++)
		{
			double new_value = inverse_squares[item] + data[c];

			if (FLOAT_LT(new_value, 0.25))
			{
				if (FLOAT_GE(new_value + max_possible_total[item+1], 0.25))
					data.push_back(new_value);
			}
			else if (FLOAT_EQ(new_value, 0.25))
			{
				cout << "!!!SOLUTION!!!" << endl;
				break;
			}
			else
				break;
		}

		sort (data.begin (), data.end ());
		cout << "data.size(): " << data.size() << "/" << pow(2, item-2) << ", first: " << data.front() << ", last: " << data.back() << endl;

		//if (data.size() >= 1*1024*1024)
		if (item >= 30)
		{
			/*
			 * generate small ones
			 * find solutions using small ones
			 * remove last ones from data
			 */

			size_t i = 0;
			vector<double>::iterator it;

			//generate_small_ones(max_value, item+1, (item+1 < max_value-45 ? inverse_squares[max_value-57]:0.25 - data[3*(data.size()/4)]));
			generate_small_ones(max_value, item+1, 0.25 - data[14*(data.size()/16)]);

			for (it = data.end()-1; it != data.begin(); --it)
			{
				while (i < small_ones.size() && FLOAT_LT(*it + small_ones[i], 0.25))
					i++;

				if (i < small_ones.size())
				{
					if (FLOAT_EQ (*it + small_ones[i], 0.25))
						cout << "!!!SOLUTION!!! (small_ones)" << endl;
				}
				else
					break;
			}

			data.erase(++it, data.end());

			cout << "data.size(): " << data.size() << "/" << pow(2, item-2) << ", first: " << data.front() << ", last: " << data.back() << " (after erase small ones)" << endl;

			generate_big_ones(max_value, item+1, 0.25 - data[3*(data.size()/8)]);
			i = small_ones.size()-1;

			for (it = data.begin()+1; it != data.end(); ++it)
			{
				while (i > 0 && FLOAT_GT(*it + small_ones[i], 0.25))
					i--;

				if (i > 0)
				{
					if (FLOAT_EQ (*it + small_ones[i], 0.25))
						cout << "!!!SOLUTION!!! (big_ones)" << endl;
				}
				else
					break;
			}

			data.erase(data.begin()+1, it);

			cout << "data.size(): " << data.size() << "/" << pow(2, item-2) << ", first: " << data.front() << ", last: " << data.back() << " (after erase big ones)" << endl;
		}
	}
}

void try2(int argc, char* argv[])
{
	unsigned int max_value = atoi(argv[1]);

	cout.precision(FLOAT_PRECISION);

	init_inverse_squares (max_value);
	init_max_possible_total (max_value);

	run_breadthsearch_step(max_value);
	//generate_small_ones(max_value, 30, 0.002);

	//for (size_t i = 0; i < small_ones.size(); i++)
	//	cout << small_ones[i] << endl;
}

void generate_cache(vector<double>& v, size_t from, size_t to)
{
	cout << "generating cache - from: " << from << ", to: " << to << endl;
	cout.flush();

	v.clear();
	v.push_back(0);

	for (size_t i = from; i <= to; i++)
	{
		size_t size = v.size();

		for (size_t c = 0; c < size; c++)
		{
			v.push_back(inverse_squares[i] + v[c]);
		}
	}

	sort (v.begin(), v.end());
	cout << "v.size(): " << v.size() << endl;
}

void try3(int argc, char* argv[])
{
	size_t max_value = 80; //atoi(argv[1]);

	size_t cache1_from = 27;
	size_t cache1_to = 53;
	size_t cache2_from = 54;
	size_t cache2_to = 80;

	/*
	size_t max_value = 40; //atoi(argv[1]);

	size_t cache1_from = 15;
	size_t cache1_to = 27;
	size_t cache2_from = 28;
	size_t cache2_to = 40;

	size_t max_value = 50; //atoi(argv[1]);

	size_t cache1_from = 25;
	size_t cache1_to = 37;
	size_t cache2_from = 38;
	size_t cache2_to = 50;
	*/

	size_t cache1_size = pow(2, cache1_to-cache1_from+1);
	size_t cache2_size = pow(2, cache2_to-cache2_from+1);

	cout << "cache1_size: " << cache1_size << ", cache2_size: " << cache2_size << endl;

	vector<double> cache1(cache1_size);
	vector<double> cache2(cache2_size);

	init_inverse_squares (max_value);
	init_max_possible_total (max_value);

	generate_cache(cache1, cache1_from, cache1_to);
	generate_cache(cache2, cache2_from, cache2_to);

	vector<double> data;
	data.push_back (0);

	for (unsigned int item = 3; item < cache1_from; item++)
	{
		cout << "checking item " << item << endl;

		size_t size = data.size ();

		for (unsigned int c = 0; c < size; c++)
		{
			double new_value = inverse_squares[item] + data[c];

			if (FLOAT_LT(new_value, 0.25))
			{
				if (FLOAT_GE(new_value + max_possible_total[item+1], 0.25))
					data.push_back(new_value);
			}
			else if (FLOAT_EQ(new_value, 0.25))
				solutions++;
			else
				break;
		}

		cout << "sorting data..." << endl;
		sort (data.begin (), data.end ());
		cout << "data.size(): " << data.size() << "/" << pow(2, item-2) << ", first: " << *(data.begin()+1) << ", last: " << data.back() << endl;

		cout << "searching for small items to erase..." << endl;

		vector<double>::iterator it;

		for (it = data.begin()+1; it != data.end(); ++it)
		{
			if (FLOAT_EQ(*it+max_possible_total[item+1], 0.25))
				solutions++;
			else if (FLOAT_GT(*it+max_possible_total[item+1], 0.25))
				break;
		}

		if (it != data.begin()+1)
		{
			cout << "erasing small ones..." << endl;
			data.erase(data.begin()+1, it);
			cout << "data.size(): " << data.size() << "/" << pow(2, item-2) << ", first: " << *(data.begin()+1) << ", last: " << data.back() << " (after erasing small ones)" << endl;
		}
	}

	cout << "data - first: " << *(data.begin()+1) << ", last: " << data.back() << endl;
	cout << "cache1 - first: " << *(cache1.begin()+1) << ", last: " << cache1.back() << endl;
	cout << "cache2 - first: " << *(cache2.begin()+1) << ", last: " << cache2.back() << endl;

	cout << "searching for solutions..." << endl;

	printf("\033[s");

	for (size_t i = 0; i < data.size(); i++)
	{
		printf("\033[u");
		cout << setw(7) << setfill('0') << (i+1) << "/" << data.size() << ", solutions: " << solutions;
		cout.flush();

		/*
		 * using binary search for both caches
		 */
		double v = 0.25-data[i]-cache2.back();
		int min = 0;
		int max = cache1.size() - 1;
		int mid;

		do
		{
			mid = min + (max - min)/2;

			if (FLOAT_LT(cache1[mid], v))
				min = mid+1;
			else
				max = mid-1;
		} while (!FLOAT_EQ(v, cache1[mid]) && min <= max);

		while (mid >= 0 && FLOAT_GE(cache1[mid], v))
			mid--;

		size_t i1 = mid+1;

		if (i1 < cache1.size())
		{
			v = 0.25-data[i]-cache1[i1];
			min = 0;
			max = cache2.size() - 1;
			mid;

			do
			{
				mid = min + (max - min)/2;

				if (FLOAT_LT(cache2[mid], v))
					min = mid+1;
				else
					max = mid-1;
			} while (!FLOAT_EQ(v, cache2[mid]) && min <= max);

			while (mid < cache2.size() && FLOAT_LE(cache2[mid], v))
				mid++;

			size_t i2 = mid-1;

			/*
			for (; i1 < cache1.size() && FLOAT_LE(data[i]+cache1[i1], 0.25); i1++)
			{
				if (FLOAT_EQ(data[i]+cache1[i1], 0.25))
					solutions++;
				else
				{
					while (i2 < cache2.size() && FLOAT_EQ(cache2[i2], cache2[i2+1]))
						i2++;

					while (i2 > 0 && FLOAT_GT(data[i]+cache1[i1]+cache2[i2], 0.25))
						i2--;

					while (i2 > 0 && FLOAT_EQ(data[i]+cache1[i1]+cache2[i2], 0.25))
					{
						solutions++;
						i2--;
					}
				}
			}
			*/
		}

		/*
		for (size_t i1 = mid+1; i1 < cache1.size() && FLOAT_LE(data[i]+cache1[i1], 0.25); i1++)
		{
			double v = 0.25-data[i]-cache1[i1];

			int min = 0;
			int max = cache2.size() - 1;
			int mid;

			do
			{
				mid = min + (max - min)/2;

				if (FLOAT_LT(cache2[mid], v))
					min = mid+1;
				else
					max = mid-1;
			} while (!FLOAT_EQ(v, cache2[mid]) && min <= max);

			while (mid > 0 && FLOAT_GE(cache2[mid], v))
				mid--;

			while (++mid < cache2.size() && FLOAT_EQ(data[i]+cache1[i1]+cache2[mid], 0.25))
				solutions++;
		}
				*/
	


		/*
		 * simple search
		 *
		if (FLOAT_LT(0.25-data[i], cache1[cache1.size()/2]))
		{
			for (size_t i1 = 0; i1 < cache1.size() && FLOAT_LE(data[i]+cache1[i1], 0.25); i1++)
			{
				if (FLOAT_LT(0.25-data[i]-cache1[i1], cache2[cache2.size()/2]))
				{
					for (size_t i2 = 0; i2 < cache2.size() && FLOAT_LE(data[i]+cache1[i1]+cache2[i2], 0.25); i2++)
					{
						if (FLOAT_EQ(data[i]+cache1[i1]+cache2[i2], 0.25))
							solutions++;
					}
				}
				else
				{
					for (int i2 = cache2.size()-1; i2 >= 0 && FLOAT_GE(data[i]+cache1[i1]+cache2[i2], 0.25); i2--)
					{
						if (FLOAT_EQ(data[i]+cache1[i1]+cache2[i2], 0.25))
							solutions++;
					}
				}
			}
		}
		else
		{
			for (int i1 = cache1.size()-1; i1 >= 0 && FLOAT_GE(data[i]+cache1[i1]+cache2.back(), 0.25); i1--)
			{
				if (FLOAT_LT(0.25-data[i]-cache1[i1], cache2[cache2.size()/2]))
				{
					for (size_t i2 = 0; i2 < cache2.size() && FLOAT_LE(data[i]+cache1[i1]+cache2[i2], 0.25); i2++)
					{
						if (FLOAT_EQ(data[i]+cache1[i1]+cache2[i2], 0.25))
							solutions++;
					}
				}
				else
				{
					for (int i2 = cache2.size()-1; i2 >= 0 && FLOAT_GE(data[i]+cache1[i1]+cache2[i2], 0.25); i2--)
					{
						if (FLOAT_EQ(data[i]+cache1[i1]+cache2[i2], 0.25))
							solutions++;
					}
				}
			}
		}
		*/
	}

	cout << endl;
	cout << "solutions: " << solutions << endl;
}

int main (int argc, char* argv[])
{
	cout.precision(FLOAT_PRECISION);
	try3(argc, argv);
}
