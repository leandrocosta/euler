#include <iostream>
using namespace std;

#include "../util/erathostenes.h"

int main ()
{
	unsigned long long total = 0;

	vector<unsigned int> primes = get_primes_by_erathostenes(2000000);

	for (vector<unsigned int>::const_iterator it = primes.begin(); it != primes.end(); ++it)
	{
		total += *it;
		cout << "prime: " << *it << ", total: " << total << endl;
	}
	return 0;
}
