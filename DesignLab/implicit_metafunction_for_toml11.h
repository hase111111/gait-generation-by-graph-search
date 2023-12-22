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

//! @brief toml::into<T>()が存在するかどうかを判定するメタ関数．
//! @details toml::into<T>()が存在する場合には，こちらが呼ばれる．
//! false_typeを継承する．
template <typename T, typename = void>
struct has_into_toml : std::false_type {};

//! @brief toml::into<T>()が存在するかどうかを判定するメタ関数．
//! @details toml::into<T>()が存在しない場合には，こちらが呼ばれる．
//! true_typeを継承する．
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


//! @brief 入力ストリームが実装されているか判断するメタ関数．
//! @details 入力ストリームが実装されていなければ，こちらが呼ばれる．
//! false_typeを継承する．
template<typename T, typename U = void>
struct has_input_operator : public std::false_type {};

//! @brief 入力ストリームが実装されているか判断するメタ関数．
//! @details 入力ストリームが実装されていれば，こちらが呼ばれる．
//! true_typeを継承する．
template<typename T>
struct has_input_operator<T, decltype(std::declval<std::istream&>() >> std::declval<T&>(), std::declval<void>())> : public std::true_type {};

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


//! @brief vector型かどうかを判定するメタ関数．
//! @details vector型でなければ，こちらが呼ばれる．
//! false_typeを継承する．
template <typename T>
struct is_vector : std::false_type {};

//! @brief vector型かどうかを判定するメタ関数．
//! @details vector型ならば，こちらが呼ばれる．
//! true_typeを継承する．
template <typename T>
struct is_vector<std::vector<T>> : std::true_type {};


//! @brief toml11で使用可能な型かどうかを判定するメタ関数．toml11はvector型にも対応しているが，こちらでは除外する．
template <typename T>
struct is_toml11_available_type_not_vector_type
{
private:

	template <typename U, typename = decltype(toml::get<U>(std::declval<toml::value>()))>
	static std::true_type test(int);

	template <typename>
	static std::false_type test(...);

public:

	//! @brief toml11 で使用可能な型ならばtrue．toml11はvector型にも対応しているが，こちらでは除外する．
	static constexpr bool value = decltype(test<T>(0))::value && !is_vector<T>::value;

};

//! @brief toml11で使用可能な型かどうかを判定するメタ関数．vector型の場合はこちらを使用して判定する．
//! @details toml11で使用可能な型ならば，こちらが呼ばれる．
template <typename T>
struct is_toml11_available_type_vector_type : std::false_type {};

//! @brief toml11で使用可能な型かどうかを判定するメタ関数．vector型の場合はこちらを使用して判定する．
//! @details toml11で使用可能な型ならば，こちらが呼ばれる．
template <typename T>
struct is_toml11_available_type_vector_type<std::vector<T>> : is_toml11_available_type_not_vector_type<T> {};


//! @brief toml11で使用可能な型かどうかを判定するメタ関数．
template <typename T>
struct is_toml11_available_type : std::conditional_t<is_vector<T>::value, is_toml11_available_type_vector_type<T>, is_toml11_available_type_not_vector_type<T>> {};


//! @brief 入力ストリームが実装されているvector型かどうかを判定するメタ関数．
//! @details 入力ストリームが実装されていなければ，こちらが呼ばれる．
//! false_typeを継承する．
template <typename T>
struct is_vector_of_has_input_operator : std::false_type {};

//! @brief 入力ストリームが実装されているvector型かどうかを判定するメタ関数．
//! @details 入力ストリームが実装されていれば，こちらが呼ばれる．
//! true_typeを継承する．
template <typename T>
struct is_vector_of_has_input_operator<std::vector<T>> : has_input_operator<T> {};

//! @brief 出力ストリームが実装されているvector型かどうかを判定するメタ関数．
//! @details 出力ストリームが実装されていなければ，こちらが呼ばれる．
//! false_typeを継承する．
template <typename T>
struct is_vector_of_has_output_operator : std::false_type {};

//! @brief 出力ストリームが実装されているvector型かどうかを判定するメタ関数．
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

}	// namespace designlab::impl


#endif // DESIGNLAB_IMPLIMPLICIT_METAFUNCTION_H_