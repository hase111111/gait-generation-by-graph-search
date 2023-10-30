#include "designlab_string_util.h"

#include "cassert_define.h"


std::vector<std::string> designlab::string_util::Split(const std::string& str, const char delim)
{
    return Split(str, std::string(1, delim));
}

std::vector<std::string> designlab::string_util::Split(const std::string& str, const std::string& delim)
{
	assert(delim.size() == 1);	// ‹æØ‚è•¶š‚Í1•¶š‚Ì‚İ

	std::vector<std::string> ans;

	int first = 0;
	int last = static_cast<int>(str.find_first_of(delim));

	while (first < str.size())
	{
		std::string subStr(str, first, last - first);

		ans.push_back(subStr);

		first = last + 1;
		last = static_cast<int>(str.find_first_of(delim, first));

		if (last == std::string::npos)
		{
			last = static_cast<int>(str.size());
		}
	}
    return ans;
}
