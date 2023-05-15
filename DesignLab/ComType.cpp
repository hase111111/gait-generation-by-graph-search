#include "ComType.h"


bool ComType::isAbleCoM(const int _com_pattern, const bool _ground_leg[HexapodConst::LEG_NUM])
{
	//重心位置のパターンとvの関係　　v=18,19,20．．．　のときパターン1の重心位置は取れない．ここに書かれている番号の脚状態は取れない。
	const char _comType1[9] = { 18, 19,             23, 24, 25,             29, 30, 31,             35 };	//パターン6
	const char _comType2[9] = { 18, 19, 20,             24, 25, 26,             30, 31, 32, };				//パターン1
	const char _comType3[9] = { 19, 20, 21,             25, 26, 27,             31, 32, 33, };			//パターン2
	const char _comType4[9] = { 20, 21, 22,             26, 27, 28,             32, 33, 34, };		//パターン3
	const char _comType5[9] = { 21, 22, 23,             27, 28, 29,             33, 34, 35 };	//パターン4
	const char _comType6[9] = { 18,             22, 23, 24,             28, 29, 30,             34, 35 };	//パターン5
	const char _comType7[9] = { 18,     20,     22,     24,     26,     28,     30,     32,     34, };		//パターン7
	const char _comType8[9] = { 19,     21,     23,     25,     27,     29,     31,     33,     35 };	//パターン8
	const char _comType0[18] = { 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35 };	//パターン0 どの隣りあった足も上げることができない
	const char* _comType[9] = { _comType0, _comType1, _comType2, _comType3, _comType4, _comType5, _comType6, _comType7, _comType8 };

	int n = 9;

	if (_com_pattern == 0)
	{
		n = 18;
	}

	for (int i = 0; i < n; i++)
	{
		if (_comType[_com_pattern][i] == getComTypeFromGroundLeg(_ground_leg))
		{
			return false;
		}
	}

	return true;
}

char ComType::getComTypeFromGroundLeg(const bool _ground_leg[HexapodConst::LEG_NUM])
{
	// PassFinding の変数 iHX2を持ってきたもの．また，initiHX2();を持ってきた.

	int _compare_bit = 0;	//値を比較するためにboolをbitに変換する

	for (int i = 0; i < HexapodConst::LEG_NUM; i++)
	{
		//接地しているならば
		if (_ground_leg[i] == true) 
		{
			// i番目のbitを立てる
			_compare_bit |= (1 << i);
		}
	}

	switch (_compare_bit)
	{
	case 0b111111: 
		return  0;		//HX0
	case 0b101111:
		return  1;		//HX1
	case 0b110111:
		return  2;		//HX2
	case 0b111011:
		return  3;		//HX3
	case 0b111101:
		return  4;		//HX4
	case 0b111110:
		return  5;		//HX5
	case 0b011111: 
		return  6;		//HX6
	case 0b011011: 
		return  7;		//HX7
	case 0b110110:
		return  8;		//HX8
	case 0b101101:
		return  9;		//HX9
	case 0b101011: 
		return  10;	//HX10
	case 0b110101: 
		return  11;	//HX11
	case 0b111010: 
		return  12;	//HX12
	case 0b011101: 
		return  13;	//HX13
	case 0b101110:
		return  14;	//HX14
	case 0b010111: 
		return  15;	//HX15
	case 0b101010:
		return  16;	//HX16
	case 0b010101:
		return  17;	//HX17
	case 0b001111:
		return  18;	//HX18
	case 0b100111:
		return  19;	//HX19
	case 0b110011:
		return  20;	//HX20
	case 0b111001:
		return  21;	//HX21
	case 0b111100:
		return  22;	//HX22
	case 0b011110:
		return  23;	//HX23
	case 0b001101:
		return  24;	//HX24
	case 0b100110:
		return  25;	//HX25
	case 0b010011:
		return  26;	//HX26
	case 0b101001: 
		return  27;	//HX27
	case 0b110100: 
		return  28;	//HX28
	case 0b011010: 
		return  29;	//HX29
	case 0b001011:
		return  30;	//HX30
	case 0b100101:
		return  31;	//HX31
	case 0b110010:
		return  32;	//HX32
	case 0b011001:
		return  33;	//HX33
	case 0b101100:
		return  34;	//HX34
	case 0b010110:
		return  35;	//HX35
	default:
		break;
	}

	//該当しないならば負の値を返す
	return -1;
}
