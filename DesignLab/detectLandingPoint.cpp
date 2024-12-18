//#include "callBack.h"
#include "detectLandingPoint.h"

using namespace cv;

cv::Mat tempImage;
bool selectObject = false;
int trackObject = 0;
bool showHist = true;
cv::Point origin;
cv::Rect selection;
//デフォルトコンストラクタ
detectLandingPoint::detectLandingPoint()
{
	detectLandingPoint::initializeCamera();
	//initializeCallBack();
	centerOfGravityMovement.x = 0;
	centerOfGravityMovement.y = 0;
	centerOfGravityMovement.z = 0;
}

//カメラの初期化
int detectLandingPoint::initializeCamera()
{
	_selectObject = false;
	_trackObject = 0;
	//事前にキャリブレーションしたカメラの内部パラメータの読み込み
	cv::FileStorage infs("intrinsics.yml", cv::FileStorage::READ);
	if(!infs.isOpened())//カメラの内部パラメータを正常にオープンできたか確認．
	{
		//読み込みに失敗したときの処理
		std::cout << "ERROR:open intrinsics.yml";
		return -1;
	}
	//事前にキャリブレーションしたカメラの外部パラメータの読み込み
	cv::FileStorage exfs("extrinsics.yml", cv::FileStorage::READ);
	if(!exfs.isOpened())//カメラの外部パラメータを正常にオープンできたか確認．
	{
		//読み込みに失敗したときの処理
		std::cout << "ERROR:open extrinsics.yml";
		return -1;
	}

	//カメラの内部パラメータの代入
	infs["M1"] >> _camMat1;
	infs["D1"] >> _distCoeffs1;
	infs["M2"] >> _camMat2;
	infs["D2"] >> _distCoeffs2;
	
	//カメラの外部パラメータの代入
	exfs["R"] >> _R;
	exfs["T"] >> _T;
	exfs["R1"] >> _R1;
	exfs["P1"] >> _P1;
	exfs["R2"] >> _R2;
	exfs["P2"] >> _P2;
	exfs["Q"] >> _Q;

	//ステレオカメラの基線長
	_baselineRatio = 100.0 * _Q.at<double>(3, 2);
	//_baselineRatio = 1.0;
	//カメラの画像サイズ
	_imgSize1.width = 640;
	_imgSize1.height = 480;
	_imgSize2.width = 640;
	_imgSize2.height = 480;
	//左カメラのロボット座標系から見た位置
	_cameraPosi.x = 0.0;
	_cameraPosi.y = -50.0;		//L
	//_cameraPosi.y = 50.0;		//R
	_cameraPosi.z = -170.0;

	//カメラの画角，焦点距離，主点の位置を求める
	cv::calibrationMatrixValues(_camMat1, _imgSize1, _apWidth, _apHeight, _fovx, _fovy, _focalLength, _priPoint, _aspectRatio);

	//カメラのキャリブレーションデータから各カメラの平行化変換された歪み補正マップを求める
	cv::initUndistortRectifyMap(_camMat1, _distCoeffs1, _R1, _P1, _imgSize1, CV_16SC2, _rmap[0][0], _rmap[0][1]);
	cv::initUndistortRectifyMap(_camMat2, _distCoeffs2, _R2, _P2, _imgSize2, CV_16SC2, _rmap[1][0], _rmap[1][1]);
	
	_showTemplate = false;
	_showHistogram = false;
	_showMesh = true;
	_showDisparity = true;
	_showDepth = false;
	_showMap = true;
	std::cout << "Initialize camera.\n";
	return 0;
}

//カメラストリームの表示
void detectLandingPoint::streamingCamera()
{
	cv::imshow("LeftCameraStream", _frameLeft);
	cv::imshow("RightCameraStream", _frameRight);
}

//歪み補正と平行化変換
void detectLandingPoint::remapImage(cv::Mat* input_leftImg, cv::Mat* input_rightImg)
{
	cv::Mat rmapLeft, rmapRight;
	//歪み補正マップをもとにカメラ画像の補正と平行化変換を行う
	cv::remap(_frameLeft, rmapLeft, _rmap[0][0], _rmap[0][1], cv::INTER_LINEAR);
	cv::remap(_frameRight, rmapRight, _rmap[1][0], _rmap[1][1], cv::INTER_LINEAR);
	rmapLeft.copyTo(_frameLeft);
	rmapRight.copyTo(_frameRight);
}

//テンプレート画像の取得
int detectLandingPoint::getTemplateImage(cv::Mat* input_img, cv::Rect* selectedRoi, cv::Mat* output_img)
{
	cv::Mat originalImg, templateImg;
	cv::Rect tempRoi;
	originalImg = *input_img;
	templateImg = *output_img;
	tempRoi = *selectedRoi;				//マウスでドラッグした範囲をテンプレート画像とする
	templateImg = originalImg(tempRoi);
	if(_showTemplate == true)
	{
		imshow("templateImg",templateImg);
	}
	*output_img = templateImg.clone();
	return 0;
}

//ヒストグラムの計算
int detectLandingPoint::calTempHist(cv::Mat* input_img, cv::Mat* output_hist)
{
	cv::Mat hsvTemp;
	cv::cvtColor(*input_img, hsvTemp, cv::COLOR_BGR2HSV);
	//cv::destroyWindow("input_template");
	//cv::imshow("input_template",*input_img);
	//テンプレート画像のヒストグラムの計算
	////色相を 30 分割レベルで，彩度を 32 分割レベルで量子化
	int hbins = 30, sbins = 32;
	int histSize[] = {hbins, sbins};
	////cvtColor にあるように，色相の範囲は 0 から 179
	float hranges[] = { 0, 180 };
	////彩度の範囲は 0 （黒-灰色-白）から 255 （純粋なスペクトルカラー）まで
	float sranges[] = { 0, 256 };
	const float* ranges[] = { hranges, sranges };
	cv::Mat hist;
	////0 番目と 1 番目のチャンネルからヒストグラムを求める．
	int channels[] = {0, 1};
	////二次元ヒストグラムの計算//マウスで選択中の部分のヒストグラムを計算する
	cv::calcHist( &hsvTemp, 1, channels, cv::Mat(), hist, 2, histSize, ranges, true, false );
	////ヒストグラムの最大値の計算
	double maxVal=0;
	cv::minMaxLoc(hist, 0, &maxVal, 0, 0);
	////ヒストグラムのスケーリングと描画
	int scale = 10;
	cv::Mat histImg = cv::Mat::zeros(sbins*scale, hbins*10, CV_8UC3);
	for( int h = 0; h < hbins; h++ )
	{
		for( int s = 0; s < sbins; s++ )
		{
			float binVal = hist.at<float>(h, s);
			int intensity = cvRound(binVal*255/maxVal);
			cv::rectangle( histImg, cv::Point(h*scale, s*scale), cv::Point( (h+1)*scale - 1, (s+1)*scale - 1), cv::Scalar::all(intensity), cv::FILLED );
		}
	}
	//cv::cvtColor(tempRoi, tempRoi, CV_HSV2BGR);
	if(_showHistogram == true)
	{
		cv::imshow("histogram", histImg);
	}
	*output_hist = hist.clone();
	return 0;
}

//頻出色値を求める
int* detectLandingPoint::getFrequentVal(cv::Mat* input_hist, int* frequentHue, int* frequentSat)
{
	cv::Mat input, copyHist;
	input = *input_hist;
	copyHist = input.clone();
	double tileMaxBin[3];
	cv::Point tileMaxLoc[3];
	for(int i = 0; i < 3; i++)
	{
		cv::minMaxLoc(copyHist, 0, tileMaxBin + i, 0, tileMaxLoc + i);
		//std::cout << "tileMax" << i << " = " << tileMaxBin[i] << ", " << tileMaxLoc[i] << "\n";	//出現頻度，色相，彩度
		frequentHue[i] = tileMaxLoc[i].y * 6 + 3;
		frequentSat[i] = tileMaxLoc[i].x * 8 + 4;
		//frequentHue[i] = tileMaxLoc[i].x * 6 + 3;
		//frequentSat[i] = tileMaxLoc[i].y * 8 + 4;
		//std::cout << "tileMaxHue" << i << " = " << frequentHue[i] << ", tileMaxSat" << i << " = " << frequentSat[i] << "\n\n";
		copyHist.at<float>(tileMaxLoc[i]) = 0.0;
	}
	return 0;
}

//頻出色値を求める
int* detectLandingPoint::getFrequentVal(cv::Mat* input_hist, cv::Vec2i* frequentColor)
{
	cv::Mat input, copyHist;
	input = *input_hist;
	copyHist = input.clone();
	double tileMaxBin[3];
	cv::Point tileMaxLoc[3];
	for(int i = 0; i < 3; i++)
	{
		cv::minMaxLoc(copyHist, 0, tileMaxBin + i, 0, tileMaxLoc + i);
		//std::cout << "tileMax" << i << " = " << tileMaxBin[i] << ", " << tileMaxLoc[i] << "\n";	//出現頻度，色相，彩度
		//frequentColor[0][i] = tileMaxLoc[i].y * 6 + 3;
		//frequentColor[1][i] = tileMaxLoc[i].x * 8 + 4;
		frequentColor[0][i] = tileMaxLoc[i].x * 6 + 3;
		frequentColor[1][i] = tileMaxLoc[i].y * 8 + 4;
		//std::cout << "tileMaxHue" << i << " = " << frequentColor[0][i] << ", tileMaxSat" << i << " = " << frequentColor[1][i] << "\n\n";
		copyHist.at<float>(tileMaxLoc[i]) = 0.0;
	}
	return 0;
}

