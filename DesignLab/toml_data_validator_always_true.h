
//! @file      toml_data_validator_always_true.h
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory, Saitama University All right reserved.

#ifndef DESIGNLAB_TOML_DATA_VALIDATOR_ALWAYS_TRUE_H_
#define DESIGNLAB_TOML_DATA_VALIDATOR_ALWAYS_TRUE_H_

#include <string>
#include <tuple>

#include "cmdio_util.h"
#include "interface_toml_data_validator.h"


namespace designlab
{

//! @class TomlDataValidatorAlwaysTrue
//! @brief 常に trueを返す ITomlDataValidator の実装クラス．
//! @tparam T 検証対象の型．
template <typename T>
class TomlDataValidatorAlwaysTrue final : public ITomlDataValidator<T>
{
public:
    std::tuple<bool, std::string> Validate([[maybe_unused]] const T& toml_data) const override
    {
        CmdIOUtil::InfoOutput("(No validation is performed in the current configuration.)");
        return { true, "" };
    }
};

}  // namespace designlab


#endif  // DESIGNLAB_TOML_DATA_VALIDATOR_ALWAYS_TRUE_H_
