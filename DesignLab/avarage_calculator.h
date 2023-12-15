//! @file designlab_avarage.h
//! @brief 平均値，分散，標準偏差を求めるテンプレートクラス．

#ifndef	DESIGNLAB_AVERAGE_CALCULATOR_H_
#define DESIGNLAB_AVERAGE_CALCULATOR_H_

#include <cmath>
#include <optional>
#include <vector>


namespace designlab
{

//! @class AverageCalculator
//! @brief 平均値，分散，標準偏差を求めるテンプレートクラス．
//! @tparam T 平均値，分散，標準偏差を求める型．
template <typename T, typename = std::enable_if_t<std::is_arithmetic<T>::value>>
class AverageCalculator final
{
public:

	AverageCalculator() :
		data_(),
		sum_(std::nullopt),
		average_(std::nullopt),
		variance_(std::nullopt),
		standard_deviation_(std::nullopt)
	{
	}

	//! @brief データを追加する．
	//! @n この関数を呼び出すたびに，平均値，分散，標準偏差が再計算される．
	//! @param[in] data 追加するデータ．
	//! @param[in] do_calc 平均値，分散，標準偏差を再計算するかどうか．デフォルトでは再計算する．
	void AddData(const T& data, const bool do_calc = true)
	{
		data_.push_back(data);

		if (sum_.has_value())
		{
			sum_ = sum_.value() + data;
		}
		else
		{
			sum_ = data;
		}

		if (do_calc)
		{
			Calculate();
		}
	}

	//! @brief データをクリアする．
	//! @note 平均値，分散，標準偏差はnulloptになる．
	void Clear()
	{
		data_.clear();
		sum_ = std::nullopt;
		average_ = std::nullopt;
		variance_ = std::nullopt;
		standard_deviation_ = std::nullopt;
	}

	//! @brief 平均値，分散，標準偏差を計算する．
	void Calculate()
	{
		//データが空の場合は，平均値，分散，標準偏差をnulloptにする．
		if (data_.empty())
		{
			average_ = std::nullopt;
			variance_ = std::nullopt;
			standard_deviation_ = std::nullopt;
			return;
		}

		// 平均値を求める
		average_ = sum_.value() / data_.size();

		// 分散を求める
		T variance = 0;
		for (const auto& data : data_)
		{
			variance += (data - average_.value()) * (data - average_.value());
		}
		variance_ = variance / data_.size();

		// 標準偏差を求める
		standard_deviation_ = sqrt(variance_.value());
	}

	//! @brief 平均値を取得する．
	//! @return std::optional<T> 平均値．データが空の場合はnullopt．
	std::optional<T> GetAverage() const
	{
		return average_;
	}

	//! @brief 分散を取得する．
	//! @return std::optional<T> 分散．データが空の場合はnullopt．
	std::optional<T> GetVariance() const
	{
		return variance_;
	}

	//! @brief 標準偏差を取得する．
	//! @return std::optional<T> 標準偏差．データが空の場合はnullopt．
	std::optional<T> GetStandardDeviation() const
	{
		return standard_deviation_;
	}

	//! @brief データ数を取得する．
	//! @return size_t データ数．
	size_t GetDataNum() const
	{
		return data_.size();
	}

	//! @brief データの合計を取得する．
	//! @return std::optional<T> データの合計．データが空の場合はnullopt．
	std::optional<T> GetSum() const
	{
		return sum_;
	}

private:
	std::vector<T> data_;
	std::optional<T> sum_;
	std::optional<T> average_;
	std::optional<T> variance_;
	std::optional<T> standard_deviation_;
};

}	// namespace designlab


#endif	// DESIGNLAB_AVERAGE_CALCULATOR_H_