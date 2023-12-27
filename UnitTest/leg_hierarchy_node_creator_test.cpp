#include "pch.h"

#include <cmath>

#include "../DesignLab/node_creator_leg_hierarchy.h"
#include "../DesignLab/node_creator_leg_hierarchy.cpp"


namespace dl = ::designlab;
namespace dllf = ::designlab::leg_func;


namespace 
{
	const std::vector<std::array<bool, HexapodConst::kLegNum>> oneleg_lifted_testcase{
		{ false, true, true, true, true, true },
		{ true, false, true, true, true, true },
		{ true, true, false, true, true, true },
		{ true, true, true, false, true, true },
		{ true, true, true, true, false, true },
		{ true, true, true, true, true, false }
	};

	const std::vector<std::array<bool, HexapodConst::kLegNum>> twoleg_lifted_testcase{
		{ false, false, true, true, true, true },
		{ false, true, false, true, true, true },
		{ false, true, true, false, true, true },
		{ false, true, true, true, false, true },
		{ false, true, true, true, true, false },
		{ true, false, false, true, true, true },
		{ true, false, true, false, true, true },
		{ true, false, true, true, false, true },
		{ true, false, true, true, true, false },
		{ true, true, false, false, true, true },
		{ true, true, false, true, false, true },
		{ true, true, false, true, true, false },
		{ true, true, true, false, false, true },
		{ true, true, true, false, true, false },
		{ true, true, true, true, false, false }
	};

	const std::vector<std::array<bool, HexapodConst::kLegNum>> threeleg_lifted_testcase{
		//{ false, false, false, true, true, true },
		{ false, false, true, false, true, true },
		{ false, false, true, true, false, true },
		//{ false, false, true, true, true, false },
		{ false, true, false, false, true, true },
		{ false, true, false, true, false, true },
		{ false, true, false, true, true, false },
		{ false, true, true, false, false, true },
		{ false, true, true, false, true, false },
		//{ false, true, true, true, false, false },
		//{ true, false, false, false, true, true },
		{ true, false, false, true, false, true },
		{ true, false, false, true, true, false },
		{ true, false, true, false, false, true },
		{ true, false, true, false, true, false },
		{ true, false, true, true, false, false },
		//{ true, true, false, false, false, true },
		{ true, true, false, false, true, false },
		{ true, true, false, true, false, false },
		//{ true, true, true, false, false, false } //実際のロボットは連続する3脚を遊脚すると倒れるので，それらを除く．
	};

	const std::vector<DiscreteLegPos> all_discrete_leg_pos({
		DiscreteLegPos::kBack,
		DiscreteLegPos::kCenter,
		DiscreteLegPos::kFront,
		DiscreteLegPos::kLowerBack,
		DiscreteLegPos::kLowerFront,
		DiscreteLegPos::kUpperBack,
		DiscreteLegPos::kUpperFront
		}
	);

	//! @brief テスト用のノードを作成する関数．
	//! @param [in] is_ground 接地しているかどうかの配列
	RobotStateNode MakeTestNode(const std::array<bool, HexapodConst::kLegNum>& is_ground)
	{
		RobotStateNode test_node;

		std::array<DiscreteLegPos, HexapodConst::kLegNum> discrete_leg_pos{
			DiscreteLegPos::kCenter, DiscreteLegPos::kCenter, DiscreteLegPos::kCenter,
			DiscreteLegPos::kCenter, DiscreteLegPos::kCenter, DiscreteLegPos::kCenter,
		};


		test_node.leg_state = dllf::MakeLegStateBit(DiscreteComPos::kCenterBack, is_ground, discrete_leg_pos);

		test_node.leg_pos = {
			dl::Vector3{ 0.f, -120.f, -120.f }, dl::Vector3{ 0.f,-120.f,-120.f }, dl::Vector3{ 0.f,-120.f,-120.f },
			dl::Vector3{ 0.f, 120.f, -120.f }, dl::Vector3{ 0.f,120.f,-120.f }, dl::Vector3{ 0.f,120.f,-120.f }
		};
		test_node.leg_reference_pos = {
			dl::Vector3{ 0.f, -120.f, -120.f }, dl::Vector3{ 0.f,-120.f,-120.f }, dl::Vector3{ 0.f,-120.f,-120.f },
			dl::Vector3{ 0.f, 120.f, -120.f }, dl::Vector3{ 0.f,120.f,-120.f }, dl::Vector3{ 0.f,120.f,-120.f }
		};
		test_node.center_of_mass_global_coord = dl::Vector3{ 100.f, 0.f, 0.f };
		test_node.quat = { 1.0f, 0.f, 0.f, 0.0f };

		test_node.next_move = HexapodMove::kLegHierarchyChange;
		test_node.depth = 0;
		test_node.parent_index = -1;

		return test_node;
	}

