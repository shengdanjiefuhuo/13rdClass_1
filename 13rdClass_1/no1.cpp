#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

void on_threshold(int, void*);//��ֵ���ص�����
void Gauss_Ksize(int, void*); //��˹�˲�Ksize�ص�����
Mat frame;
Mat subMat;     //��ֽ��
Mat dst;        //��ֶ�ֵ�����
int thresh = 20; //��ֵ����ֵ
int KsizeValue = 3;  //Ksize��������ʼλ��

int main()
{
	Mat backMat;    //����
	int cnt = 0;	//����ֵ

	VideoCapture cap(0);//������ͷ
	//�����Ƶ��ʧ��
	if (!cap.isOpened())
	{
		cout << "���ܴ���Ƶ" << endl;
		return -1;
		destroyAllWindows();
	}

	while (1)
	{
		cap >> frame;
		cvtColor(frame, frame, COLOR_BGR2GRAY);
		namedWindow("dst");
		createTrackbar("�ں˳ߴ�", "dst", &KsizeValue, 15, Gauss_Ksize);
		//���ûص�����
		Gauss_Ksize(0, 0);

		if (frame.empty())
		{
			cout << "���ܶ�ȡ��Ƶ֡" << endl;
			return -1;
			destroyAllWindows();
		}

		//�Ե�һ֡��Ϊ����ͼ��
		if (!cnt)
		{
			frame.copyTo(backMat);
		}
		//�ӵڶ�֡��ʼ�������
		else
		{
			//����ͼ��͵�ǰͼ����������ȡ����ֵ������һ����0~255
			absdiff(frame, backMat, subMat);
			//��������������ֽ����ֵ��
			createTrackbar("threshold", "dst", &thresh, 255, on_threshold);
			on_threshold(0, 0);//���ûص�����
			imshow("background", backMat);//��ʾ����
			imshow("frame", frame);
		}

		//��ʱ30ms
		//�ȴ�������Ӧ������ESC���˳�
		if (waitKey(30) == 27)
		{
			destroyAllWindows();
			break;
		}
		cnt++;
	}
	return 0;
}

//��ֵ���ص�����
void on_threshold(int, void*)
{

	threshold(subMat, dst, thresh, 255, THRESH_BINARY);
	imshow("dst", dst);//��ʾ���
}

void Gauss_Ksize(int, void*)       //��˹�˲�Ksize�ص�����
{
	int length = KsizeValue * 2 + 1;
	GaussianBlur(frame, frame, Size(length, length), 3.5, 3.5);
}