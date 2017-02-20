/*
example code

#include "CommandLineParser.hpp"
p::CommandLineParser parser(argc, argv);

int input = parser.addOption<int>("-i",-17,"test int");
std::string allo = parser.addOption<std::string>("-s","coucou","test string");
std::string peep = parser.addOption<std::string>("-p","salut");
parser.CompileHelpFromOptions();

*/

#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <cstdarg>
#include <cstdlib>// EXIT_SUCCESS
#include <vector>
#include <exception>
#include <map>

// #include "version.hpp"


namespace p
{
	class CommandLineParser
	{
	private:
		std::map<std::string, std::string> _argmap;
		std::map<std::string, std::string> _descriptionmap;
		bool _displayHelp;
		std::string _appName;


		template <typename T>
		T ConvertTo ( const std::string &Text )
		{
			std::istringstream ss(Text);
			T result;
			return ss >> result ? result : 0;
		}
		template <typename T>
		std::string ConvertToString ( const T &Number )
		{
			std::ostringstream ss;
			ss << Number;
			return ss.str();
		}

	public:
		CommandLineParser(int argc, char** argv)
		{
			int numArg = argc;
			std::vector<std::string> argList;

			_displayHelp = false;
			_appName = argv[0];

			std::string key = "/";
			std::size_t found = _appName.rfind(key);
			if (found!=std::string::npos)
			_appName.replace (0,found+1,"");



			// convert char** to vector<string>

			for (int i = 1; i < argc; i++)
			{
				std::stringstream ss;
				std::string		  s;
				ss << argv[i];
				ss >> s;
				argList.push_back(s);

				if(s=="-h" || s=="--help")
				{
					_displayHelp = true;
				}
				if(s=="--version")
				{
					DisplayOption();
				}
			}

			// map <option , Argument>

			for (unsigned int i = 0; i < argList.size(); i++)
			{
				if (argList[i][0] == '-')
				{
					if(i!=argList.size()-1) // argList[i + 1] exists
					{
						if (argList[i + 1][0] != '-')
						_argmap.insert(std::pair<std::string, std::string>(argList[i], argList[i + 1]) );
						else
						_argmap.insert(std::pair<std::string, std::string>(argList[i],  "") );
					}
					else
					{
						_argmap.insert(std::pair<std::string, std::string>(argList[i],  "") );
					}
				}
			}
		}

		template<typename Type>
		Type addOption(std::string optName, Type defaultValue, std::string description = "default description")
		{
			Type result;

			//if option is found

			if (_argmap.find(optName) != _argmap.end() )
			result = ConvertTo<Type>(_argmap.find(optName)->second);

			//if option not found, assign default value

			else
			result = defaultValue;


			description = description+" [default:"+ConvertToString(defaultValue)+"]";
			_descriptionmap.insert(std::pair<std::string, std::string>(optName, description) );

			return result;
		}

		// Needs to be added at the end of all addOption calls
		void CompileHelpFromOptions(std::string s="")
		{
			if(_displayHelp)
			{
				std::cout<<std::endl<<"Usage:"<<std::endl;
				std::cout<<_appName<<" [options]"<<std::endl;
				std::cout<<"Options:"<<std::endl;
				for (auto& opt: _descriptionmap)
				{
					std::cout <<"\t"<< opt.first << ":\t" << opt.second << '\n';
				}
				std::cout <<"\t--version:\tPrint the version number of "+_appName+" and exit.\n";

				std::cout << std::endl<<s<<std::endl;

				exit(EXIT_SUCCESS);

			}
		}

		void DisplayOption()
		{

			// std::cout<<std::endl<<_appName<<" version: "<<PROJECT_VERSION<<std::endl<<std::endl;

			exit(EXIT_SUCCESS);
		}


	};

}
