/*
Copyright 2013 Anton Hammerschmidt

This file is part of PowerPi.

PowerPi is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

PowerPi is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with PowerPi.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "tools.h"

std::string Tools::convertIntToStr(int Number)
{
    std::ostringstream ss;
    ss << Number;
    return ss.str();
}

int Tools::convertStrToInt(std::string str)
{
    std::stringstream ss(str);
    int i;
    ss >> i;
    return i;
}

std::vector<std::string> Tools::explode(std::string delimiter, std::string str)
{
    std::vector<std::string> arr;

    int strleng = str.length();
    int delleng = delimiter.length();
    if (delleng==0)
        return arr;//no change

    int i=0;
    int k=0;
    while( i<strleng )
    {
        int j=0;
        while (i+j<strleng && j<delleng && str[i+j]==delimiter[j])
            j++;
        if (j==delleng)//found delimiter
        {
            arr.push_back(  str.substr(k, i-k) );
            i+=delleng;
            k=i;
        }
        else
        {
            i++;
        }
    }
    arr.push_back(  str.substr(k, i-k) );
    return arr;
}
