#include <getopt.h>
#include <sstream>
#include "config.h"

Config::Config()
{

}

Config::~Config()
{

}

bool Config::Init(char* ini_path)
{
    if(ini_parse(ini_path, &Config::IniHandler, (void*)&params) < 0)
        return false;
    return true;
}

int Config::IniHandler(void *user, const char *section, const char *name, const char *value)
{
    PARAMS* handle_params = (PARAMS*)(user);

    if(strcmp(section, "SYSTEM") == 0)
    {
		if(strcmp(name, "RTSP_ADDR") == 0){
		    handle_params->rtsp_addr = value;
		}
		else if(strcmp(name, "COWS") == 0)
		    handle_params->cows = atoi(value);
		else if(strcmp(name, "ROWS") == 0)
			handle_params->rows = atoi(value);
		else if(strcmp(name, "PIXELS") == 0)
		    handle_params->pixels = (double)atof(value);
		else if(strcmp(name, "SHIFT_X") == 0)
		    handle_params->shift_x = (double)atof(value);
		else if(strcmp(name, "SHIFT_Y") == 0)
		    handle_params->shift_y = (double)atof(value);
		else
		    return 0;
    } 

    return 1;
}

bool Config::ParseBoolValue(const char *value)
{
    if(!value || value[0] == 'f' || value[0] == 'F' || value[0] == '0')
        return false;
    return true;
}

/**
 * @brief: clear head space and tail space of string
 * @return: string
**/

std::string Config::ClearHeadTailSpace(std::string str)
{
    if (str.empty())
    {
        return str;
    }
 
    str.erase(0,str.find_first_not_of(" "));
    str.erase(str.find_last_not_of(" ") + 1);
    
    return str;
}
