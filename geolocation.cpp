// reading a text file
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <time.h>
#include "library.h"


using namespace std;

struct Coverage {
	int north;
	int south;
	int east;
	int west;
	string file;

	Coverage() {
		//empty constructor
	}

	Coverage(string s) {
		int pos = s.find(" ");
		north = stoi(s.substr(0, pos));

		s = s.substr(pos + 1);
		pos = s.find(" ");
		south = stoi(s.substr(0, pos));

		s = s.substr(pos + 1);
		pos = s.find(" ");
		west = stoi(s.substr(0, pos));

		s = s.substr(pos + 1);
		pos = s.find(" ");
		east = stoi(s.substr(0, pos));

		file = s.substr(pos + 1);
	}
};

struct Map {
	int row;
	int col;
	int bbp;
	int spp;
	int lls;
	int tls;
	int min;
	int max;
	int sv;

	short int** values;

	Map() {
		//empty constructor
	}

	Map(string mRow, string mCol, string mBbp, string mSpp, string mLls, string mTls, string mMin, string mMax, string mSv) {
		row = stoi(mRow);
		col = stoi(mCol);
		bbp = stoi(mBbp);
		spp = stoi(mSpp);
		lls = stoi(mLls);
		tls = stoi(mTls);
		min = stoi(mMin);
		max = stoi(mMax);
		sv = stoi(mSv);
	}

};


const int hashsize = 60000;

struct city {
	int code;
	string state;
	string name;
	int population;
	double area;
	double latitude;
	double longitude;
	int intersection;
	double distance;

	city() {
		//got to make sure every entry added to the table has a value for code
		code = NULL;
	}
};

struct HashTable {
	city * cities;
	int hashsize, numentries, collisionCount;

	HashTable(int size) {
		cities = new city[size]; //array of cities
		hashsize = size; //size of array large value normally
		numentries = 0; //how many entries we got as of now
		collisionCount = 0;
	}

	unsigned int thash(string s) {
		unsigned int h = 278364;
		int len = s.length();
		for (int i = 0; i < len; i += 1)
			h = h * 69 + s[i];
		return h % hashsize;
	}

	void add(int key, city *entry) {
		//TODO: Remove duplicates by returning if already present
		if (numentries == 0) {
			cities[key] = *entry;
			numentries++;
		}
		else if (cities[key].code == NULL) {
			cities[key] = *entry;
			numentries++;
		}
		else { //collision
			//cout<<"Collision detected"<<endl;
			collisionCount++;
			int originalKey = key;
			while (key != (originalKey - 1)) {
				if (cities[key].code == NULL) {
					cities[key] = *entry;
					numentries++;
					break; //get out of while loop
				} /*else if(key >= hashsize){
				  //if were iterating find a spot for a collision and reach then end then we got to the beginning of the table
				  key = 0;
				  //cerr<< "Hash table too small"<<endl;
				  //exit(1);
				  }*/ else {
					key++; //iterate til you find an empty spot
				}
			}
		}
		/*
		TODO: Resize
		double lam = numentries/(double) hashsize;

		if(lam > "some fullness threshold")
		resize() //resize the array
		*/
	}

	void print() {
		int emptyCount = 0;
		cout << "==============PRINTING TREEE===================" << endl;
		for (int i = 0; i < hashsize; i++) {
			if (cities[i].code != NULL) {
				cout << "i: " << i << " '" << cities[i].name << "'" << " , " << cities[i].name << endl;
			}
			else {
				cout << "i: " << i << " is empty" << endl;
				emptyCount++;
			}
		}
		cout << "===============END TREE =======================" << endl;
		cout << "STATS: " << endl;
		cout << "Num entries: " << numentries << endl;
		cout << "Collision Count: " << collisionCount << endl;
		cout << "Empty locations: " << emptyCount << endl;
	}

