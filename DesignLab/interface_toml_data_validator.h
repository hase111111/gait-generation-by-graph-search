//! @file interface_toml_data_validator.h
//! @brief TOMLファイルのデータの検証を行うインターフェース．

#ifndef DESIGNLAB_INTERFACE_TOML_DATA_VALIDATOR_H_
#define DESIGNLAB_INTERFACE_TOML_DATA_VALIDATOR_H_

#include <string>
#include <tuple>


template <typename T>
class ITomlDataValidator
{
public:
	virtual ~ITomlDataValidator() = default;

	virtual std::tuple<bool, std::string> Validate(const T& toml_data) const = 0;
};


#endif	//DESIGNLAB_INTERFACE_TOML_DATA_VALIDATOR_H_