//色距離の計算
int detectLandingPoint::calColorDistance(cv::Mat* input_img, int* frequentHue, int* frequentSat, cv::Mat* output_hueDistance, cv::Mat* output_satDistance)
{
	////色距離判定用の行列
	cv::Mat hueDistance(cv::Size(FRAMESIZE_W, FRAMESIZE_H), CV_8UC3, cv::Scalar::all(128));
	cv::Mat satDistance(cv::Size(FRAMESIZE_W, FRAMESIZE_H), CV_8UC3, cv::Scalar::all(128));
	cv::Mat hsvFrame, src_img;
	src_img = *input_img;
	//cv::imshow("src_img",src_img);
	//hueDistance.copySize(src_img);
	//satDistance.copySize(src_img);
	cv::cvtColor(src_img, hsvFrame, cv::COLOR_BGR2HSV);
	//cv::Mat channels[3];
	//cv::split(hsvFrame,channels);
	//cv::imshow("channel1",channels[0]);
	//cv::imshow("channel2",channels[1]);
	//cv::imshow("channel3",channels[2]);
	unsigned char frameHue, frameSat, frameVal, ahDistance, asDistance;
	double hDistance[3], hDistance1[3], hDistance2[3], sDistance[3], cDistance;
	//std::cout << "src_img.rows = " << src_img.rows << ", src_img.cols = " << src_img.cols << "\n";
	for(int y = 0; y < src_img.rows; y++)
	{
		for(int x = 0; x < src_img.cols; x++)
		{
			//カメラ画像中の1つのピクセルの色空間ベクトルを求める．
			cv::Vec3b pixColor = hsvFrame.at<cv::Vec3b>(y, x);
			frameHue = pixColor[0];
			frameSat = pixColor[1];
			frameVal = pixColor[2];
			//std::cout << "frameHue = " << frameHue << ", frameSat = " << frameSat << "\n";
			cDistance = 255;
			ahDistance = 255;
			asDistance = 255;

			//タイルの色空間ベクトルとカメラ画像の色空間ベクトルの距離を求める．
			for(int i = 0; i < 3; i++)
			{
				hDistance1[i] = frequentHue[i] - frameHue;
				//hDistance2[i] = hDistance1[i] + 180;
				//if(abs(hDistance1[i]) <= abs(hDistance2[i]))	hDistance[i] = hDistance1[i];
				//else											hDistance[i] = hDistance2[i];
				if(abs(hDistance1[i]) < 90)	hDistance[i] = abs(hDistance1[i]);
				else						hDistance[i] = 180 - abs(hDistance1[i]);
				//if(abs(hDistance1[i]) < 128)	hDistance[i] = abs(hDistance1[i]);
				//else							hDistance[i] = abs(hDistance1[i]) - 128;
				sDistance[i] = frequentSat[i] - frameSat;
				if(abs(sDistance[i]) < 128)	sDistance[i] = abs(sDistance[i]);
				else						sDistance[i] = 256 - abs(sDistance[i]);
				//std::cout << "hDistance" << i << " = " << hDistance[i] << ", sDistance" << i << " = " << sDistance[i] <<"\n";
				//double distance = sqrt(((hDistance[i] * hDistance[i]) + (sDistance[i] * sDistance[i])));
				//if(distance < cDistance) cDistance = distance;
				if(abs(hDistance[i]) < abs(ahDistance))	ahDistance = abs((unsigned char)hDistance[i]);
				if(abs(sDistance[i]) < abs(asDistance))	asDistance = abs((unsigned char)sDistance[i]);
			}
			//hueDistance.at<uchar>(y, x) = ahDistance;
			//satDistance.at<uchar>(y, x) = asDistance;
			cv::rectangle( hueDistance, cv::Point(x, y), cv::Point(x, y), cv::Scalar(ahDistance,ahDistance,ahDistance), cv::FILLED );
			cv::rectangle( satDistance, cv::Point(x, y), cv::Point(x, y), cv::Scalar(asDistance,asDistance,asDistance), cv::FILLED );
		}
	}
	*output_hueDistance = hueDistance.clone();
	*output_satDistance = satDistance.clone();
	//hueDistance.convertTo(hueDistance,CV_8U,255/45);
	//satDistance.convertTo(satDistance,CV_8U,255/64);
	//cv::imshow("hueDistance",hueDistance);
	//cv::imshow("satDistance",satDistance);
	//cv::imshow("output_hueDistance",*output_hueDistance);
	//cv::imshow("output_satDistance",*output_satDistance);
	return 0;
}

//色距離の計算
int detectLandingPoint::calColorDistance(cv::Mat* input_img, cv::Vec2i* frequentColor, cv::Mat* output_hueDistance, cv::Mat* output_satDistance)
{
	////色距離判定用の行列
	cv::Mat hueDistance(cv::Size(FRAMESIZE_W, FRAMESIZE_H), CV_8UC3, cv::Scalar::all(128));
	cv::Mat satDistance(cv::Size(FRAMESIZE_W, FRAMESIZE_H), CV_8UC3, cv::Scalar::all(128));
	cv::Mat hsvFrame, src_img;
	src_img = *input_img;
	//hueDistance.copySize(src_img);
	//satDistance.copySize(src_img);
	cv::cvtColor(src_img, hsvFrame, cv::COLOR_BGR2HSV);
	unsigned char frameHue, frameSat, frameVal, ahDistance, asDistance;
	double hDistance[3], hDistance1[3], hDistance2[3], sDistance[3], cDistance;
	for(int y = 0; y < src_img.rows; y++)
	{
		for(int x = 0; x < src_img.cols; x++)
		{
			//カメラ画像中の1つのピクセルの色空間ベクトルを求める．
			cv::Vec3b pixColor = hsvFrame.at<cv::Vec3b>(y, x);
			frameHue = pixColor[0];
			frameSat = pixColor[1];
			frameVal = pixColor[2];
			//std::cout << "frameHue = " << frameHue << ", frameSat = " << frameSat << "\n";
			cDistance = 255;
			ahDistance = 255;
			asDistance = 255;

			//タイルの色空間ベクトルとカメラ画像の色空間ベクトルの距離を求める．
			for(int i = 0; i < 3; i++)
			{
				hDistance1[i] = frequentColor[0][i] - frameHue;
				//hDistance2[i] = hDistance1[i] + 180;
				//if(abs(hDistance1[i]) <= abs(hDistance2[i]))	hDistance[i] = hDistance1[i];
				//else											hDistance[i] = hDistance2[i];
				if(abs(hDistance1[i]) < 90)	hDistance[i] = abs(hDistance1[i]);
				else						hDistance[i] = abs(hDistance1[i]) - 90;
				sDistance[i] = frequentColor[1][i] - frameSat;
				//std::cout << "hDistance" << i << " = " << hDistance[i] << ", sDistance" << i << " = " << sDistance[i] <<"\n";
				double distance = sqrt(((hDistance[i] * hDistance[i]) + (sDistance[i] * sDistance[i])));
				if(distance < cDistance) cDistance = distance;
				if(abs(hDistance[i]) < abs(ahDistance))	ahDistance = (unsigned char)hDistance[i];
				if(abs(sDistance[i]) < abs(asDistance))	asDistance = (unsigned char)sDistance[i];
			}
			//hueDistance.at<uchar>(y, x) = ahDistance;
			//satDistance.at<uchar>(y, x) = asDistance;
			cv::rectangle( hueDistance, cv::Point(x, y), cv::Point(x, y), cv::Scalar(ahDistance,ahDistance,ahDistance), cv::FILLED );
			cv::rectangle( satDistance, cv::Point(x, y), cv::Point(x, y), cv::Scalar(asDistance,asDistance,asDistance), cv::FILLED );
		}
	}
	*output_hueDistance = hueDistance.clone();
	*output_satDistance = satDistance.clone();
	return 0;
}