	city search(string cityName) {
		int which = 0;
		int whichOne;
		bool found = false;
		int theHash;
		do { //do until a city is found
			cout << "Search for: " << cityName << endl;
			std::transform(cityName.begin(), cityName.end(), cityName.begin(), ::tolower);
			int theOriginalHash = thash(cityName);
			theHash = theOriginalHash;//TODO: duplicate to know if we made it all the way round the list
			//cout << "Hash: " << theHash << endl;
			if (cities[theHash].code == NULL) {
				cout << "No cities with that name!" << endl;
				found = false;
				cout << "Enter new city: " << endl;
				cin >> cityName;
			}
			else {
				//start at the hash and go until null (not until the next != to cityName)
				//because if miami is entered then another with the same hash then another miami then it'll come out in the middle
				//TODO: accounts for making it all the way around the hash table
				while (cities[theHash].code != NULL) {
					//transform the name to lowercase to compare
					string theCity = cities[theHash].name;
					std::transform(theCity.begin(), theCity.end(), theCity.begin(), ::tolower);
					if (theCity == cityName) { //city name is still lowercase
						cout <<" Select " << which << " for: " << cities[theHash].name << " , " << cities[theHash].state << endl;
					}
					theHash++;
					which++;
				}
				cout << "Select one of the cities above: ";
				do {
					cin >> whichOne;
					//cout << "HI" << endl;
				} while (whichOne > which);
				theHash = theHash - which;
				//cout << "Were here city is: " << cities[theHash + whichOne].name << " , " << cities[theHash + whichOne].state << endl;
				found = true;
			}
		} while (!found);
		return cities[theHash + whichOne];
	}
};


city* makeCity(string line) {

	city * theCity = new city;
	//first 8 digits
	string first = line.substr(0, 8);
	theCity->code = atoi(first.c_str());
	//state is the next 2 digits 
	string second = line.substr(8, 2);
	theCity->state = second;

	//everything after state til a double space is the city
	line = line.substr(10);
	int pos = line.find("  "); //two spaces cause some cities can have a space in the name
	string city = line.substr(0, pos);
	theCity->name = city;
	//cout<<"City: '"<<city<<"'"<<endl;

	//cout<<"F:"<<first<<"\nS:"<<second<<"\nCity:"<<city<<endl;

	//update line to have everything after this
	line = line.substr(pos);

	//cout<<"Remainding line: "<<line<<endl;

	//get rid of spaces
	int count = 0;
	while (line.compare(count, 1, " ") == 0) {
		count++;
	}
	line = line.substr(count);
	//cout<<"Line without spaces: "<<line<<endl;

	//get the next value
	pos = line.find("  ");
	string third = line.substr(0, pos);
	theCity->population = atoi(third.c_str());
	//cout<<"T:"<<third<<endl;

	//remove this value from string
	line = line.substr(pos);
	//cout<<"Remainding: "<<line<<endl;

	//lose the spaces
	count = 0;
	while (line.compare(count, 1, " ") == 0) {
		count++;
	}
	line = line.substr(count);
	//cout<<"Line without spaces: "<<line<<endl;

	//get area by finding the decimal and go to the next 6 spaces
	pos = line.find(".");
	string area = line.substr(0, pos + 7);
	theCity->area = atof(area.c_str());
	//cout<<"Area:"<<area<<endl;

	line = line.substr(pos + 7);
	//cout<<"Remainding line: "<<line<<endl;

	//lose the spaces
	count = 0;
	while (line.compare(count, 1, " ") == 0) {
		count++;
	}
	line = line.substr(count);
	//cout<<"Line without spaces: "<<line<<endl;

	//get latitude
	pos = line.find(".");
	string latitude = line.substr(0, pos + 7);
	theCity->latitude = atof(latitude.c_str());

	//cout<<theCity->latitude<<endl;

	//cout<<"Lat:"<<latitude<<endl;

	line = line.substr(pos + 7);
	//cout<<"Remainding line: "<<line<<endl;

	//lose the spaces
	count = 0;
	while (line.compare(count, 1, " ") == 0) {
		count++;
	}
	line = line.substr(count);
	//cout<<"Line without spaces: "<<line<<endl;

	//get longitude
	pos = line.find(".");
	string longitude = line.substr(0, pos + 7);
	theCity->longitude = atof(longitude.c_str());

	//cout<<"Lon:"<<longitude<<endl;

	line = line.substr(pos + 7);
	//cout<<"Remainding line: "<<line<<endl;

	//lose the spaces
	count = 0;
	while (line.compare(count, 1, " ") == 0) {
		count++;
	}
	line = line.substr(count);
	//cout<<"Line without spaces: "<<line<<endl;

	//get intersection
	//pos = line.find(" ");
	//cout<<"Count: "<<count<<endl;
	string intersection = line.substr(0, 5 - count);
	theCity->intersection = atoi(intersection.c_str());
	//cout<<"Intersection:"<<intersection<<endl;

	//update line
	line = line.substr(5 - count);
	//lose the spaces
	count = 0;
	while (line.compare(count, 1, " ") == 0) {
		count++;
	}
	string distance = line.substr(count);
	//cout<<"distance:"<<distance<<endl;
	theCity->distance = atof(distance.c_str());
	return theCity;
}

