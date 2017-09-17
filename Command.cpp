#include "Command.h"

Command::Command()
{
	//fill in container with corresponding commands
	command.insert(pair<string, char>(string("APPEND"), 'a'));
	command.insert(pair<string, char>(string("INSERT"), 'i'));
	command.insert(pair<string, char>(string("REMOVE"), 'r'));
	command.insert(pair<string, char>(string("PRINT"), 'p'));
	command.insert(pair<string, char>(string("PRINT_LINENUMB"), 'n'));
	command.insert(pair<string, char>(string("CHANGE"), 'c'));
	command.insert(pair<string, char>(string("MOVEUP"), 'u'));
	command.insert(pair<string, char>(string("MOVEDOWN"), 'd'));
	command.insert(pair<string, char>(string("WRITE"), 'w'));
	command.insert(pair<string, char>(string("CURRENT_LINE"), '='));
	command.insert(pair<string, char>(string("QUIT"), 'q'));
}

void Command::Parse(string& command_line)
{
	line_address1 = "";
	line_address2 = "";
	_command = "";
	if (command_line.empty())			//empty line case
	{
		flag = 1;
		return;
	}
	//trimming out whitespaces and tabs
	string::iterator end_pos = std::remove(command_line.begin(), command_line.end(), ' '); //remove all spaces
	command_line.erase(end_pos, command_line.end());				//replace the end iterator to new position
	string::iterator end_pos1 = std::remove(command_line.begin(), command_line.end(), '\t'); //remove all tabs
	command_line.erase(end_pos1, command_line.end()); 
	
	//Check for invalid command with 2 or more letters. E.g. 12,55dd or 12D,55i etc
	if (CountLetters(command_line) > 1) 
	{
		cout << "Invalid command line. Too much letters\n";
		return;
	}

	//check if any valid command present in the command line
	//if yes, initialize it
	if (!CommandExisted(command_line))
		return;

	//Identifying line addresses 1, 2 and if comma symbol is presented

	if (command_line.find(',') != std::string::npos)	//command line contains comma
	{
		comma = 1;
		std::size_t current, previous = 0;
		current = command_line.find(',');				//find the position of comma
		line_address1 = command_line.substr(previous, current);
		previous = ++current;
		current = command_line.find(_command);
		line_address2 = command_line.substr(previous, current - previous);
	}
	else												//no comma in the command line
	{
		comma = 0;
		std::size_t current, previous = 0;
		if (!_command.size())
		{
			line_address1 = command_line.substr(previous);
			return;
		}
		current = command_line.find(_command);
		line_address1 = command_line.substr(previous, current);
	}
	
}

bool Command::CommandExisted(const string& command_line)
{
	char last_symbol = command_line.back();
	std::map<string, char>::iterator start = command.begin(); //pointer to the first element
	while (start != command.end())
	{
		if (start->second == last_symbol) //checking if the command line contains a valid command
		{
			_command = command_line.back();
			//remove the command from the command line
			//string::iterator end_pos = std::remove(command_line.begin(), command_line.end(), last_symbol);
			//command_line.erase(end_pos, command_line.end());
			return true;
		}
		start++;				//next element
	}
	//Case when the last character in the command line is invalid 
	if (!(last_symbol >= '0' && last_symbol <= '9' || last_symbol == '$' || last_symbol == '.' || last_symbol == ','))
	{
		cout << "Invalid line address or command " << last_symbol << endl;
		cout << "Use only one of the following commands [a, i, r, p, n, c, u, d, w, q, =]" << endl;
		flag = 0;
		return false;
	}
	_command = "";
	return true;				//empty line is a valid command
		
}

int Command::CountLetters(const string & command_line)
{
	int numb = 0;
	std::string::const_iterator start = command_line.begin();
	while (start != command_line.end())
	{
		if ((*start <= 'z' && *start >= 'a') || (*start <= 'Z' && *start >= 'A'))
			numb++;
		start++;
	}
	return numb;
}

bool Command::Quit(string& command_line)
{
	if (command_line.size() == 1 && command_line == "q")
		return true;
	return false;
}

