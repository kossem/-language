#include <iostream>
#include <fstream>
#include <set>
#include <algorithm>
#include <vector>
#include <cstring>
#include "stdlib.h"
using namespace std;
struct Elem {
        set<string> pair;
	string st;
	string var;
	float shog;
	int chast;
};
 
set<string> Funpair(const string &a)
{
	int i=1;
	set<string> pair; 
	pair.insert(string({a[0],a[1]}));
	while(i < a.length()){
		pair.insert(string({a[i - 1], a[i]}));
		i++;
	}
	return pair;
}
 
float usebigrams(const set<string> &a, const set<string> &b) {
	vector<string> intersect, a_union;     
	set_intersection(a.begin(), a.end(),b.begin(), b.end(), inserter(intersect, intersect.begin()));
	if (!intersect.size()) {
		return 0;
	}
	set_union(a.begin(), a.end(),	b.begin(), b.end(), inserter(a_union, a_union.begin()));
	return (float)intersect.size()/(float)a_union.size();
}
int main()
{
	vector<Elem> a;
	set<string> pair;
	float shog,k;
	int length,i,chast;
	string st;
	length = 1;
	for(;getline(cin, st);) {
		for(i=0; i<length;i++)
		{
			a.push_back(Elem());
			a.back().pair = Funpair(st);
			a.back().var = "";
			a.back().shog = 0;
			a.back().chast = -1;
			break;
		}
	} 
	ifstream file("count_big.txt");
	string line, correct; 
	while (file) {
		file >> correct >> chast;
		pair = Funpair(correct);
		for (auto help = a.begin(); help != a.end(); help++) {
			shog = usebigrams(help->pair, pair);                      
			if (help->chast == -1 ) {              
				help->var = correct;
				help->shog = shog;
				help->chast = chast;
			}
			else {
				if (shog != help->shog) {
					k = shog - help->shog;
				} else if (chast != help->chast) {
					k = chast - help->chast;
				} else if (correct < help->var) {
					k = -1;
				} else {
					k = 1;
				}
				if(k > 0){	  
					help->var = correct;
					help->shog = shog;
					help->chast = chast;
				}
			}
		}
	}
	file.close();
	for (auto help = a.begin(); help != a.end(); help++) {
		cout << help->var <<"\n";
	}
	return 0;
}
