//! @file interface_graphic_main.h
//! @brief GraphicMainのインターフェース

#ifndef DESIGNLAB_INTERFACE_GRAPHIC_MAIN_H_
#define DESIGNLAB_INTERFACE_GRAPHIC_MAIN_H_


//! @class IGraphicMain
//! @brief GraphicMainのインターフェース
//! @details 描画の処理を行うクラスは必ずこのクラスを継承すること．逆に言えば継承すれば自由に処理が交換可能になる．
//! @n ともかく，交換可能なグラフィックのパーツを作るための金型がこのクラスだということ．
class IGraphicMain
{
public:

	IGraphicMain() = default;
	virtual ~IGraphicMain() = default;


	//! @brief 描画画面の更新を行う．純粋仮想関数のため，継承先では必ずoverrideする必要がある．
	//! @return bool ループを抜け，グラフィックの表示を終了するならばfalseをかえす．
	virtual bool Update() = 0;

	//! @brief 描画を行う．ここでは描画系の処理のみを行い内部のデータを更新しないためconstを付けている．純粋仮想関数．
	virtual void Draw() const = 0;

};


#endif // !DESIGNLAB_INTERFACE_GRAPHIC_MAIN_H_