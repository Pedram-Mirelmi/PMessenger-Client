#include "./stringTools.hpp"

std::string toRaw(const std::string &str)
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
        case '\'':
            s << "\\'";
            break;
        default:
            s << chr;
            break;
        }
    return s.str();
}