	//! @brief 接地・遊脚の配列を文字列に変換する関数．
	std::string ToString(const std::array<bool, HexapodConst::kLegNum>& is_ground)
	{
		std::string str = "{";

		for (const auto& i : is_ground)
		{
			str += i ? "接地" : "遊脚";
			str += ",";
		}

		str += "}";

		return str;
	}
}


namespace designlab::test::node::node_creator 
{
	//テストフィクスチャ
	class LegHierarchyNodeCreatorTest : public ::testing::Test
	{
	protected:

		const HexapodMove next_move_ = HexapodMove::kLegHierarchyChange;

		const int next_node_index_ = 0;

		std::unique_ptr<NodeCreatorLegHierarchy> creator_ptr_;

		virtual void SetUp() 
		{
			creator_ptr_ = std::make_unique<NodeCreatorLegHierarchy>(next_move_);
		}

		virtual void TearDown() 
		{
			creator_ptr_.reset();
		}
	};

	TEST_F(LegHierarchyNodeCreatorTest, CreateTestNodeNumCheckCaseOfOneLegLifted)
	{
		// 1本の脚が遊脚している場合，出力されるノードの数のテスト

		for (const auto &i : oneleg_lifted_testcase)
		{
			const RobotStateNode test_node = MakeTestNode(i);

			std::vector<RobotStateNode> output_nodes;

			// テスト対象の関数を実行
			creator_ptr_->Create(test_node, next_node_index_, &output_nodes);

			// 出力されたノードの数を確認
			std::string error_message = ToString(i) + "\n" + 
				test_node.ToString() + "\n_";

			EXPECT_EQ(output_nodes.size(), all_discrete_leg_pos.size()) << error_message;
		}
	}

	TEST_F(LegHierarchyNodeCreatorTest, CreateTestNodeNumCheckCaseOfTwoLegLifted)
	{
		// 2本の脚が遊脚している場合，出力されるノードの数のテスト

		for (const auto& i : twoleg_lifted_testcase)
		{
			const RobotStateNode test_node = MakeTestNode(i);

			std::vector<RobotStateNode> output_nodes;

			// テスト対象の関数を実行
			creator_ptr_->Create(test_node, next_node_index_, &output_nodes);

			// 出力されたノードの数を確認
			std::string error_message = ToString(i) + "\n" +
				"親ノード" + test_node.ToString() + "\n_";

			EXPECT_EQ(output_nodes.size(), pow(all_discrete_leg_pos.size(), 2)) << error_message;
		}
	}

	TEST_F(LegHierarchyNodeCreatorTest, CreateTestNodeNumCheckCaseOfThreeLegLifted)
	{
		// 3本の脚が遊脚している場合，出力されるノードの数のテスト

		for (const auto& i : threeleg_lifted_testcase)
		{
			const RobotStateNode test_node = MakeTestNode(i);

			std::vector<RobotStateNode> output_nodes;

			// テスト対象の関数を実行
			creator_ptr_->Create(test_node, next_node_index_, &output_nodes);

			// 出力されたノードの数を確認
			std::string error_message = ToString(i) + "\n" +
				"親ノード" + test_node.ToString() + "\n_";

			EXPECT_EQ(output_nodes.size(), pow(all_discrete_leg_pos.size(), 3)) << error_message;
		}
	}

	TEST_F(LegHierarchyNodeCreatorTest, CreateTestNodeNumCheckCaseOfNoLegLifted)
	{
		// 全ての脚が接地している場合，出力されるノードの数のテスト

		std::array<bool, HexapodConst::kLegNum> is_ground = { true, true, true, true, true, true };

		const RobotStateNode test_node = MakeTestNode(is_ground);

		std::vector<RobotStateNode> output_nodes;

		// テスト対象の関数を実行
		creator_ptr_->Create(test_node, next_node_index_, &output_nodes);

		// 出力されたノードの数を確認
		std::string error_message = ToString(is_ground) + "\n" +
			"親ノード" + test_node.ToString() + "\n_";

		EXPECT_EQ(output_nodes.size(), 1) << error_message;
	}

