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

/*
#define FLOAT_EQ(x,y) ((x) == (y))
#define FLOAT_GT(x,y) ((x) > (y))
#define FLOAT_GE(x,y) ((x) >= (y))
#define FLOAT_LT(x,y) ((x) < (y))
#define FLOAT_LE(x,y) ((x) <= (y))
*/

double* inverse_squares;
double* max_possible_total;
vector<pair<double, unsigned short> > cache_pair;
vector<double> cache;
vector<double> small_ones;
unsigned int first_small_one;

vector<string> files;

size_t items_in_cache;
size_t solutions = 0;

static int cmp(const void* p1, const void* p2) 
{
	if (*(static_cast<const double*>(p1)) < *(static_cast<const double*>(p2)))
		return -1; 
	else if (*(static_cast<const double*>(p1)) > *(static_cast<const double*>(p2)))
		return 1;
	else
		return 0;
}

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

//void generate_cache(vector<double>& v, size_t from, size_t to)
void generate_cache(double v[], size_t from, size_t to)
{
	cout << "generating cache - from: " << from << ", to: " << to << endl;
	cout.flush();

	//v.clear();
	//v.push_back(0);

	size_t index = 0;
	v[index++] = 0;

	for (size_t i = from; i <= to; i++)
	{
		//size_t size = v.size();
		size_t size = index;

		for (size_t c = 0; c < size; c++)
		{
			//v.push_back(inverse_squares[i] + v[c]);
			v[index++] = inverse_squares[i] + v[c];
		}
	}

	/*
	sort (v.begin(), v.end());
	cout << "v.size(): " << v.size() << endl;
	*/
	qsort(v, index, sizeof(double), cmp);
}

int binary_search(const double cache[], const size_t& cache_size, const double& v)
{
	int mid;
	int min = 0;
	//int max = cache.size() - 1;
	int max = cache_size - 1;

	do
	{
		mid = min + (max - min)/2;

		if (FLOAT_LT(cache[mid], v))
			min = mid+1;
		else
			max = mid-1;
	} while (!FLOAT_EQ(v, cache[mid]) && min <= max);

	return mid;
}

void generate_data(vector<double>& data, const size_t& cache1_from)
{
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
}

