
#include "string_util.h"

#include "cassert_define.h"


namespace designlab::string_util
{

std::vector<std::string> Split(const std::string& str, const std::string& separator)
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

        while (true)
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

}  // namespace designlab::string_util
