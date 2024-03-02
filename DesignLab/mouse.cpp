
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory,
//!  Saitama University All right reserved.

#include "mouse.h"

#include <cmath>

#include <Dxlib.h>

#include "math_util.h"


namespace designlab
{

Mouse::Mouse() :
    kMouseKeyCodes
{
  MOUSE_INPUT_RIGHT,
  MOUSE_INPUT_LEFT,
  MOUSE_INPUT_MIDDLE,
  MOUSE_INPUT_4,
  MOUSE_INPUT_5,
  MOUSE_INPUT_6,
  MOUSE_INPUT_7,
  MOUSE_INPUT_8
},
cursor_pos_x_(0),
cursor_pos_y_(0),
cursor_past_pos_x_(0),
cursor_past_pos_y_(0),
pushing_counter_({}),
releasing_counter_({}),
wheel_rot_(0)
{
}

void Mouse::Update()
{
    // マウスの位置取得．
    cursor_past_pos_x_ = cursor_pos_x_;
    cursor_past_pos_y_ = cursor_pos_y_;
    GetMousePoint(&cursor_pos_x_, &cursor_pos_y_);

    // マウスのクリック取得．
    const int mouse_input = GetMouseInput();

    for (const auto& i : kMouseKeyCodes)
    {
        if (mouse_input & i)
        {
            // 押されているなら．
            pushing_counter_[i]++;
            releasing_counter_[i] = 0;
        }
        else
        {
            // 離されているなら．
            pushing_counter_[i] = 0;
            releasing_counter_[i]++;
        }
    }

    // ホイール回転を取得，GetMouseWheelRotVol()は前回の呼び出し以降の回転量を返す．
    wheel_rot_ = GetMouseWheelRotVol();
}

int Mouse::GetPressingCount(const int mouse_code) const
{
    // std::mapでは find も count も処理速度は同じくらい，
    // multimap や multiset は find を推奨する．
    if (releasing_counter_.count(mouse_code) == 0)
    {
        return -1;
    }

    return pushing_counter_.at(mouse_code);
}

int Mouse::GetReleasingCount(const int mouse_code) const
{
    if (releasing_counter_.count(mouse_code) == 0)
    {
        return -1;
    }

    return releasing_counter_.at(mouse_code);
}

int Mouse::GetDiffPosX() const
{
    return cursor_pos_x_ - cursor_past_pos_x_;
}

int Mouse::GetDiffPosY() const
{
    return cursor_pos_y_ - cursor_past_pos_y_;
}

double Mouse::GetDiffPos() const
{
    return sqrt(static_cast<double>(math_util::Squared(GetDiffPosY()) +
                math_util::Squared(GetDiffPosX())));
}

}  // namespace designlab