void search_for_solution(const vector<double>& data, const double cache1[], const size_t& cache1_size, const double cache2[], const size_t& cache2_size)
{
	cout << "searching for solutions..." << endl;

	size_t data_size = data.size();
	size_t i1, i2;
	size_t i1_init, i2_init;
	double v, value_i1_i2;
	int mid;
	vector<double>::const_iterator it = data.begin();

	printf("\033[s");

	for (size_t i = 0; i < data_size; i++)
	{
		//printf("\033[u");
		//cout << setw(7) << setfill('0') << (i+1) << "/" << data_size << ", solutions: " << solutions;
		//cout.flush();

		const double& value = *(it++);

		v = 0.25-value-cache2[cache2_size-1];
		mid = binary_search(cache1, cache1_size, v);

		/*
		while (mid >= 0 && FLOAT_GE(cache1[mid], v))
			mid--;

		i1 = mid+1;
		*/
		while (mid > 0 && FLOAT_GE(cache1[mid], v))
			mid--;
		i1 = mid;

		//printf("\033[u");
		//cout << setw(7) << setfill('0') << (i+1) << "/" << data_size << ", solutions: " << solutions << ", i1: " << i1;
		//cout.flush();

		if (i1 < cache1_size)
		{
			i1_init = i1;
			v = 0.25-value-cache1[i1];
			mid = binary_search(cache2, cache2_size, v);

			/*
			while (mid < cache2_size && FLOAT_LE(cache2[mid], v))
				mid++;

			i2 = mid-1;
			*/
			while (mid < cache2_size-1 && FLOAT_LE(cache2[mid], v))
				mid++;
			i2 = mid;
			i2_init = i2;

			//printf("\033[u");
			//cout << setw(7) << setfill('0') << (i+1) << "/" << data_size << ", solutions: " << solutions << ", i1: " << i1 << ", i2: " << i2 << ", v: " << value << ", v1: " << cache1[i1] << ", v2: " << cache2[i2] << ", v_i1_i2: " << (value+cache1[i1]+cache2[i2]);
			//cout.flush();

			for (; i1 < cache1_size && FLOAT_LT(value+cache1[i1], 0.25); i1++)
			{
				if (i2 > 0)
				{
					value_i1_i2 = value+cache1[i1]+cache2[i2];

					while (i2 < cache2_size && FLOAT_EQ(cache2[i2], cache2[i2+1]))
						i2++;

					while (i2 > 0 && FLOAT_GT(value_i1_i2, 0.25))
						i2--;

					while (i2 > 0 && FLOAT_EQ(value_i1_i2, 0.25))
					{
						solutions++;
						i2--;
					}
				}
			}

			while (i1 < cache1_size && FLOAT_EQ(value+cache1[i1++], 0.25))
				solutions++;

			printf("\033[u");
			//cout << setw(7) << setfill('0') << (i+1) << "/" << data_size << ", solutions: " << solutions << ", i1(init): " << i1_init << ", i1(end): " << i1 << ", i2(init): " << i2_init << ", i2(end): " << i2 << ", v: " << value << ", v1_init: " << cache1[i1_init] << ", v2_init: " << cache2[i2_init] << ", v_i1_i2_init: " << (value+cache1[i1_init]+cache2[i2_init]);
			cout << setw(7) << setfill('0') << (i+1) << "/" << data_size << ", solutions: " << solutions << ", i1(init): " << i1_init << ", i1(end): " << i1 << ", i2(init): " << i2_init << ", i2(end): " << i2 << ", v: " << value << ", v1_init: " << cache1[i1_init] << ", v2_init: " << cache2[i2_init];
			cout.flush();
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
}

void try3(int argc, char* argv[])
{
	/*
	size_t max_value = 80; //atoi(argv[1]);

	size_t cache1_from = 27;
	size_t cache1_to = 53;
	size_t cache2_from = 54;
	size_t cache2_to = 80;
	*/

	/*
	size_t max_value = 40; //atoi(argv[1]);

	size_t cache1_from = 15;
	size_t cache1_to = 27;
	size_t cache2_from = 28;
	size_t cache2_to = 40;
	*/

	size_t max_value = 50; //atoi(argv[1]);

	size_t cache1_from = 25;
	size_t cache1_to = 37;
	size_t cache2_from = 38;
	size_t cache2_to = 50;

	size_t cache1_size = pow(2, cache1_to-cache1_from+1);
	size_t cache2_size = pow(2, cache2_to-cache2_from+1);

	cout << "cache1_size: " << cache1_size << ", cache2_size: " << cache2_size << endl;

	double* cache1 = new double[cache1_size];
	double* cache2 = new double[cache2_size];

	init_inverse_squares (max_value);
	init_max_possible_total (max_value);

	generate_cache(cache1, cache1_from, cache1_to);
	generate_cache(cache2, cache2_from, cache2_to);

	vector<double> data;
	generate_data(data, cache1_from);

	cout << "data - first: " << *(data.begin()+1) << ", last: " << data.back() << endl;
	cout << "cache1 - first: " << cache1[1] << ", last: " << cache1[cache1_size-1] << endl;
	cout << "cache2 - first: " << cache2[1] << ", last: " << cache2[cache2_size-1] << endl;

	search_for_solution(data, cache1, cache1_size, cache2, cache2_size);

	cout << "solutions: " << solutions << endl;
}

int main (int argc, char* argv[])
{
	cout.precision(FLOAT_PRECISION);
	cout.setf(ios::fixed,ios::floatfield);
	try3(argc, argv);
}
