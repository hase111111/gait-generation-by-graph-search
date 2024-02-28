
//! @file      interface_toml_data_validator.h
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory, Saitama University All right reserved.

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
    //! @return 検証結果．1つ目の要素は検証結果，2つ目の要素は検証失敗の理由．
    virtual std::tuple<bool, std::string> Validate(const T& toml_data) const = 0;
};

}  // namespace designlab


#endif  // DESIGNLAB_INTERFACE_TOML_DATA_VALIDATOR_H_
