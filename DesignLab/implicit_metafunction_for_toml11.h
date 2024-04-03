
//! @file      implicit_metafunction_for_toml11.h
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory, Saitama University All right reserved.

#ifndef DESIGNLAB_IMPLICIT_METAFUNCTION_FOR_TOML11_H_
#define DESIGNLAB_IMPLICIT_METAFUNCTION_FOR_TOML11_H_

#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

#include "toml11_define.h"


//! @namespace designlab::impl
//! @brief 明示的に使用することのない関数をまとめた名前空間．
namespace designlab::impl
{

//! @brief toml::into<T>()が存在するかどうかを判定するメタ関数．
//! @details toml::into<T>()が存在する場合には，こちらが呼ばれる．
//! false_typeを継承する．
template <typename T, typename = void>
struct has_into_toml : std::false_type {};

//! @brief toml::into<T>()が存在するかどうかを判定するメタ関数．
//! @details toml::into<T>()が存在しない場合には，こちらが呼ばれる．
//! true_typeを継承する．
template <typename T>
struct has_into_toml<T, std::void_t<decltype(toml::into<T>())> > :
    std::true_type {};

//! @brief toml::from<T>()が存在するかどうかを判定するメタ関数．
//! @details toml::from<T>()が存在する場合には，こちらが呼ばれる．
//! false_typeを継承する．
template <typename T, typename = void>
struct has_from_toml : std::false_type {};

//! @brief toml::from<T>()が存在するかどうかを判定するメタ関数．
//! @details toml::from<T>()が存在しない場合には，こちらが呼ばれる．
//! true_typeを継承する．
template <typename T>
struct has_from_toml<T, std::void_t<decltype(toml::from<T>())> > :
    std::true_type {};


//! @brief 入力ストリームが実装されているか判断するメタ関数．
//! @details 入力ストリームが実装されていなければ，こちらが呼ばれる．
//! false_typeを継承する．
template<typename T, typename U = void>
struct has_input_operator : public std::false_type {};

//! @brief 入力ストリームが実装されているか判断するメタ関数．
//! @details 入力ストリームが実装されていれば，こちらが呼ばれる．
//! true_typeを継承する．
template<typename T>
struct has_input_operator<T,
    decltype(std::declval<std::istream&>() >> std::declval<T&>(),
    std::declval<void>())> :
    public std::true_type {};

//! @brief 出力ストリームが実装されているか判断するメタ関数．
//! @details 出力ストリームが実装されていなければ，こちらが呼ばれる．
//! false_typeを継承する．
template<typename T, typename U = void>
struct has_output_operator : public std::false_type {};

//! @brief 出力ストリームが実装されているか判断するメタ関数．
//! @details 出力ストリームが実装されていれば，こちらが呼ばれる．
//! true_typeを継承する．
template<typename T>
struct has_output_operator<T, decltype(std::declval<std::ostream&>() << std::declval<T&>(), std::declval<void>())> : public std::true_type {};

static_assert(has_input_operator<int>::value == true, "int型は入力オペレーターに対応している．");
static_assert(has_input_operator<int*>::value == false, "int*型は入力オペレーターに対応していない．");
static_assert(has_input_operator<std::vector<int>>::value == false, "std::vector<int>型は入力オペレーターに対応していない．");

static_assert(has_output_operator<int>::value == true, "int型は出力オペレーターに対応している．");
static_assert(has_output_operator<int*>::value == true, "int*型は出力オペレーターに対応している．");
static_assert(has_output_operator<std::vector<int>>::value == false, "std::vector<int>型は出力オペレーターに対応していない．");


//! @brief vector型かどうかを判定するメタ関数．
//! @details vector型でなければ，こちらが呼ばれる． false_typeを継承する．
template <typename T>
struct is_vector : std::false_type {};

//! @brief vector型かどうかを判定するメタ関数．
//! @details vector型ならば，こちらが呼ばれる．
//! true_typeを継承する．
template <typename T>
struct is_vector<std::vector<T>> : std::true_type {};

static_assert(is_vector<int>::value == false, "An int type is not a vector type.");
static_assert(is_vector<std::string>::value == false, "std::string type is not a vector type.");

static_assert(is_vector<std::vector<int>>::value == true, "The type std::vector<int> is a vector type.");
static_assert(is_vector<std::vector<std::string>>::value == true, "The type std::vector<std::string> is a vector type.");


//! @brief toml11で使用可能な型かどうかを判定するメタ関数．
//! toml11は vector型にも対応しているが，こちらでは除外する．
template <typename T>
struct is_toml11_available_type_not_vector_type
{
private:
    template <typename U, typename = decltype(toml::get<U>(std::declval<toml::value>()))>
    static std::true_type test(int);

