#include "stability_margin_renderer.h"

#include <vector>

#include "DxLib.h"

#include "dxlib_util.h"
#include "designlab_polygon.h"
#include "leg_state.h"

namespace dldu = designlab::dxlib_util;


StabilityMarginRenderer::StabilityMarginRenderer() : kMarginColor(GetColor(0, 255, 0)), kMarginErrorColor(GetColor(255, 0, 0)), kAlpha(128)
{
}


void StabilityMarginRenderer::Draw(const SNode& node) const
{
	designlab::SPolygon2 polygon_xy;			//ïΩñ Ç…ìäâeÇµÇΩëΩäpå`

	std::vector<designlab::Vector3> polygon;	//ëΩäpå`ÇÃí∏ì_

	designlab::Vector3 polygon_sum{0, 0, 0};	//ëΩäpå`ÇÃí∏ì_ÇÃçáåv


	for (int i = 0; i < HexapodConst::LEG_NUM; i++)
	{
		if (dl_leg::isGrounded(node.leg_state, i))
		{
			polygon.push_back(m_hexapod_state_calclator.getGlobalLegPos(node, i, true));

			polygon.back() += designlab::Vector3{0, 0, 5};

			polygon_xy.addVertex(polygon.back().ProjectedXY());

			polygon_sum += polygon.back();
		}

	}

	designlab::Vector3 center = polygon_sum / static_cast<float>(polygon.size());


	for (size_t i = 0; i < polygon.size(); i++)
	{
		VECTOR poly[3] = {
			dldu::ConvertToDxlibVec(polygon[i]),
			dldu::ConvertToDxlibVec(polygon[(i + 1) % polygon.size()]),
			dldu::ConvertToDxlibVec(center)
		};

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, kAlpha);

		if (polygon_xy.isInside(node.global_center_of_mass.ProjectedXY()))
		{
			DrawTriangle3D(poly[0], poly[1], poly[2], kMarginColor, TRUE);
		}
		else
		{
			DrawTriangle3D(poly[0], poly[1], poly[2], kMarginErrorColor, TRUE);
		}


		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	//ìäéÀÇµÇΩèdêSÇï`âÊÇ∑ÇÈ
	VECTOR projected_center = dldu::ConvertToDxlibVec({ node.global_center_of_mass.x,node.global_center_of_mass.y, center.z + 10 });

	DrawSphere3D(projected_center, 5, 10, 10, GetColor(255, 255, 255), TRUE);
}
