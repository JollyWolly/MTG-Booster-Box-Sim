/**
 * Noah Harbor
 * MTG booster randomizer
 */

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <stdlib.h>
#include <map>
#include <vector>
#include <stdio.h>
#include <time.h>

#define DEFAULT_SET "Limited Edition Alpha.csv"
#define DEFAULT_COMM 10
#define DEFAULT_UNCM 4
#define DEFAULT_RARE 1
#define NUM_COL 1
#define NAM_COL 2
#define RAR_COL 5

using namespace std;

//typedef mt19937 MyRNG;
//uint32_t seed_val;
//MyRNG rng;
//uniform_int_distribution<uint32_t> uint_dist;

class Card {
	private:
	// elements
	int id;
	string name;
	char rarity;

	public:
	// constructor
	Card(int id, char rarity, string name) {
		this->id = id;
		this->name = name;
		this->rarity = rarity;
	}

	void print() {
		cout << rarity << " " << name;
	}

	string toString() {
		string s = "X ";
		s += name;
		s[0] = rarity;
		return s;
	}
};

class Booster {
	private:
	vector<int> c_ls;
	vector<int> u_ls;
	vector<int> r_ls;

	void pullFromRarity(int cnt, vector<int>& pack, vector<int> q_ls)
	{
		int r;
		for (int i=0; i<cnt; i++)
		{
			r = rand()%q_ls.size();
			//cout << r << '\n';
			pack.push_back(q_ls.at(r));
			//cout << "adding card: " << q_ls.at(r) << endl;
		}
	}


	public:
	void add(char rarity, int id)
	{
		switch(rarity)
		{
		case 'C' : c_ls.push_back(id);
		break;
		case 'U' : u_ls.push_back(id);
		break;
		case 'R' : r_ls.push_back(id);
		break;
		}
	}

	vector<int> openBooster(int commons, int uncommons, int rares)
	{
		vector<int> pack;
		pullFromRarity(commons, pack, c_ls);
		pullFromRarity(uncommons, pack, u_ls);
		pullFromRarity(rares, pack, r_ls);

		return pack;
	}
};

int main(int argc, char *argv[]) {

	// seed random number generator
	srand((unsigned)time(NULL));

	string set_file = DEFAULT_SET;
        int c_cnt = DEFAULT_COMM;
        int u_cnt = DEFAULT_UNCM;
        int r_cnt = DEFAULT_RARE;



	// options
	if (argc == 1)
	{
		cout << "BoosterSim -h | --help : for help\n";
		cout << "No options selected\n";
		cout << "Running Default\n";
	}
	else
	{
		cout << argv[1] << endl;
		if (string(argv[1]) == "-h" || string(argv[1]) == "--help")
		{
			cout << "Usage:\n";
			cout << "   BoosterSim [options] [<argument>]\n";
			cout << "Options:\n";
			cout << "   -f <file>			# select different set\n";
			cout << "   -b <c> <u> <r>		# change quantities of commons<c> uncommons<u> & rares<r> in per booster pack\n";
			cout << "   -fb <file> <c> <u> <r>	# both -f & -b\n\n";
			cout << "Exiting Program\n";
			return 0;
		}
		if (string(argv[1]) == "-f" || string(argv[1]) == "-fb")
		{
			set_file = argv[2];
		}
		if (string(argv[1]) == "-b" || string(argv[1]) == "-fb")
		{
			int offset = 0;
			if (string(argv[1]) == "-fb")
				offset = 1;
			c_cnt = stoi(argv[2+offset]);
			u_cnt = stoi(argv[3+offset]);
			r_cnt = stoi(argv[4+offset]);
		}
	}



	// open file
	ifstream f(set_file);
	if (!f.is_open())
	{
		cout << "ERROR with target set file\n";
		exit(EXIT_FAILURE);
	}

	string line;
	getline(f, line); // skip 1st line
	// containers
	map<int, Card> set;
	Booster setBooster;

	// read file info into containers
	while (getline(f, line))
	{
		istringstream iss(line);
		string token;
		int col = 0;

		// card info
		string name;
		char rarity;
		int id;

		while (getline(iss, token, ','))
		{
			switch (col)
			{
			case NUM_COL: id = stoi(token);
			break;
			case NAM_COL: name = token;
			break;
			case RAR_COL: rarity = token[0];
			break;
			}
			col++;
		}

		// organize rarities
		setBooster.add(rarity, id);

		set.insert(pair<int, Card>(id, Card(id, rarity, name)));
	}
	f.close(); // close file

	int packsToOpen;
	cout << "How many packs of " << set_file.substr(0, set_file.size() - 4);
	cout << " would you like to open?: ";
	cin >> packsToOpen;
	cout << "\nOpening " << packsToOpen << " packs\n";

	map <int, int> collection;

	for (int i=packsToOpen; i>0; i--)
	{
		// cout << "OPENING BOOSTER: \n";
		vector<int> pack = setBooster.openBooster(c_cnt, u_cnt, r_cnt);

		for (vector<int>::iterator it=pack.begin(); it != pack.end(); ++it)
		{
			if (collection.find(*it) == collection.end() )
			{
				collection.insert(pair<int, int>(*it, 1) );
			}
			else
			{
				collection.at(*it)++;
			}
			//set.at(*it).print();
		}
	}

	string collectionString = "";
	for (auto const& k : collection)
	{
		cout << to_string(k.second) << "x ";
		set.at(k.first).print();
		cout << "\n";
	}

	cout << "\nPress \'s\' to save: ";
	char save;
	cin >> save;

	// Save Option
	if (save == 's')
	{
		cout << "\nFilename: ";
		string FileName;
		cin >> FileName;
		FileName += ".txt";

		ofstream outfile(FileName);

	        for (auto const& k : collection)
        	{
        	        outfile << to_string(k.second) << "x ";
                	outfile << set.at(k.first).toString();
                	outfile << "\n";
        	}
		outfile << endl;

		cout << FileName << " saved\n";
	}

	cout << "Exiting Program\n";
	return 0;
}
