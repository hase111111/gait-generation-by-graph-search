//! @file designlab_impl.h
//! @brief 型判別用のメタ関数．

#ifndef DESIGNLAB_IMPL_H_
#define DESIGNLAB_IMPL_H_

#include <iostream>
#include <type_traits>


//! @namespace designlab::impl
//! @brief 明示的に使用することのない関数をまとめた名前空間．
namespace designlab::impl
{

//! @brief 入力ストリームが存在するか調べるメタ関数の宣言．
template <typename T>
struct has_input_operator final
{
private:

	template <typename U, typename = decltype(std::declval<std::istream&>() >> std::declval<U>())>
	static std::true_type test(int);

	template <typename>
	static std::false_type test(...);

public:

	static constexpr bool value = decltype(test<T>(0))::value;
};


//! @brief 出力ストリームが存在するか調べるメタ関数の宣言．
template <typename T>
struct has_output_operator final
{
private:

	template <typename U, typename = decltype(std::declval<std::ostream&>() << std::declval<U>())>
	static std::true_type test(int);

	template <typename>
	static std::false_type test(...);

public:

	static constexpr bool value = decltype(test<T>(0))::value;
};


//! @breif toml11 で使用可能な型かどうかを判定するメタ関数．
template <typename T>
struct is_toml11_available_type final
{
private:

	template <typename U, typename = decltype(toml::get<U>(std::declval<toml::value>()))>
	static std::true_type test(int);

	template <typename>
	static std::false_type test(...);

public:
	// メタ関数本体
	static constexpr bool value = decltype(test<T>(0))::value;

};

}	// namespace designlab::impl


#endif // !DESIGNLAB_IMPL_H_