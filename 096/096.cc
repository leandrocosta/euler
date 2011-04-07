#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

unsigned int** getsudoku(istream& stream)
{
	string line;
	getline(stream, line);

	if (line.empty())
		return NULL;

	cout << line << endl;

	unsigned int** sudoku = new unsigned int*[9];

	for (size_t i = 0; i < 9; i++)
	{
		sudoku[i] = new unsigned int[9];

		if (stream.good())
		{
			getline(stream, line);
			//cout << "line: " << line << endl;

			for (size_t j = 0; j < 9; j++)
			{
				char c = line[j];
				sudoku[i][j] = atoi(&c);
			}
		}
		else
			throw string("end of file");
	}

	return sudoku;
}

pair<size_t,size_t>* getgoodpair(vector<unsigned int> mavailable[][9])
{
	for (size_t i = 0; i < 9; i++)
		for (size_t j = 0; j < 9; j++)
			if (mavailable[i][j].size() == 1)
				return new pair<size_t,size_t>(i,j);

	return NULL;
}

const bool isnotvalid(unsigned int** sudoku)
{
	unsigned int nums_array[] = {1,2,3,4,5,6,7,8,9};

	// check columns
	for (size_t i = 0; i < 9; i++)
	{
		vector<unsigned int> nums (nums_array, nums_array + sizeof(nums_array)/sizeof(unsigned int));

		for (size_t j = 0; j < 9; j++)
		{
			if (sudoku[i][j] != 0)
			{
				vector<unsigned int>::iterator it = find(nums.begin(), nums.end(), sudoku[i][j]);
				if(it != nums.end())
					nums.erase(it);
				else
					return true;
			}
		}
	}

	// check lines
	for (size_t j = 0; j < 9; j++)
	{
		vector<unsigned int> nums (nums_array, nums_array + sizeof(nums_array)/sizeof(unsigned int));

		for (size_t i = 0; i < 9; i++)
		{
			if (sudoku[i][j] != 0)
			{
				vector<unsigned int>::iterator it = find(nums.begin(), nums.end(), sudoku[i][j]);
				if(it != nums.end())
					nums.erase(it);
				else
					return true;
			}
		}
	}

	// check boxes
	for (size_t x = 0; x < 9; x++)
	{
		//cout << "x: " << x << endl;
		vector<unsigned int> nums (nums_array, nums_array + sizeof(nums_array)/sizeof(unsigned int));

		for (size_t i = 3*(x/3); i < 3*(x/3)+3; i++)
		{
			for (size_t j = 3*(x%3); j < 3*(x%3)+3; j++)
			{
				//cout << "i: " << i << ", j: " << j << endl;
				if (sudoku[i][j] != 0)
				{
					vector<unsigned int>::iterator it = find(nums.begin(), nums.end(), sudoku[i][j]);
					if(it != nums.end())
						nums.erase(it);
					else
						return true;
				}
			}
		}
	}

	return false;
}

const bool issolution(unsigned int** sudoku)
{
	for (size_t i = 0; i < 9; i++)
	{
		for (size_t j = 0; j < 9; j++)
		{
			if (sudoku[i][j] == 0)
				return false;
		}
	}

	return !isnotvalid(sudoku);
}

pair<size_t,size_t> getbestpair(vector<unsigned int> mavailable[][9])
{
	size_t best_i = 0;
	size_t best_j = 0;

	for (size_t i = 0; i < 9; i++)
	{
		for (size_t j = 0; j < 9; j++)
		{
			if (mavailable[i][j].size() != 0 && (mavailable[best_i][best_j].size() == 0 || mavailable[i][j].size() < mavailable[best_i][best_j].size()))
			{
				best_i = i;
				best_j = j;
			}
		}
	}

	return make_pair<size_t,size_t>(best_i,best_j);
}

