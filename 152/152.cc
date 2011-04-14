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

#include <limits>

const unsigned int FLOAT_PRECISION = 15;
//const double DELTA = 1.0/pow(10,FLOAT_PRECISION+1);

//#define FLOAT_EQ(x,y) (fabs((x)-(y)) <= DELTA)
#define FLOAT_EQ(x,y) (std::fabs((x)-(y)) < std::numeric_limits<double>::epsilon())
/*
#define FLOAT_GT(x,y) ((x)-(y) > DELTA)
#define FLOAT_GE(x,y) (FLOAT_GT((x),(y)) || FLOAT_EQ((x),(y)))
#define FLOAT_LT(x,y) ((y)-(x) > DELTA)
#define FLOAT_LE(x,y) (FLOAT_LT((x),(y)) || FLOAT_EQ((x),(y)))
*/

#define FLOAT_GT(x,y) ((x)-(y) >= std::numeric_limits<double>::epsilon())
#define FLOAT_GE(x,y) (FLOAT_GT((x),(y)) || FLOAT_EQ((x),(y)))
#define FLOAT_LT(x,y) ((y)-(x) >= std::numeric_limits<double>::epsilon())
#define FLOAT_LE(x,y) (FLOAT_LT((x),(y)) || FLOAT_EQ((x),(y)))

/*
#define FLOAT_GT(x,y) ((x)>(y))
#define FLOAT_GE(x,y) (FLOAT_GT((x),(y)) || FLOAT_EQ((x),(y)))
#define FLOAT_LT(x,y) ((x)<(y))
#define FLOAT_LE(x,y) (FLOAT_LT((x),(y)) || FLOAT_EQ((x),(y)))
*/

double* inverse_squares;
double* max_possible_total;

size_t solutions = 0;

//ofstream s("solutions.txt");

/*
size_t max_value = 80;

size_t cache1_from = 27;
size_t cache1_to = 53;
size_t cache2_from = 54;
size_t cache2_to = 80;
*/

/*
size_t max_value = 70;

size_t cache1_from = 17;
size_t cache1_to = 43;
size_t cache2_from = 44;
size_t cache2_to = 70;
*/

/*
size_t max_value = 60;

size_t cache1_from = 7;
size_t cache1_to = 33;
size_t cache2_from = 34;
size_t cache2_to = 60;
*/

size_t max_value = 60;

size_t cache1_from = 17;
size_t cache1_to = 38;
size_t cache2_from = 39;
size_t cache2_to = 60;

/*
size_t max_value = 60;

size_t cache1_from = 25;
size_t cache1_to = 44;
size_t cache2_from = 45;
size_t cache2_to = 60;
*/

/*
size_t max_value = 60;

size_t cache1_from = 15;
size_t cache1_to = 39;
size_t cache2_from = 40;
size_t cache2_to = 60;
*/

/*
size_t max_value = 50;

size_t cache1_from = 5;
size_t cache1_to = 27;
size_t cache2_from = 28;
size_t cache2_to = 50;
*/

/*
size_t max_value = 50;

size_t cache1_from = 25;
size_t cache1_to = 37;
size_t cache2_from = 38;
size_t cache2_to = 50;
*/

/*
size_t max_value = 40;

size_t cache1_from = 15;
size_t cache1_to = 27;
size_t cache2_from = 28;
size_t cache2_to = 40;
*/

	/*
void write_solution(size_t i1, size_t i2, size_t i3)
{
	s << 2;

	for (size_t i = 3; i < cache1_from; i++)
		if ((i1 >> (i-3)) & 0x1)
			s << ", " << i;

	for (size_t i = cache1_from; i <= cache1_to; i++)
		if ((i2 >> (i-cache1_from)) & 0x1)
			s << ", " << i;

	for (size_t i = cache2_from; i <= cache2_to; i++)
		if ((i3 >> (i-cache2_from)) & 0x1)
			s << ", " << i;

	s << endl;
}
	*/

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
		inverse_squares[i] = 1.0/pow(i, 2);
}

void init_max_possible_total (const unsigned int& max_value)
{
	max_possible_total = new double[max_value+1];

	max_possible_total[max_value] = inverse_squares[max_value];

	for (unsigned int i = max_value-1; i >= 2; i--)
		max_possible_total[i] = inverse_squares[i] + max_possible_total[i+1];
}

