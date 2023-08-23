#include "movement_locus_renderer.h"

#include "DxLib.h"

#include"designlab_dxlib.h"


MovementLocusRenderer::MovementLocusRenderer() : LOCUS_COLOR(GetColor(239, 237, 84))
{
}


void MovementLocusRenderer::addMovementLocusPoint(const dl_vec::SVector& locus)
{
	m_movement_locus.push_back(locus);
}


void MovementLocusRenderer::setMovementLocus(const std::vector<SNode>& locus)
{
	m_movement_locus.clear();

	for (const auto& i : locus)
	{
		m_movement_locus.push_back(i.global_center_of_mass);
	}
}


void MovementLocusRenderer::draw() const
{
	const size_t kSize = m_movement_locus.size();

	if (kSize < 2)return;

	for (size_t i = 0; i < kSize - 1; i++)
	{
		if (i < 0 && kSize - 1 <= i) { break; }

		VECTOR start = dl_dxlib::convertToDxVec(m_movement_locus.at(i));
		VECTOR end = dl_dxlib::convertToDxVec(m_movement_locus.at(i + 1));

		if (m_movement_locus.at(i).distanceFrom(m_movement_locus.at(i + 1)) < LOCUS_LINE_MAX_WIDTH)
		{
			DrawLine3D(start, end, LOCUS_COLOR);
		}

	}
}