void Command::CalculateLineRange(int cur_line, list<string> &buf)
{
	if (buf.size() == 0)						//if buffer is empty
	{
		line_address1 = line_address2 = "1";
		flag = 1;
		return;
	}
	flag = 0;							//invalid command line
	size_t last = buf.size();
	// replace $ and .(dot) signs with corresponding address
	if (line_address1 == "$")
		line_address1 = to_string(last);
	if (line_address1 == ".")
		line_address1 = to_string(cur_line);
	if (line_address2 == "$")
		line_address2 = to_string(last);
	if (line_address2 == ".")
		line_address2 = to_string(cur_line);
	
	
	// Line range calculation
	if (!line_address1.size() && !line_address2.size() && !_command.size()) //blank command
	{
		flag = 1;
		return;
	}
	
	if (line_address1.size() && !line_address2.size() && !comma) //case1 from Table4
	{
		int y = std::stoi(line_address1);
		if (y >= 1 && y <= last)								//Constraints condition
		{
			line_address2 = line_address1;
			flag = 1;
		}
		else
		{
			cout << "line address 1 is out of range\n";
			cout << "line address 1 must be in range 1 to " << last << "\n:";
		}
		return;
	}
	if (!line_address1.size() && line_address2.size() && comma) //case2 from Table4
	{
		int y = std::stoi(line_address2);
		if (cur_line >= 1 && y >= cur_line && y <= last)		//Constraints condition
		{
			line_address1 = to_string(cur_line);
			flag = 1;											//valid command line
		}
		else
		{
			cout << "line address 2 or current line is out of range\n";
			cout << "line address 2 must be in range 1 to " << last <<"\n:" ;
		}
		return;
	}
	if (line_address1.size() && !line_address2.size() && comma) //case3 from Table4
	{
		int x = std::stoi(line_address1);
		if (1 <= x && x <= cur_line && cur_line <= last)		//Constraints condition
		{
			line_address2 = to_string(cur_line);
			flag = 1;
		}
		else
		{
			cout << "line address 1 or current line is out of range\n";
			cout << "line address 1 must be in range 1 to " << last << "\n:";
		}
		return;
	}
	if (line_address1.size() && line_address2.size() && comma) //case4 from Table4
	{	
		int x = std::stoi(line_address1);
		int y = std::stoi(line_address2);
		if (1 <= x && x <= y && y <= last)						//Constraints condition
			flag = 1;
		else
		{
			cout << "line address 1 or(and) line address 2 is out of range\n";
			cout << "line addresses must be in range 1 to " << last << "\n:";
		}
		return;
	}
	
	//cases (z) and (,z) from Tables 2 and 3
	if (!line_address1.size() && !line_address2.size()) 
	{
		if (_command != "u" && _command != "d")
		{
			if (1 <= cur_line && cur_line <= last)
			{
				line_address1 = to_string(cur_line);
				line_address2 = line_address1;
				flag = 1;
				return;
			}
			else
			{
				cout << "current line is out of range\n";
				cout << "current line must be in range 1 to " << last << "\n:";
			}
		}
		line_address1 = "1";
		line_address2 = "1";
		flag = 1;
		return;
	}
	
	cout << "Command line error\n:";

}

void Command::print(int &cur_line, list<string>& buf)
{
	int address1 = std::stoi(line_address1) - 1;       //convert string to integer
	int address2 = std::stoi(line_address2);
	do
	{	
		list<string>::iterator runner = buf.begin();  //set iterator to the start of the lsit
		std::advance(runner, address1);				  //get access to the neccessary element of the list
		cout << *runner;
		address1++;
	} while (address1 != address2);
	cur_line = address2;
	cout << ":";									  //command mode
}

void Command::printL(int & cur_line, list<string>& buf)
{
	int address1 = std::stoi(line_address1) - 1;	  //convert string to integer
	int address2 = std::stoi(line_address2);
	do
	{
		list<string>::iterator runner = buf.begin();
		std::advance(runner, address1); //get access to the neccessary element of the list
		cout << address1 + 1 <<"  "<< *runner;
		address1++;
	} while (address1 != address2);
	cur_line = address2;
	cout << ":";						//command mode
}

void Command::printCurrentLine(int &cur_line, list<string> &buf)
{
	/* print current line
	list<string>::iterator runner = buf.begin();
	std::advance(runner, cur_line - 1);
	cout << *runner << "\n:";
	*/
	cout << cur_line << "\n:";
}

