#include "stability_margin_renderer.h"

#include <vector>

#include <Dxlib.h>

#include "dxlib_util.h"
#include "designlab_polygon2.h"
#include "leg_state.h"


namespace dl = ::designlab;
namespace dllf = ::designlab::leg_func;
namespace dldu = ::designlab::dxlib_util;


StabilityMarginRenderer::StabilityMarginRenderer(const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr) :
	kMarginColor(GetColor(0, 255, 0)), 
	kMarginErrorColor(GetColor(255, 0, 0)), 
	kAlpha(128),
	converter_ptr_(converter_ptr)
{
}


void StabilityMarginRenderer::Draw(const RobotStateNode& node) const
{
	dl::Polygon2 polygon_xy;			//平面に投影した多角形．

	std::vector<dl::Vector3> polygon;	//多角形の頂点．

	dl::Vector3 polygon_sum{0, 0, 0};	//多角形の頂点の合計，重心を求めるために使用する


	for (int i = 0; i < HexapodConst::kLegNum; i++)
	{
		if (dllf::IsGrounded(node.leg_state, i))
		{
			polygon.push_back(
				converter_ptr_->ConvertLegToGlobalCoordinate(node.leg_pos[i], i, node.global_center_of_mass, node.rot, true)
			);

			polygon.back() += dl::Vector3{0, 0, 5};	//わかりやすさのため，高さを少し上げる

			polygon_xy.AddVertex(polygon.back().ProjectedXY());

			polygon_sum += polygon.back();
		}

	}

	// 重心の座標
	const dl::Vector3 center = polygon_sum / static_cast<float>(polygon.size());


	//多角形を描画する
	for (size_t i = 0; i < polygon.size(); i++)
	{
		const VECTOR poly[3] = {
			dldu::ConvertToDxlibVec(polygon[i]),
			dldu::ConvertToDxlibVec(polygon[(i + 1) % polygon.size()]),
			dldu::ConvertToDxlibVec(center)
		};

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, kAlpha);

		if (polygon_xy.IsInside(node.global_center_of_mass.ProjectedXY()))
		{
			DrawTriangle3D(poly[0], poly[1], poly[2], kMarginColor, TRUE);
		}
		else
		{
			DrawTriangle3D(poly[0], poly[1], poly[2], kMarginErrorColor, TRUE);
		}


		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	//投射した重心を描画する
	VECTOR projected_center_pos = dldu::ConvertToDxlibVec(
		//わかりやすさのため，重心の高さを少し上げる
		{ node.global_center_of_mass.x,node.global_center_of_mass.y, center.z + 10 }	
	);

	DrawSphere3D(projected_center_pos, 5, 10, 10, GetColor(255, 255, 255), TRUE);
}
