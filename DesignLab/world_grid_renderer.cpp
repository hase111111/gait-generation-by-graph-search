#include "world_grid_renderer.h"

#include "DxLib.h"


WorldGridRenderer::WorldGridRenderer() :
	MAIN_GRID_X_COLOR(GetColor(217, 0, 0)), MAIN_GRID_Y_COLOR(GetColor(0, 217, 0)),
	SUB_GRID_X_COLOR(GetColor(63, 0, 0)), SUB_GRID_Y_COLOR(GetColor(0, 63, 0)),
	MAIN_GRID_INTERVAL(500.0f), SUB_GRID_DEVIDE_NUM(5), GRID_LINE_Z(-50.0f)
{
}


void WorldGridRenderer::Draw() const
{
	const int kMainGridNum = 20;

	//äiéqê¸ÇÇ«Ç±Ç‹Ç≈ï`âÊÇ∑ÇÈÇ©
	const float kGridMaxX = static_cast<float>(kMainGridNum) * MAIN_GRID_INTERVAL;
	const float kGridMinX = -kGridMaxX;
	const float kGridMaxY = kGridMaxX;
	const float kGridMinY = -kGridMaxY;


	//Ç‹Ç∏ÇÕå¥ì_Çï`âÊÇ∑ÇÈ
	DrawLine3D(VGet(kGridMinX, 0.0f, GRID_LINE_Z), VGet(kGridMaxX, 0.0f, GRID_LINE_Z), MAIN_GRID_X_COLOR);
	DrawLine3D(VGet(0.0f, kGridMinY, GRID_LINE_Z), VGet(0.0f, kGridMaxY, GRID_LINE_Z), MAIN_GRID_Y_COLOR);


	//äiéqê¸Ç3DãÛä‘Ç…ï`âÊÇ∑ÇÈ
	for (int i = 0; i < kMainGridNum + 1 + kMainGridNum; i++)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 96);	//îºìßñæÇ…Ç∑ÇÈ

		//ÉÅÉCÉìÇÃäiéqê¸Çï`âÊÇ∑ÇÈ
		VECTOR start_pos = VGet(kGridMinX, kGridMinY + MAIN_GRID_INTERVAL * i, GRID_LINE_Z);
		VECTOR end_pos = VGet(kGridMaxX, kGridMinY + MAIN_GRID_INTERVAL * i, GRID_LINE_Z);
		DrawLine3D(start_pos, end_pos, MAIN_GRID_X_COLOR);

		start_pos = VGet(kGridMinX + MAIN_GRID_INTERVAL * i, kGridMinY, GRID_LINE_Z);
		end_pos = VGet(kGridMinX + MAIN_GRID_INTERVAL * i, kGridMaxY, GRID_LINE_Z);
		DrawLine3D(start_pos, end_pos, MAIN_GRID_Y_COLOR);


		//ÉTÉuÇÃäiéqê¸Çï`âÊÇ∑ÇÈ
		for (int j = 0; j < SUB_GRID_DEVIDE_NUM - 1; j++)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 32);	//îºìßñæÇ…Ç∑ÇÈ

			start_pos = VGet(kGridMinX, kGridMinY + MAIN_GRID_INTERVAL * i + MAIN_GRID_INTERVAL / SUB_GRID_DEVIDE_NUM * (j + 1), GRID_LINE_Z);
			end_pos = VGet(kGridMaxX, kGridMinY + MAIN_GRID_INTERVAL * i + MAIN_GRID_INTERVAL / SUB_GRID_DEVIDE_NUM * (j + 1), GRID_LINE_Z);
			DrawLine3D(start_pos, end_pos, SUB_GRID_X_COLOR);

			start_pos = VGet(kGridMinX + MAIN_GRID_INTERVAL * i + MAIN_GRID_INTERVAL / SUB_GRID_DEVIDE_NUM * (j + 1), kGridMinY, GRID_LINE_Z);
			end_pos = VGet(kGridMinX + MAIN_GRID_INTERVAL * i + MAIN_GRID_INTERVAL / SUB_GRID_DEVIDE_NUM * (j + 1), kGridMaxY, GRID_LINE_Z);
			DrawLine3D(start_pos, end_pos, SUB_GRID_Y_COLOR);
		}

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	//îºìßñæÇâèúÇ∑ÇÈÅDÇ±ÇÍÇñYÇÍÇÈÇ∆ï`âÊÇ™Ç®Ç©ÇµÇ≠Ç»ÇÈ
	}
}