//二値化画像の作成
int detectLandingPoint::makeBinaryImage(cv::Mat* input_hueDistance, cv::Mat* input_satDistance, int hueThreshold, int satThreshold, cv::Mat* output_binaryImage)
{
	cv::Mat hue_img, sat_img, colorDistance(cv::Size(FRAMESIZE_W, FRAMESIZE_H), CV_8UC3, cv::Scalar::all(128));
	//hue_img = * input_hueDistance;
	//sat_img = * input_satDistance;
	colorDistance = _frameLeft.clone();		//L
	//colorDistance = _frameRight.clone();	//R
	double hDistance, sDistance;
	cv::cvtColor(* input_hueDistance, hue_img, cv::COLOR_BGR2GRAY);
	cv::cvtColor(* input_satDistance, sat_img, cv::COLOR_BGR2GRAY);
	for(int y = 0; y < hue_img.rows; y++)
	{
		for(int x = 0; x < hue_img.cols; x++)
		{
			hDistance = hue_img.at<uchar>(y,x);
			sDistance = sat_img.at<uchar>(y,x);
			//色距離が一定値以下ならば，そのピクセルの値を0に，それ以外なら255にする．
			if(abs(hDistance) <= hueThreshold && abs(sDistance) <= satThreshold)
				cv::rectangle( colorDistance, cv::Point(x, y), cv::Point(x, y), cv::Scalar(255,255,255), cv::FILLED );
			else
				cv::rectangle( colorDistance, cv::Point(x, y), cv::Point(x, y), cv::Scalar(0,0,0), cv::FILLED );
			//if(abs(hDistance) <= hueThreshold && abs(sDistance) <= satThreshold)
			//	cv::rectangle( colorDistance, cv::Point(y,x), cv::Point(y,x), cv::Scalar(255,255,255), CV_FILLED );
			//else
			//	cv::rectangle( colorDistance, cv::Point(y,x), cv::Point(y,x), cv::Scalar(0,0,0), CV_FILLED );
		}
	}
	cv::medianBlur(colorDistance, colorDistance, 5);
	cv::imshow("colorDistance",colorDistance);
	*output_binaryImage = colorDistance.clone();
	return 0;
}

//二値化画像をメッシュに区切る
int detectLandingPoint::makeMeshImage(cv::Mat* input_img, cv::Point3d cameraPosition, double radsOfCameraDip, std::vector<cv::Point2d>* tilePosition2d)
{
	//メッシュを作成する．
	cv::Mat bin_img, src_img, pers_img, mosaic/*, inpers_img*/;
	//std::vector<cv::Point2d> tilePosi;
	bin_img = *input_img;
	src_img = bin_img.clone();
	double ths, thf, thp, cameraH, existenceTile, halfTileWidth, halfTileHeight;
	ths = radsOfCameraDip;							//カメラの俯角
	thf = _fovy * CV_PI / 180 / 2;					//カメラの縦方向の視野角
	thp = _fovx * CV_PI / 180 / 2;					//カメラの横方向の視野角
	cameraH = -cameraPosition.z + BODYHEIGHT;		//カメラの床面からの高さ
	existenceTile = EXISTENCE_TILE;					//メッシュ内におけるタイルの割合の閾値．タイルの割合がこの値以上のときメッシュの交点部分にタイルが存在すると判定する．
	halfTileWidth = TILE_SIZE / MESH_SIZE / 2;
	halfTileHeight = TILE_SIZE / MESH_SIZE / 2;
	cv::Point2f pts1[] = {cv::Point2f(0,0), cv::Point2f(0,FRAMESIZE_H), cv::Point2f(FRAMESIZE_W,FRAMESIZE_H), cv::Point2f(FRAMESIZE_W,0)};
	cv::Point2f pts2[] = {cv::Point2f(0,0), cv::Point2f((float)(-FRAMESIZE_W/2*sin(ths-thf)/sin(ths+thf)+FRAMESIZE_W/2),FRAMESIZE_H), cv::Point2f((float)(FRAMESIZE_W/2*sin(ths-thf)/sin(ths+thf)+FRAMESIZE_W/2),FRAMESIZE_H), cv::Point2f(FRAMESIZE_W,0)};
	// 透視変換行列を計算
	cv::Mat perspective_matrix = cv::getPerspectiveTransform(pts1, pts2);
	cv::Mat in_perspective_matrix = cv::getPerspectiveTransform(pts2, pts1);
	// 変換
	//cv::warpPerspective(_frameLeft, pers_img, perspective_matrix, src_img.size(), cv::INTER_LINEAR);
	//cv::imshow("pers_img",pers_img);
	cv::warpPerspective(src_img, pers_img, perspective_matrix, src_img.size(), cv::INTER_LINEAR);
	cv::Mat persCopy = pers_img.clone();
	cv::Point2f after;
	//int lineX = 2*cameraH*tan(thp)/(sin(ths-thf)*MESH_SIZE);						//縦線の本数
	//int lineY = 2*cameraH*(tan(CV_PI-(ths-thf))-tan(CV_PI-(ths+thf)))/MESH_SIZE;	//横線の本数
	int lineX = 2*cameraH*tan(thp)*cos(thf)/sin(ths-thf)/MESH_SIZE;						//縦線の本数
	int lineY = cameraH*(1/tan(ths-thf)-1/tan(ths+thf))/MESH_SIZE;	//横線の本数
	double meshX = FRAMESIZE_W / (double)(lineX);
	double meshY = FRAMESIZE_H / (double)(lineY);
	//double meshX = FRAMESIZE_W / (2*cameraH*tan(thp)/(sin(ths-thf)*MESH_SIZE));
	//double meshY = FRAMESIZE_H / (cameraH*(tan(CV_PI-(ths-thf))-tan(CV_PI-(ths+thf)))/MESH_SIZE);
	//std::cout << "line : " << lineX << " X " << lineY << "\n";
	//std::cout << "mesh : " << meshX << " X " << meshY << "\n";
	//cv::resize(pers_img, mosaic, cv::Size(lineX, lineY), 0, 0, /*CV_INTER_NN*/cv::INTER_LINEAR/*cv::INTER_NEAREST*/);
	cv::Mat dst_img;
	//cv::resize(mosaic, dst_img, cv::Size(FRAMESIZE_W, FRAMESIZE_H), 0, 0, /*CV_INTER_NN*/cv::INTER_NEAREST);
	//cv::imshow("bmosaicImg", mosaic);
	for(double /*i = 1*/i = halfTileWidth; i < /*lineX+1*/lineX-halfTileWidth; i++)
	{
		for(double /*j = 1*/j = halfTileHeight; j < /*lineY+1*/lineY-halfTileHeight; j++)
		{
			cv::line(persCopy,cv::Point(0,cvRound(j*meshY)),cv::Point(640,cvRound(j*meshY)), cv::Scalar(128,255,128));
			cv::line(persCopy,cv::Point(cvRound(i*meshX),0),cv::Point(cvRound(i*meshX),480), cv::Scalar(128,255,128));
		}
	}
	for(double /*i = 1*/i = halfTileWidth; i < /*lineX+1*/lineX-halfTileWidth; i++)
	{
		for(double /*j = 1*/j = halfTileHeight; j < /*lineY+1*/lineY-halfTileHeight; j++)
		{
			int channel3ave = 0;
			int k = 0;
			for(double v = cvRound((/*i-1.0*/i-halfTileWidth)* meshX); v < cvRound((i+halfTileWidth)* meshX); v++)
			{
				for(double w = cvRound((/*j-1.0*/j-halfTileHeight)* meshY); w < cvRound((j+halfTileHeight)* meshY); w++)
				{
					//cv::Vec3b pixColor = pers_img.at<cv::Vec3b>(j* meshY, i* meshX);
					cv::Vec3b pixColor = pers_img.at<cv::Vec3b>((int)w, (int)v);
					unsigned char channel1 = pixColor[0];
					unsigned char channel2 = pixColor[1];
					unsigned char channel3 = pixColor[2];
					channel3ave += channel3;
					k++;
					//if(v > (i+0.0)* meshX-1 && w > (j+0.0)* meshY -1)
					//std::cout << v << " , " << w << "\n";
				}
			}
			channel3ave = channel3ave / k;
			if(channel3ave >= 255 * existenceTile)
			{
				after.x = (in_perspective_matrix.at<double>(0,0) * meshX * (i/*-0.5*/) + in_perspective_matrix.at<double>(0,1) * meshY * (j/*-0.5*/) + in_perspective_matrix.at<double>(0,2))
					/ (in_perspective_matrix.at<double>(2,0) * meshX * (i/*-0.5*/) + in_perspective_matrix.at<double>(2,1) * meshY * (j/*-0.5*/) + in_perspective_matrix.at<double>(2,2));
				after.y = (in_perspective_matrix.at<double>(1,0) * meshX * (i/*-0.5*/) + in_perspective_matrix.at<double>(1,1) * meshY * (j/*-0.5*/) + in_perspective_matrix.at<double>(1,2))
					/ (in_perspective_matrix.at<double>(2,0) * meshX * (i/*-0.5*/) + in_perspective_matrix.at<double>(2,1) * meshY * (j/*-0.5*/) + in_perspective_matrix.at<double>(2,2));
				cv::Point2f p1 = cv::Point2f(cvRound(meshX * (i/*-0.5*//*+1.5*/)), cvRound(meshY * (j/*-0.5*//*+1.5*/)));
				cv::rectangle(persCopy,  p1,  p1, cv::Scalar(255,0,255), 3);
				cv::rectangle(src_img,  after,  after, cv::Scalar(255,0,255), 3);
				//std::cout << after << "\n";
				tilePosition2d->push_back(after);
			}
			//else
			//{
			//	cv::Point2f p2 = cv::Point2f(cvRound(meshX * (i/*-0.5*//*+1.5*/)), cvRound(meshY * (j/*-0.5*//*+1.5*/)));
			//	cv::rectangle(persCopy,  p2,  p2, cv::Scalar(255,255,0), 2);
			//}
			//画像をモザイク化して処理
			//cv::Vec3b pixColor = mosaic.at<cv::Vec3b>(j, i);
			//unsigned char channel1 = pixColor[0];
			//unsigned char channel2 = pixColor[1];
			//unsigned char channel3 = pixColor[2];
			//if(channel3 > 255 * 0.75)
			//{
			//	after.x = (in_perspective_matrix.at<double>(0,0) * meshX * i + in_perspective_matrix.at<double>(0,1) * meshY * j + in_perspective_matrix.at<double>(0,2))
			//		/ (in_perspective_matrix.at<double>(2,0) * meshX * i + in_perspective_matrix.at<double>(2,1) * meshY * j + in_perspective_matrix.at<double>(2,2));
			//	after.y = (in_perspective_matrix.at<double>(1,0) * meshX * i + in_perspective_matrix.at<double>(1,1) * meshY * j + in_perspective_matrix.at<double>(1,2))
			//		/ (in_perspective_matrix.at<double>(2,0) * meshX * i + in_perspective_matrix.at<double>(2,1) * meshY * j + in_perspective_matrix.at<double>(2,2));
			//	cv::Point2f p1 = cv::Point2f(cvRound(meshX * (i/*+1.5*/) + meshX/2.0), cvRound(meshY * (j/*+1.5*/) + meshY/2.0));
			//	cv::rectangle(pers_img,  p1,  p1, cv::Scalar(255,0,255), 3);
			//	cv::rectangle(mosaic,  cv::Point2f(i,j),  cv::Point2f(i,j), cv::Scalar(255,255,0), 1);
			//	cv::rectangle(dst_img,  p1,  p1, cv::Scalar(255,0,255), 3);
			//	std::cout << after << "\n";
			//	tilePosition2d->push_back(after);
			//}
		}
	}
	//std::cout << tilePosition2d->size() /*<< "\t" << tilePosition2d->at(1) << "\t" << tilePosition2d->at(2) */<< "\n";
	if(_showMesh == true)
	{
		cv::imshow("meshImg",persCopy);
	}
	cv::imshow("colorDistance",src_img);
	//cv::imshow("mosaicImg", mosaic);
	//cv::imshow("largeMosaic",dst_img);
	if(tilePosition2d->size() == 0)
	{
		std::cout << "Not found tiles.\n";
		return 1;
	}

	return 0;
}

