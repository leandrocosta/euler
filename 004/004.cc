#include <cmath>
#include <iostream>
using namespace std;

const bool is_palindrome(const unsigned int& number)
{
	cout << "check "<< number << endl;
	size_t digits = ceil(log10(number));

	for (size_t i = 1; i <= digits/2; i++)
	{
		unsigned int n1 = (number % (unsigned int) pow(10,i)) / (unsigned int) pow(10,i-1);
		unsigned int n2 = (number % (unsigned int) pow(10,digits-i+1)) / (unsigned int) pow(10,digits-i);

		if(n1 != n2)
			return false;
	}

	return true;
}

int main()
{
	unsigned int palindrome = 0;

	for (unsigned int i = 999; i >= 100; i--)
	{
		if (i*999 < palindrome)
			break;

		for (unsigned int j = 999; j >= 100; j--)
		{
			unsigned int number = i*j;

			if (number < palindrome)
				break;

			if (is_palindrome(i*j))
			{
				if (i*j > palindrome)
				{
					palindrome = i*j;
					break;
				}
			}
		}
	}

	cout << "palindrome: " << palindrome << endl;

	return 0;
}
