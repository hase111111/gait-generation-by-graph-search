
//! @file      dead_lock_checker.h
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory, Saitama University All right reserved.

#ifndef DESIGNLAB_DEAD_LOCK_CHECKER_H_
#define DESIGNLAB_DEAD_LOCK_CHECKER_H_

#include <deque>

#include "robot_state_node.h"


namespace designlab
{

//! @class DeadLockChecker
//! @brief 動作のループを感知するクラス．
//! @details
//! グラフ探索を用いてロボットの歩容生成をしていると，
//! 最善の行動をとると，同様の操作をし続けて動作がループしてしまう場合がある．
//! それを探知して，歩容生成に失敗していることを通達するクラス．
//! @b [deque(デック)について]
//! std::vectorの亜種．
//! vectorとの違いとして，先頭と末尾の要素の追加・削除が高速である．
//! @n 参考 : https://cpprefjp.github.io/reference/deque/deque.html (アクセス日 2023/12/27)
class DeadLockChecker final
{
public:
    //! @brief ロボットが行った動作を追加する．上限数を超えたら，古いものから削除する．
    //! @param [in] node ロボットの現在の状態．
    void AddNode(const RobotStateNode& node);

    //! @brief 直近に行った動作を比較して，ロボットが正しく動作できているか判断する．
    //! @retval true 動作がデッドロックしている．つまり，正しく動作できていない．
    //! @retval false 動作がデッドロックしていない．つまり，正しく動作できている．
    [[nodiscard]] bool IsDeadLock() const;

private:
    //! 全てのデータを保持し続けると非常に重たくなるので，
    //! この変数の値の数までデータを持つ．
    //! 先行研究のプログラムでは30個程持つようにしていたので，その値を参考にする．
    static constexpr int kMaxDataNum = 30;

    std::deque<RobotStateNode> node_vec_;  //!< 歩容生成した物を保持する．
};

}  // namespace designlab


#endif  // DESIGNLAB_DEAD_LOCK_CHECKER_H_