	TEST_F(LegHierarchyNodeCreatorTest, CreateTestParentCheckCaseOfOneLegLifted)
	{
		// 1本の脚が遊脚している場合，出力されるノードの親が正しいかどうかのテスト

		for (const auto& i : oneleg_lifted_testcase)
		{
			const RobotStateNode test_node = MakeTestNode(i);

			std::vector<RobotStateNode> output_nodes;

			// テスト対象の関数を実行
			creator_ptr_->Create(test_node, next_node_index_, &output_nodes);

			for (const auto& j : output_nodes)
			{
				EXPECT_EQ(j.parent_index, next_node_index_) << "Create関数の引数で指定したindexになる必要があります．";
				EXPECT_EQ(j.depth, test_node.depth + 1) << "深さが1つ深くなる必要があります．";
				EXPECT_EQ(j.next_move, next_move_) << "次の動作が指定したものになっている必要があります．";
			}
		}
	}

	TEST_F(LegHierarchyNodeCreatorTest, CreateTestParentCheckCaseOfTwoLegLifted)
	{
		// 2本の脚が遊脚している場合，出力されるノードの親が正しいかどうかのテスト

		for (const auto& i : twoleg_lifted_testcase)
		{
			const RobotStateNode test_node = MakeTestNode(i);

			std::vector<RobotStateNode> output_nodes;

			// テスト対象の関数を実行
			creator_ptr_->Create(test_node, next_node_index_, &output_nodes);

			for (const auto& j : output_nodes)
			{
				EXPECT_EQ(j.parent_index, next_node_index_) << "Create関数の引数で指定したindexになる必要があります．";
				EXPECT_EQ(j.depth, test_node.depth + 1) << "深さが1つ深くなる必要があります．";
				EXPECT_EQ(j.next_move, next_move_) << "次の動作が指定したものになっている必要があります．";
			}
		}
	}

	TEST_F(LegHierarchyNodeCreatorTest, CreateTestParentCheckCaseOfThreeLegLifted)
	{
		// 3本の脚が遊脚している場合，出力されるノードの親が正しいかどうかのテスト

		for (const auto& i : threeleg_lifted_testcase)
		{
			const RobotStateNode test_node = MakeTestNode(i);

			std::vector<RobotStateNode> output_nodes;

			// テスト対象の関数を実行
			creator_ptr_->Create(test_node, next_node_index_, &output_nodes);

			for (const auto& j : output_nodes)
			{
				EXPECT_EQ(j.parent_index, next_node_index_) << "Create関数の引数で指定したindexになる必要があります．";
				EXPECT_EQ(j.depth, test_node.depth + 1) << "深さが1つ深くなる必要があります．";
				EXPECT_EQ(j.next_move, next_move_) << "次の動作が指定したものになっている必要があります．";
			}
		}
	}

	TEST_F(LegHierarchyNodeCreatorTest, CreateTestParentCheckCaseOfNoLegLifted)
	{
		// 全ての脚が接地している場合，出力されるノードの親が正しいかどうかのテスト

		std::array<bool, HexapodConst::kLegNum> is_ground = { true, true, true, true, true, true };

		const RobotStateNode test_node = MakeTestNode(is_ground);

		std::vector<RobotStateNode> output_nodes;

		// テスト対象の関数を実行
		creator_ptr_->Create(test_node, next_node_index_, &output_nodes);

		for (const auto& j : output_nodes)
		{
			EXPECT_EQ(j.parent_index, next_node_index_) << "Create関数の引数で指定したindexになる必要があります．";
			EXPECT_EQ(j.depth, test_node.depth + 1) << "深さが1つ深くなる必要があります．";
			EXPECT_EQ(j.next_move, next_move_) << "次の動作が指定したものになっている必要があります．";
		}
	}

	TEST_F(LegHierarchyNodeCreatorTest, CreateTestOtherValueCheckCaseOfOneLegLifted)
	{
		// 1本の脚が遊脚している場合，その他の変更されてはいけない値が変更されていないかのテスト

		for (const auto& i : oneleg_lifted_testcase)
		{
			const RobotStateNode test_node = MakeTestNode(i);

			std::vector<RobotStateNode> output_nodes;

			// テスト対象の関数を実行
			creator_ptr_->Create(test_node, next_node_index_, &output_nodes);

			for (const auto& j : output_nodes)
			{
				// 出力されたノードを確認する．
				std::string error_message = ToString(i) + "\n" +
					"生成されたノード" + j.ToString() + "\n_";

				EXPECT_EQ(j.leg_pos, test_node.leg_pos) << error_message << "脚位置は変化しません．\n_";
				EXPECT_EQ(j.leg_reference_pos, test_node.leg_reference_pos) << error_message << "脚の基準位置は変化しません．\n_";
				EXPECT_EQ(j.center_of_mass_global_coord, test_node.center_of_mass_global_coord) << error_message << "重心位置は変化しません．\n_";
				EXPECT_EQ(j.quat, test_node.quat) << error_message << "姿勢は変化しません．\n_";

			}
		}
	}

