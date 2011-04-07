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
 * Algorithm getAll
 * INPUT:	The number of elements: K
 * 			The total sum: T
 * 			The max element permited: M
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
 */

#include <bits/types.h>
#include <cstdlib>
#include <iostream>
#include <vector>
using namespace std;

void printPossibility(const vector<__uint32_t>& p)
{
	cout << p[0];
	for (size_t i = 1; i < p.size(); i++)
		cout << ", " << p[i];
	cout << endl;
}

vector<vector<__uint32_t> > getAll(__uint32_t k, __uint32_t t, __uint32_t m)
{
	//cout << "getAll for k: " << k << ", t: " << t << ", m: " << m << endl;

	vector<vector<__uint32_t> > possibilities;

	if (k > t || (k == 1 && t > m))
		return possibilities;
	else if (k == t)
	{
		possibilities.push_back(vector<__uint32_t>(k, 1));
		return possibilities;
	}
	else if (k == 1)
	{
		possibilities.push_back(vector<__uint32_t>(1, t));
		return possibilities;
	}

	for (__uint32_t last = m; last >= 1; last--)
	{
		if (last < t)
		{
			vector<vector<__uint32_t> > subsets = getAll(k-1, t-last, last);

			for (size_t i = 0; i < subsets.size(); i++)
			{
				subsets[i].push_back(last);
				possibilities.push_back(subsets[i]);
			}
		}
	}

	return possibilities;
}

void visitAllPossibilities(__uint32_t k, __uint32_t total)
{
	vector<vector<__uint32_t> > possibilities = getAll(k, total, total-k+1);

	for (size_t i = 0; i < possibilities.size(); i++)
		printPossibility(possibilities[i]);
}

int main(int argc, char* argv[])
{
	__uint32_t k = atoi(argv[1]);
	__uint32_t total = atoi(argv[2]);

	visitAllPossibilities(k, total);

	return 0;
}
