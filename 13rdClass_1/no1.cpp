#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

void on_threshold(int, void*);//二值化回调函数
void Gauss_Ksize(int, void*); //高斯滤波Ksize回调函数
Mat frame;
Mat subMat;     //差分结果
Mat dst;        //差分二值化结果
int thresh = 20; //二值化阈值
int KsizeValue = 3;  //Ksize滑动条初始位置

int main()
{
	Mat backMat;    //背景
	int cnt = 0;	//计数值

	VideoCapture cap(0);//打开摄像头
	//如果视频打开失败
	if (!cap.isOpened())
	{
		cout << "不能打开视频" << endl;
		return -1;
		destroyAllWindows();
	}

	while (1)
	{
		cap >> frame;
		cvtColor(frame, frame, COLOR_BGR2GRAY);
		namedWindow("dst");
		createTrackbar("内核尺寸", "dst", &KsizeValue, 15, Gauss_Ksize);
		//调用回调函数
		Gauss_Ksize(0, 0);

		if (frame.empty())
		{
			cout << "不能读取视频帧" << endl;
			return -1;
			destroyAllWindows();
		}

		//以第一帧作为背景图像
		if (!cnt)
		{
			frame.copyTo(backMat);
		}
		//从第二帧开始背景差分
		else
		{
			//背景图像和当前图像相减，结果取绝对值，并归一化到0~255
			absdiff(frame, backMat, subMat);
			//创建滑动条，差分结果二值化
			createTrackbar("threshold", "dst", &thresh, 255, on_threshold);
			on_threshold(0, 0);//调用回调函数
			imshow("background", backMat);//显示背景
			imshow("frame", frame);
		}

		//延时30ms
		//等待键盘相应，按下ESC键退出
		if (waitKey(30) == 27)
		{
			destroyAllWindows();
			break;
		}
		cnt++;
	}
	return 0;
}

//二值化回调函数
void on_threshold(int, void*)
{

	threshold(subMat, dst, thresh, 255, THRESH_BINARY);
	imshow("dst", dst);//显示结果
}

void Gauss_Ksize(int, void*)       //高斯滤波Ksize回调函数
{
	int length = KsizeValue * 2 + 1;
	GaussianBlur(frame, frame, Size(length, length), 3.5, 3.5);
}