/*
	BattlEye Auto-Exception Generator (BE_AEG)
	created by eraser1

	This work is protected by Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0). By using, downloading, or copying any of the work contained, you agree to the license included.
	http://creativecommons.org/licenses/by-nc-sa/4.0/


The following overview is a human-readable summary of (and not a substitute for) [the full license](http://creativecommons.org/licenses/by-nc-sa/4.0/legalcode).

You are free to:

Share - copy and redistribute the material in any medium or format.
Adapt - remix, transform, and build upon the material.


Under the following terms:

Attribution - You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial - You may not use the material for commercial purposes.
ShareAlike - If you remix, transform, or build upon the material, you must distribute your contributions under the **same license** as the original.
No additional restrictions - You may not apply legal terms or technological measures that legally restrict others from doing anything the license permits.


Notices:

You do not have to comply with the license for elements of the material in the public domain or where your use is permitted by an applicable exception or limitation
No warranties are given. The license may not give you all of the permissions necessary for your intended use. For example, other rights such as publicity, privacy, or moral rights may limit how you use the material.
*/

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <streambuf>
#include <string>
#include <regex>
#include <map>
#include <time.h>


//#define USE_CHRONO_THREAD_SLEEP 1
#ifdef USE_CHRONO_THREAD_SLEEP

#include <chrono>
#include <thread>

#else

	#ifdef WIN32
	#include <windows.h>
	#else
	#include <unistd.h>
	#endif	//WIN32

#endif	//USE_CHRONO_THREAD_SLEEP


using namespace std;


// Value-Definitions of the different String values
static enum StringValue
{
	ev_undefined,
	ev_noScriptLogBackup,
	ev_noLogs,
	ev_enableDebugConsole,
	//ev_noScripts,
	ev_runOnce
};

// Map to associate the strings with the enum values
static map<string, StringValue> s_mapStringValues;


// Initialize variables used for getting time/date.
errno_t err;
char time_buffer[9];
string time_date;



/* HELPER FUNCTIONS */

void my_sleep(unsigned int sleep_time) // cross-platform sleep function
{
	#ifdef USE_CHRONO_THREAD_SLEEP

	this_thread::sleep_for(chrono::milliseconds(sleep_time));

	#else

	#ifdef WIN32
	Sleep(sleep_time);
	#else
	usleep(sleep_time * 1000000);
	#endif // win32

	#endif
}

string get_TimeDate()
{
	_strdate_s(time_buffer);
	if (err)
	{
		cout << "_strdate_s failed!\n";
		return "ERROR GETTING DATE";
	}

	time_date = time_buffer;


	time_date += " ";


	_strtime_s(time_buffer);
	if (err)
	{
		cout << "_strtime_s failed!\n";
		return "ERROR GETTING TIME";
	}
	time_date += time_buffer;

	return time_date;
}


string fileToString(string fileName)
{
	ifstream file(fileName);
	string fileStr;
	/*
	file.seekg(0, ios::end);
	fileStr.reserve(file.tellg());
	file.seekg(0, ios::beg);
	*/
	fileStr.assign( ( istreambuf_iterator<char>(file) ), istreambuf_iterator<char>() );

	return fileStr;
};


bool in_vector(vector<string> vec, string find)
{
	if (vec.size() == 0) return false;
	for (unsigned int i=0; i < vec.size(); ++i)
	{
		if (vec[i] == find) return true;
	}

	return false;
};


string vector_get(vector<string>& vec, unsigned int index)
{
	if (vec.size() < (index + 1))
	{
		vec.resize(index + 1);
	}

	return vec[index];
}


vector<string> vector_get(vector< vector<string> >& vec, unsigned int index)
{
	if (vec.size() < (index + 1))
	{
		vec.resize(index + 1);
	}

	return vec[index];
}


