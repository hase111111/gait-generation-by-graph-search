#pragma once

//シミュレーションの結果を格納するクラス．変数をごちゃごちゃさせたくないので作成

class SimulateResult final
{
public:
	SimulateResult();
	~SimulateResult() = default;

	//各種変数の値を取得する変数
	int getClearNum() const;
	int getFailedByGatePatternLoop() const;
	int getFailedByNoGatePattern() const;
	int getGatePatternGenerateSum() const;

	int getDistanceMoveYSum() const;
	int getDistanceMoveYMax() const;
	int getDistanceMoveYMin() const;

	double getGatePatternGenerateTimeSum() const;
	double getGatePatternGenerateTimeMax() const;
	double getGatePatternGenerateTimeMin() const;

	// clear_numの値を+1する
	void countupClearNum();

	// gate_pattern_generate_sumの値を+1する
	void countupGatePatternGenerateSum();

	// failed_by_gate_pattern_loopの値を+1する
	void countupFailedByGatePatternLoop();

	// failed_by_no_gate_patternの値を+1する
	void countupFailedByNoGatePattern();

	// 引数の値から最大・最小値と総和を更新する
	void updateDistanceMoveY(const int _y);

	// 引数の値から最大・最小値と総和を更新する
	void updateGatePatternGenerateTime(const double _time);

private:
	// C++では変数の値はprivateにするべきで，値の取得はget???関数を使う.
	// しかし，そもそもgetterやsetterを使うようなプログラムにすべきじゃない．Don't ask. Tellの原則に従おう

	int m_clear_num;							//一定距離歩行できてシミュレーションが終了した回数
	int m_failed_by_gate_pattern_loop;			//同じ動作を繰り返してシミュレーションが終了した回数
	int m_failed_by_no_gate_pattern;			//歩容パターンが得られずにシミュレーションが終了した回数
	int m_gate_pattern_generate_sum;			//全シミュレーションで出力された歩容パターンの総数

	int m_distance_move_Y_sum;					//全シミュレーションで進んだ距離
	int m_distance_move_Y_max;
	int m_distance_move_Y_min;
	
	double m_gate_parttern_generate_time_sum;	//全シミュレーションで歩容パターン生成にかかった時間の総和[s]
	double m_gate_parttern_generate_time_max;
	double m_gate_parttern_generate_time_min;
};
