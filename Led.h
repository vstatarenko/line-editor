#pragma once
#include "Command.h"

using namespace std;

class Led 
{
private:
	string filename;						//entered file
	bool file_exist;						//is the file new or existed one
	bool is_file_edited;					//indicates whether the file content were changed or not
	bool is_changes_saved;					//indicates whether the changes we saved into the file or not
	int current_line;						//current line address in the buffer
	int last_line;							//last line address in the buffer
	list<string> buffer;					//buffer
	void FileIntoBuffer(string& fname);		//Store the content of the file into buffer
	int CountLines(string& fname);			//counts the number of lines in the file
public:

	Led();
	Led(const string& fname);	
	void display();							//print the content of the buffer
	void run();								//run our editor
	void execute(Command& cmd);				//Execute command
	int GetCurrentLine() {return current_line;}
};
