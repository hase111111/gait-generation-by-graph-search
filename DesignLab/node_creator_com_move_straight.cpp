﻿#include "node_creator_com_move_straight.h"

namespace dl = ::designlab;
namespace dlm = ::designlab::math_util;
namespace dllf = ::designlab::leg_func;


NodeCreatorComMoveStraight::NodeCreatorComMoveStraight(
	const DevideMapState& devide_map,
	const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr,
	const std::shared_ptr<const IHexapodStatePresenter>& presenter_ptr,
	const std::shared_ptr<const IHexapodVaildChecker>& checker_ptr,
	HexapodMove next_move
) :
	map_(devide_map),
	next_move_(next_move),
	converter_ptr_(converter_ptr),
	presenter_ptr_(presenter_ptr),
	checker_ptr_(checker_ptr)
{
	for (size_t i = 0; i < kCandidateDirectionNum; i++)
	{
		const float rad = static_cast<float>(i) * 2.f * dlm::kFloatPi / static_cast<float>(kCandidateDirectionNum);

		candidate_directions_[i] = ::designlab::Vector3(std::cos(rad), std::sin(rad), 0.0f);
	}
}

void NodeCreatorComMoveStraight::Create(const RobotStateNode& current_node, int current_num, std::vector<RobotStateNode>* output_graph) const
{
	RobotStateNode next_node;

	std::array<::designlab::Vector3, kCandidateDirectionNum> candidate_directions_rotated;

	for (size_t i = 0; i < kCandidateDirectionNum; i++)
	{
		candidate_directions_rotated[i] = dl::RotateVector3(candidate_directions_[i], current_node.quat);
	}

	for (size_t i = 0; i < kCandidateDirectionNum; i++)
	{
		bool is_able = false;

		next_node = current_node;
		int able_count = 0;

		for (float j = kMoveDistanceStep; j <= kMaxMoveDistance; j += kMoveDistanceStep)
		{
			next_node.ChangeGlobalCenterOfMass(current_node.global_center_of_mass + candidate_directions_rotated[i] * j, false);

			//IsLegInRange，IsStable，IsBodyInterferingを確認する．
			if (checker_ptr_->IsAllLegInRange(next_node.leg_state, next_node.leg_pos) &&
				checker_ptr_->IsStable(next_node.leg_state, next_node.leg_pos) &&
				!checker_ptr_->IsBodyInterferingWithGround(next_node, map_))
			{
				is_able = true;
				able_count = static_cast<int>(j);
			}
			else
			{
				break;
			}
		}

		if (is_able)
		{
			next_node.ChangeGlobalCenterOfMass(current_node.global_center_of_mass + candidate_directions_rotated[i] * static_cast<float>(able_count), false);
			next_node.ChangeToNextNode(current_num, next_move_);

			//discreate_leg_posを更新する．
			for (int j = 0; j < HexapodConst::kLegNum; j++)
			{
				dllf::ChangeDiscreteLegPos(
					j, DiscreteLegPos::kCenter, &next_node.leg_state
				);
			}

			output_graph->push_back(next_node);
		}
	}
}