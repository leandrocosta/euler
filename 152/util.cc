#include <cmath>
#include <iostream>
using namespace std;

const unsigned int MAX_INDEX = 80;
const unsigned int FLOAT_PRECISION = 16;

float items[MAX_INDEX+1];

void init()
{
	for (unsigned int i = 2; i <= MAX_INDEX; i++)
	{
		items[i] = 1/powf(i,2);
		cout << "item " << i << ": " << items[i] << endl;
	}
}

float count(unsigned int from, unsigned int to)
{
	float total = 0;

	for (unsigned int i = from; i <= to; i++)
	{
		total += items[i];
	}

	return total;
}

float sum_others()
{
	return items[6] + count(8, 11) + count(13,14) + count(16,19) + count(21,27) + count(29,34) + count(36,45);
}

int main ()
{
	cout.precision(FLOAT_PRECISION);

	init();

	cout << "total (2-" << MAX_INDEX << "): " << count(2, MAX_INDEX) << endl;
	cout << "total (46-" << MAX_INDEX << "): " << count(46, MAX_INDEX) << endl;
	cout << "total (6,8-11,13-14,16-19,21-27,29-34,36-45): " << (items[6] + count(8,11) + count(13,14) + count(16,19) + count(21,27) + count(29,34) + count(36,45)) << endl;
	cout << "total (2-5,7,12,15,20,28,35): " << (count(2,5) + items[7] + items[12] + items[15] + items[20] + items[28] + items[35]) << endl;
	cout << "3-END: " << count(3, MAX_INDEX) << endl;

	cout << "item 3: " << items[3] << endl;
	cout << "item 35: " << items[35] << endl;
	cout << "others: " << sum_others() << endl;

	cout << "------" << endl;

	cout << "sum (float): " << (1/powf(2,2) + 1/powf(3,2) + 1/powf(4,2)) << endl;
	cout << "sum (double): " << (1/pow(2,2) + 1/pow(3,2) + 1/pow(4,2)) << endl;

	cout << "total (2-80): " << count(2,80) << endl;
	cout << "total (3-80): " << count(3,80) << endl;
	cout << "total (4-80): " << count(4,80) << endl;
	cout << "total (5-80): " << count(5,80) << endl;
	cout << "total (6-80): " << count(6,80) << endl;
	cout << "total (7-80): " << count(7,80) << endl;
	cout << "total (8-80): " << count(8,80) << endl;
	cout << "total (2-49): " << count(2,49) << endl;
	cout << "total (50-80): " << count(50,80) << endl;
	cout << "total (2,3): " << (items[2]+items[3]) << ", rem: " << (0.5-items[2]-items[3]) << endl;
	cout << "total (2,4): " << (items[2]+items[4]) << ", rem: " << (0.5-items[2]-items[4]) << endl;
	cout << "total (3,4): " << (items[3]+items[4]) << endl;

	return 0;
}
