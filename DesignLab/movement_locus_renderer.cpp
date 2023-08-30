#include "movement_locus_renderer.h"

#include "DxLib.h"

#include"designlab_dxlib.h"


MovementLocusRenderer::MovementLocusRenderer() : LOCUS_BASE_COLOR(GetColor(173, 187, 50)), LOCUS_DISPLAY_LINE_COLOR(GetColor(239, 237, 84)), LOCUS_ALPHA(128)
{
}


void MovementLocusRenderer::setSimuEndIndex(const std::vector<size_t>& index)
{
	m_simu_end_index.clear();

	for (const auto& i : index)
	{
		m_simu_end_index.push_back(i);
	}
}


void MovementLocusRenderer::setMovementLocus(const std::vector<SNode>& locus)
{
	m_movement_locus.clear();

	for (const auto& i : locus)
	{
		m_movement_locus.push_back(i.global_center_of_mass);
	}
}


void MovementLocusRenderer::draw(const size_t draw_simu_num, const bool draw_all_simulation)  const
{
	const size_t kSize = m_movement_locus.size();

	if (kSize < 2)return;

	for (size_t i = 0; i < kSize - 1; i++)
	{
		//範囲外アクセスを防ぐ
		if (i < 0 && kSize - 1 <= i) { break; }


		int now_simu_num = (int)m_simu_end_index.size();	//現在のシミュレーション回数

		bool do_draw = true;	//描画するかどうか


		//始点のインデックスがシミュレーション終了インデックスに含まれているならば描画を飛ばす
		for (size_t j = 0; j < m_simu_end_index.size(); j++)
		{
			if (i == m_simu_end_index[j]) { do_draw = false; }

			if (i < m_simu_end_index[j])
			{
				now_simu_num = (int)j;
				break;
			}
		}

		//始点と終点の座標を描画座標に変換する
		VECTOR start = dl_dxlib::convertToDxVec(m_movement_locus.at(i));
		VECTOR end = dl_dxlib::convertToDxVec(m_movement_locus.at(i + 1));


		//描画
		if (do_draw)
		{
			if ((int)draw_simu_num == now_simu_num || draw_all_simulation)
			{
				DrawLine3D(start, end, LOCUS_DISPLAY_LINE_COLOR);
			}
			else
			{
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, LOCUS_ALPHA);

				DrawLine3D(start, end, LOCUS_BASE_COLOR);

				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			}
		}
	}
}
