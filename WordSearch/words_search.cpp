#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <Windows.h>
#include <filesystem>
#include <map>
#include <locale>
#include <numeric>
#include <sstream>

// struct for inverted index
// to be stored in vector
struct inverted_index
{
	int nfile;
	int nlocation;
};

// get sort index from large to small
template <typename T>
std::vector<size_t> sort_indexes(const std::vector<T>& v)
{
	// initialize original index locations
	std::vector<size_t> idx(v.size());
	iota(idx.begin(), idx.end(), 0);

	// sort indexes based on comparing values in v
	sort(idx.begin(), idx.end(),
	     [&v](const size_t i1, const size_t i2) { return v[i1] < v[i2]; });

	std::reverse(idx.begin(), idx.end());
	return idx;
}

void getsinglewordresult(std::map<std::string, std::vector<inverted_index>>, std::string, std::vector<std::string>);
void sort_display(std::vector<unsigned>, std::vector<std::string>);

int main(int argc, char* argv[])
{
	std::cout << "Welcome !" << std::endl;

	// get current path
	char buffer[MAX_PATH];
	GetModuleFileName(nullptr, buffer, MAX_PATH);
	const auto pos = std::string(buffer).find_last_of("\\/");
	const auto path = std::string(buffer).substr(0, pos);

	// get all txt files
	std::cout << "Start scanning txt files in current directory !" << std::endl;
	std::vector<std::string> files;
	for (auto& ifile : std::experimental::filesystem::directory_iterator(path))
	{
		auto s_extension = ifile.path().extension();
		if (s_extension == ".txt" || s_extension == ".TXT")
		{
			std::cout << "Txt file detected: " << ifile.path().string() << std::endl;
			files.push_back(ifile.path().string());
		}
	}

	// construct the inverted index dictionary
	std::map<std::string, std::vector<inverted_index>> dict;
	std::string filedata;
	std::fstream infile;

	// get all keys and it's inverted index
	for (auto i = 0; i < files.size(); ++i)
	{
		auto file = files[i];
		infile.open(file);
		// read data by loop
		auto n_location = 0;
		while (infile >> filedata)
		{
			// word clean
			//		only remain num and alphabet
			//		considered to be removed as it may affact the accruate search (will loss all special signals)
			//		but without it the result will also be affacted (will loss some count)
			// There are some words with encoding error, have to be cleaned first
			// Cannot use isalpha or isalnum, the program will crash due to the encoding error
			auto istr = 0;
			while (filedata[istr])
			{
				if (IsCharAlphaNumericA(filedata[istr]))
				{
					if (isalpha(filedata[istr]))
					{
						filedata[istr] = tolower(filedata[istr]);
					}
					istr++;
				}
				else
				{
					filedata.erase(istr, 1);
				}
			}

			if (filedata == "")
			{
				continue;
			}

			// insert information to dict
			inverted_index index;
			index.nfile = i;
			index.nlocation = n_location;

			dict[filedata].push_back(index);
			n_location++;
		}

		infile.close();
	}

	// Start messege loop
	// use stringstream to tokenize the input words, a more C++ way :)
	std::istringstream iss;
	while (true)
	{
		// Re-initialize the vars to ensure correct
		std::vector<unsigned> dcount(files.size(), 0);
		auto mode = 0;
		std::string s_tosearch = "";
		std::string stemp = "";
		auto is_new = true;
		int nlocation;
		// Cleaning stream is very important !
		iss.clear();

		std::cout << "Please enter number to choose search mode: " << std::endl;
		std::cout << "    1: search for single word" << std::endl;
		std::cout << "    2: search for a phrase" << std::endl;
		std::cout << "    3: search for multiple words" << std::endl;
		std::cout << "    4: search in wide card query (use * to extend search)" << std::endl;
		std::cout << "    5: Quit" << std::endl;
		std::cin >> mode;
		std::cout << "\n***************************************** " << std::endl;

		// Start working
		switch (mode)
		{
		case 1:
			std::cout << "Please enter a word to search " << std::endl;
			std::cin >> s_tosearch;

			// lower the input
			for (auto i = 0; i < s_tosearch.size(); ++i)
			{
				s_tosearch[i] = tolower(s_tosearch[i]);
			}

			getsinglewordresult(dict, s_tosearch, files);

			std::cout << "\n***************************************** " << std::endl;
			break;

		case 2:
			std::cout << "Please enter a phrase to search " << std::endl;
			std::cin.ignore(); // to release the thread used by previous cin
			std::getline(std::cin, s_tosearch);

			// lower the input
			for (auto i = 0; i < s_tosearch.size(); ++i)
			{
				s_tosearch[i] = tolower(s_tosearch[i]);
			}

			iss.str(s_tosearch);

			iss >> stemp;
			if (dict.count(stemp))
			{
				// get the last word dict
				auto dtemp = dict[stemp];

				do
				{
					iss >> stemp;
					if (dict.count(stemp))
					{
						// for every record with stemp, 
						// check if there are intersections with last word record
						// get the result
						auto i = 0;
						auto isfind = false;
						while (!isfind && i < dtemp.size())
						{
							const auto nloc2check = dtemp[i].nlocation + 1;
							for (auto element : dict[stemp])
							{
								if (element.nfile == dtemp[i].nfile && element.nlocation == nloc2check)
								{
									isfind = true;
								}
							}

							// if we can find the word in next position:
							//	  loc + 1
							if (isfind)
							{
								dtemp[i].nlocation++;
								i++;
								isfind = false;
							}
							else
								// if we cannot find, delete this record in temp
							{
								dtemp.erase(dtemp.begin() + i);
							}
						}
					}
					else
					{
						dtemp.clear();
						break;
					}
				}
				while (iss.good());

				// if still have record
				if (dtemp.size())
				{
					for (auto record : dtemp)
					{
						dcount[record.nfile]++;
					}

					sort_display(dcount, files);
				}
				else
				{
					std::cout << "There is no " << s_tosearch << " in our system" << std::endl;
				}
			}
			else
			{
				std::cout << "There is no " << s_tosearch << " in our system" << std::endl;
			}
			std::cout << "\n***************************************** " << std::endl;
			break;

		case 3:
			std::cout <<
				"Caution: If all input words do not show off in the same file, the count wil be zero and will be ignored."
				<< std::endl;
			std::cout << "Please enter words to search " << std::endl;
			std::cin.ignore(); // to release the thread used by previous cin
			std::getline(std::cin, s_tosearch);

			// lower the input
			for (auto i = 0; i < s_tosearch.size(); ++i)
			{
				s_tosearch[i] = tolower(s_tosearch[i]);
			}

			// get the count of different files
			iss.str(s_tosearch);
			do
			{
				iss >> stemp;
				if (dict.count(stemp))
				{
					for (auto record : dict[stemp])
					{
						// if words do not show off in the same file, this file will be ignored
						if (is_new || dcount[record.nfile])
						{
							dcount[record.nfile]++;
							is_new = false;
						}
					}
				}
				else
				{
					// if there are some word cannot be found in dict, destroy all results.
					dcount.clear();
					break;
				}
			}
			while (iss.good());

			// sort result
			if (!dcount.empty())
			{
				sort_display(dcount, files);
			}
			else
			{
				std::cout << "There is no " << s_tosearch << " in our system" << std::endl;
			}
			std::cout << "\n***************************************** " << std::endl;
			break;

		case 4:
			std::cout << "Please enter a wild card word to search, end with * " << std::endl;
			std::cin >> s_tosearch;

			nlocation = s_tosearch.find_first_of("*");
			s_tosearch = s_tosearch.substr(0, nlocation);

			for (auto element : dict)
			{
				if (element.first.find(s_tosearch) == 0)
				{
					std::cout << element.first << ": " << std::endl;
					getsinglewordresult(dict, element.first, files);
				}
			}
			std::cout << "\n***************************************** " << std::endl;
			break;

		case 5:
			std::cout << "Farewell ! " << std::endl;
			std::cout << "\n***************************************** " << std::endl;
			return 0;

		default:
			std::cout << "Undefined mode! " << std::endl;
			std::cout << "\n***************************************** " << std::endl;
			break;
		}
	}
}

/*
 *  Get the single word search result
 */
void getsinglewordresult(std::map<std::string, std::vector<inverted_index>> dict, const std::string s_tosearch,
                         std::vector<std::string> files)
{
	std::vector<unsigned> dcount(files.size(), 0);
	std::string stemp = "";

	if (dict.count(s_tosearch))
	{
		// get the result
		for (auto record : dict[s_tosearch])
		{
			dcount[record.nfile]++;
		}
		sort_display(dcount, files);
	}
	else
	{
		std::cout << "There is no " << s_tosearch << " in our system" << std::endl;
	}
}

/*
 *  sort dcount and display the result
 */
void sort_display(std::vector<unsigned> dcount, std::vector<std::string> files)
{
	// sort result
	auto index = sort_indexes(dcount);

	// output result
	for (auto i = 0; i < index.size(); ++i)
	{
		if (dcount[index[i]])
		{
			std::cout << std::experimental::filesystem::path(files[index[i]]).filename()
				<< ", " << dcount[index[i]] << std::endl;
		}
	}
}