void generate_cache(double v[], size_t from, size_t to)
{
	cout << "generating cache - from: " << from << ", to: " << to << endl;
	cout.flush();

	size_t index = 0;
	v[index++] = 0;

	for (size_t i = from; i <= to; i++)
	{
		size_t size = index;

		for (size_t c = 0; c < size; c++)
			v[index++] = inverse_squares[i] + v[c];
	}

	qsort(v, index, sizeof(double), cmp);
}

int binary_search(const double cache[], const size_t& cache_size, const double& v)
{
	int mid;
	int min = 0;
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

			if (FLOAT_GT(new_value, 0.25))
				break;
			else if (FLOAT_EQ(new_value, 0.25) || FLOAT_EQ(new_value + max_possible_total[item+1], 0.25))
				solutions++;
			else if (FLOAT_GT(new_value + max_possible_total[item+1], 0.25))
				data.push_back(new_value);
		}

		sort (data.begin (), data.end ());

		vector<double>::iterator it;

		for (it = data.begin()+1; it != data.end(); ++it)
		{
			if (FLOAT_EQ(*it+max_possible_total[item+1], 0.25))
				solutions++;
			else if (FLOAT_GT(*it+max_possible_total[item+1], 0.25))
				break;
		}

		if (it != data.begin()+1)
			data.erase(data.begin()+1, it);
	}
}

void search_for_solution(const vector<double>& data, const double cache1[], const size_t& cache1_size, const double cache2[], const size_t& cache2_size)
{
	cout << "searching for solutions..." << endl;

	size_t i1, i2;
	vector<double>::const_iterator it = data.begin();
	size_t data_size = data.size();

	size_t i1_init = cache1_size-1;
	size_t i2_init = cache2_size-1;

	printf("\033[s");

	for (size_t i = 0; i < data_size; i++)
	{
		const double& value = *(it++);

		while (i1_init > 0 && FLOAT_GT(value+cache1[i1_init]+cache2[cache2_size-1], 0.25))
			i1_init--;

		while (i2_init > 0 && FLOAT_GT(value+cache1[i1_init]+cache2[i2_init], 0.25))
			i2_init--;

		i1 = i1_init;
		i2 = i2_init;

		for (; i2 > 0 && i1 < cache1_size && FLOAT_LT(value+cache1[i1], 0.25); i1++)
		{
			while (i2 < cache2_size && FLOAT_EQ(cache2[i2], cache2[i2+1]))
				i2++;

			while (i2 > 0 && FLOAT_GT(value+cache1[i1]+cache2[i2], 0.25))
				i2--;

			while (i2 > 0 && FLOAT_EQ(value+cache1[i1]+cache2[i2], 0.25))
			{
				solutions++;
				i2--;
			}
		}

		while (i1 < cache1_size && FLOAT_LT(value+cache1[i1], 0.25))
			i1++;

		while (i1 < cache1_size && FLOAT_EQ(value+cache1[i1], 0.25))
		{
			solutions++;
			i1++;
		}

		printf("\033[u");
		cout << setw(7) << setfill('0') << (i+1) << "/" << data_size << ", solutions: " << solutions << ", i1: " << i1_init << " - " << i1 << ", i2: " << i2_init << " - " << i2 << ", v: " << value << ", v1_init: " << cache1[i1_init] << ", v2_init: " << cache2[i2_init];
		cout.flush();
	}

	cout << endl;
}

void try3(int argc, char* argv[])
{
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
	cout << "cache1 - 0: " << cache1[0] << ", first: " << cache1[1] << ", last: " << cache1[cache1_size-1] << endl;
	cout << "cache2 - 0: " << cache2[0] << ", first: " << cache2[1] << ", last: " << cache2[cache2_size-1] << endl;

	search_for_solution(data, cache1, cache1_size, cache2, cache2_size);

	cout << "solutions: " << solutions << endl;
}

int main (int argc, char* argv[])
{
	cout.precision(FLOAT_PRECISION);
	cout.setf(ios::fixed,ios::floatfield);
	try3(argc, argv);
}
