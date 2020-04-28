/*
 * Copyright 2019 <copyright holder> <email>
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 */

#ifndef CONFIG_H
#define CONFIG_H

#include "ini.h"
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>

typedef struct{
    std::string rtsp_addr;
    int cows;
    int rows;
    double square_size;
    std::string calib_file;
    double pixels;
    double shift_x;
    double shift_y;
}PARAMS;
    
class Config
{
public:
    Config();
    ~Config();
    bool Init(char* ini_path);
    std::string ClearHeadTailSpace(std::string str);
    
public:
    PARAMS params;
    
private:
    static int IniHandler(void *user, const char *section, const char *name, const char *value);
    static bool ParseBoolValue(const char *value);
};

#endif // CONFIG_H
