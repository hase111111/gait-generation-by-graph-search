#include "world_grid_renderer.h"

#include <Dxlib.h>


WorldGridRenderer::WorldGridRenderer() :
	kMainGridXColor(GetColor(217, 0, 0)),
	kMainGridYColor(GetColor(0, 217, 0)),
	kSubGridXColor(GetColor(63, 0, 0)),
	kSubGridYColor(GetColor(0, 63, 0)),
	kMainGridNum(20),
	kMainGridInterval(500.0f),
	kSubGridDevideNum(5),
	kGridLineZPos(-50.0f)
{
}


void WorldGridRenderer::Draw() const
{
	//Šiqü‚ğ‚Ç‚±‚Ü‚Å•`‰æ‚·‚é‚©
	const float kGridMaxX = static_cast<float>(kMainGridNum) * kMainGridInterval;
	const float kGridMinX = -kGridMaxX;
	const float kGridMaxY = kGridMaxX;
	const float kGridMinY = -kGridMaxY;


	// X²‚ÆY²‚ÌŠiqü‚ğ•`‰æ‚·‚é
	DrawLine3D(VGet(kGridMinX, 0.0f, kGridLineZPos), VGet(kGridMaxX, 0.0f, kGridLineZPos), kMainGridXColor);
	DrawLine3D(VGet(0.0f, kGridMinY, kGridLineZPos), VGet(0.0f, kGridMaxY, kGridLineZPos), kMainGridYColor);


	//Šiqü‚ğ3D‹óŠÔ‚É•`‰æ‚·‚é

	const int kMainGridAlpha = 96;	//ƒƒCƒ“‚ÌŠiqü‚Ì“§–¾“x
	const int kSubGridAlpha = 32;	//ƒTƒu‚ÌŠiqü‚Ì“§–¾“x

	for (int i = 0; i < kMainGridNum + 1 + kMainGridNum; i++)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, kMainGridAlpha);	//”¼“§–¾‚É‚·‚é

		//ƒƒCƒ“‚ÌŠiqü‚ğ•`‰æ‚·‚é
		VECTOR start_pos = VGet(kGridMinX, kGridMinY + kMainGridInterval * i, kGridLineZPos);
		VECTOR end_pos = VGet(kGridMaxX, kGridMinY + kMainGridInterval * i, kGridLineZPos);
		DrawLine3D(start_pos, end_pos, kMainGridXColor);

		start_pos = VGet(kGridMinX + kMainGridInterval * i, kGridMinY, kGridLineZPos);
		end_pos = VGet(kGridMinX + kMainGridInterval * i, kGridMaxY, kGridLineZPos);
		DrawLine3D(start_pos, end_pos, kMainGridYColor);


		//ƒTƒu‚ÌŠiqü‚ğ•`‰æ‚·‚é
		for (int j = 0; j < kSubGridDevideNum - 1; j++)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, kSubGridAlpha);	//”¼“§–¾‚É‚·‚é

			start_pos = VGet(kGridMinX, kGridMinY + kMainGridInterval * i + kMainGridInterval / kSubGridDevideNum * (j + 1), kGridLineZPos);
			end_pos = VGet(kGridMaxX, kGridMinY + kMainGridInterval * i + kMainGridInterval / kSubGridDevideNum * (j + 1), kGridLineZPos);
			DrawLine3D(start_pos, end_pos, kSubGridXColor);

			start_pos = VGet(kGridMinX + kMainGridInterval * i + kMainGridInterval / kSubGridDevideNum * (j + 1), kGridMinY, kGridLineZPos);
			end_pos = VGet(kGridMinX + kMainGridInterval * i + kMainGridInterval / kSubGridDevideNum * (j + 1), kGridMaxY, kGridLineZPos);
			DrawLine3D(start_pos, end_pos, kSubGridYColor);
		}

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	//”¼“§–¾‚ğ‰ğœ‚·‚éD‚±‚ê‚ğ–Y‚ê‚é‚Æ•`‰æ‚ª‚¨‚©‚µ‚­‚È‚é
	}
}
