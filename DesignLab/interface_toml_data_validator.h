#ifndef DESIGNLAB_INTERFACE_TOML_DATA_VALIDATOR_H_
#define DESIGNLAB_INTERFACE_TOML_DATA_VALIDATOR_H_

#include <string>
#include <tuple>


namespace designlab
{

//! @class ITomlDataValidator
//! @brief TOMLファイルのデータの検証を行う処理のインターフェース．
template <typename T>
class ITomlDataValidator
{
public:
	virtual ~ITomlDataValidator() = default;

	//! @brief TOMLファイルのデータの検証を行う．
	//! @param[in] toml_data TOMLファイルのデータ．
	//! @return std::tuple<bool, std::string> 検証結果．1つ目の要素は検証結果，2つ目の要素は検証結果の詳細．
	virtual std::tuple<bool, std::string> Validate(const T& toml_data) const = 0;
};

}	// namespace designlab


#endif	//DESIGNLAB_INTERFACE_TOML_DATA_VALIDATOR_H_