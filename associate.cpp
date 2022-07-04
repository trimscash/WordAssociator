#include <iostream>
#include <string>
#include <stdio.h>
#include <fstream>
#include <vector>
#include "nlohmann/json.hpp"

using namespace nlohmann;
using namespace std;

json wordList;

#define DIR "c:\\wordList.json" //wordList.json の絶対パス


string inputWord(string printWord){
	string returnWord;
	cout <<printWord;
	getline(std::cin,returnWord);
	return returnWord;
}

int isThereWord(string word){
	for(auto it=wordList.begin();it!=wordList.end();++it){
		if(it.key()==word){
			return 1;
		}
	}
	return 0;
}

void printWord(string word){
	cout<<"["<<word<<"]"<<endl;
	if (wordList[word]["explanation"].empty()) {
		cout<<"\n#"<<wordList[word]["explanation"];
	}
	int c=0;
	for (auto itr = wordList[word]["associatedWords"].begin(); itr != wordList[word]["associatedWords"].end(); itr++) {
		cout << *itr << " ";
		if(!(c%6)&&c!=0){
			cout << endl;
		}
		c++;
	}
	if (!(!((c-1) % 6) && (c-1) != 0)) {
		cout <<endl;
	}
}

void saveJSON() {
	ofstream oStream(DIR);
	if (!oStream) {
		cout << "ERROR. Can't open File..GOOD BYE.." << endl;
		exit(0);
	}
	oStream << std::setw(4) << wordList << std::endl;
}

void addToWordList(string word) {
	wordList[word]["word"] = word;
	wordList[word]["explanation"] = "";
	wordList[word]["associatedWords"] = json::array();

	saveJSON();
}

int addToWordListWithAlert(string word) {//作製に成功したら１
	if (!(isThereWord(word))) {
		string s;
		cout << "There is not cout '" << word << "' in WordList." << endl;
		cout << "Do you want to add the word to WordList?";
		s = inputWord("[y/n]");
		if (s[0] == 'y') {
			wordList[word]["word"] = word;
			wordList[word]["explanation"] = "";
			wordList[word]["associatedWords"] = json::array();

			saveJSON();
			cout << "'" << word << "' was added to WordList." << endl
				 << endl;
		} else {
			cout << "Cancelled.." << endl
				 << endl;
			return 0;
		}
	}
	return 1;
}



vector<string> split(const string &str, char sep){
	vector<string> vec;
	auto first=str.begin();
	while(first!=str.end()){
		auto last = first;
		while(*last!=sep&&last!=str.end()){
			last++;
		}
		vec.push_back(string(first,last));

		if(last!=str.end()){
			last++;//sepを飛ばす
		}
		first=last;
	}
	return vec;
}

void removeWord(string word){
	wordList.erase(word);

	saveJSON();
}

int isThereAssociatedWord(string from, string word) {//なかたら０あたら１
	auto itr = find(wordList[from]["associatedWords"].begin(), wordList[from]["associatedWords"].end(), word);
	if (itr == wordList[from]["associatedWords"].end()) {
		return 0;
	} else {
		return 1;
	}
}

void removeAssociatedWord(string from,string removeWord){
	if (!(wordList[from]["associatedWords"].empty())) {
		wordList[from]["associatedWords"].erase(find(wordList[from]["associatedWords"].begin(), wordList[from]["associatedWords"].end(), removeWord));
		saveJSON();
	}
}

int addAssociatedWord(string to,string addWord){
	wordList[to]["associatedWords"].push_back(addWord);
	wordList[addWord]["associatedWords"].push_back(to);

	saveJSON();

	return 1;
}

void printWordList(int n){
	if (wordList.size()<n) {
		n=wordList.size();
	}
	int c=0;
	for(auto itr=wordList.begin();itr!=wordList.end();itr++){
		if(c!=n){
			cout<<(*itr)["word"]<<endl;
		}else{
			return;
		}
		c++;
	}
}

void associationModeAddWordMode(string word, string addWord) {
	if (isThereAssociatedWord(word, addWord)) {
		cout << "Already exists." << endl;
	} else {
		if (!(isThereWord(addWord))) {
			if (addToWordListWithAlert(addWord)) {
				addAssociatedWord(word, addWord);
				cout << "'" << addWord << "' was added." << endl;
			}
		}else{
			addAssociatedWord(word, addWord);
			cout << "'" << addWord << "' was added." << endl;
		}
	}
}

void associationModePrintWordMode(string word) {
	if (isThereWord(word)) {
		printWord(word);
	} else {
		cout << "There is not '" << word << "' in wordlist." << endl;
	}
}

