#pragma once
#include<vector>
#include<iostream>
#include<fstream>
#include <string>
#include <list>
#include<algorithm>
#include<map>

using namespace std;

class Command
{
private:
	string line_address1;	//line address 1
	bool comma;				//is comma presented in the command line
	string line_address2;	//line address2
	string _command;		//command
	bool flag = 1;			//flag indicating whether the input command line is valid or not

	map<string, char> command;						  //storage for all commands
	bool CommandExisted(const string& command_line);  //check if a commang given in a command line is valid or existed
	int CountLetters(const string& command_line);     //returns number of letters in the string
public:
	Command();
	void Parse(string &command_line);				           //Parsing the command line
	bool Quit(string& command_line);				           //check whether or not 'q' was entered to quit
	void CalculateLineRange(int cur_line, list<string>& buf);  //Line range calculation related on Tables(2,3,4,) p.6
	
	string GetCommand() { return _command; }
	string GetLineAddress1() { return line_address1; }
	string GetLineAddress2() { return line_address2; }
	bool GetFlag() { return flag; }

	//main commands
	void print(int &cur_line, list<string> &buf);	  //print buffer (p command)
	void printL(int &cur_line, list<string> &buf);	  //print buffer with line number (n command)
	void remove(int &cur_line, list<string> &buf);	  //remove the line in specified range
	void edit(int &cur_line, list<string> &buf);	  //replace string in specidied range
	void append(int &cur_line, list<string> &buf);	  //reads and appends lines to the buffer AFTER specified line
	void insert(int &cur_line, list<string> &buf);	  //reads and appends lines to the buffer BEFORE specified line
	void blank(int &cur_line, list<string> &buf);	  //nothing were entered. Execute same as 1d
	void moveUp(int &cur_line, list<string> &buf);	  //moves the current line UP by specified number of lines,
													  //but never beyond the first line. Prints the new line
	void moveDown(int &cur_line, list<string> &buf);  //moves the current line DOWN by specified number of lines,
												      //but never beyond the last line. Prints the new line
	
	void printCurrentLine(int &cur_line, list<string> &buf);  //print current line address (= command)
	
	
	void write(int &cur_line, list<string> &buf, string filename); //writes out entire buffer to its associated file
	
};