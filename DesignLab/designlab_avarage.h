//! @file designlab_avarage.h
//! @brief 平均値を求める関数をまとめたもの．


#ifndef	DESIGNLAB_AVERAGE_H_
#define DESIGNLAB_AVERAGE_H_

#include <cmath>
#include <optional>
#include <vector>


namespace designlab 
{
	//! @class AverageCalculator
	//! @brief 平均値，分散，標準偏差を求めるテンプレートクラス．
	//! @tparam T 平均値，分散，標準偏差を求める型．
	template <typename T>
	class AverageCalculator final
	{
		// T は足し算・割り算ができることを前提としている．
		static_assert(std::is_arithmetic<T>::value, "T must be arithmetic type.");

	public:

		AverageCalculator() :
			data_(),
			sum_(std::nullopt),
			average_(std::nullopt),
			variance_(std::nullopt),
			standard_deviation_(std::nullopt)
		{
		}

		void AddData(const T& data)
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

			Calculate();
		}
		
		void Clear()
		{
			data_.clear();
			sum_ = std::nullopt;
			average_ = std::nullopt;
			variance_ = std::nullopt;
			standard_deviation_ = std::nullopt;
		}

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

		std::optional<T> GetAverage() const
		{
			return average_;
		}

		std::optional<T> GetVariance() const
		{
			return variance_;
		}

		std::optional<T> GetStandardDeviation() const
		{
			return standard_deviation_;
		}

		size_t GetDataNum() const
		{
			return data_.size();
		}

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
}

#endif	// DESIGNLAB_AVERAGE_H_