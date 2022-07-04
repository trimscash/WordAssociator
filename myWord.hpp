#ifndef MYWORD_HPP
#define MYWORD_HPP

#include <string>
#include <vector>
using namespace std;
typedef struct{
	int wordNum;
	string word;
	string wordDescriptions[1000];
	vector<myWord> associationWord;
}myWord;

#endif //MYWORD_HPP