#pragma once

//Dxlibの3Dのカメラの処理を行うクラス
class GraphicCamera final
{
public:
	GraphicCamera() = default;
	~GraphicCamera() = default;

	void init() const;
	void update();

private:

};
