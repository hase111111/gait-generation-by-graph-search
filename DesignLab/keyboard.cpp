﻿
//! @file keyboard.cpp

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#include "keyboard.h"

#include <Dxlib.h>


namespace designlab
{

Keyboard::Keyboard()
{
    for (int i = 0; i < kKeyNum; i++)
    {
        key_releasing_counter_[i] = 0;
        key_pressing_counter_[i] = 0;
    }
}

void Keyboard::Update()
{
    char now_key_status[kKeyNum];
    GetHitKeyStateAll(now_key_status);  // 今のキーの入力状態を取得.

    for (int i = 0; i < kKeyNum; i++)
    {
        if (now_key_status[i] != 0)
        {
            // i番のキーが押されていたら.

            if (key_releasing_counter_[i] > 0)
            {
                // 離されカウンタが0より大きければ.
                key_releasing_counter_[i] = 0;  // 0に戻す.
            }

            key_pressing_counter_[i]++;  // 押されカウンタを増やす.
        }
        else
        {
            // i番のキーが離されていたら.
            if (key_pressing_counter_[i] > 0)
            {
                // 押されカウンタが0より大きければ.
                key_pressing_counter_[i] = 0;  // 0に戻す.
            }

            key_releasing_counter_[i]++;  // 離されカウンタを増やす.
        }
    }
}

int Keyboard::GetPressingCount(const int key_code) const
{
    if (!IsAvailableCode(key_code))
    {
        return -1;
    }

    return key_pressing_counter_[key_code];
}

int Keyboard::GetReleasingCount(const int key_code) const
{
    if (!IsAvailableCode(key_code))
    {
        return -1;
    }

    return key_releasing_counter_[key_code];
}

bool Keyboard::IsAvailableCode(const int key_code) const
{
    if (!(0 <= key_code && key_code < kKeyNum))
    {
        return false;
    }

    return true;
}

}  // namespace designlab
