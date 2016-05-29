#ifndef __CJVXCOMMANDLINE_H__
#define __CJVXCOMMANDLINE_H__

#include <vector>
#include <string>

class CjvxCommandline
{
protected:

	struct
	{
		std::string configFilename;
		std::string changeWorkDir;
		std::vector<std::string> dllsDirectories;
	} _command_line_parameters;
public:

	CjvxCommandline(){};
	~CjvxCommandline(){};

	void reset_command_line()
	{
		_command_line_parameters.configFilename = "";
		_command_line_parameters.changeWorkDir = "";
		_command_line_parameters.dllsDirectories.clear();
	};

	void parse_command_line(char* argv[], int argc)
	{
		int cnt = 0;
		bool cont = true;

		while(cont && (cnt < argc))
		{
			std::string nextToken = argv[cnt++];
			std::string tmp;
			if(nextToken == "--config")
			{
				tmp = "";
				if(cnt < argc)
				{
					tmp = argv[cnt++];
				}
				_command_line_parameters.configFilename = tmp;
			}
			else if(nextToken == "--workdir")
			{
				if(cnt < argc)
				{
					_command_line_parameters.changeWorkDir = argv[cnt++];
				}
			}
			else if(nextToken == "--jvxdir")
			{
				tmp = "";
				if(cnt < argc)
				{
					tmp = argv[cnt++];
				}
				_command_line_parameters.dllsDirectories.push_back(tmp);
			}
		}
	};
};

#endif
