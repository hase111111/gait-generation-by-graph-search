#include "pch.h"

#include "../DesignLab/designlab_math.h"
#include "../DesignLab/designlab_math.cpp"


namespace dl_math_test
{
	TEST(Math, isEqualFloat)
	{
		//float型のオーバーロード
		EXPECT_TRUE(dl_math::isEqual(2.0f, 2.0f));
		EXPECT_TRUE(dl_math::isEqual(-9.0f, -9.0f));
		EXPECT_TRUE(dl_math::isEqual(71.0f, 71.0f));
		EXPECT_TRUE(dl_math::isEqual(-54.0f, -54.0f));
		EXPECT_TRUE(dl_math::isEqual(129.0f, 129.0f));
		EXPECT_TRUE(dl_math::isEqual(-628.0f, -628.0f));
		EXPECT_TRUE(dl_math::isEqual(4289.0f, 4289.0f));
		EXPECT_TRUE(dl_math::isEqual(-9125.0f, -9125.0f));

		EXPECT_FALSE(dl_math::isEqual(9.0f, 1.0f));
		EXPECT_FALSE(dl_math::isEqual(9.0f, -9.0f));
		EXPECT_FALSE(dl_math::isEqual(25.0f, 64.0f));
		EXPECT_FALSE(dl_math::isEqual(-91.0f, -26.0f));

		EXPECT_TRUE(dl_math::isEqual(5.0f, 5.0f + dl_math::ALLOWABLE_ERROR / 10.0f));
		EXPECT_TRUE(dl_math::isEqual(5.0f, 5.0f + dl_math::ALLOWABLE_ERROR * 5.0f / 10.0f));
		EXPECT_TRUE(dl_math::isEqual(5.0f, 5.0f + dl_math::ALLOWABLE_ERROR * 2.0f / 7.0f));
		EXPECT_TRUE(dl_math::isEqual(5.0f, 5.0f + dl_math::ALLOWABLE_ERROR * 9.0f / 10.0f));
		EXPECT_FALSE(dl_math::isEqual(5.0f, 5.0f + dl_math::ALLOWABLE_ERROR * 10.0f));
		EXPECT_FALSE(dl_math::isEqual(5.0f, 5.0f + dl_math::ALLOWABLE_ERROR * 100.0f));
		EXPECT_FALSE(dl_math::isEqual(5.0f, 5.0f + dl_math::ALLOWABLE_ERROR * 1000.0f));

		EXPECT_TRUE(dl_math::isEqual(5.0f, 5.0f - dl_math::ALLOWABLE_ERROR / 10.0f));
		EXPECT_TRUE(dl_math::isEqual(5.0f, 5.0f - dl_math::ALLOWABLE_ERROR * 2.0f / 10.0f));
		EXPECT_TRUE(dl_math::isEqual(5.0f, 5.0f - dl_math::ALLOWABLE_ERROR * 6.0f / 17.0f));
		EXPECT_TRUE(dl_math::isEqual(5.0f, 5.0f - dl_math::ALLOWABLE_ERROR * 9.0f / 10.0f));
		EXPECT_FALSE(dl_math::isEqual(5.0f, 5.0f - dl_math::ALLOWABLE_ERROR * 10.0f));
		EXPECT_FALSE(dl_math::isEqual(5.0f, 5.0f - dl_math::ALLOWABLE_ERROR * 100.0f));
		EXPECT_FALSE(dl_math::isEqual(5.0f, 5.0f - dl_math::ALLOWABLE_ERROR * 1000.0f));

		EXPECT_TRUE(dl_math::isEqual(100000.0f, 100000.0f));
		EXPECT_TRUE(dl_math::isEqual(5200.0f, 5200.0f));
		EXPECT_TRUE(dl_math::isEqual(62000.0f, 62000.0f));
	}