//カメラ画像中のタイルの座標の取得
int detectLandingPoint::tilePositionOnImage(cv::Mat* input_meshedImage, std::vector<cv::Point2d>* tilePosition2d)
{
	//メッシュ処理に統合
	return 0;
}

//ステレオマッチング
int detectLandingPoint::stereoMatching(cv::Mat* input_cameraFrameLeft, cv::Mat* input_cameraFrameRight, cv::Mat* output_disparty)
{
	//ステレオマッチ用画像の準備
	cv::Mat imgLeft, imgRight;
	//imgLeft = _frameLeft.clone();
	//imgRight = _frameRight.clone();
	cv::Mat srcLeft, srcRight;
	srcLeft = *input_cameraFrameLeft;
	srcRight = *input_cameraFrameRight;
	imgLeft = srcLeft.clone();
	imgRight = srcRight.clone();
	//cv::cvtColor(_frameLeft, imgLeft, CV_BGR2GRAY);		//入力画像のグレースケール化
	//cv::cvtColor(_frameRight, imgRight, CV_BGR2GRAY);	//
	//cv::cvtColor(_frameLeft, imgLeft, CV_BGR2HSV);		//入力画像のHSV化
	//cv::cvtColor(_frameRight, imgRight, CV_BGR2HSV);	//
	if( imgLeft.empty() || imgRight.empty() )
	{
		std::cout<< " --(!) Error reading images " << std::endl;
		return -1;
	}
	//視差画像を保存
	cv::Mat imgDisparity16S = cv::Mat( imgLeft.rows, imgLeft.cols, CV_16S );	//16倍された視差画像
	cv::Mat imgDisparity8U = cv::Mat( imgLeft.rows, imgLeft.cols, CV_8UC1 );	//確認用視差画像

	//ステレオマッチ用コンストラクタの呼び出し
	int ndisparitiesSGBM = 16*11;   /**< Range of disparity */
	int SADWindowSizeSGBM = 9;		/**< Size of the block window. Must be odd */
	Ptr<cv::StereoSGBM> ssgbm = cv::StereoSGBM::create(0, ndisparitiesSGBM, SADWindowSizeSGBM, 8*3*SADWindowSizeSGBM*SADWindowSizeSGBM, 32*3*SADWindowSizeSGBM*SADWindowSizeSGBM);
	ssgbm->setSpeckleWindowSize(200);	//ノイズスペックルや無効なピクセルが考慮されたなめらかな視差領域の最大サイズ．
	ssgbm->setSpeckleRange(16);			//それぞれの連結成分における最大視差値．

	//ステレオマッチ
	ssgbm->compute(imgLeft, imgRight, imgDisparity16S);

	//視差画像の確認
	double minVal, maxVal;
	cv::minMaxLoc( imgDisparity16S, &minVal, &maxVal );
	//std::cout << "Min disp: " << minVal	<< " Max value: " << maxVal << "\n";
	imgDisparity16S.convertTo( imgDisparity8U, CV_8UC1, 255/(maxVal - minVal));
	if(_showDisparity == true)
	{
		cv::imshow("disparityImg", imgDisparity8U);
	}
	cv::Mat disparityMap;
	imgDisparity16S.convertTo( disparityMap, CV_32F, 1.0/16.0);
	* output_disparty = disparityMap.clone();

	return 0;
}

//三次元再投影
int detectLandingPoint::reproject3dPosition(cv::Mat* input_disparty, cv::Mat* output_3dImage)
{
	cv::Mat disparityMap, reprojectImage;
	disparityMap = input_disparty->clone();
	std::vector<cv::Point3d>objectPoints;
	cv::reprojectImageTo3D(disparityMap, reprojectImage, _Q);
	cv::Mat coordinate[3];
	cv::split(reprojectImage, coordinate);
	float x, y, z;
	cv::Mat depth = cv::Mat(reprojectImage.size(), CV_8UC1, Scalar::all(255));
	double minDepth, maxDepth;
	const double depthLimit = 10000;
	cv::minMaxLoc( reprojectImage, &minDepth, &maxDepth );
	if(minDepth < 0) minDepth = 0;
	//std::cout << "_baselineRatio = " << _baselineRatio << "\n";
	for(int i=0; i<reprojectImage.rows; i++)
	{
		for(int j=0; j<reprojectImage.cols; j++)
		{
			x = coordinate[0].at<float>(i,j) * _baselineRatio;
			y = coordinate[1].at<float>(i,j) * _baselineRatio;
			z = coordinate[2].at<float>(i,j) * _baselineRatio;
			coordinate[0].at<float>(i,j) = x;
			coordinate[1].at<float>(i,j) = y;
			coordinate[2].at<float>(i,j) = z;
			objectPoints.push_back(cv::Point3d(x, y, z));
			if(z > depthLimit)
				depth.at<uchar>(i,j) = 0;
			else
				depth.at<uchar>(i,j) = 255 - (z - minDepth) * 255 / (depthLimit - minDepth);
		}
	}
	if(_showDepth == true)
	{
		cv::imshow("depthImage", depth);
	}
	*output_3dImage = reprojectImage.clone();
	return 0;
}

//ロボット座標系におけるタイルの三次元座標を求める
int detectLandingPoint::coordinateOfTile3d(cv::Mat* input_3dImageOnCamera, std::vector<cv::Point2d>* tilePosition2d, cv::Point3d cameraPosition, double radsOfCameraDip, std::vector<cv::Point3d>* output_tilePosition3d)
{
	int tileNum = tilePosition2d->size();
	cv::Point2d tilePosiSQ;
	cv::Point3d tilePosiCU;
	cv::Mat coordinate[3];
	float c = cos((float)radsOfCameraDip), s = sin((float)radsOfCameraDip);
	cv::Mat Rot = (cv::Mat_<float>(3,3) << 1.0, 0.0, 0.0, 0.0, c, s, 0.0, -s, c);
	//cv::Mat Rot = (cv::Mat_<float>(3,3) << 0.0, 1.0, 0.0, -s, 0.0, -c, -c, 0.0, s);
	cv::Mat Trans = (cv::Mat_<float>(3,1) << cameraPosition.x, cameraPosition.y, cameraPosition.z);
	cv::split(*input_3dImageOnCamera, coordinate);
	if(tileNum == 0)
	{
		std::cout << "No tile was found.\n";
		return 1;
	}
	for(int i = 0; i < tileNum; i++)
	{
		tilePosiSQ = tilePosition2d->at(i);
		tilePosiCU.x = coordinate[0].at<float>(tilePosiSQ) * _baselineRatio;
		tilePosiCU.y = coordinate[1].at<float>(tilePosiSQ) * _baselineRatio;
		tilePosiCU.z = coordinate[2].at<float>(tilePosiSQ) * _baselineRatio;
		//std::cout << "tileNum" << i << " : " << tilePosiCU << "->";
		cv::Mat tilePosi = (cv::Mat_<float>(3,1) << tilePosiCU.x, tilePosiCU.y, tilePosiCU.z);
		cv::Mat tilePosi2;
		tilePosi2 = Rot * tilePosi;
		//tilePosi2 = /*tilePosi * */Rot * tilePosi + Trans;
		tilePosiCU.x = tilePosi2.at<float>(2,0) + cameraPosition.x;
		tilePosiCU.y = tilePosi2.at<float>(0,0) + cameraPosition.y;
		tilePosiCU.z = tilePosi2.at<float>(1,0) + cameraPosition.z;
		double tilePosiCU_L = std::sqrt(tilePosiCU.x * tilePosiCU.x + tilePosiCU.y * tilePosiCU.y + tilePosiCU.z * tilePosiCU.z);
		//if(tilePosiCU.z>0)
		//if(tilePosiCU.z > (BODYHEIGHT - ERROR_LIMIT_Z) && tilePosiCU.z < (BODYHEIGHT + ERROR_LIMIT_Z))
		//if(tilePosiCU.x < DETECT_LIMIT_X)
		//if(tilePosiCU_L < DETECT_LIMIT_L)
		if(tilePosiCU.z > (BODYHEIGHT - ERROR_LIMIT_Z) && tilePosiCU.z < (BODYHEIGHT + ERROR_LIMIT_Z) && tilePosiCU.x < DETECT_LIMIT_X_MAX && tilePosiCU.x > DETECT_LIMIT_X_MIN)
		{
			//tilePosiCU.x = tilePosi2.at<float>(0,0);
			//tilePosiCU.y = tilePosi2.at<float>(1,0);
			//tilePosiCU.z = tilePosi2.at<float>(2,0);
			//std::cout << tilePosiCU << "\n";
			output_tilePosition3d->push_back(tilePosiCU);
		}
	}
	return 0;
}