unsigned int** getsolution(unsigned int** sudoku, vector<unsigned int> mavailable[][9])
{
	/*
	cout << endl;
	cout << "getsolution for:" << endl;
	for (size_t i = 0; i < 9; i++)
	{
		cout << sudoku[i][0];

		for (size_t j = 1; j < 9; j++)
			cout << " " << sudoku[i][j];

		cout << endl;
	}

	cout << "mavailable sizes:" << endl;
	for (size_t i = 0; i < 9; i++)
	{
		for (size_t j = 0; j < 9; j++)
		{
			cout << mavailable[i][j].size() << " ";
		}
		cout << endl;
	}
	*/

	pair<size_t,size_t>* p;

	while ((p = getgoodpair(mavailable)) != NULL)
	{
		//cout << "we have good pairs: mavailable[" << p->first << "][" << p->second << "]: " << mavailable[p->first][p->second].front() << endl;

		sudoku[p->first][p->second] = mavailable[p->first][p->second].front();

		for (size_t i = 0; i < 9; i++)
		{
			for (size_t j = 0; j < 9; j++)
			{
				if (i == p->first || j == p->second)
				{
					vector<unsigned int>::iterator it = find(mavailable[i][j].begin(), mavailable[i][j].end(), sudoku[p->first][p->second]);
					if (it != mavailable[i][j].end())
						mavailable[i][j].erase(it);
				}
			}
		}

		/*
		cout << "sudoku after applying good pairs:" << endl;
		for (size_t i = 0; i < 9; i++)
		{
			cout << sudoku[i][0];

			for (size_t j = 1; j < 9; j++)
				cout << " " << sudoku[i][j];

			cout << endl;
		}
		cout << "mavailable sizes after applying good pairs:" << endl;
		for (size_t i = 0; i < 9; i++)
		{
			for (size_t j = 0; j < 9; j++)
			{
				cout << mavailable[i][j].size() << " ";
			}
			cout << endl;
		}
		*/
	}

	//cout << "no more good pairs..." << endl;

	if (isnotvalid(sudoku))
	{
		//cout << "invalid solution!" << endl;
		return NULL;
	}
	else if (issolution(sudoku))
	{
		//cout << "solution!" << endl;
		unsigned int** new_sudoku = new unsigned int*[9];

		for (size_t i = 0; i < 9; i++)
		{
			new_sudoku[i] = new unsigned int[9];

			for (size_t j = 0; j < 9; j++)
				new_sudoku[i][j] = sudoku[i][j];
		}

		return new_sudoku;
	}
	else
	{
		pair<size_t,size_t> bp = getbestpair(mavailable);

		if (mavailable[bp.first][bp.second].size() > 0)
		{
			/*
			cout << "best pair: mavailable[" << bp.first << "][" << bp.second << "]: " << mavailable[bp.first][bp.second].front();
			for(vector<unsigned int>::const_iterator it = mavailable[bp.first][bp.second].begin()+1; it != mavailable[bp.first][bp.second].end(); ++it)
				cout << ", " << *it;
			cout << endl;
			*/

			for(vector<unsigned int>::const_iterator it = mavailable[bp.first][bp.second].begin(); it != mavailable[bp.first][bp.second].end(); ++it)
			{
				//cout << "trying: " << *it << endl;
				unsigned int** new_sudoku = new unsigned int*[9];

				for (size_t i = 0; i < 9; i++)
				{
					new_sudoku[i] = new unsigned int[9];

					for (size_t j = 0; j < 9; j++)
						new_sudoku[i][j] = sudoku[i][j];
				}

				new_sudoku[bp.first][bp.second] = *it;

				vector<unsigned int> new_mavailable[9][9];

				for (size_t i = 0; i < 9; i++)
				{
					for (size_t j = 0; j < 9; j++)
					{
						if (i != bp.first || j != bp.second)
						{
							for(vector<unsigned int>::const_iterator it2 = mavailable[i][j].begin(); it2 != mavailable[i][j].end(); ++it2)
							{
								if ((i != bp.first && j != bp.second) || *it != *it2)
									new_mavailable[i][j].push_back(*it2);
							}
						}
					}
				}

				unsigned int** solution = getsolution(new_sudoku, new_mavailable);

				for (size_t i = 0; i < 9; i++)
					delete[] new_sudoku[i];
				delete[] new_sudoku;

				if (solution)
					return solution;
			}
		}
		else
		{
			/*
			cout << "best pair with empty set: " << bp.first << ", " << bp.second << ", mavailable sizes:" << endl;

			for (size_t i = 0; i < 9; i++)
			{
				for (size_t j = 0; j < 9; j++)
				{
					cout << mavailable[i][j].size() << " ";
				}
				cout << endl;
			}
			*/

			//throw string("best pair with empty set");
			return NULL;
		}
	}

	return NULL;
}

