#pragma once

#include <memory>
#include <string>
#include <stdexcept>
#include <sstream>

std::string toRaw(const std::string& str)
{
    std::stringstream s;
    for (const char& chr : str)
        switch (chr)
        {
            case '\t':
                s << "\\t";
                break;
            case '\n':
                s << "\\n";
                break;
            default:
                s << chr;
        }
    return s.str();
}

