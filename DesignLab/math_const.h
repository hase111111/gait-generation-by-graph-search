#ifndef DESIGNLAB_MATH_CONST_H_
#define DESIGNLAB_MATH_CONST_H_


namespace designlab
{

//float と double以外は作成しない．
template<typename T>
struct MathConst
{
	//Tがfloat でも double でもない場合，エラーを出す．
	static_assert(std::is_same<T, float>::value || std::is_same<T, double>::value, "T は float とdouble 型にのみ提供されます．");
};


template<>
struct MathConst<float>
{
	static constexpr float kPi = 3.14159265358979323846f;
	static constexpr float kAllowableError = 0.001f;
	static constexpr float kRoundAngle = 360.0f;
};

template<>
struct MathConst<double>
{
	static constexpr double kPi = 3.14159265358979323846;
	static constexpr double kAllowableError = 0.001;
	static constexpr double kRoundAngle = 360.0;
};


} // namespace designlab

#endif //DESIGNLAB_MATH_CONST_H_