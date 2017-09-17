#include "Led.h"
#include "Command.h"


Led::Led()
{
	Led("?");										//call another constructor with "?" as a filename
}

Led::Led(const string& fname)
{
	filename = fname;
	ifstream f(filename); //open file
	
	if (f.fail())									//File does not existed in the current directory
	{
		cout << "Unable to open file " << filename << endl;
		cout << filename << " [New File]" << endl;
		file_exist = 0;								//new file
		
	}
	else
	{	
		file_exist = 1;								//file existed in current directory
		FileIntoBuffer(filename);
		cout << filename << " " << CountLines(filename) << " lines" << endl;
		current_line = buffer.size();
		last_line = buffer.size();
	}
	is_changes_saved = 0;							
	cout << "Entering command mode.\n:";
}

void Led::display()
{
	list<string>::iterator start  = buffer.begin();
	list<string>::iterator finish = buffer.end();
	while (start != finish)
		cout << *start++;
}

void Led::FileIntoBuffer(string & fname)
{
	string line;
	ifstream fin(fname);
	while (getline(fin, line))
		buffer.push_back(line + "\n");				//add the read line at the end of the buffer
}

int Led::CountLines(string& fname)
{
	int numb_of_lines = 0;
	string current_line;
	ifstream fin(fname);							//open file
	
	while(getline(fin, current_line))				//read file line by line
		numb_of_lines++;
	return numb_of_lines;
}

void Led::run()
{
	string command_line;
	Command cmd;
	do
	{
		getline(cin, command_line);					//read a command line
		cmd.Parse(command_line);					//parse command line
		execute(cmd);								//execute the command
	} while (cmd.GetCommand() != "q");
}

void Led::execute(Command & cmd)
{
	if (cmd.GetCommand() == "q")				   //check if the quit command were called
	{
		if (!is_changes_saved)
		{
			cout << "Save changes to " << filename << " (y/n)?";
			string answer;
			getline(cin, answer);					
			while (!(answer == "y" || answer == "n"))
			{
				cout << "Invalid answer: " << answer << endl;
				cout << "enter y for yes and n for no.\n";
				cout << "Save changes to " << filename << " (y/n)?";
				getline(cin, answer);
			}
			if (answer == "y")
				cmd.write(current_line, buffer, filename);
		}
		cout << "quitting led . . . bye.";
		return;									   //terminate Led
	}
	if (buffer.size() == 0)						   //buffer is empty
	{
		if (!(cmd.GetCommand() == "i"))			   //if not insert command
		{
			cout << "Buffer is empty. Add few lines to the buffer by using 'i' command only\n:";
			return;
		}
	}

	cmd.CalculateLineRange(current_line, buffer);  //initialize line addresses and flag
	if (cmd.GetFlag())							   //check if valid command were entered
	{
		string command = cmd.GetCommand();
		string line1 = cmd.GetLineAddress1();
		string line2 = cmd.GetLineAddress2();

		if (command == "a" || command == "r" || command == "c" || command == "i")
			is_file_edited = 1; 
		if (command == "w")
		{
			is_changes_saved = 1;
			is_file_edited = 0;
		}

		if (command == "a")
			cmd.append(current_line, buffer);
		else if (command == "i")
			cmd.insert(current_line, buffer);
		else if (command == "r")
			cmd.remove(current_line, buffer);
		else if (command == "p")
			cmd.print(current_line, buffer);
		else if (command == "n")
			cmd.printL(current_line, buffer);
		else if (command == "c")
			cmd.edit(current_line, buffer);
		else if (command == "u")
			cmd.moveUp(current_line, buffer);
		else if (command == "d")
			cmd.moveDown(current_line, buffer);
		else if (command == "w")
			cmd.write(current_line, buffer, filename);
		else if (command == "=")
			cmd.printCurrentLine(current_line, buffer);
		else if (command == "" && line1 == "" && line2 == "") //Only the return key was pressed
			cmd.blank(current_line, buffer);
		else
			cmd.print(current_line, buffer);
	}
}

