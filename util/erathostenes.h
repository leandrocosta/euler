#include <cmath>
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
        if (range[i])
        {
            primes.push_back(i);

            for (size_t m = pow(i, 2); m < range.size(); m += i)
                range[m] = 0;
        }
    }

    for (size_t i = trunc(sqrt(max))+1; i <= max; i++)
    {
        if (range[i])
            primes.push_back(i);

    }

    return primes;
}