int main(int argc, char* argv[])
{
	// Variable initialization

	// Define regular expressions
	regex rgx_script_restr ( "\\(\\d+?\\.\\d+?\\.\\d+?\\.\\d+?:\\d+?\\)\\s(?:\\w+|-)\\s-\\s#(\\d+)\\s\"((?:(?!\\n\\d\\d\\.\\d\\d\\.\\d\\d\\d\\d\\s\\d\\d:\\d\\d:\\d\\d:\\s)(?:\\s|\\S))*)" );	// The single line that took over 12 hours of work.
	regex rgx_match_newlines("\\n");
	//regex rgx_match_rgx_chars("[\\^\\$\\.\\|\\{\\}\\[\\]\\(\\)\\*\\+\\?\\\\\"]");
	regex rgx_match_rgx_chars("[\\\\\"]");

	vector< vector<string> > previous_exceptions;		// This is so we don't add duplicate (script) exceptions


	// Launch options
	bool backup_script_logs = true;
	bool use_debug_log = true;
	bool use_debug_console = false;
	bool run_once = false;
	//bool generate_script_exceptions = true;


	// Initialize the string map
	s_mapStringValues["noScriptLogBackup"] = ev_noScriptLogBackup;
	s_mapStringValues["noLogs"] = ev_noLogs;
	s_mapStringValues["enableDebugConsole"] = ev_enableDebugConsole;
	s_mapStringValues["runOnce"] = ev_runOnce;
	//s_mapStringValues["noScripts"] = ev_noScripts;

	string launch_params = "";
	for (int i=1; i < argc; ++i)
	{
		string param(argv[i]);
		launch_params += param;
		switch (s_mapStringValues[param])
		{
			case ev_noScriptLogBackup:
			{
				backup_script_logs = false;
				cout << "Script logs disabled!\n";
				break;
			}

			case ev_noLogs:
			{
				use_debug_log = false;
				cout << "Debug logs disabled!\n";
				break;
			}

			case ev_enableDebugConsole:
			{
				use_debug_console = true;
				cout << "Debug console enabled!\n";
				break;
			}

			case ev_runOnce:
			{
				run_once = true;
				cout << "BE Auto Exception Generator (BE_AEG) will only run once!\n";
				break;
			}
			/*
			case ev_noScripts:
			{
				generate_script_exceptions = false;
				cout << "Script exceptions will not be generated!\n";
				break;
			}
			*/
			default:
			{
				cout << "Unknown command line parameter: " << argv[i] << endl;
				launch_params += "?";
			}
		}
		launch_params += " ";
	}



	ofstream debug_file;
	if (use_debug_log)
	{
		debug_file.open("BE_AutoExceptionGenerator.log", ios::app);

		debug_file << "\n\nLAUNCH BE_AEG v1.0.5 (" << get_TimeDate() << ") launch params: " << launch_params << endl;	// Write launch and version info to debug log.
	}


	scripts_exceptions:
		//if (generate_script_exceptions)
		if (true)
		{
			// Copy file to string
			string fileStr = fileToString("scripts.log");


			if (fileStr == "")
			{
				// scripts.log is empty, so restart the check, or just go to the end if you want to run once.
				if (run_once)
				{
					goto the_end;
				}
				else
				{
					my_sleep(500);
					goto scripts_exceptions;
				}
			}


			if (backup_script_logs)
			{
				ofstream scripts_log_backup("BE_AEG_scripts.log", ios::app);
				scripts_log_backup << "\n" << fileStr;						// Add an extra newline before appending the new script logs just in case.
			}


			smatch match;
			unsigned int match_ctr = 0;
			unsigned int new_exception_ctr = 0;
			vector<string> new_exceptions;						// Initialize the list of exceptions to be added now

			while (regex_search(fileStr,match,rgx_script_restr))
			{
				// Get the restriction # and code that triggered it.
				string restriction = match[1];
				string code = match[2];
				unsigned int restriction_num = atoi(restriction.c_str());

				if (code[code.length() - 1] == '\n')
				{
					code.pop_back();
				}
				code.pop_back();

				vector<string> prev_exceptions_restriction_num = vector_get(previous_exceptions, restriction_num);

				if (!in_vector(prev_exceptions_restriction_num, code))
				{
					// Escape double quotes and backslashes
					string exception = regex_replace(code,rgx_match_rgx_chars, "\\$&");

					// Replace newlines, add a "!=" to the beginning, as well as wrap the exception in quotes
					exception = " !=\"" + regex_replace(exception,rgx_match_newlines, "\\n") + "\"";

					string new_exceptions_line = vector_get(new_exceptions ,restriction_num) + exception;

					new_exceptions[restriction_num] = new_exceptions_line;

					prev_exceptions_restriction_num.push_back(code);							// Add the exception code to the list of exceptions for this restriction number.
					previous_exceptions[restriction_num] = prev_exceptions_restriction_num;		// Update the main exception list.


					if (debug_file.is_open())
					{
						debug_file << "(" << get_TimeDate() << ") Script Restriction #" << restriction << " produced exception: " << exception << endl;
					}
					++new_exception_ctr;
				}
				else if (debug_file.is_open())
				{
					debug_file << "(" << get_TimeDate() << ") DUPLICATE Script Restriction #" << restriction << ".\n";
				}


				if (use_debug_console)
				{
					cout << "\n------------------------------------------------------\n" << "Match #" << match_ctr << endl;
					cout << "Restriction #" << restriction << ":\n" << code << "\n\n";
				}

				++match_ctr;
				fileStr = match.suffix();
			}

			if (use_debug_console)
			{
				cout << "No More Matches Found!\n";
			}
			// Just so you know the program is doing something :p
			cout << "Generated " << new_exception_ctr << " new exceptions.\n";


			write_to_scriptsTXT:

			ifstream file("scripts.txt");
			if (!file)
			{
				cout << "Could not access file: \"scripts.txt\"\n";
				// scripts.txt can't be accessed, so we keep trying on a quarter second delay
				my_sleep(250);
				goto write_to_scriptsTXT;
			}

			int current_restriction = -1;
			int new_exceptions_size = new_exceptions.size();
			string scripts_fileStr = "";
			string line;
			while (getline(file, line))
			{
				scripts_fileStr += line;
				if (regex_search(line, regex("^\\d")))
				{
					++current_restriction;

					if (new_exceptions_size >= (current_restriction + 1))
					{
						scripts_fileStr += new_exceptions[current_restriction];
					}
				};
				scripts_fileStr += "\n";
			}
			file.close();

			// Rewrite scripts.txt with the updated exceptions
			ofstream ofile("scripts.txt", ios::trunc);
			ofile << scripts_fileStr;
			ofile.close();


			// Empty the scripts log since we've now generated the exceptions
			ofstream scripts_log("scripts.log", ios::trunc);
			scripts_log.close();
		}
	// End of script exceptions

	if (!run_once)
	{
		my_sleep(500);
		goto scripts_exceptions;
	}


	the_end:

	string thisistokeeptheconsoleopen;
	cout << "\n\nBattlEye Auto-Exception Generator is finished running. Type and enter something to exit the program: ";
	cin >> thisistokeeptheconsoleopen;
	return 0;
}