//脚接地候補点座標の記憶
int detectLandingPoint::saveLnadingPoint(std::vector<cv::Point3d>* input_newLandingPoint, std::vector<cv::Point3d>* allLandingPoint, cv::Mat* landingPointMap)
{
	cv::Mat LPMap = cv::Mat::zeros(MAP_SIZE / MAP_MESH, MAP_SIZE / MAP_MESH, CV_32FC4);
	int foundPoint = input_newLandingPoint->size();
	int allPoint = allLandingPoint->size();
	for(int j = 0; j < allPoint; j++)
	{
		int m = (MAP_SIZE / 2 - (allLandingPoint->at(j).x)) / MAP_MESH;
		int n = (allLandingPoint->at(j).y + MAP_SIZE / 2) / MAP_MESH;
		
		if(m > MAP_SIZE/MAP_MESH || n > MAP_SIZE/MAP_MESH || m < 0 || n < 0)
		{
			std::cout << "Out of MAP : oldFoundPoint" << j << "\n";
		}
		else
		{
			if(LPMap.at<cv::Vec4f>(m,n)[3] == 0)	//未検出のタイル
			{
				LPMap.at<cv::Vec4f>(m,n)[0] = allLandingPoint->at(j).x;
				LPMap.at<cv::Vec4f>(m,n)[1] = allLandingPoint->at(j).y;
				LPMap.at<cv::Vec4f>(m,n)[2] = allLandingPoint->at(j).z;
				LPMap.at<cv::Vec4f>(m,n)[3] = j + 1;
			}
			else	//検出済みのタイル
			{
				LPMap.at<cv::Vec4f>(m,n)[0] = (allLandingPoint->at(j).x + LPMap.at<cv::Vec4f>(m,n)[0]) / 2;
				LPMap.at<cv::Vec4f>(m,n)[1] = (allLandingPoint->at(j).y + LPMap.at<cv::Vec4f>(m,n)[1]) / 2;
				LPMap.at<cv::Vec4f>(m,n)[2] = (allLandingPoint->at(j).z + LPMap.at<cv::Vec4f>(m,n)[2]) / 2;
			}
		}
	}
	int k = 0;
	for(int i = 0; i < foundPoint; i++)
	{
		int v = (MAP_SIZE / 2 - (input_newLandingPoint->at(i).x)) / MAP_MESH;
		int w = (input_newLandingPoint->at(i).y + MAP_SIZE / 2) / MAP_MESH;
		if(v > MAP_SIZE/MAP_MESH || w > MAP_SIZE/MAP_MESH || v < 0 || w < 0)
		{
			std::cout << "Out of MAP : foundPoint" << i << "\n";
		}
		else
		{
			if(LPMap.at<cv::Vec4f>(v,w)[3] == 0)	//未検出のタイル
			{
				LPMap.at<cv::Vec4f>(v,w)[0] = input_newLandingPoint->at(i).x;
				LPMap.at<cv::Vec4f>(v,w)[1] = input_newLandingPoint->at(i).y;
				LPMap.at<cv::Vec4f>(v,w)[2] = input_newLandingPoint->at(i).z;
				LPMap.at<cv::Vec4f>(v,w)[3] = k + allPoint + 1;
				k++;
				allLandingPoint->push_back(input_newLandingPoint->at(i));
			}
			else	//検出済みのタイル
			{
				LPMap.at<cv::Vec4f>(v,w)[0] = (input_newLandingPoint->at(i).x + LPMap.at<cv::Vec4f>(v,w)[0]) / 2;
				LPMap.at<cv::Vec4f>(v,w)[1] = (input_newLandingPoint->at(i).y + LPMap.at<cv::Vec4f>(v,w)[1]) / 2;
				LPMap.at<cv::Vec4f>(v,w)[2] = (input_newLandingPoint->at(i).z + LPMap.at<cv::Vec4f>(v,w)[2]) / 2;
				allLandingPoint->at(LPMap.at<cv::Vec4f>(v,w)[3] - 1).x = LPMap.at<cv::Vec4f>(v,w)[0];
				allLandingPoint->at(LPMap.at<cv::Vec4f>(v,w)[3] - 1).y = LPMap.at<cv::Vec4f>(v,w)[1];
				allLandingPoint->at(LPMap.at<cv::Vec4f>(v,w)[3] - 1).z = LPMap.at<cv::Vec4f>(v,w)[2];
			}
		}
	}
	* landingPointMap = LPMap.clone();
	return 0;
}

//脚接地候補点座標の記憶(半径)
int detectLandingPoint::saveLnadingPointRound(std::vector<cv::Point3d>* input_newLandingPoint, std::vector<cv::Point3d>* allLandingPoint, cv::Mat* landingPointMap)
{
	cv::Mat LPMap = cv::Mat::zeros(MAP_SIZE / MAP_MESH, MAP_SIZE / MAP_MESH, CV_32FC4);
	int foundPoint = input_newLandingPoint->size();
	int allPoint = allLandingPoint->size();

	for(int i = 0; i < foundPoint; i++)
	{
		int alreadyDetected = 0;
		correctDisparity(&input_newLandingPoint->at(i).x, &input_newLandingPoint->at(i).y, &input_newLandingPoint->at(i).z, DEG40);	//視差の補正

		for(int j = 0; j < allPoint; j++)
		{
			double distance;
			distance = std::pow(allLandingPoint->at(j).x - input_newLandingPoint->at(i).x,2.0) + std::pow(allLandingPoint->at(j).y - input_newLandingPoint->at(i).y,2.0);
			distance = std::sqrt(distance);
			if(distance < MAP_MESH)
			{
				alreadyDetected++;
				if(distance < MAP_MESH / 2)
				{
					allLandingPoint->at(j).x = (allLandingPoint->at(j).x + input_newLandingPoint->at(i).x) / 2;
					allLandingPoint->at(j).y = (allLandingPoint->at(j).y + input_newLandingPoint->at(i).y) / 2;
					allLandingPoint->at(j).z = (allLandingPoint->at(j).z + input_newLandingPoint->at(i).z) / 2;
				}
				//break;
			}
		}

		if(alreadyDetected == 0)
		{
			allLandingPoint->push_back(input_newLandingPoint->at(i));
		}

		allPoint = allLandingPoint->size();
	}

	for(int j = 0; j < allPoint; j++)
	{
		int m = (MAP_SIZE / 2 - (allLandingPoint->at(j).x)) / MAP_MESH;
		int n = (allLandingPoint->at(j).y + MAP_SIZE / 2) / MAP_MESH;
		
		if(m > MAP_SIZE/MAP_MESH || n > MAP_SIZE/MAP_MESH || m < 0 || n < 0)
		{
			std::cout << "Out of MAP : oldFoundPoint" << j << "\n";
		}
		else
		{
			if(LPMap.at<cv::Vec4f>(m,n)[3] == 0)	//未検出のタイル
			{
				LPMap.at<cv::Vec4f>(m,n)[0] = allLandingPoint->at(j).x;
				LPMap.at<cv::Vec4f>(m,n)[1] = allLandingPoint->at(j).y;
				LPMap.at<cv::Vec4f>(m,n)[2] = allLandingPoint->at(j).z;
				LPMap.at<cv::Vec4f>(m,n)[3] = j + 1;
			}
			else	//検出済みのタイル
			{
				LPMap.at<cv::Vec4f>(m,n)[0] = (allLandingPoint->at(j).x + LPMap.at<cv::Vec4f>(m,n)[0]) / 2;
				LPMap.at<cv::Vec4f>(m,n)[1] = (allLandingPoint->at(j).y + LPMap.at<cv::Vec4f>(m,n)[1]) / 2;
				LPMap.at<cv::Vec4f>(m,n)[2] = (allLandingPoint->at(j).z + LPMap.at<cv::Vec4f>(m,n)[2]) / 2;
			}
		}
	}
	//int k = 0;
	//for(int i = 0; i < foundPoint; i++)
	//{
	//	int v = (MAP_SIZE / 2 - (input_newLandingPoint->at(i).x)) / MAP_MESH;
	//	int w = (input_newLandingPoint->at(i).y + MAP_SIZE / 2) / MAP_MESH;
	//	if(v > MAP_SIZE/MAP_MESH || w > MAP_SIZE/MAP_MESH || v < 0 || w < 0)
	//	{
	//		std::cout << "Out of MAP : foundPoint" << i << "\n";
	//	}
	//	else
	//	{
	//		if(LPMap.at<cv::Vec4f>(v,w)[3] == 0)	//未検出のタイル
	//		{
	//			LPMap.at<cv::Vec4f>(v,w)[0] = input_newLandingPoint->at(i).x;
	//			LPMap.at<cv::Vec4f>(v,w)[1] = input_newLandingPoint->at(i).y;
	//			LPMap.at<cv::Vec4f>(v,w)[2] = input_newLandingPoint->at(i).z;
	//			LPMap.at<cv::Vec4f>(v,w)[3] = k + allPoint + 1;
	//			k++;
	//			allLandingPoint->push_back(input_newLandingPoint->at(i));
	//		}
	//		else	//検出済みのタイル
	//		{
	//			LPMap.at<cv::Vec4f>(v,w)[0] = (input_newLandingPoint->at(i).x + LPMap.at<cv::Vec4f>(v,w)[0]) / 2;
	//			LPMap.at<cv::Vec4f>(v,w)[1] = (input_newLandingPoint->at(i).y + LPMap.at<cv::Vec4f>(v,w)[1]) / 2;
	//			LPMap.at<cv::Vec4f>(v,w)[2] = (input_newLandingPoint->at(i).z + LPMap.at<cv::Vec4f>(v,w)[2]) / 2;
	//			allLandingPoint->at(LPMap.at<cv::Vec4f>(v,w)[3] - 1).x = LPMap.at<cv::Vec4f>(v,w)[0];
	//			allLandingPoint->at(LPMap.at<cv::Vec4f>(v,w)[3] - 1).y = LPMap.at<cv::Vec4f>(v,w)[1];
	//			allLandingPoint->at(LPMap.at<cv::Vec4f>(v,w)[3] - 1).z = LPMap.at<cv::Vec4f>(v,w)[2];
	//		}
	//	}
	//}
	* landingPointMap = LPMap.clone();
	return 0;
}

