
//! @file      fps_controller.h
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory,
//!  Saitama University All right reserved.

#ifndef DESIGNLAB_FPS_CONTROLLER_H_
#define DESIGNLAB_FPS_CONTROLLER_H_

#include <list>


namespace designlab
{

//! @class FpsController
//! @brief FPSを一定にキープするためのクラス．
//! @details
//! 基本的には何もしなくともFPSは一定になるのだが，
//! 144Fpsや240Fps程度のゲーミングモニターを使っている場合，
//! 画面がおかしくなることがある．
//! @n よってこのクラスを使ってFPSを制御する．
//! FPSは Frames per Second：1秒当たりの画面更新回数のこと．
//! First Person Shootingのことではない．
//! @n 当方のPCのモニターの問題なので，必要ないならば消してしまってもかまわないが，
//! あったところで問題はないので放置しておいてもらって構わない．
//! @n
//! [参考]
//! @li https://dixq.net/rp2/07.html (アクセス日 2023/12/31)
//! @li https://dixq.net/forum/viewtopic.php?t=20224 (アクセス日 2023/12/31)
class FpsController final
{
public:
    FpsController() = delete;
    explicit FpsController(int target_fps);

    //! @brief 処理が早すぎる場合，FPSを一定にするために待つ．
    void Wait();

    //! @brief 60Hz以上のモニター使用時に処理が詰まって画面がちらつかないように，
    //! 描画処理をスキップするかどうかを判定する．
    //! @return 処理が詰まって描画を飛ばしたいときに
    //!  true を返す．その後フラグを false にする．
    bool SkipDrawScene();

    void DrawFps() const;

private:
    //! @brief 現在の時刻を記録する関数．
    //! @param[in] now_time 現在の時刻(ミリ秒)
    void RegisterTime(int now_time);

    //! @brief どれだけ待てばよいか返す関数．
    //! また，コマ落ちしているかどうかも判定する．
    //! @note 機能が2つあるので分けたほうがよい．
    //! @param[out] wait_time 待つべき時間 (ミリ秒)．
    //! @return コマ落ちしている場合は false．
    bool CheckNeedSkipDrawScreen(int* wait_time) const;

    //! @brief 目標のFPSが正しいかどうかを判定する関数．
    //! @return 負の値，または60より大きい値であれば false．
    bool TargetFpsIsValid() const;


    //! 目標のFPS，推奨値は 60 or 30．Dxlibの使用上 60 を上回る値を推奨しない．
    const int kTargetFpsValue;

    //! 1フレーム当たりにかかる時間(ミリ秒)
    const int kOneFrameTime;

    //! リストに2秒分のフレームごとにかかった時間を記録するため，
    //! リストの最大サイズを決める．
    const int kListMax;

    //! 1フレームごとにかかった時間を記録するリスト．
    std::list<int> time_list_;

    //! コマ落ちを実装するためのフラグ．
    //! trueであれば 1フレーム描画を飛ばし，その後フラグを折る
    bool need_skip_draw_screen_;
};

}  // namespace designlab


#endif  // DESIGNLAB_FPS_CONTROLLER_H_
