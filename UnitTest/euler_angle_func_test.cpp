#include "pch.h"

#include "../DesignLab/designlab_euler.h"


namespace dl = ::designlab;
namespace dlm = ::designlab::math_util;


namespace designlab::test::common::math
{
	TEST(EulerXYZTest, SetDegTest)
	{
		dl::EulerXYZ euler;
		euler.SetDeg(0.f, 30.f, -30.f);

		EXPECT_FLOAT_EQ(euler.x_angle, 0.f);
		EXPECT_FLOAT_EQ(euler.y_angle, dlm::kFloatPi / 6.f);
		EXPECT_FLOAT_EQ(euler.z_angle, -dlm::kFloatPi / 6.f);
	}

	TEST(EulerXYZTest, RotateVector3TestCaseOfOneAxisRotation)
	{
		std::vector<std::tuple<dl::EulerXYZ, dl::Vector3, dl::Vector3>> testcase_list{
			
			{ dl::EulerXYZ::MakeEulerXYZDeg(45.f, 0.f, 0.f), dl::Vector3{100.f, 0.f, 0.f},  dl::Vector3{100.f, 0.f, 0.f}},
			{ dl::EulerXYZ::MakeEulerXYZDeg(0.f, 45.f, 0.f), dl::Vector3{100.f, 0.f, 0.f},  dl::Vector3{70.71067812f, 0.f, -70.71067812f}},
			{ dl::EulerXYZ::MakeEulerXYZDeg(0.f, 0.f, 45.f), dl::Vector3{100.f, 0.f, 0.f},  dl::Vector3{70.71067812f, 70.71067812f, 0.f}},

			{ dl::EulerXYZ::MakeEulerXYZDeg(-190.f, 0.f, 0.f), dl::Vector3{10.f, -20.f, 30.f},  dl::Vector3{10, 14.48670f, -33.01719f}},
			{ dl::EulerXYZ::MakeEulerXYZDeg(0.f, -215.f, 0.f), dl::Vector3{10.f, -20.f, 30.f},  dl::Vector3{9.01578f, -20.f, -30.31032f}},
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
}