//脚接地候補点座標の再計算
int detectLandingPoint::recalLnadingPoint(std::vector<cv::Point3d>* allLandingPoint, myvector::VECTOR* movementAmount)
{
	int allPoint = allLandingPoint->size();
	for(int i = 0; i < allPoint; i++)
	{
		allLandingPoint->at(i) = allLandingPoint->at(i) - cv::Point3d(movementAmount->x, movementAmount->y, movementAmount->z);
	}
	return 0;
}
//脚接地候補点座標の再計算
int detectLandingPoint::recalLnadingPoint2(std::vector<cv::Point3d>* allLandingPoint, myvector::VECTOR MovementAmount, myvector::VECTOR Centerposition, myvector::VECTOR Robotposture)
{
	int allPoint = allLandingPoint->size();
	//for(int i = 0; i < allPoint; i++)
	//{
	//	allLandingPoint->at(i) = allLandingPoint->at(i) - cv::Point3d(movementAmount->x, movementAmount->y, movementAmount->z);
	//}
	myvector::VECTOR temp,temp2;
	for(int i = 0;i < allPoint; i++ ){	
		temp.x = allLandingPoint->at(i).x;
		temp.y = allLandingPoint->at(i).y;
		temp.z = allLandingPoint->at(i).z;
		temp2 = myvector::VRot(temp,Centerposition, -Robotposture.x, -Robotposture.y, -Robotposture.z);
		temp2 = myvector::VAdd(temp2,Centerposition);
		temp2 = myvector::VSub(temp2,myvector::VAdd(Centerposition,MovementAmount));
		temp2 = myvector::VRot(temp2,myvector::VAdd(Centerposition,MovementAmount), Robotposture.x, Robotposture.y, Robotposture.z);
		temp = myvector::VSub(temp2, temp);
		allLandingPoint->at(i) = allLandingPoint->at(i) - cv::Point3d(temp.x, temp.y, temp.z);
		//dummyMapDate[i].x -= movementAmount.x;
		//dummyMapDate[i].y -= movementAmount.y;
		//dummyMapDate[i].z -= movementAmount.z;
	}
	return 0;
}

//脚接地候補点座標の再計算
int detectLandingPoint::rotationLnadingPoint(std::vector<cv::Point3d>* allLandingPoint, myvector::VECTOR* rotationAmount, myvector::VECTOR Centerposition)
{
	int allPoint = allLandingPoint->size();
	myvector::VECTOR temp;
	for(int i = 0;i < allPoint; i++ ){
		temp.x = allLandingPoint->at(i).x;
		temp.y = allLandingPoint->at(i).y;
		temp.z = allLandingPoint->at(i).z;
		temp = myvector::VRot(temp,Centerposition,rotationAmount->x,rotationAmount->y,rotationAmount->z);
		allLandingPoint->at(i) = allLandingPoint->at(i) - cv::Point3d(temp.x, temp.y, temp.z);
	}
	return 0;
}

//脚接地候補点座標の受け渡し
int detectLandingPoint::sendLandingPoint(cv::Mat* landingPointMap, std::vector<myvector::VECTOR>* output_landingPoint)
{
	cv::Mat LPMap;
	LPMap = landingPointMap->clone();
	cv::Mat nearbyLP = LPMap(cv::Rect((MAP_SIZE - ROI_SIZE) / 2 / MAP_MESH, (MAP_SIZE - ROI_SIZE) / 2 / MAP_MESH, ROI_SIZE / MAP_MESH, ROI_SIZE / MAP_MESH));

	int k = 0;
	nearbyLandingPointNum = 0;
	std::vector<myvector::VECTOR>().swap(nearbyLandingPointVECTOR);
	myvector::VECTOR landingPoint;
	for(int i = 0; i < nearbyLP.rows; i++)
	{
		for(int j = 0; j < nearbyLP.cols; j++)
		{
			if(nearbyLP.at<cv::Vec4f>(i,j)[3] > 0)
			{
				landingPoint.x = nearbyLP.at<cv::Vec4f>(i,j)[0];
				landingPoint.y = nearbyLP.at<cv::Vec4f>(i,j)[1];
				landingPoint.z = nearbyLP.at<cv::Vec4f>(i,j)[2];
				output_landingPoint->push_back(landingPoint);
				nearbyLandingPointVECTOR.push_back(landingPoint);
				nearbyLandingPointNum++;
			}
		}
	}
	p_nearbyLandingPoint = &nearbyLandingPointVECTOR[0];
	return 0;
}

//脚接地候補点の描画
void detectLandingPoint::showLandingPoint(cv::Mat* landingPointMap)
{
	int mapScale = 3;
	cv::Mat mapImg(landingPointMap->rows * mapScale, landingPointMap->cols * mapScale, CV_8UC1, cv::Scalar(0));
	for(int i = 0; i < landingPointMap->rows; i++)
	{
		for(int j = 0; j < landingPointMap->cols; j++)
		{
			if(landingPointMap->at<cv::Vec4f>(i,j)[3] > 0)
			{
				cv::rectangle(mapImg, cv::Point(j*mapScale, i*mapScale), cv::Point((j+1)*mapScale, (i+1)*mapScale), cv::Scalar(255), cv::FILLED);
				//std::cout << "nearby landing point : " << landingPointMap->at<cv::Vec4f>(i,j) << "\n";
			}
		}
	}

	//for(int k=0; k<nearbyLandingPointNum; k++)
	//{
	//	std::cout << "nearby landing point : " << nearbyLandingPointVECTOR[k].x << ", " << nearbyLandingPointVECTOR[k].y << ", " << nearbyLandingPointVECTOR[k].z << "\n";
	//}

	cv::line(mapImg,cv::Point(landingPointMap->cols/2*mapScale,(landingPointMap->rows/2-1)*mapScale),cv::Point(landingPointMap->cols/2*mapScale,(landingPointMap->rows/2+1)*mapScale),cv::Scalar(255));
	cv::line(mapImg,cv::Point((landingPointMap->cols/2-1)*mapScale,landingPointMap->rows/2*mapScale),cv::Point((landingPointMap->cols/2+1)*mapScale,landingPointMap->rows/2*mapScale),cv::Scalar(255));
	if(_showMap == true)
	{
		cv::imshow("landingPointMap", mapImg);
	}
}

