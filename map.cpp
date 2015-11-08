#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <stdlib.h>
#include <cstdint>

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
		north = stoi(s.substr(0,pos));

		s = s.substr(pos+1);
		pos = s.find(" ");
		south = stoi(s.substr(0,pos));

		s = s.substr(pos+1);
		pos = s.find(" ");
		west = stoi(s.substr(0,pos));

		s = s.substr(pos+1);
		pos = s.find(" ");
		east = stoi(s.substr(0,pos));	

		file = s.substr(pos+1);
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

int main() {
	//input all 149 coverage entries
	Coverage* c = new Coverage[149];
	int count = 0;
	ifstream fs("coverage.txt");
	string line;
	if(fs) {
		while(getline(fs,line)) {
			Coverage* newCov = new Coverage(line);
			c[count] = *newCov;
			count++;
		}
	}

	/*Test File input
	for(int i = 0; i < 149; i++) {
		cout<<c[i].file<<endl;
	}*/

	//get user input
	double lat1, lat2, lon1, lon2;
	cout<<"Enter city 1 lat and long (space divided)"<<endl;
	//cin>>lat1;
	//cin>>lon1;
	cout<<"Enter city 2 lat and long"<<endl;
	//cin>>lat2;
	//cin>>lon2;

	lat1 = 45;
	lat2 = 33;
	lon1 = -90;
	lon2 = -87;
	//find the smallest coverage area with both points
	int smallest = 50;
	Coverage result;
	for(int i = 0; i < 149; i++) {
		if((c[i].north >= lat1 && c[i].north >= lat2) &&
			(c[i].south <= lat1 && c[i].south <= lat2) &&
			(c[i].west >= lon1 && c[i].west >= lon2) &&
			(c[i].east >= lon1 && c[i].east >= lon2)) {
				if((c[i].north - c[i].south) < smallest) {
					//cout<<"Changinng result: "<<smallest;
					smallest = c[i].north - c[i].south;
					result = c[i];
					//cout<<" to "<<smallest<<endl;
				}
			//cout<<"FOUND MAP: "<<"N: "<<c[i].north<<" S:"<<c[i].south<<" "<<c[i].file<<endl;
		} 
	}
	cout<<"Smallest map: "<<smallest<<" "<<result.file<<endl;

	//open smallest map
	string fname = "all/"+result.file;
	cout<<"FILE: "<<fname<<endl;
	ifstream file(fname);
	if(file) {
		//go to the end of the file to get some values
		file.seekg(0,ios::end);
		int end = file.tellg();
		int numOfInts = end/2;
		cout<<"File ENDS: "<<end<<"NUMof ints"<<numOfInts<<endl;

		//back to the front
		file.seekg(0);

		//get first row
		string row, col, bpp, spp, lls, tls, min, max, sv;
		file>>row>>row;
		file>>col>>col;
		file>>bpp>>bpp;
		file>>spp>>spp;
		file>>lls>>lls;
		file>>tls>>tls;
		file>>min>>min;
		file>>max>>max;
		file>>sv>>sv;

		//make map object
		Map map = Map(row,col,bpp,spp,lls,tls,min,max,sv);
		map.values = new short int*[map.row];
		
		cout<<"ROW: "<<row<<endl;
		cout<<"col: "<<col<<endl;
		cout<<"TLS: "<<tls<<endl;
		signed short int p;
		int temp = stoi(row);
		//cout<<"Get dummy row: "<<file.tellg()<<" "<<(file.tellg()/2)<<endl;

		file.clear();
		
		//go to byte 1200 so end of dummy row
		file.seekg(2*stoi(row));
		cout<<"AT: "<<file.tellg()<<endl;

		int rowCount = 0;
		int colCount = 0;
		int pos = 0;
		int neg = 0;
		int special = 0;

		int N = 601;
		//while(N--) {

		while(file.tellg() != end) { 
			if(colCount%map.col == 0) {
				cout<<"NEW ROW: "<<rowCount<<endl;
				//map.values[rowCount] = new short int[map.col];
				rowCount++;
				colCount = 0;
			}
			short int lsb = (short int) file.get();
			short int msb = (short int) file.get();
			short combined = (msb << 8 ) | (lsb & 0xff);

			if(combined > 0) {
				pos++;
			} else if(combined == -500) {
				special++;
			} else if(combined != -500 && combined < 0) {
				neg++;
			} 
			//cout<<"File at: "<<file.tellg()<<" Row: "<<rowCount<<" Col: "<<colCount<<" Val: "<<combined<<endl;
			//map.values[rowCount++][colCount++] = combined;
			colCount++;
		}
		cout<<"Positive: " <<pos<<endl;
		cout<<"Special Val: -500: "<<special<<endl;
		cout<<"Negative: "<<neg<<endl;
		cout<<"Total: "<<(pos+neg+special)<<endl;

	} else {
		cout<<"FILE NOT OPENED"<<endl;
	}
    return 0;
}