void Command::remove(int & cur_line, list<string>& buf)
{
	int address1 = std::stoi(line_address1) - 1; 
	int address2 = std::stoi(line_address2);
	list<string>::iterator start = buf.begin();
	list<string>::iterator end = buf.begin();
	
	//setting new current line
	if (address2 == buf.size())
		cur_line = address1;
	else
		cur_line = address1 + 1;


	//deleting lines in [x,y] range
	std::advance(start, address1);				//set x range
	std::advance(end, address2);				//set y range
	buf.erase(start, end);						//delete lines in [x,y] range
	
	start = buf.begin();
	std::advance(start, cur_line - 1);
	cout << *start;								//print current line
	cout << ":";								//command mode

	
}

void Command::edit(int &cur_line, list<string>& buf) 
{
	//No any info in the pdf about how the current line must changes...
	//By default I set current line to the last edited line

	string pattern, line;
	cout << "\t change what? ";
	getline(cin, pattern);
	cout << "\t to what? ";
	getline(cin, line);

	int address1 = std::stoi(line_address1);
	int address2 = std::stoi(line_address2);
	std::list<string>::iterator start = buf.begin();
	std::list<string>::iterator end = buf.begin();
	std::advance(start, address1 - 1);
	std::advance(end, address2);

	size_t index;
	while (start != end)
	{
		size_t temp = std::string::npos;
		
		//the loop below  checks for all occurences of the patten in the line
		while ((index = (*start).find(pattern)) != std::string::npos && temp != index) 
		{
			temp = index;
			(*start).replace(index, pattern.size(), line);
			cur_line = std::distance(buf.begin(), start) + 1;
		}
		start++;
	}
	cout << ":";			//back to command mode
	
}

void Command::append(int & cur_line, list<string>& buf)
{
	/*list has list.insert(position, value) method. It's
	inserting new elements BEFORE the element at the specified position.
	To add new elements AFTER specified position everything is needed
	is to increase the position by one and use list.insert() fucntion
	*/
	string line;
	int address1 = std::stoi(line_address1);
	std::list<string>::iterator pos = buf.begin();
	std::advance(pos, address1);					//set pos iterator

	getline(cin, line);								//read line
	while (line != ".")
	{
		line.push_back('\n');
		buf.insert(pos, line);						//add new line into the buffer 
		getline(cin, line);
	}
	cur_line = std::distance(buf.begin(), pos);		//set new current line
	cout << ":";									//set command mode
}

void Command::insert(int & cur_line, list<string>& buf)
{
	string line;
	int address1 = std::stoi(line_address1);
	std::list<string>::iterator pos = buf.begin();
	std::advance(pos, address1 - 1);				//set pos iterator
	
	getline(cin, line);								//read line
	while (line != ".")
	{
		line.push_back('\n'); 
		buf.insert(pos, line);						//add new line into the buffer 
		getline(cin, line);
	}
	cur_line = std::distance(buf.begin(), pos);		//set new current line
	cout << ":";									//set command mode
}

void Command::write(int & cur_line, list<string>& buf, string filename)
{
	if (filename != "?")
	{
		
		list<string>::iterator runner = buf.begin();		
		ofstream file(filename);		//open file to write into
		while (runner != buf.end())		//while end of buffer no reached
		{
			file << *runner;			//write to the file the line
			runner++;					//iterator to the next line
		}
		file.close();
	}
	cout << filename << " " << buf.size() << " lines written\n:";
}

void Command::blank(int & cur_line, list<string>& buf)
{
	if (cur_line == buf.size())
	{
		cout << "EOF reached\n" << buf.back() << ":";
		return;
	}
	else
	{
		
		list<string>::iterator runner = buf.begin();
		std::advance(runner, cur_line);
		cur_line++;
		cout << *runner << "\n:";
	}

}

void Command::moveUp(int & cur_line, list<string>& buf)
{
	int pos = std::stoi(line_address1);
	if ((cur_line - pos) < 1)					//top of the buffer is reached
	{	
		cout << "BOF reached\n";
		cur_line = 1;							//set current line to the top of the buffer
	}
	else
		cur_line -= pos;
	
	list<string>::iterator runner = buf.begin();
	std::advance(runner, cur_line - 1);
	cout << *runner << "\n:";
}

void Command::moveDown(int & cur_line, list<string>& buf)
{
	int pos = std::stoi(line_address1);
	if ((cur_line + pos) > buf.size())			//bottom of the buffer is reached
	{	
		cout << "EOF reached\n";
		cur_line = buf.size();					//set current line to the top of the buffer
	}
	else
		cur_line += pos;

	list<string>::iterator runner = buf.begin();
	std::advance(runner, cur_line - 1);
	cout << *runner << "\n:";
}