//視差の補正
void detectLandingPoint::correctDisparity(double* X, double* Y, double* Z, double theta)
{
	double Xc, Yc, Zc;
	double disp, trueDisp;
	double x, y;
	double Xct, Yct, Zct;
	double Xt, Yt, Zt;
	Xc = *Y - _cameraPosi.y;
	Yc = -(*X) * std::sin(theta) + ((*Z) - _cameraPosi.z) * std::cos(theta);
	Zc = (*X) * std::cos(theta) + ((*Z) - _cameraPosi.z) * std::sin(theta);
	disp = _focalLength * BASE_LINE / Zc;
	x = Xc * disp / BASE_LINE - _priPoint.x;
	y = Yc * disp / BASE_LINE - _priPoint.y;
	trueDisp = ((y + _priPoint.y) * std::cos(theta) + _focalLength * std::sin(theta)) * BASE_LINE / (BODYHEIGHT - _cameraPosi.z);
	Xct = (x + _priPoint.x) * BASE_LINE / trueDisp;
	Yct = (y + _priPoint.y) * BASE_LINE / trueDisp;
	Zct = _focalLength * BASE_LINE / trueDisp;
	Xt = -Yct * std::sin(theta) + Zct * std::cos(theta);
	Yt = Xct + _cameraPosi.y;
	Zt = BODYHEIGHT;
	* X = Xt;
	* Y = Yt;
	* Z = Zt;
}

//表示するウインドウの制御
void detectLandingPoint::windowController()
{
	int controllFlag;

	std::cout << "Show templateImg? | 0 : Yes.    1 : No.\n" << " -> ";
	std::cin >> controllFlag;
	if(controllFlag == 0)
	{
		_showTemplate = true;
	}
	else
	{
		_showTemplate = false;
		cv::destroyWindow("templateImg");
	}

	std::cout << "Show hitogram? | 0 : Yes.    1 : No.\n" << " -> ";
	std::cin >> controllFlag;
	if(controllFlag == 0)
	{
		_showHistogram = true;
	}
	else
	{
		_showHistogram = false;
		cv::destroyWindow("hitogram");
	}

	std::cout << "Show meshImg? | 0 : Yes.    1 : No.\n" << " -> ";
	std::cin >> controllFlag;
	if(controllFlag == 0)
	{
		_showMesh = true;
	}
	else
	{
		_showMesh = false;
		cv::destroyWindow("meshImg");
	}

	std::cout << "Show disparityImg? | 0 : Yes.    1 : No.\n" << " -> ";
	std::cin >> controllFlag;
	if(controllFlag == 0)
	{
		_showDisparity = true;
	}
	else
	{
		_showDisparity = false;
		cv::destroyWindow("disparityImg");
	}

	std::cout << "Show depthImage? | 0 : Yes.    1 : No.\n" << " -> ";
	std::cin >> controllFlag;
	if(controllFlag == 0)
	{
		_showDepth = true;
	}
	else
	{
		_showDepth = false;
		cv::destroyWindow("depthImage");
	}

	std::cout << "Show landingPointMap? | 0 : Yes.    1 : No.\n" << " -> ";
	std::cin >> controllFlag;
	if(controllFlag == 0)
	{
		_showMap = true;
	}
	else
	{
		_showMap = false;
		cv::destroyWindow("landingPointMap");
	}
}

////マウスイベント
//void detectLandingPoint::_onMouse(int event, int x, int y, int flags, void* data)
//{
//	if( _selectObject )
//    {
//        _selection.x = MIN(x, _origin.x);
//        _selection.y = MIN(y, _origin.y);
//        _selection.width = std::abs(x - _origin.x);
//        _selection.height = std::abs(y - _origin.y);
//
//        _selection &= cv::Rect(0, 0, _tempImage.cols, _tempImage.rows);
//    }
//
//    switch( event )
//    {
//	case cv::EVENT_LBUTTONDOWN:
//        _origin = cv::Point(x,y);
//        _selection = cv::Rect(x,y,0,0);
//        _selectObject = true;
//        break;
//	case cv::EVENT_LBUTTONUP:
//        _selectObject = false;
//        if( _selection.width > 0 && _selection.height > 0 )
//            _trackObject = -1;
//        break;
//    }
//}
//
////トラックバーイベント
//void detectLandingPoint::_hueTrackBar(int val, void* tileMaxSat)
//{
//
//}
//
//void detectLandingPoint::_satTrackBar(int val, void* tileMaxhue)
//{
//
//}

