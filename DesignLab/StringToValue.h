#pragma once

#include <string>


namespace StrtoVal
{
	//! @brief •¶š—ñ‚ğintŒ^‚Ì•Ï”‚É•ÏŠ·‚·‚é
	//! @param [in] _str •ÏŠ·‘O‚Ì•¶š—ñ
	//! @return •ÏŠ·Œã‚ÌintŒ^‚Ì•Ï”
	//! @note •ÏŠ·‚É¸”s‚µ‚½ê‡‚Í0‚ğ•Ô‚·
	inline int StrToInt(const std::string& _str)
	{
		int res = 0;

		try
		{
			res = std::stoi(_str);
			return res;
		}
		catch (...)
		{
			return 0;
		}
	}
}