unsigned int** solvesudoku(unsigned int** sudoku)
{
	vector<unsigned int> mavailable[9][9];
	vector<unsigned int> labsent[9];
	vector<unsigned int> cabsent[9];
	vector<unsigned int> babsent[9];

	for (size_t i = 0; i < 9; i++)
	{
		for (unsigned int n = 1; n <= 9; n++)
		{
			labsent[i].push_back(n);
			cabsent[i].push_back(n);
			babsent[i].push_back(n);
		}
	}

	for (size_t i = 0; i < 9; i++)
	{
		for (size_t j = 0; j < 9; j++)
		{
			if (sudoku[i][j] != 0)
			{
				vector<unsigned int>::iterator it = find(labsent[i].begin(), labsent[i].end(), sudoku[i][j]);
				if(it != labsent[i].end())
					labsent[i].erase(it);

				it = find(cabsent[j].begin(), cabsent[j].end(), sudoku[i][j]);
				if(it != cabsent[j].end())
					cabsent[j].erase(it);

				size_t x = (3 * (i/3)) + ((j/3)%3);

				it = find(babsent[x].begin(), babsent[x].end(), sudoku[i][j]);
				if(it != babsent[x].end())
					babsent[x].erase(it);
			}
		}
	}

	/*
	for (size_t i = 0; i < 9; i++)
	{
		cout << "labsent[" << i << "]: ";

		if (!labsent[i].empty())
		{
			cout << labsent[i].front();

			for (vector<unsigned int>::iterator it = labsent[i].begin()+1; it != labsent[i].end(); ++it)
				cout << ", " << *it;
		}

		cout << endl;
	}
	*/

	for (size_t i = 0; i < 9; i++)
	{
		for (size_t j = 0; j < 9; j++)
		{
			if (sudoku[i][j] == 0)
			{
				/*
				mavailable[i][j].resize(labsent[i].size()+cabsent[j].size());
				vector<unsigned int>::iterator it = set_intersection(labsent[i].begin(), labsent[i].end(), cabsent[j].begin(), cabsent[j].end(), mavailable[i][j].begin());
				mavailable[i][j].resize(it-mavailable[i][j].begin());
				*/

				size_t x = (3 * (i/3)) + ((j/3)%3);

				vector<unsigned int> lc (labsent[i].size()+cabsent[j].size()+babsent[x].size());
				vector<unsigned int>::iterator it = set_intersection(labsent[i].begin(), labsent[i].end(), cabsent[j].begin(), cabsent[j].end(), lc.begin());
				lc.resize(it-lc.begin());

				mavailable[i][j].resize(lc.size()+babsent[x].size());
				it = set_intersection(lc.begin(), lc.end(), babsent[x].begin(), babsent[x].end(), mavailable[i][j].begin());
				mavailable[i][j].resize(it-mavailable[i][j].begin());
			}
		}
	}

	/*
	if (mavailable[0][0].size() != 0)
	{
		cout << "mavailable[0][0]: " << mavailable[0][0][0];

		for (size_t i = 1; i < mavailable[0][0].size(); i++)
			cout << ", " << mavailable[0][0][i];
		cout << endl;
	}
	*/

	return getsolution(sudoku, mavailable);
}

int main(int argc, char* argv[])
{
	string filename = argv[1];

	//cout << "filename: " << filename << endl;

	unsigned long long sum = 0;

	ifstream stream (filename.c_str(), ifstream::in);

	while (stream.good())
	{
		unsigned int** sudoku = getsudoku(stream);

		if(sudoku)
		{
			/*
			for (size_t i = 0; i < 9; i++)
			{
				cout << sudoku[i][0];

				for (size_t j = 1; j < 9; j++)
					cout << " " << sudoku[i][j];

				cout << endl;
			}
			*/

			try
			{
				unsigned int** solution = solvesudoku(sudoku);

				if (solution)
				{
					cout << "solution:" << endl;

					for (size_t i = 0; i < 9; i++)
					{
						cout << solution[i][0];

						for (size_t j = 1; j < 9; j++)
							cout << " " << solution[i][j];

						cout << endl;
					}

					unsigned long long num = (100*solution[0][0] + 10*solution[0][1] + solution[0][2]);
					//cout << "num: " << num << endl;
					sum += num;
					//cout << "sum: " << sum << endl;
				}
			}
			catch (string e)
			{
				cout << "exception: " << e << endl;
			}
		}
	}

	cout << "sum: " << sum << endl;
}
