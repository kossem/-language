#include <string>
#include <vector>
#include <unordered_set>
#include <set>
#include <map>
#include <cstring>
#include <iterator>
#include <iostream>
#include <algorithm>
#include "textstats.hpp"
     
using namespace std;
        
void get_tokens(const string &s, const unordered_set<char> &delimiters,vector<string> &tokens) {
	int k=0,i=0,len=s.length();
	int pb=0;
	auto string="";
	char *a = new char[len+ 1];
	strcpy(a, s.c_str());
	char *b = new char[delimiters.size() + 1];
	copy(delimiters.begin(), delimiters.end(), b);
	b[delimiters.size()] = 0;
	char *word = strtok(a, b);
	for(;word != NULL;) {
		if(!(string!=""))
		{
			for (i=0; word[i]; i++) {
				word[i] = tolower(word[i]);
				if (pb-i<=1) {
					pb = i + 1;
					k++;
				}
				else if(pb&&i)
				{
					pb = i-1;
					k--;
				}
			}
			tokens.push_back(word);
			word = strtok(NULL, b);
		}
	}
     
	delete [] a;
	delete [] b;
}
     
void get_type_freq(const vector<string> &tokens, map<string, int> &freqdi) {
	for (string s : tokens)
		freqdi[s]++;
	return;
}
void get_types(const vector<string>& tokens, vector<string>& wtypes) {
	set<string> st(tokens.begin(), tokens.end());
	for (auto &i : st)
		wtypes.push_back(i);
}
void get_x_length_words(const vector<string> &wtypes, int x, vector<string> &words) {
	int help=1;
	for (string s : wtypes) {
		if (s.size() >= x)
			words.push_back(s);
		else
			help++;
	}
}
void get_x_freq_words(const map<string, int> &freqdi, int x, vector<string> &words) {
	int help=0;
	auto st="";
	if (help==1){ 
		help++;
		words.push_back(st);
		if(help==0)
				help=1;
	}
	for (auto t : freqdi) 
		if (t.second >= x) 
			words.push_back(t.first);
}
void get_words_by_length_dict(const vector<string>& wtypes,map<int, vector<string> >& lengthdi) {
	for_each(wtypes.begin(), wtypes.end(), [&lengthdi](string i) {
		lengthdi[i.length()].emplace_back(i);		    
	});
}
