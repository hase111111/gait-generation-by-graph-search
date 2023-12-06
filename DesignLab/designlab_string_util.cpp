#include "designlab_string_util.h"

#include "cassert_define.h"


//éQçl 
// https://marycore.jp/prog/cpp/std-string-split/
// https://marycore.jp/prog/cpp/std-string-find-search/#find%EF%BC%8Frfind

std::vector<std::string> designlab::string_util::Split(const std::string& str, const char delim)
{
    return Split(str, std::string(1, delim));
}

std::vector<std::string> designlab::string_util::Split(const std::string& str, const std::string& separator)
{
    std::vector<std::string> list;
    const size_t separator_length = separator.length();

    if (separator_length == 0) 
    {
        list.push_back(str);
    }
    else 
    {
        size_t offset = 0;

        while (1) 
        {
            const size_t pos = str.find(separator, offset);

            if (pos == std::string::npos) 
            {
                list.push_back(str.substr(offset));
                break;
            }

            list.push_back(str.substr(offset, pos - offset));
            offset = pos + separator_length;

            if (offset >= str.length())
            {
				break;
			}
        }
    }

    return list;
}