	TEST(Math, isEqualDouble)
	{
		//double型のオーバーロード
		EXPECT_TRUE(dl_math::isEqual(2.0, 2.0));
		EXPECT_TRUE(dl_math::isEqual(-9.0, -9.0));
		EXPECT_TRUE(dl_math::isEqual(71.0, 71.0));
		EXPECT_TRUE(dl_math::isEqual(-54.0, -54.0));
		EXPECT_TRUE(dl_math::isEqual(129.0, 129.0));
		EXPECT_TRUE(dl_math::isEqual(-628.0, -628.0));
		EXPECT_TRUE(dl_math::isEqual(4289.0, 4289.0));
		EXPECT_TRUE(dl_math::isEqual(-9125.0, -9125.0));

		EXPECT_FALSE(dl_math::isEqual(9.0, 1.0));
		EXPECT_FALSE(dl_math::isEqual(9.0, -9.0));
		EXPECT_FALSE(dl_math::isEqual(25.0, 64.0));
		EXPECT_FALSE(dl_math::isEqual(-91.0, -26.0));

		EXPECT_TRUE(dl_math::isEqual(5.0, 5.0 + dl_math::kDoubleAllowableError / 10.0));
		EXPECT_TRUE(dl_math::isEqual(5.0, 5.0 + dl_math::kDoubleAllowableError * 5.0 / 10.0));
		EXPECT_TRUE(dl_math::isEqual(5.0, 5.0 + dl_math::kDoubleAllowableError * 2.0 / 7.0));
		EXPECT_TRUE(dl_math::isEqual(5.0, 5.0 + dl_math::kDoubleAllowableError * 9.0 / 10.0));
		EXPECT_FALSE(dl_math::isEqual(5.0, 5.0 + dl_math::kDoubleAllowableError * 10.0));
		EXPECT_FALSE(dl_math::isEqual(5.0, 5.0 + dl_math::kDoubleAllowableError * 100.0));
		EXPECT_FALSE(dl_math::isEqual(5.0, 5.0 + dl_math::kDoubleAllowableError * 1000.0));

		EXPECT_TRUE(dl_math::isEqual(5.0, 5.0 - dl_math::kDoubleAllowableError / 10.0));
		EXPECT_TRUE(dl_math::isEqual(5.0, 5.0 - dl_math::kDoubleAllowableError * 2.0 / 10.0));
		EXPECT_TRUE(dl_math::isEqual(5.0, 5.0 - dl_math::kDoubleAllowableError * 6.0 / 17.0));
		EXPECT_TRUE(dl_math::isEqual(5.0, 5.0 - dl_math::kDoubleAllowableError * 9.0 / 10.0));
		EXPECT_FALSE(dl_math::isEqual(5.0, 5.0 - dl_math::kDoubleAllowableError * 10.0));
		EXPECT_FALSE(dl_math::isEqual(5.0, 5.0 - dl_math::kDoubleAllowableError * 100.0));
		EXPECT_FALSE(dl_math::isEqual(5.0, 5.0 - dl_math::kDoubleAllowableError * 1000.0));

		EXPECT_TRUE(dl_math::isEqual(100000.0, 100000.0));
		EXPECT_TRUE(dl_math::isEqual(5200.0, 5200.0));
		EXPECT_TRUE(dl_math::isEqual(62000.0, 62000.0));
	}

	TEST(Math, Squared)
	{
		int int_num = 5;
		EXPECT_EQ(dl_math::squared(int_num), int_num * int_num);

		EXPECT_EQ(dl_math::squared(-13), 169);
		EXPECT_EQ(dl_math::squared(4), 16);
		EXPECT_EQ(dl_math::squared(0), 0);

		EXPECT_TRUE(dl_math::isEqual(dl_math::squared(4.8), 23.04));
		EXPECT_TRUE(dl_math::isEqual(dl_math::squared(125.2), 15675.04));
		EXPECT_TRUE(dl_math::isEqual(dl_math::squared(0.1), 0.01));

		EXPECT_TRUE(dl_math::isEqual(dl_math::squared(4.8f), 23.04f));
		EXPECT_TRUE(dl_math::isEqual(dl_math::squared(125.2f), 15675.04f));
		EXPECT_TRUE(dl_math::isEqual(dl_math::squared(0.1f), 0.01f));


		char _char_num = 8;
		char _char_res = 64;
		EXPECT_EQ(dl_math::squared(_char_num), _char_res);

		_char_num = -7;
		_char_res = 49;
		EXPECT_EQ(dl_math::squared(_char_num), _char_res);


		short _short_num = -25;
		short _short_res = 625;
		EXPECT_EQ(dl_math::squared(_short_num), _short_res);

		_short_num = 11;
		_short_res = 121;
		EXPECT_EQ(dl_math::squared(_short_num), _short_res);
	}

}