//オペレーター
void detectLandingPoint::operator()()
{
	//カメラの初期化
	cv::VideoCapture capLeft(CAM1C01);
	if(!capLeft.isOpened())//カメラデバイスが正常にオープンしたか確認．
	{
		//読み込みに失敗したときの処理
		std::cout << "ERROR:open LeftCamera";
		return;
	}
	cv::VideoCapture capRight(CAM1C10);
	if(!capRight.isOpened())//カメラデバイスが正常にオープンしたか確認．
	{
		//読み込みに失敗したときの処理
		std::cout << "ERROR:open RightCamera";
		return;
	}
	cv::Size cap_size(capLeft.get(cv::CAP_PROP_FRAME_WIDTH), capLeft.get(cv::CAP_PROP_FRAME_HEIGHT));//カメラのキャプチャサイズ
	cv::Mat colorDistance(cap_size, CV_8UC3, cv::Scalar::all(128));
	cv::Mat templateImg;
	cv::Mat templateHist;
	cv::Mat disparityMap, reprojectImage;
	//double tileMaxBin[3];
	int tileMaxHue[3];
	int tileMaxSat[3];
	cv::Vec2i freqColor[3];
	cv::Mat hueDistance, satDistance;
	//cv::Point tileMaxLoc[3];
	int hueThreshold = 10, satThreshold = 10;
	cv::namedWindow("LeftCameraStream");
	cv::namedWindow("RightCameraStream");
	cv::namedWindow("colorDistance");
	cv::setMouseCallback( "LeftCameraStream", onMouse, 0 );
	cv::createTrackbar("hueThreshold", "colorDistance", &hueThreshold, 180/*, 0, 0*/);
	cv::createTrackbar("satThreshold", "colorDistance", &satThreshold, 256/*, 0, 0*/);
	myvector::VECTOR movementAmount;
	std::vector<cv::Point3d> allLandingPoint;
	//detectLandingPoint::Control_flag controll;
	cv::Mat LPMap;
	double tickCount, tickFrequency;
	double startTime, nowTime, timeCount, loopTime;	//ms
	bool countFlag = false;
	bool timerFlag = false;
	startTime = 0.0;
	nowTime = 0.0;
	timeCount = 0.0;
	loopTime = 1000.0;
	//cv::Mat HSVmodel = cv::imread("C:/Users/tasaki/Documents/Visual Studio 2010/Projects/detectLandingPoint/detectLandingPoint/HSV.png");
	
	while(1)
	{
		capLeft >> _frameLeft;
		capRight >> _frameRight;
		detectLandingPoint::remapImage(&_frameLeft, &_frameRight);
		//_frameLeft = HSVmodel.clone();
		//detectLandingPoint::streamingCamera();
		cv::imshow("LeftCameraStream", _frameLeft);
		cv::imshow("RightCameraStream", _frameRight);
		movementAmount.x = 0.0;
		movementAmount.y = 0.0;
		movementAmount.z = 0.0;
		//int trackObject = getTrackObject();
		if(selectObject)
		{
			tempImage = _frameLeft.clone();
		}
		if(trackObject < 0)
		{
			trackObject = 1;
			//std::cout << "call back\n";
			//cv::Rect selection = getSelection();
			//テンプレート画像の取得
			detectLandingPoint::getTemplateImage(&_frameLeft, &selection, &templateImg);
			//テンプレート画像のヒストグラムの計算
			detectLandingPoint::calTempHist(&templateImg, &templateHist);
			//頻出色値を求める
			detectLandingPoint::getFrequentVal(&templateHist, tileMaxHue, tileMaxSat);
			//detectLandingPoint::getFrequentVal(&templateHist, freqColor);
			//for(int i=0;i<3;i++)std::cout << "tileMaxHue" << i << " = " << tileMaxHue[i] << ", tileMaxSat" << i << " = " << tileMaxSat[i] << "\n\n";
			//色距離の計算
			detectLandingPoint::calColorDistance(&_frameLeft, tileMaxHue, tileMaxSat, &hueDistance, &satDistance);
			//detectLandingPoint::calColorDistance(&_frameLeft, freqColor, &hueDistance, &satDistance);
			//二値化画像の作成
			detectLandingPoint::makeBinaryImage(&hueDistance, &satDistance, hueThreshold, satThreshold, &colorDistance);
		}

		int key = cv::waitKey(1);
		//if(key == 'Q') controlDetection = FINISH_DETECTION;
		if(key == 'q' || controlDetection == FINISH_DETECTION)	break;
		if(key == 'b' || controlDetection == REMAKE_COLORDISTANCE)
		{
			//色距離の計算
			detectLandingPoint::calColorDistance(&_frameLeft, tileMaxHue, tileMaxSat, &hueDistance, &satDistance);	//L
			//detectLandingPoint::calColorDistance(&_frameRight, tileMaxHue, tileMaxSat, &hueDistance, &satDistance);	//R
			//二値化画像の作成
			detectLandingPoint::makeBinaryImage(&hueDistance, &satDistance, hueThreshold, satThreshold, &colorDistance);
			controlDetection = WAIT;
		}
		if(key == 't' || key == 'T' || controlDetection == LOOP_START || controlDetection == LOOP_STOP || controlDetection == ONCE)
		{
			timerFlag = !timerFlag;
			if(key == 'T' || controlDetection == ONCE)
			{
				timerFlag = false;
				controlDetection = WAIT;
			}
			if(timerFlag)
			{
				startTime = (double)cv::getTickCount() / (double)cv::getTickFrequency() * 1000;
				countFlag = true;
				controlDetection = LOOP_CONTENUE;
				//std::cout << "Timer start. : " << startTime << "\n";
			}
			else if(controlDetection == LOOP_STOP)
			{
				countFlag = false;
				timerFlag = false;
				controlDetection = WAIT;
				//std::cout << "Timer stop. : " << (double)cv::getTickCount() / (double)cv::getTickFrequency() * 1000 << "\n";
			}
			else
			{
				countFlag = false;
				//std::cout << "Timer stop. : " << (double)cv::getTickCount() / (double)cv::getTickFrequency() * 1000 << "\n";
				controlDetection = WAIT;
			}
			std::vector<cv::Point2d> tilePosition2d;
			std::vector<cv::Point3d> tilePosition3d;
			std::vector<myvector::VECTOR> output_landingPoint;
			//色距離の計算
			detectLandingPoint::calColorDistance(&_frameLeft, tileMaxHue, tileMaxSat, &hueDistance, &satDistance);	//L
			//detectLandingPoint::calColorDistance(&_frameRight, tileMaxHue, tileMaxSat, &hueDistance, &satDistance);	//R
			//detectLandingPoint::calColorDistance(&_frameLeft, freqColor, &hueDistance, &satDistance);
			//二値化画像の作成
			detectLandingPoint::makeBinaryImage(&hueDistance, &satDistance, hueThreshold, satThreshold, &colorDistance);
			//二値化画像からタイルの位置を求める
			detectLandingPoint::makeMeshImage(&colorDistance, _cameraPosi, DEG40, &tilePosition2d);
			//std::cout << tilePosition2d.size() /*<< "\t" << tilePosition2d.at(1) << "\t" << tilePosition2d.at(2)*/ << "\n";
			//ステレオマッチング
			detectLandingPoint::stereoMatching(&_frameLeft, &_frameRight, &disparityMap);	//L
			//detectLandingPoint::stereoMatching(&_frameRight, &_frameLeft, &disparityMap);	//R
			//三次元再投影
			detectLandingPoint::reproject3dPosition(&disparityMap, &reprojectImage);
			//ロボット座標系から見たタイルの三次元位置を求める
			detectLandingPoint::coordinateOfTile3d(&reprojectImage, &tilePosition2d, _cameraPosi, DEG40, &tilePosition3d);
			//detectLandingPoint::coordinateOfTile3d(&reprojectImage, &tilePosition2d, cv::Point3d(0.0,0.0,0.0), 0, &tilePosition3d);
			//重心移動量の受け取り
			//
			movementAmount.x = centerOfGravityMovement.x;
			movementAmount.y = centerOfGravityMovement.y;
			movementAmount.z = centerOfGravityMovement.z;
			//ここまで
			//重心移動があったかの判定
			if(movementAmount.x != 0 || movementAmount.y != 0)
			{
				//脚接地候補点座標の再計算
				detectLandingPoint::recalLnadingPoint(&allLandingPoint, &movementAmount);
				centerOfGravityMovement.x = 0;
				centerOfGravityMovement.y = 0;
				centerOfGravityMovement.z = 0;
			}
			//////脚接地候補点座標の記憶
			//detectLandingPoint::saveLnadingPoint(&tilePosition3d, &allLandingPoint, &LPMap);
			////脚接地候補点座標の記憶
			detectLandingPoint::saveLnadingPointRound(&tilePosition3d, &allLandingPoint, &LPMap);
			////脚接地候補点座標の受け渡し
			detectLandingPoint::sendLandingPoint(&LPMap, &output_landingPoint);
			//std::cout << "allLandingPoint : \n" << allLandingPoint << "\n\n";
			detectLandingPoint::showLandingPoint(&LPMap);
		}
		//if(key == 'm')
		//{
		//	detectLandingPoint::stereoMatching(&_frameLeft, &_frameRight, &disparityMap);
		//	detectLandingPoint::reproject3dPosition(&disparityMap, &reprojectImage);
		//}
		//if(key == 'g')
		//{
		//	cv::Mat grayImg;
		//	cv::cvtColor(_frameLeft, grayImg, CV_BGR2GRAY);
		//	cv::imshow("grayImg",grayImg);
		//}

		//if(key == 'W')
		//{
		//	centerOfGravityMovement.x = 300;
		//	centerOfGravityMovement.y = 0;
		//	centerOfGravityMovement.z = 0;
		//	std::cout << "CoG moved. \n";
		//}
		//if(key == 'S')
		//{
		//	centerOfGravityMovement.x = -300;
		//	centerOfGravityMovement.y = 0;
		//	centerOfGravityMovement.z = 0;
		//	std::cout << "CoG moved. \n";
		//}
		//if(key == 'A')
		//{
		//	centerOfGravityMovement.x = 0;
		//	centerOfGravityMovement.y = -300;
		//	centerOfGravityMovement.z = 0;
		//	std::cout << "CoG moved. \n";
		//}
		//if(key == 'D')
		//{
		//	centerOfGravityMovement.x = 0;
		//	centerOfGravityMovement.y = 300;
		//	centerOfGravityMovement.z = 0;
		//	std::cout << "CoG moved. \n";
		//}

		nowTime = (double)cv::getTickCount() / (double)cv::getTickFrequency() * 1000;
		if(countFlag) timeCount = nowTime - startTime;
		else	timeCount = 0.0;
		if(timeCount >= loopTime)
		{
			startTime = nowTime;
			//std::cout << "Timer is counting. : " << nowTime << "\n";
			//std::cout << "Loop time : " << timeCount << "\n";
			timeCount = 0.0;

			std::vector<cv::Point2d> tilePosition2d;
			std::vector<cv::Point3d> tilePosition3d;
			std::vector<myvector::VECTOR> output_landingPoint;
			//色距離の計算
			detectLandingPoint::calColorDistance(&_frameLeft, tileMaxHue, tileMaxSat, &hueDistance, &satDistance);
			//detectLandingPoint::calColorDistance(&_frameLeft, freqColor, &hueDistance, &satDistance);
			//二値化画像の作成
			detectLandingPoint::makeBinaryImage(&hueDistance, &satDistance, hueThreshold, satThreshold, &colorDistance);
			//二値化画像からタイルの位置を求める
			detectLandingPoint::makeMeshImage(&colorDistance, _cameraPosi, DEG40, &tilePosition2d);
			//std::cout << tilePosition2d.size() /*<< "\t" << tilePosition2d.at(1) << "\t" << tilePosition2d.at(2)*/ << "\n";
			//ステレオマッチング
			detectLandingPoint::stereoMatching(&_frameLeft, &_frameRight, &disparityMap);
			//三次元再投影
			detectLandingPoint::reproject3dPosition(&disparityMap, &reprojectImage);
			//ロボット座標系から見たタイルの三次元位置を求める
			detectLandingPoint::coordinateOfTile3d(&reprojectImage, &tilePosition2d, _cameraPosi, DEG40, &tilePosition3d);
			//detectLandingPoint::coordinateOfTile3d(&reprojectImage, &tilePosition2d, cv::Point3d(0.0,0.0,0.0), 0, &tilePosition3d);
			//重心移動量の受け取り
			//
			movementAmount.x = centerOfGravityMovement.x;
			movementAmount.y = centerOfGravityMovement.y;
			movementAmount.z = centerOfGravityMovement.z;
			//ここまで
			//重心移動があったかの判定
			if(movementAmount.x != 0 || movementAmount.y != 0)
			{
				//脚接地候補点座標の再計算
				detectLandingPoint::recalLnadingPoint(&allLandingPoint, &movementAmount);
				centerOfGravityMovement.x = 0;
				centerOfGravityMovement.y = 0;
				centerOfGravityMovement.z = 0;
			}
			//////脚接地候補点座標の記憶
			//detectLandingPoint::saveLnadingPoint(&tilePosition3d, &allLandingPoint, &LPMap);
			////脚接地候補点座標の記憶
			detectLandingPoint::saveLnadingPointRound(&tilePosition3d, &allLandingPoint, &LPMap);
			////脚接地候補点座標の受け渡し
			detectLandingPoint::sendLandingPoint(&LPMap, &output_landingPoint);
			//std::cout << "allLandingPoint : \n" << allLandingPoint << "\n\n";
			detectLandingPoint::showLandingPoint(&LPMap);
		}
	}
	cv::destroyAllWindows();
}

void onMouse(int event, int x, int y, int, void*)
{
	if( selectObject )
	{
		selection.x = MIN(x, origin.x);
		selection.y = MIN(y, origin.y);
		selection.width = std::abs(x - origin.x);
		selection.height = std::abs(y - origin.y);

		selection &= cv::Rect(0, 0, tempImage.cols, tempImage.rows);
	}

	switch( event )
	{
	case cv::EVENT_LBUTTONDOWN:
		origin = cv::Point(x,y);
		selection = cv::Rect(x,y,0,0);
		selectObject = true;
		break;
	case cv::EVENT_LBUTTONUP:
		selectObject = false;
		if( selection.width > 0 && selection.height > 0 )
			trackObject = -1;
		break;
	}
}

void hueTrackBar(int val, void* freqColor)
{

}

void satTrackBar(int val, void* freqColor)
{

}
