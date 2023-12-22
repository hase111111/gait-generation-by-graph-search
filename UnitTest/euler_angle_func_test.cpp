#include "pch.h"

#include "../DesignLab/math_euler.h"


namespace dl = ::designlab;
namespace dlm = ::designlab::math_util;


namespace designlab::test::common::math
{
	TEST(EulerXYZTest, SetDegTest)
	{
		dl::EulerXYZ euler1;
		euler1.SetDeg(0.f, 30.f, -30.f);

		EXPECT_FLOAT_EQ(euler1.x_angle, 0.f);
		EXPECT_FLOAT_EQ(euler1.y_angle, dlm::kFloatPi / 6.f);
		EXPECT_FLOAT_EQ(euler1.z_angle, -dlm::kFloatPi / 6.f);

		dl::EulerXYZ euler2;
		euler2.SetDeg(-60.f, 0.f, 90.f);

		EXPECT_FLOAT_EQ(euler2.x_angle, -dlm::kFloatPi / 3.f);
		EXPECT_FLOAT_EQ(euler2.y_angle, 0.f);
		EXPECT_FLOAT_EQ(euler2.z_angle, dlm::kFloatPi / 2.f);
	}

	TEST(EulerXYZTest, RotateVector3TestCaseOfOneAxisRotation)
	{
		//_xyzEuler各は y = 90 [deg] でジンバルロックが発生する．
		// y = 90 [deg]の付近で正確に計算できなくなる可能性が高いのでテストしない．

		std::vector<std::tuple<dl::EulerXYZ, dl::Vector3, dl::Vector3>> testcase_list{
			
			{ dl::EulerXYZ::MakeEulerXYZDeg(45.f, 0.f, 0.f), dl::Vector3{100.f, 0.f, 0.f},  dl::Vector3{100.f, 0.f, 0.f}},
			{ dl::EulerXYZ::MakeEulerXYZDeg(0.f, 45.f, 0.f), dl::Vector3{100.f, 0.f, 0.f},  dl::Vector3{70.71067812f, 0.f, -70.71067812f}},
			{ dl::EulerXYZ::MakeEulerXYZDeg(0.f, 0.f, 45.f), dl::Vector3{100.f, 0.f, 0.f},  dl::Vector3{70.71067812f, 70.71067812f, 0.f}},

			{ dl::EulerXYZ::MakeEulerXYZDeg(-190.f, 0.f, 0.f), dl::Vector3{10.f, -20.f, 30.f},  dl::Vector3{10, 14.48670f, -33.01719f}},
			{ dl::EulerXYZ::MakeEulerXYZDeg(0.f, -60.f, 0.f), dl::Vector3{10.f, -20.f, 30.f},  dl::Vector3{-20.98076f, -20.f, 23.66025f}},
			{ dl::EulerXYZ::MakeEulerXYZDeg(0.f, 0.f, -270.f), dl::Vector3{10.f, -20.f, 30.f},  dl::Vector3{20.f, 10.f, 30.f}},

		}; 

		for (const auto& [euler, vec, expected] : testcase_list)
		{
			const auto actual = dl::RotateVector3(vec, euler);

			std::string str = " euler : " + euler.ToStringDeg() + ",\n" +
				" vec : " + vec.ToString() + ",\n" + 
				" expected : " + expected.ToString() + ",\n" + 
				" actual : " + actual.ToString() + "\n_";

			EXPECT_FLOAT_EQ(actual.x, expected.x) << str;
			EXPECT_FLOAT_EQ(actual.y, expected.y) << str;
			EXPECT_FLOAT_EQ(actual.z, expected.z) << str;
		}
	}

	TEST(EulerXYZTest, RotateVector3TestCaseOfTwoAxisRotation)
	{
		std::vector<std::tuple<dl::EulerXYZ, dl::Vector3, dl::Vector3>> testcase_list{
			{dl::EulerXYZ::MakeEulerXYZDeg(45.f, 45.f, 0.f), dl::Vector3{100.f, 0.f, 0.f},  dl::Vector3{70.71067f, 0.f, -70.71067f}},
			{dl::EulerXYZ::MakeEulerXYZDeg(45.f, 0.f, 45.f), dl::Vector3{100.f, 0.f, 0.f},  dl::Vector3{70.71067f, 70.71067f, 0.f}},
			{dl::EulerXYZ::MakeEulerXYZDeg(0.f, 45.f, 45.f), dl::Vector3{100.f, 0.f, 0.f},  dl::Vector3{50.f, 50.f, -70.71067f}},
		};

		for (const auto& [euler, vec, expected] : testcase_list)
		{
			const auto actual = dl::RotateVector3(vec, euler);

			std::string str = " euler : " + euler.ToStringDeg() + ",\n" +
				" vec : " + vec.ToString() + ",\n" +
				" expected : " + expected.ToString() + ",\n" +
				" actual : " + actual.ToString() + "\n_";

			EXPECT_FLOAT_EQ(actual.x, expected.x) << str;
			EXPECT_FLOAT_EQ(actual.y, expected.y) << str;
			EXPECT_FLOAT_EQ(actual.z, expected.z) << str;
		}
	}

	TEST(EulerXYZTest, RotateVector3TestCaseOfAllAxisRotation)
	{
		std::vector<std::tuple<dl::EulerXYZ, dl::Vector3, dl::Vector3>> testcase_list{
			{dl::EulerXYZ::MakeEulerXYZDeg(45.f, 45.f, 45.f), dl::Vector3{100.f, 0.f, 0.f},  dl::Vector3{50.f, 50.f, -70.71067f}},
			{dl::EulerXYZ::MakeEulerXYZDeg(30.f, -60.f, 15.f), dl::Vector3{-34.f, 73.f, 68.f},  dl::Vector3{-103.77845f, 2.443265f, 18.25f}},
		};

		for (const auto& [euler, vec, expected] : testcase_list)
		{
			const auto actual = dl::RotateVector3(vec, euler);

			std::string str = " euler : " + euler.ToStringDeg() + ",\n" +
				" vec : " + vec.ToString() + ",\n" +
				" expected : " + expected.ToString() + ",\n" +
				" actual : " + actual.ToString() + "\n_";

			EXPECT_FLOAT_EQ(actual.x, expected.x) << str;
			EXPECT_FLOAT_EQ(actual.y, expected.y) << str;
			EXPECT_FLOAT_EQ(actual.z, expected.z) << str;
		}
	}
}