    template <typename>
    static std::false_type test(...);

public:
    //! @brief toml11 で使用可能な型ならば true．
    //! toml11は vector型にも対応しているが，こちらでは除外する．
    static constexpr bool value = decltype(test<T>(0))::value && !is_vector<T>::value;
};

//! @brief toml11で使用可能な型かどうかを判定するメタ関数．
//! vector型の場合はこちらを使用して判定する．
//! @details toml11で使用可能な型ならば，こちらが呼ばれる．
template <typename T>
struct is_toml11_available_type_vector_type : std::false_type {};

//! @brief toml11で使用可能な型かどうかを判定するメタ関数．
//! vector型の場合はこちらを使用して判定する．
//! @details toml11で使用可能な型ならば，こちらが呼ばれる．
template <typename T>
struct is_toml11_available_type_vector_type<std::vector<T>> : is_toml11_available_type_not_vector_type<T> {};

static_assert(is_toml11_available_type_not_vector_type<int>::value == true, "The type int is a type that can be used in toml11.");
static_assert(is_toml11_available_type_not_vector_type<int*>::value == false, "The type int* is not a usable type in toml11.");
static_assert(is_toml11_available_type_not_vector_type<std::vector<int>>::value == false, "The type std::vector<int> is a std::vector type.");

static_assert(is_toml11_available_type_vector_type<int>::value == false, "int type is not a std::vector type.");
static_assert(is_toml11_available_type_vector_type<int*>::value == false, "int* type is not a std::vector type.");
static_assert(is_toml11_available_type_vector_type<std::vector<int>>::value == true, "The type std::vector<int> is a type of std::vector and can used in toml11.");
static_assert(is_toml11_available_type_vector_type<std::vector<int*>>::value == false, "type std::vector<*int> is a std::vector type, but it cannot use toml11.");


//! @brief toml11で使用可能な型かどうかを判定するメタ関数．
template <typename T>
struct is_toml11_available_type :
    std::conditional_t<is_vector<T>::value,
    is_toml11_available_type_vector_type<T>,
    is_toml11_available_type_not_vector_type<T>> {};

static_assert(
    is_toml11_available_type<int>::value == true,
    "Int is a type that can be used in toml11.");
static_assert(
    is_toml11_available_type<int*>::value == false,
    "The type int* is not a usable type in toml11.");
static_assert(
    is_toml11_available_type<std::vector<int>>::value == true,
    "The type std::vector<int> is a type of std::vector and can be used in toml11.");
static_assert(
    is_toml11_available_type<std::vector<int*>>::value == false,
    "type std::vector<*int> is a std::vector type, but it is not a usable type.");


//! @brief 入力ストリームが実装されている vector 型かどうかを判定するメタ関数．
//! @details 入力ストリームが実装されていなければ，こちらが呼ばれる．
//! false_typeを継承する．
template <typename T>
struct is_vector_of_has_input_operator : std::false_type {};

//! @brief 入力ストリームが実装されている vector 型かどうかを判定するメタ関数．
//! @details 入力ストリームが実装されていれば，こちらが呼ばれる．
//! true_typeを継承する．
template <typename T>
struct is_vector_of_has_input_operator<std::vector<T>> : has_input_operator<T> {};

//! @brief 出力ストリームが実装されている vector 型かどうかを判定するメタ関数．
//! @details 出力ストリームが実装されていなければ，こちらが呼ばれる．
//! false_typeを継承する．
template <typename T>
struct is_vector_of_has_output_operator : std::false_type {};

//! @brief 出力ストリームが実装されている vector 型かどうかを判定するメタ関数．
//! @details 出力ストリームが実装されていれば，こちらが呼ばれる．
//! true_typeを継承する．
template <typename T>
struct is_vector_of_has_output_operator<std::vector<T>> : has_output_operator<T> {};


//! @brief vector<enum> 型かどうかを判定するメタ関数．
//! @details vector<enum>型でなければ，こちらが呼ばれる．
//! false_typeを継承する．
template <typename T>
struct is_vector_of_enum : std::false_type {};

//! @brief vector<enum> 型かどうかを判定するメタ関数．
//! @details vector<enum>型ならば，こちらが呼ばれる．
template <typename T>
struct is_vector_of_enum<std::vector<T>> : std::is_enum<T> {};

}  // namespace designlab::impl


#endif  // DESIGNLAB_IMPLICIT_METAFUNCTION_FOR_TOML11_H_