	TEST_F(LegHierarchyNodeCreatorTest, CreateTestOtherValueCheckCaseOfTwoLegLifted)
	{
		// 2本の脚が遊脚している場合，その他の変更されてはいけない値が変更されていないかのテスト

		for (const auto& i : twoleg_lifted_testcase)
		{
			const RobotStateNode test_node = MakeTestNode(i);

			std::vector<RobotStateNode> output_nodes;

			// テスト対象の関数を実行
			creator_ptr_->Create(test_node, next_node_index_, &output_nodes);

			for (const auto& j : output_nodes)
			{
				// 出力されたノードを確認する．
				std::string error_message = ToString(i) + "\n" +
					"生成されたノード" + j.ToString() + "\n_";

				EXPECT_EQ(j.leg_pos, test_node.leg_pos) << error_message << "脚位置は変化しません．\n_";
				EXPECT_EQ(j.leg_reference_pos, test_node.leg_reference_pos) << error_message << "脚の基準位置は変化しません．\n_";
				EXPECT_EQ(j.center_of_mass_global_coord, test_node.center_of_mass_global_coord) << error_message << "重心位置は変化しません．\n_";
				EXPECT_EQ(j.quat, test_node.quat) << error_message << "姿勢は変化しません．\n_";

			}
		}
	}

	TEST_F(LegHierarchyNodeCreatorTest, CreateTestOtherValueCheckCaseOfThreeLegLifted)
	{
		// 3本の脚が遊脚している場合，その他の変更されてはいけない値が変更されていないかのテスト

		for (const auto& i : threeleg_lifted_testcase)
		{
			const RobotStateNode test_node = MakeTestNode(i);

			std::vector<RobotStateNode> output_nodes;

			// テスト対象の関数を実行
			creator_ptr_->Create(test_node, next_node_index_, &output_nodes);

			for (const auto& j : output_nodes)
			{
				// 出力されたノードを確認する．
				std::string error_message = ToString(i) + "\n" +
					"生成されたノード" + j.ToString() + "\n_";

				EXPECT_EQ(j.leg_pos, test_node.leg_pos) << error_message << "脚位置は変化しません．\n_";
				EXPECT_EQ(j.leg_reference_pos, test_node.leg_reference_pos) << error_message << "脚の基準位置は変化しません．\n_";
				EXPECT_EQ(j.center_of_mass_global_coord, test_node.center_of_mass_global_coord) << error_message << "重心位置は変化しません．\n_";
				EXPECT_EQ(j.quat, test_node.quat) << error_message << "姿勢は変化しません．\n_";

			}
		}
	}

	TEST_F(LegHierarchyNodeCreatorTest, CreateTestOtherValueCheckCaseOfNoLegLifted)
	{
		// 全ての脚が接地している場合，その他の変更されてはいけない値が変更されていないかのテスト

		std::array<bool, HexapodConst::kLegNum> is_ground = { true, true, true, true, true, true };

		const RobotStateNode test_node = MakeTestNode(is_ground);

		std::vector<RobotStateNode> output_nodes;

		// テスト対象の関数を実行
		creator_ptr_->Create(test_node, next_node_index_, &output_nodes);

		for (const auto& j : output_nodes)
		{
			// 出力されたノードを確認する．
			std::string error_message = ToString(is_ground) + "\n" +
				"生成されたノード" + j.ToString() + "\n_";

			EXPECT_EQ(j.leg_pos, test_node.leg_pos) << error_message << "脚位置は変化しません．\n_";
			EXPECT_EQ(j.leg_reference_pos, test_node.leg_reference_pos) << error_message << "脚の基準位置は変化しません．\n_";
			EXPECT_EQ(j.center_of_mass_global_coord, test_node.center_of_mass_global_coord) << error_message << "重心位置は変化しません．\n_";
			EXPECT_EQ(j.quat, test_node.quat) << error_message << "姿勢は変化しません．\n_";

		}
	}
}