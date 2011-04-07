/*
 * example of execution:
 *
 * visitAll(4, 7):
 * 		getAll(elements=4, total=7, max=4);
 * 			last = 4
 * 			getAll(elements=3, total=3, max=4)
 * 				return 1,1,1							=> 1,1,1,4
 * 			last = 3
 * 			getAll(elements=3, total=4, max=3)
 * 				last = 3
 * 				getAll(elements=2, total=1, max=3)
 * 					return NULL	// elements > total
 * 				last = 2
 * 				getAll(elements=2, total=2, max=2)
 * 					return 1,1							=> 1,1,2,3
 * 				last = 1
 * 				getAll(elements=2, total=3, max=1)
 * 					last = 1
 * 					getAll(elements=1, total=2, max=1)
 * 						return NULL // elements = 1 AND total > max
 * 			last = 2
 * 			getAll(elements=3, total=5, max=2)
 * 				last = 2
 * 				getAll(elements=2, total=3, max=2)
 * 					last = 2
 * 					getAll(elements=1, total=1, max=2)
 * 						return 1						=> 1,2,2,2
 * 				last = 1
 * 				getAll(elements=2, total=4, max=1)
 * 					last = 1
 * 					getAll(elements=1, total=3, max=1)
 * 						return NULL // elements = 1 AND total > max
 * 			last = 1
 * 			getAll(elements=3, total=6, max=1)
 * 				last = 1
 * 				getAll(elements=2, total=5, max=1)
 * 					last = 1
 * 					getAll(elements=1, total=4, max=1)
 * 						return NULL // elements = 1 AND total > max
 */

/*
 * Algorithm VisitAllPossibilities
 *
 * INPUT:	The number of elements: K
 * 			The total sum: T
 *
 * BEGIN
 * 		subsets = getAll(K, T, T-K+1)
 * 		print subsets
 * END
 *
 * ***
 *
 * Algorithm getAll
 * INPUT:	The number of elements: K
 * 			The total sum: T
 * 			The max element permited: M
 * 
 * OUTPUT:	Set of all possibilities S
 *
 * BEGIN
 * 		S = empty set
 *
 * 		IF (K > T) OR (K = 1 AND T > M)
 * 			RETURN S
 * 		ELSIF (K = T)
 * 			RETURN set of K elements 1
 * 		ELSIF (K = 1)
 * 			RETURN SET of 1 element T
 * 		END IF
 *
 * 		FOR last FROM M TO 1
 * 			IF last < T
 * 				subsets = getAll(K-1, T-last, last)
 * 			
 * 				FOREACH s FROM subsets
 * 					add s U {last} to S
 * 				END FOREACH
 * 			END IF
 * 		END FOR
 *
 * 		RETURN S
 * END
 *
 * ***
 *
 * Algorithm getMinimalProductSumNumber
 *
 * INPUT:	The number of elements: K
 *
 * OUTPUT:	The minimal product-sum number
 *
 * BEGIN
 * 		FOR T from K to infinite
 * 			subsets = getAll(K, T, T-K+1)
 *
 * 			FOREACH s FROM subsets
 * 				IF s is a product-sum number
 * 					RETURN product-sum number of s
 * 				END IF
 * 			END FOREACH
 * 		END FOR
 * END
 *
 * ***
 *
 * Algorithm getSumOfMinimalProductSumNumbers
 *
 * INPUT:	The maximum k: MAXK
 *
 * OUTPUT:	The sum of minimal product-sum numbers from k=2 until k=MAXK
 *
 * BEGIN
 * 		vec <= empty vector of booleans
 *
 * 		FOR k from 2 to MAXK
 * 			n = getMinimalProductSumNumber(k)
 *
 *			IF vec.size < n+1
 *				resize vec to n+1
 *			END IF
 *
 *			vec[n] = true
 * 		END FOR
 *
 * 		sum = 0
 *
 * 		FOREACH position (i) of vec
 * 			IF vec[i] is true
 * 				sum += i
 * 			END IF
 * 		END FOREACH
 *
 * 		RETURN sum
 * END
 *
 */

#include <stdint.h>
#include <cstdlib>
#include <iostream>
#include <vector>
using namespace std;

void printPossibility(const vector<uint32_t>& p)
{
	cout << p[0];
	for (size_t i = 1; i < p.size(); i++)
		cout << ", " << p[i];
	cout << endl;
}

vector<vector<uint32_t> > getAllPossibilities(uint32_t k, uint32_t t, uint32_t m)
{
	cout << "getAllPossibilities for k: " << k << ", t: " << t << ", m: " << m << endl;

	vector<vector<uint32_t> > possibilities;

	if (k > t || k*m < t)
		return possibilities;
	else if (k == t)
	{
		possibilities.push_back(vector<uint32_t>(k, 1));
		return possibilities;
	}
	else if (k == 1)
	{
		possibilities.push_back(vector<uint32_t>(1, t));
		return possibilities;
	}

	for (uint32_t last = m; last >= 1; last--)
	{
		//if (last < t)
		if (last < t && (k-1) <= (t-last) && ((k-1)*last >= (t-last)))
		{
			vector<vector<uint32_t> > subsets = getAllPossibilities(k-1, t-last, last);

			for (size_t i = 0; i < subsets.size(); i++)
			{
				subsets[i].push_back(last);
				possibilities.push_back(subsets[i]);
			}
		}
	}

	return possibilities;
}

void visitAllPossibilities(uint32_t k, uint32_t total)
{
	vector<vector<uint32_t> > possibilities = getAllPossibilities(k, total, total-k+1);

	for (size_t i = 0; i < possibilities.size(); i++)
		printPossibility(possibilities[i]);
}

uint32_t getProductSum(const vector<uint32_t>& p)
{
	uint32_t sum = 0;
	uint32_t mul = 1;

	for (vector<uint32_t>::const_iterator it = p.begin(); it < p.end(); ++it)
	{
		sum += *it;
		mul *= *it;
	}

	if (sum == mul)
		return sum;
	else
		return 0;
}

uint32_t getMinimalProductSumNumber(const uint32_t& k)
{
	uint32_t t = k;
	uint32_t sum = 0;

	do
	{
		cout << "try t: " << t << endl;
		vector<vector<uint32_t> > ps = getAllPossibilities(k, t, t-k+1);

		for (vector<vector<uint32_t> >::const_iterator it = ps.begin(); it != ps.end(); ++it)
		{
			if ((sum = getProductSum(*it)) > 0)
				return sum;
		}

		t++;
	} while (sum == 0);

	return 0;
}

uint32_t getSumOfMinimalProductSumNumbers(const uint32_t& maxk)
{
	vector<bool> vec;

	for (uint32_t k = 2; k <= maxk; k++)
	{
		cout << "k: " << k << endl;
		uint32_t n = getMinimalProductSumNumber(k);

		if (vec.size() < n+1)
			vec.resize(n+1, false);

		vec[n] = true;
	}

	uint32_t sum = 0;

	for (size_t i = 1; i < vec.size(); i++)
	{
		if (vec[i])
			sum += i;
	}

	return sum;
}

int main(int argc, char* argv[])
{
	//uint32_t k = atoi(argv[1]);
	//uint32_t total = atoi(argv[2]);

	//visitAllPossibilities(k, total);

	/*
	uint32_t k = atoi(argv[1]);
	cout << getMinimalProductSumNumber(k) << endl;
	*/

	uint32_t maxk = atoi(argv[1]);
	cout << getSumOfMinimalProductSumNumbers(maxk) << endl;

	return 0;
}
