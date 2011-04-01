#include <iostream>
using namespace std;

// primality test
template <int p, int i>
class is_prime {
	public:
		enum
		{
			prim = (p==2) || (p%i && is_prime< (i>2?p:0), i-1>::prim )
		};
};

template<> class is_prime<0,0> { public: enum {prim=1}; };
template<> class is_prime<0,1> { public: enum {prim=1}; };

// loop to print N prime numbers greater or equal to i
template <int i, int N>
class Prime_print {
	public:
		Prime_print<i+1, N - is_prime<i,i-1>::prim> a;

		enum
		{
			prim = is_prime<i,i-1>::prim
		};

		void f()
		{
			if(prim)
				cout << i << endl;
			a.f();
		}
};

template<int i> class Prime_print<i, 0> { public: enum {prim=0}; void f() {} };

int main()
{
	const int N = 10;
	//const int N = 10001;
	Prime_print<2, N> a;
	a.f();
}