void associationModeRemoveMode(string word, string removeWord) {
	if (isThereAssociatedWord(word, removeWord)) {
		removeAssociatedWord(word, removeWord);
		cout << removeWord << " was removed from AssociatedList." << endl;
	} else {
		cout << "There is not '" << removeWord << " in wordlist.'" << endl;
	}
}

void associationModeHelp(){
	cout << "!a [Word] --Add the word to Associated Wordlist." << endl;
	cout << "!p --Print this mode's word.\n!p [Word] --Print the word info." << endl;
	cout << "!r [Word] --Remove the word from Associated Wordlist" << endl;
	cout << "!c [Word] --Change Association mode's word" << endl;
	cout << "!q --Quit this mode." << endl;
	cout << "!h --Print this help." << endl;
}

void associationMode(string word){

	addToWordListWithAlert(word);

	cout << "'!q' to quit This mode. or '!h' to help'" << endl;

	while (1) {
		string arg;
		vector<string> args;
		arg = inputWord(word + ">>");
		args = split(arg, ' ');
		if (args.size() == 1) {
			if (args[0] == "!q" || args[0] == "!quit") {
				return;

			} else if (args[0] == "!h" || args[0] == "!help") {
				associationModeHelp();

			} else if (args[0] == "!p" || args[0] == "!print") {
				associationModePrintWordMode(word);

			} else {
				cout << "No such mode or you need more argument. you can use 'help'" << endl;

			}
		} else if (args.size() == 2) {
			if (args[0] == "!r" || args[0] == "!remove") {
				associationModeRemoveMode(word,args[1]);

			} else if (args[0] == "!a" || args[0] == "!add") {
				associationModeAddWordMode(word, args[1]);

			} else if (args[0] == "!c" || args[0] == "!change") {
				associationMode(args[1]);
				return;

			} else {
				cout << "No such mode. you can use 'help'" << endl;

			}
		} else if (args.size() == 0) {
			continue;

		} else {
			cout << "No such mode. you can use 'help'" << endl;

		}
	}
}

void mainModeMakeMode(string word){
	if (isThereWord(word)) {
		cout << "Already exists." << endl;
	} else {
		addToWordList(word);
		cout << "'" << word << "' was added." << endl;
	}
}

void mainModeRemoveMode(string word){
	if (isThereWord(word)) {
		removeWord(word);
		cout << word << "was removed." << endl;
	} else {
		cout << "There is not '" << word << " in wordlist.'" << endl;
	}
}

void mainModePrintMode(string word){
	if (isThereWord(word)) {
		printWord(word);
	} else {
		cout << "There is not '" << word << " in wordlist.'" << endl;
	}
}

void mainModeHelp() {
	cout << "a [Word] --Enter to Association mode." << endl;
	cout << "m [Word] --Make new word." << endl;
	cout << "r [Word] --Remove the word from Wordlist" << endl;
	cout << "p [Word] --Print the word info" << endl;
	cout << "l [Num] --Print Num pieces of word." << endl;
	cout << "l f [Num] --Print Num pieces of word in decreasing order in quantity of Associated word." << endl;
	cout << "l m [Num] --Print Num pieces of word in ascendeing order in quantity of Associated word." << endl;
	cout << "h --Print this help." << endl;
}

int main(){
	cout<<"Loading..."<<endl;
	ifstream iFile(DIR);
	if(!iFile){
		cout<<"ERROR. Can't open File..GOOD BYE.."<<endl;
		return 0;
	}
	iFile>>wordList;


	while(1){
		string arg;
		vector<string> args;
		arg = inputWord(">>");
		args = split(arg, ' ');


		if(args.size()==2){
			if (args[0] == "a" || args[0] == "association") {
				associationMode(args[1]);

			}else if (args[0] == "m" || args[0] == "make") {
				mainModeMakeMode(args[1]);

			}else if (args[0] == "r" || args[0] == "remove") {
				mainModeRemoveMode(args[1]);

			} else if (args[0] == "p" || args[0] == "print") {
				mainModePrintMode(args[1]);

			} else if (args[0] == "l" || args[0] == "list") {
				int a;
				sscanf(args[1].c_str(), "%d", &a);
				printWordList(a);
				
			} else {
				cout << "No such mode. you can use 'help'." << endl;
			}

		}else if(args.size()==1){
			if (args[0] == "h" || args[0] == "help") {
				mainModeHelp();
			} else if (args[0] == "l" || args[0] == "list" ||args[0] == "a" || args[0] == "association" || args[0] == "r" || args[0] == "remove" || args[0] == "p" || args[0] == "print" || args[0] == "m" || args[0] == "make") {
				cout<<"You need more argument. you can use 'help'"<<endl;
			} else {
				cout<<"No such mode. you can use 'help'"<<endl;
			}
		} else if (args.size() == 0) {
			continue;
		} else {
			cout << "No such mode. you can use 'help'" << endl;
		}
	}

}
