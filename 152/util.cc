#include <cmath>
#include <iostream>
using namespace std;

const unsigned int MAX_INDEX = 80;

double items[MAX_INDEX+1];

void init()
{
	for (unsigned int i = 2; i <= MAX_INDEX; i++)
	{
		items[i] = 1/pow(i,2);
		cout << "item " << i << ": " << items[i] << endl;
	}
}

double count(unsigned int from, unsigned int to)
{
	double total = 0;

	for (unsigned int i = from; i <= to; i++)
	{
		total += items[i];
	}

	return total;
}

double sum_others()
{
	return items[6] + count(8, 11) + count(13,14) + count(16,19) + count(21,27) + count(29,34) + count(36,45);
}

int main ()
{
	init();

	cout << "total: " << count(2, MAX_INDEX) << endl;
	cout << "3-END: " << count(3, MAX_INDEX) << endl;

	cout << "item 3: " << items[3] << endl;
	cout << "item 35: " << items[35] << endl;
	cout << "others: " << sum_others() << endl;

	return 0;
}
