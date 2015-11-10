// reading a text file
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <time.h>

using namespace std;


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

unsigned int hash(string s) {
  unsigned int h = 278364;
  int len = s.length();
  for (int i = 0; i < len; i += 1)
    h = h * 69 + s[i];
  return h % hashsize; 
}

struct HashTable {
	city * cities;
	int hashsize, numentries, collisionCount;

	HashTable (int size) {
		cities = new city[size]; //array of cities
		hashsize = size; //size of array large value normally
		numentries = 0; //how many entries we got as of now
		collisionCount = 0;
	}

	void add (int key, city *entry) {
		//TODO: Remove duplicates by returning if already present
		if(numentries == 0) {
			cities[key] = *entry;
			numentries++;
		} else if(cities[key].code == NULL) {
			cities[key] = *entry;
			numentries++;
		} else { //collision
			//cout<<"Collision detected"<<endl;
			collisionCount++;
			int originalKey = key;
			while(key != (originalKey-1) ) {
				if(cities[key].code == NULL) {
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
		cout<<"==============PRINTING TREEE==================="<<endl;
		for(int i = 0; i < hashsize; i++) {
			if(cities[i].code != NULL) {
				cout<<"i: "<<i<<" '"<<cities[i].name<<"'"<<" , "<<cities[i].name<<endl;
			} else {
				cout<<"i: "<<i<<" is empty"<<endl;
				emptyCount++;
			}
		}
		cout<<"===============END TREE ======================="<<endl;
		cout<<"STATS: "<<endl;
		cout<<"Num entries: "<<numentries<<endl;
		cout<<"Collision Count: "<<collisionCount<<endl;
		cout<<"Empty locations: "<<emptyCount<<endl;
	}

	//TODO:
	//search
	void search(string cityName) {
		cout<<"Search for: "<<cityName<<endl;
		std::transform(cityName.begin(), cityName.end(), cityName.begin(), ::tolower);
		int theOriginalHash = hash(cityName);
		int theHash = theOriginalHash;//TODO: duplicate to know if we made it all the way round the list
		cout<<"Hash: "<<theHash<<endl;
		if(cities[theHash].code == NULL) {
			cout<<"No cities with that name"<<endl;
		} else {
			//start at the hash and go until null (not until the next != to cityName)
			//because if miami is entered then another with the same hash then another miami then it'll come out in the middle
			//TODO: accounts for making it all the way around the hash table
			while(cities[theHash].code != NULL) {
				//transform the name to lowercase to compare
				string theCity = cities[theHash].name;
				std::transform(theCity.begin(), theCity.end(), theCity.begin(), ::tolower);
				if(theCity == cityName) { //city name is still lowercase
					cout<<"Found: "<<cities[theHash].name<<","<<cities[theHash].state<<endl;
				}
				theHash++;
			}
		}
	}
};


city* makeCity(string line) {

	city * theCity = new city;
	//first 8 digits
	string first = line.substr(0,8);
	theCity->code = atoi(first.c_str());
	//state is the next 2 digits 
	string second = line.substr(8,2);
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
	while(line.compare(count,1, " ") == 0) {
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
	while(line.compare(count,1, " ") == 0) {
		count++;
	}
	line = line.substr(count);
	//cout<<"Line without spaces: "<<line<<endl;

	//get area by finding the decimal and go to the next 6 spaces
	pos = line.find(".");
	string area = line.substr(0, pos+7);
	theCity->area = atof(area.c_str());
	//cout<<"Area:"<<area<<endl;

	line = line.substr(pos+7);
	//cout<<"Remainding line: "<<line<<endl;

	//lose the spaces
	count = 0;
	while(line.compare(count,1, " ") == 0) {
		count++;
	}
	line = line.substr(count);
	//cout<<"Line without spaces: "<<line<<endl;

	//get latitude
	pos = line.find(".");
	string latitude = line.substr(0, pos+7);
	theCity->latitude = atof(latitude.c_str());

	//cout<<theCity->latitude<<endl;

	//cout<<"Lat:"<<latitude<<endl;

	line = line.substr(pos+7);
	//cout<<"Remainding line: "<<line<<endl;

	//lose the spaces
	count = 0;
	while(line.compare(count,1, " ") == 0) {
		count++;
	}
	line = line.substr(count);
	//cout<<"Line without spaces: "<<line<<endl;

	//get longitude
	pos = line.find(".");
	string longitude = line.substr(0, pos+7);
	theCity->longitude = atof(longitude.c_str());

	//cout<<"Lon:"<<longitude<<endl;

	line = line.substr(pos+7);
	//cout<<"Remainding line: "<<line<<endl;

	//lose the spaces
	count = 0;
	while(line.compare(count,1, " ") == 0) {
		count++;
	}
	line = line.substr(count);
	//cout<<"Line without spaces: "<<line<<endl;

	//get intersection
	//pos = line.find(" ");
	//cout<<"Count: "<<count<<endl;
	string intersection = line.substr(0,5-count);
	theCity->intersection = atoi(intersection.c_str());
	//cout<<"Intersection:"<<intersection<<endl;

	//update line
	line = line.substr(5-count);
	//lose the spaces
	count = 0;
	while(line.compare(count,1, " ") == 0) {
		count++;
	}
	string distance = line.substr(count);
	//cout<<"distance:"<<distance<<endl;
	theCity->distance = atof(distance.c_str());
	return theCity;
}

int main () {

	HashTable *h = new HashTable(hashsize);


	string line;
	ifstream myfile ("named-places.txt");
	if (myfile) {
	  	while(getline(myfile,line)) { 
		  	city* newestCity = makeCity(line);
		  	//get the hash of the city name
		  	string cityAsLowercase = newestCity->name;
		  	std::transform(cityAsLowercase.begin(), cityAsLowercase.end(), cityAsLowercase.begin(), ::tolower);
		  	int hashOfLine = hash(cityAsLowercase);
		  	//cout<<"N: "<<cityAsLowercase<<" H: "<<hashOfLine<<endl;
		  	h->add(hashOfLine, newestCity);
		}
	} else {
		cout << "Unable to open file"; 
	}

	string input;
	while(true) {
		cout<<"============TEST THE HashTable============="<<endl;
		cout<<"Enter a city((E)xit or (P)rintHashTable): ";
		cin>>input;
		if(input == "E") {
			break;
		} else if(input == "P") {
			h->print();
		} else {
			clock_t t1, t2;
			t1=clock();

			h->search(input);

			t2 = clock();
			//cout<<"T1 :"<<t1<<" T2: "<<t2<<endl;
			float diff = ((float)t2-(float)t1);

			cout<<"===============END TEST=============="<<endl;
			cout<<"RESULTS: "<<endl;
			cout<<"RUNTIME (To search and print): "<<diff<<endl;
		}
	}
	return 0;
}
