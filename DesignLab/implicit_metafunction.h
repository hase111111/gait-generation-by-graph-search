#ifndef DESIGNLAB_IMPLIMPLICIT_METAFUNCTION_H_
#define DESIGNLAB_IMPLIMPLICIT_METAFUNCTION_H_

#include <iostream>
#include <type_traits>
#include <vector>

#include "toml11_define.h"


//! @namespace designlab::impl
//! @brief 明示的に使用することのない関数をまとめた名前空間．
namespace designlab::impl
{

template <typename T, typename = void>
struct has_into_toml : std::false_type {};

template <typename T>
struct has_into_toml<T, std::void_t<decltype(toml::into<T>())> > : std::true_type {};

//! @brief toml::from<T>()が存在するかどうかを判定するメタ関数．
//! @details toml::from<T>()が存在する場合には，こちらが呼ばれる．
//! false_typeを継承する．
template <typename T, typename = void>
struct has_from_toml : std::false_type {};

//! @brief toml::from<T>()が存在するかどうかを判定するメタ関数．
//! @details toml::from<T>()が存在しない場合には，こちらが呼ばれる．
//! true_typeを継承する．
template <typename T>
struct has_from_toml<T, std::void_t<decltype(toml::from<T>())> > : std::true_type {};


//入力ストリームに対応しているかどうかを判断するメタ関数
template<typename T, typename U = void>
struct has_input_operator : public std::false_type {};
template<typename T>
struct has_input_operator<T, decltype(std::declval<std::istream&>() >> std::declval<T&>(), std::declval<void>())> : public std::true_type {};

//出力ストリームに対応しているかどうかを判断するメタ関数
template<typename T, typename U = void>
struct has_output_operator : public std::false_type {};
template<typename T>
struct has_output_operator<T, decltype(std::declval<std::ostream&>() << std::declval<T&>(), std::declval<void>())> : public std::true_type {};


//! @brief vector型かどうかを判定するメタ関数．
template <typename T>
struct is_vector : std::false_type {};

//! @brief vector型かどうかを判定するメタ関数．
template <typename T>
struct is_vector<std::vector<T>> : std::true_type {};


//! @breif toml11 で使用可能な型かどうかを判定するメタ関数．
template <typename T>
struct is_toml11_available_type_not_vector_type
{
private:

	template <typename U, typename = decltype(toml::get<U>(std::declval<toml::value>()))>
	static std::true_type test(int);

	template <typename>
	static std::false_type test(...);

public:

	//! @brief toml11 で使用可能な型ならばtrue．
	static constexpr bool value = decltype(test<T>(0))::value && !is_vector<T>::value;

};


template <typename T>
struct is_toml11_available_type_vector_type : std::false_type {};

template <typename T>
struct is_toml11_available_type_vector_type<std::vector<T>> : is_toml11_available_type_not_vector_type<T> {};



template <typename T>
struct is_toml11_available_type : std::conditional_t<is_vector<T>::value, is_toml11_available_type_vector_type<T>, is_toml11_available_type_not_vector_type<T>> {};


template <typename T>
struct is_vector_of_has_input_operator : std::false_type {};

template <typename T>
struct is_vector_of_has_input_operator<std::vector<T>> : has_input_operator<T> {};

template <typename T>
struct is_vector_of_has_output_operator : std::false_type {};

template <typename T>
struct is_vector_of_has_output_operator<std::vector<T>> : has_output_operator<T> {};


template <typename T>
struct is_vector_of_enum : std::false_type {};

template <typename T>
struct is_vector_of_enum<std::vector<T>> : std::is_enum<T> {};

}	// namespace designlab::impl


#endif // DESIGNLAB_IMPLIMPLICIT_METAFUNCTION_H_