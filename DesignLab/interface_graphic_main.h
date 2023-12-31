
//! @file      interface_graphic_main.h
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#ifndef DESIGNLAB_INTERFACE_GRAPHIC_MAIN_H_
#define DESIGNLAB_INTERFACE_GRAPHIC_MAIN_H_


namespace designlab
{

//! @class IGraphicMain
//! @brief GraphicMainのインターフェース．
//! @details 描画の処理を行うクラスは必ずこのクラスを継承する必要がある．
//! しかし，継承すれば自由に処理が交換可能になる．
//! つまり，交換可能なグラフィック処理のパーツを作るための
//! 金型がこのクラスだということを理解すればよい．
class IGraphicMain
{
public:
    IGraphicMain() = default;
    virtual ~IGraphicMain() = default;

    //! @brief 描画画面の更新を行う．純粋仮想関数のため，継承先では必ず override する必要がある．
    //! @return bool ループを抜け，グラフィックの表示を終了するならば false をかえす．
    virtual bool Update() = 0;

    //! @brief 描画を行う．
    //! ここでは描画系の処理のみを行い内部のデータを更新しないため const を付けている．
    virtual void Draw() const = 0;
};

}  // namespace designlab


#endif  // DESIGNLAB_INTERFACE_GRAPHIC_MAIN_H_
