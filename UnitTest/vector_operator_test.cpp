#include "pch.h"

#include "../DesignLab/designlab_vector.h"


namespace dl_vec_test
{
	TEST(SVector, EqualityOperator)
	{
		//ìôâøââéZéqÇÃÉeÉXÉg

		EXPECT_TRUE(dl_vec::SVector(10, -5, 0) == dl_vec::SVector(10, -5, 0));
		EXPECT_TRUE(dl_vec::SVector(-20, 60, 10) == dl_vec::SVector(-20, 60, 10));
		EXPECT_TRUE(dl_vec::SVector(0.4f, 6.6f, -7.8f) == dl_vec::SVector(0.4f, 6.6f, -7.8f));
		EXPECT_TRUE(dl_vec::SVector(100000, -5200, 62000) == dl_vec::SVector(100000, -5200, 62000));

		EXPECT_FALSE(dl_vec::SVector(10, -5, 0) != dl_vec::SVector(10, -5, 0));
		EXPECT_FALSE(dl_vec::SVector(-20, 60, 10) != dl_vec::SVector(-20, 60, 10));
		EXPECT_FALSE(dl_vec::SVector(0.4f, 6.6f, -7.8f) != dl_vec::SVector(0.4f, 6.6f, -7.8f));
		EXPECT_FALSE(dl_vec::SVector(100000, -5200, 62000) != dl_vec::SVector(100000, -5200, 62000));


		EXPECT_FALSE(dl_vec::SVector(4, 8, -9) == dl_vec::SVector(-9, 3, 1));
		EXPECT_FALSE(dl_vec::SVector(10, -20, 53) == dl_vec::SVector(-34, 31, -95));
		EXPECT_FALSE(dl_vec::SVector(0.7f, -4.1f, 6.7f) == dl_vec::SVector(5.2f, 6.8f, 9.1f));
		EXPECT_FALSE(dl_vec::SVector(2500, 71200, -91200) == dl_vec::SVector(-9900, 250000, 23400));

		EXPECT_TRUE(dl_vec::SVector(4, 8, -9) != dl_vec::SVector(-9, 3, 1));
		EXPECT_TRUE(dl_vec::SVector(10, -20, 53) != dl_vec::SVector(-34, 31, -95));
		EXPECT_TRUE(dl_vec::SVector(0.7f, -4.1f, 6.7f) != dl_vec::SVector(5.2f, 6.8f, 9.1f));
		EXPECT_TRUE(dl_vec::SVector(2500, 71200, -91200) != dl_vec::SVector(-9900, 250000, 23400));


		EXPECT_TRUE(dl_vec::SVector(10, -5, 0) != dl_vec::SVector(13, -5, 0));
		EXPECT_TRUE(dl_vec::SVector(10, -5, 0) != dl_vec::SVector(10, 55, 0));
		EXPECT_TRUE(dl_vec::SVector(10, -5, 0) != dl_vec::SVector(10, -5, 8));
	}

	TEST(SVector, ComparisonOperator)
	{
		//î‰ärââéZéqÇÃÉeÉXÉgÅDî‰ärââéZéqÇÕÉxÉNÉgÉãÇÃí∑Ç≥ÇópÇ¢Çƒî‰ärÇçsÇ§ÅD

		EXPECT_TRUE(dl_vec::SVector(0, 0, 0) < dl_vec::SVector(5, 10, -6));
		EXPECT_TRUE(dl_vec::SVector(2, -6, 3) < dl_vec::SVector(56, 20, -94));
		EXPECT_TRUE(dl_vec::SVector(5, 10, -6) > dl_vec::SVector(0, 0, 0));
		EXPECT_TRUE(dl_vec::SVector(56, 20, -94) > dl_vec::SVector(2, -6, 3));

		EXPECT_TRUE(dl_vec::SVector(0, 0, 0) <= dl_vec::SVector(5, 10, -6));
		EXPECT_TRUE(dl_vec::SVector(2, -6, 3) <= dl_vec::SVector(56, 20, -94));
		EXPECT_TRUE(dl_vec::SVector(5, 10, -6) >= dl_vec::SVector(0, 0, 0));
		EXPECT_TRUE(dl_vec::SVector(56, 20, -94) >= dl_vec::SVector(2, -6, 3));

		EXPECT_TRUE(dl_vec::SVector(-23, 85, 91) <= dl_vec::SVector(-23, 85, 91));
		EXPECT_TRUE(dl_vec::SVector(-23, 85, 91) >= dl_vec::SVector(-23, 85, 91));
	}

	TEST(SVector, PlusOperator)
	{
		//ë´ÇµéZ
		EXPECT_EQ(dl_vec::SVector(2, 2, 2), dl_vec::SVector(1, 1, 0) + dl_vec::SVector(1, 1, 2));
		EXPECT_EQ(dl_vec::SVector(10, 0, 0), dl_vec::SVector(5, 0, 0) + dl_vec::SVector(5, 0, 0));
		EXPECT_EQ(dl_vec::SVector(0, 10, 0), dl_vec::SVector(0, 3, 0) + dl_vec::SVector(0, 7, 0));
		EXPECT_EQ(dl_vec::SVector(0, 0, 10), dl_vec::SVector(0, 0, 6) + dl_vec::SVector(0, 0, 4));
		EXPECT_EQ(dl_vec::SVector(100, 100, 100), dl_vec::SVector(60, 120, 200) + dl_vec::SVector(40, -20, -100));
		EXPECT_EQ(dl_vec::SVector(2.8f, 21.6f, 12.89f), dl_vec::SVector(0.2f, 34.8f, 6.78f) + dl_vec::SVector(2.6f, -13.2f, 6.11f));
	}

	TEST(SVector, MinusOperator)
	{
		//à¯Ç´éZ
		EXPECT_EQ(dl_vec::SVector(10, 6, 10), dl_vec::SVector(15, 66, 1010) - dl_vec::SVector(5, 60, 1000));
		EXPECT_EQ(dl_vec::SVector(-5, 150, 50), dl_vec::SVector(0, 210, -50) - dl_vec::SVector(5, 60, -100));
		EXPECT_EQ(dl_vec::SVector(10, 6, 10), dl_vec::SVector(15, 66, 1010) - dl_vec::SVector(5, 60, 1000));
	}

	TEST(SVector, MultiplyOperator)
	{
		//ä|ÇØéZ
		EXPECT_EQ(dl_vec::SVector(10, 6, 21) * 10, dl_vec::SVector(100, 60, 210));
		EXPECT_EQ(dl_vec::SVector(-2, 7, 100) * -4, dl_vec::SVector(8, -28, -400));
		EXPECT_EQ(10 * dl_vec::SVector(10, 6, 21), dl_vec::SVector(100, 60, 210));
		EXPECT_EQ(-4 * dl_vec::SVector(-2, 7, 100), dl_vec::SVector(8, -28, -400));

		EXPECT_EQ(dl_vec::SVector(62, 28, -84) * 3.7f, dl_vec::SVector(229.4f, 103.6f, -310.8f));
		EXPECT_EQ(3.7f * dl_vec::SVector(62, 28, -84), dl_vec::SVector(229.4f, 103.6f, -310.8f));

		EXPECT_EQ(dl_vec::SVector(0.256f, -34.21f, 6.002f) * 25.3f, dl_vec::SVector(6.4768f, -865.513f, 151.8506f));
		EXPECT_EQ(25.3f * dl_vec::SVector(0.256f, -34.21f, 6.002f), dl_vec::SVector(6.4768f, -865.513f, 151.8506f));
	}
}
