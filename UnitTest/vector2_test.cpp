#include "pch.h"

#include "../DesignLab/designlab_vector2.h"
#include "../DesignLab/designlab_vector2.cpp"	//どこか1つのファイルで，cppファイルをインクルードする必要がある


namespace dl = ::designlab;


namespace designlab::test::common::math
{
	TEST(Vector2Test, DefaultConstructor)
	{
		//デフォルトコンストラクタのテスト
		dl::Vector2 vec;

		std::string str = "デフォルトコンストラクタは(0,0)となる";
		EXPECT_FLOAT_EQ(vec.x, 0.f) << str;
		EXPECT_FLOAT_EQ(vec.y, 0.f) << str;
	}

	TEST(Vector2Test, Constructor)
	{
		//コンストラクタのテスト
		dl::Vector2 vec(1.f, 2.f);

		std::string str = "コンストラクタは与えられた値で，Vector2を作成する";
		EXPECT_FLOAT_EQ(vec.x, 1.f) << str;
		EXPECT_FLOAT_EQ(vec.y, 2.f) << str;
	}

	TEST(Vector2Test, CopyConstructor)
	{
		//コピーコンストラクタのテスト
		dl::Vector2 vec(1.f, 2.f);
		dl::Vector2 vec2(vec);

		std::string str = "コピーコンストラクタは与えられた値をコピーして，Vector2を作成する";
		EXPECT_FLOAT_EQ(vec2.x, 1.f) << str;
		EXPECT_FLOAT_EQ(vec2.y, 2.f) << str;
	}

	TEST(Vector2Test, AssignmentOperator)
	{
		// Assignment Operator = 代入演算子
		dl::Vector2 vec(1.f, 2.f);
		dl::Vector2 vec2 = vec;

		std::string str = "代入演算子は与えられた値を代入して，Vector2を作成する";
		EXPECT_FLOAT_EQ(vec2.x, 1.f) << str;
		EXPECT_FLOAT_EQ(vec2.y, 2.f) << str;
	}

}	// namespace designlab::test::common::math