unsigned int myhash(string s) {
	unsigned int h = 278364;
	int len = s.length();
	for (int i = 0; i < len; i += 1)
		h = h * 69 + s[i];
	return h % hashsize;
}

void main() {
	HashTable *h = new HashTable(hashsize);
	string line;
	city first, second;
	ifstream myfile("named-places.txt");
	if (myfile) {
		while (getline(myfile, line)) {
			city* newestCity = makeCity(line);
			//get the hash of the city name
			string cityAsLowercase = newestCity->name;
			std::transform(cityAsLowercase.begin(), cityAsLowercase.end(), cityAsLowercase.begin(), ::tolower);
			int hashOfLine = myhash(cityAsLowercase);
			//cout<<"N: "<<cityAsLowercase<<" H: "<<hashOfLine<<endl;
			h->add(hashOfLine, newestCity);
		}
	}
	else {
		cout << "Unable to open file";
	}

	string input;
	cout << "============Enter 2 points=============" << endl;
	cout << "Enter city 1 or (E)xit: ";
	cin >> input;
	if (input == "E") {
		return;
	}
	else {
		first = h->search(input);
		cout << "You selected: " << first.name << " in " << first.state << endl;

		cout << "Enter city 2: ";
		cin >> input;
		second = h->search(input);

		cout << "You selected: " << first.name << " in " << first.state<<endl;// << first.latitude << " " << first.longitude << endl;
		cout << "and: " << second.name << " in " << second.state<<endl;// << second.latitude << " " << second.longitude << endl;
		cout << "===============END SEARCH==============" << endl;
	}

	//input all 149 coverage entries
	Coverage* c = new Coverage[149];
	int count = 0;
	ifstream fs("coverage.txt");
	string cline;
	if (fs) {
		while (getline(fs, cline)) {
			Coverage* newCov = new Coverage(cline);
			c[count] = *newCov;
			count++;
		}
	}

	double lat1, lat2, lon1, lon2;
	lat1 = first.latitude;
	lat2 = second.latitude;
	lon1 = first.longitude;
	lon2 = second.longitude;
	//find the smallest coverage area with both points
	int smallest = 50;
	Coverage result;
	for (int i = 0; i < 149; i++) {
		if ((c[i].north >= lat1 && c[i].north >= lat2) &&
			(c[i].south <= lat1 && c[i].south <= lat2) &&
			(c[i].west <= lon1 && c[i].west <= lon2) &&
			(c[i].east >= lon1 && c[i].east >= lon2)) {
			if ((c[i].north - c[i].south) < smallest) {
				//cout<<"Changinng result: "<<smallest;
				smallest = c[i].north - c[i].south;
				result = c[i];
				//cout<<" to "<<smallest<<endl;
			}
			//cout<<"FOUND MAP: "<<"N: "<<c[i].north<<" S:"<<c[i].south<<" "<<c[i].file<<endl;
		}
	}
	cout << "Smallest map with both cities: " << smallest << " " << result.file << endl;

	//open smallest map
	string fname = "all/" + result.file;

	//testing other files
	//string fname = "all/usaW105N25D5.dat";
	//string fname = "all/usaW130N50D80.dat";

	//cout << "FILE: " << fname << endl;
	ifstream file(fname, ifstream::binary);
	if (file) {
		//go to the end of the file to get some values to understand the file
		file.seekg(0, ios::end);
		int end = file.tellg();
		int numOfInts = end / 2;
		//cout << "File Size in bytes: " << end << endl;
		//cout << "Num of short ints: " << numOfInts << endl;

		//back to the front
		file.seekg(0);

		//get first row
		string row, col, bpp, spp, lls, tls, min, max, sv;
		file >> row >> row;
		file >> col >> col;
		file >> bpp >> bpp;
		file >> spp >> spp;
		file >> lls >> lls;
		file >> tls >> tls;
		file >> min >> min;
		file >> max >> max;
		file >> sv >> sv;

		//make map object
		Map map = Map(row, col, bpp, spp, lls, tls, min, max, sv);
		/*cout << "======================MAP DETAILS=======================" << endl;
		cout << "ROW: " << row << endl;
		cout << "COL: " << col << endl;
		cout << "BPP: " << bpp << endl;
		cout << "SPP: " << spp << endl;
		cout << "LLS: " << lls << endl;
		cout << "TLS: " << tls << endl;
		cout << "MINS: " << min << endl;
		cout << "MAX: " << max << endl;
		cout << "SPECIAL: " << sv << endl;
		cout << "==================END MAP DETAILS=======================" << endl;*/

		file.clear();

		//go to byte 1200 end of dummy row
		file.seekg(2 * map.col);
		//cout << "Starting AT: " << file.tellg() << endl;

		int rowCount = 0;
		int colCount = 0;
		make_window(map.col, map.row);

		//for testing
		int N = 100;

		int pos = 0;
		int neg = 0;
		int special = 0;

		while ((int)file.tellg() != end) {
			//for testing
			//while (N--) {
			if (colCount%map.col == 0) {
				//cout << "New row!" << endl;
				rowCount++;
				colCount = 0;
			}

			short int lsb = (short int)file.get();
			short int msb = (short int)file.get();
			short combined = (msb << 8) | (lsb & 0xff);

			if (combined < 0 && combined != -500) {
				neg++;
			}
			else if (combined >= 0) {
				pos++;
			}
			else if (combined == -500) {
				special++;
			}
			//cout << "TellG: "<<file.tellg() <<"Loc: " << rowCount << "," <<colCount<< " Val:" << combined << endl;

			double normalize = ((double)(combined - map.min) / (double)(map.max - map.min));
			//double color = normalize*(255.0);
			//cout << "C: :" << combined << " NORMALIZED: " << normalize << " " << color << endl;

			if (combined < 0) {
				set_pixel_color(colCount, rowCount, color::blue);
			}
			else {
				set_pixel_color(colCount, rowCount, make_color(0.0, normalize, 0.0));
			}
			colCount++;
		}
		//cout << "pos:" << pos << endl;
		//cout << "neg: " << neg << endl;
		//cout << "special: " << special << endl;
		//cout << "total: " << (pos + neg + special) << endl;


		int mapx1 = (lon1 * 3600 - stoi(lls)) / stoi(spp);
		int mapy1 = (lat1 * 3600 - stoi(tls)) / stoi(spp);
		int mapx2 = (lon2 * 3600 - stoi(lls)) / stoi(spp);
		int mapy2 = (lat2 * 3600 - stoi(tls)) / stoi(spp);
		cout << "This map is: " << row << " x " << col << " pixels"<<endl;
		cout << "First city: (" << mapx1 << " , " << -mapy1 <<") pixels coordinates"<< endl;
		cout << "Second city: (" << mapx2 << " , " << -mapy2 << ") pixel coordinates" << endl;
		set_pen_width_color(15, color::dark_red);
		string city1 = first.name + "," + first.state;
		string city2 = second.name + "," + second.state;
		move_to(mapx1, -mapy1);
		write_string(city1, direction::north);
		move_to(mapx2, -mapy2);
		write_string(city2, direction::north);
		draw_point(mapx1, -mapy1);
		draw_point(mapx2, -mapy2);
	}
	else {
		cout << "FILE NOT OPENED" << endl;
	}
	return;
}
