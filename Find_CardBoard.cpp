// Find_CardBoard.cpp : �w�q�D���x���ε{�����i�J�I�C
//

#include "stdafx.h"


#pragma comment(lib,"opencv_core2413d.lib")	//��ܹϤ��ϥ�
#pragma comment(lib,"opencv_highgui2413d.lib") 
#pragma comment(lib,"opencv_imgproc2413d.lib")
#pragma comment(lib,"opencv_video2413d.lib")
#pragma comment(lib,"opencv_legacy2413d.lib")
#pragma comment(lib,"opencv_objdetect2413d.lib")

#include <opencv2/highgui/highgui.hpp>  //���hGUI
#include <opencv2/opencv.hpp>
#include <opencv2/legacy/legacy.hpp>
using namespace cv;

//image
//int g_H_L=19;
//int g_H_H=41;
//
//int g_S_L=26;
//int g_S_H=114;
//
//int g_V_L=145;
//int g_V_H=250;
//
//int g_erode_size=21;

//video
//int g_H_L=0;
//int g_H_H=53;
//
//int g_S_L=10;
//int g_S_H=110;
//
//int g_V_L=140;
//int g_V_H=255;

int g_H_L=0;
int g_H_H=7;

int g_S_L=70;
int g_S_H=253;

int g_V_L=81;
int g_V_H=146;

int g_erode_size=21;
RNG g_rng(12345);

#define MASK_WINDOW "mask"
#define ERODE_MASK_WINDOW "erode_mask"
#define OPENING_MASK_WINDOW "opening_mask"

void on_TrackbarNumcharge(int,void*)
{
	g_H_H=g_H_H;
}
void on_ErodeSizeChange(int,void*)
{
	g_erode_size=g_erode_size;
}

//=========//
//==Image==//
//=========//
/*
int _tmain(int argc, _TCHAR* argv[])
{
	Mat src=imread("CardBoard.png");

	Mat mask=Mat::zeros(src.rows,src.cols, CV_8U); //���F�o����L�C��
    
	Mat hsv;
	Mat	CardBoardColor,dst;
    cvtColor(src,hsv,CV_BGR2HSV);//�নhsv����
    
    inRange(hsv,Scalar(g_H_L,g_S_L,g_V_L) , Scalar(g_H_H,g_S_H,g_V_H), CardBoardColor);  //�G�ȤơGh�Ȥ���20~40 & s�Ȥ���0~100 & v�Ȥ���100~255

	mask=CardBoardColor;

	//src.copyTo(dst,mask);
  
    imshow("src",src);//show b
    imshow("mask",mask);//show mask
    //imshow("dst",dst);//show���G
    
	createTrackbar("HL",MASK_WINDOW,&g_H_L,255,on_TrackbarNumcharge);
	createTrackbar("HH",MASK_WINDOW,&g_H_H,255,on_TrackbarNumcharge);
	
	createTrackbar("SL",MASK_WINDOW,&g_S_L,255,on_TrackbarNumcharge);
	createTrackbar("SH",MASK_WINDOW,&g_S_H,255,on_TrackbarNumcharge);
	
	createTrackbar("VL",MASK_WINDOW,&g_V_L,255,on_TrackbarNumcharge);
	createTrackbar("VH",MASK_WINDOW,&g_V_H,255,on_TrackbarNumcharge);

	namedWindow(ERODE_MASK_WINDOW,WINDOW_AUTOSIZE);
	createTrackbar("erode size",ERODE_MASK_WINDOW,&g_erode_size,30,on_ErodeSizeChange);

	while(1)
	{
		//find out card board color
		inRange(hsv,Scalar(g_H_L,g_S_L,g_V_L) , Scalar(g_H_H,g_S_H,g_V_H), CardBoardColor);  //�G�ȤơGh�Ȥ���20~40 & s�Ȥ���0~100 & v�Ȥ���100~255
		mask=CardBoardColor;

		//erode
		Mat element=getStructuringElement(MORPH_RECT,Size(g_erode_size,g_erode_size));
		Mat erode_mask;
		erode(mask,erode_mask,element);

		//dilate
		Mat opening_mask; //erode and dilate
		dilate(erode_mask,opening_mask,element);

		//==canny==//
		Mat canny_output;
		//Mat blur_mask;
		//blur(opening_mask,blur_mask,Size(20,20));
		Canny(opening_mask,canny_output,3,9,3);

		//==find countours==//
		Mat contour_out;
		vector<Vec4i> g_vHierarchy;
		vector<vector<Point>> g_vCountours;
		findContours(canny_output,g_vCountours,g_vHierarchy,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);

		//==draw contours to dst
		//Mat drawing=Mat::zeros(canny_output.size(),CV_8UC3);
		Mat dst=src.clone();
		
		//for(int i=0;i<g_vCountours.size();i++) //g_vCountours.size() ��ܦ����X�ӽ���
		//{
		//	//Scalar color=Scalar(g_rng.uniform(0,255),g_rng.uniform(0,255),g_rng.uniform(0,255));
		//	Scalar color=Scalar(0,255,0);
		//	drawContours(dst,g_vCountours,i,color,5,8,g_vHierarchy,0,Point());
		//}

		//==find minima area rectangel==//
		RotatedRect rRect = minAreaRect(g_vCountours[1]);

		Point2f vertices[4]={0};

		rRect.points(vertices);

		char text[100];

		for (int i = 0; i < 4; i++)
		{
			circle(dst,vertices[i],6,Scalar(0,0,200),2);//��X�����I
			line(dst, vertices[i], vertices[(i+1)%4], Scalar(0,200,0),3);//�N4�����I�s�u
			sprintf_s(text, "(%3.2f,%3.2f)", vertices[i].x, vertices[i].y);//�е��|�����y��

			putText(dst, text, Point(vertices[i].x,vertices[i].y), FONT_HERSHEY_PLAIN, 1, CV_RGB(0,255,0));
		}
		//for (int i = 0; i < 4; i++)
		//	
		//rectangle(dst, rRect, Scalar(255,0,0), 1, 8, 0);

		//Rect brect = rRect.boundingRect(); //��X�⥦�]���~��
		//rectangle(dst, brect, Scalar(0,0,255));

		//show
		imshow(MASK_WINDOW,mask);//show mask
		imshow(ERODE_MASK_WINDOW,erode_mask);
		imshow(OPENING_MASK_WINDOW,opening_mask);
		//imshow("blur_mask",blur_mask);
		imshow("canny output",canny_output);
		imshow("dst",dst);//show���G
		waitKey(30);
	}


   

	return 0;
}
*/

//=========//
//==video==//
//=========//

int _tmain(int argc, _TCHAR* argv[])
{
	VideoCapture capture(0);
	
	Mat src;

	Mat mask=Mat::zeros(src.rows,src.cols, CV_8U); //���F�o����L�C��
    
	Mat hsv;
	Mat	CardBoardColor,dst;
   // cvtColor(src,hsv,CV_BGR2HSV);//�নhsv����
    
    //inRange(hsv,Scalar(g_H_L,g_S_L,g_V_L) , Scalar(g_H_H,g_S_H,g_V_H), CardBoardColor);  //�G�ȤơGh�Ȥ���20~40 & s�Ȥ���0~100 & v�Ȥ���100~255

	//mask=CardBoardColor;

	//src.copyTo(dst,mask);
  
    namedWindow("src",WINDOW_AUTOSIZE);//show b
    namedWindow(MASK_WINDOW,WINDOW_AUTOSIZE);//show mask
    //imshow("dst",dst);//show���G
    
	createTrackbar("HL",MASK_WINDOW,&g_H_L,255,on_TrackbarNumcharge);
	createTrackbar("HH",MASK_WINDOW,&g_H_H,255,on_TrackbarNumcharge);
	
	createTrackbar("SL",MASK_WINDOW,&g_S_L,255,on_TrackbarNumcharge);
	createTrackbar("SH",MASK_WINDOW,&g_S_H,255,on_TrackbarNumcharge);
	
	createTrackbar("VL",MASK_WINDOW,&g_V_L,255,on_TrackbarNumcharge);
	createTrackbar("VH",MASK_WINDOW,&g_V_H,255,on_TrackbarNumcharge);

	namedWindow(ERODE_MASK_WINDOW,WINDOW_AUTOSIZE);
	createTrackbar("erode size",ERODE_MASK_WINDOW,&g_erode_size,30,on_ErodeSizeChange);

	while(1)
	{
		//==Load src==//
		capture>>src;

		//==transfer to HSV==//
		cvtColor(src,hsv,CV_BGR2HSV);//�নhsv����

		//find out card board color
		inRange(hsv,Scalar(g_H_L,g_S_L,g_V_L) , Scalar(g_H_H,g_S_H,g_V_H), CardBoardColor);  //�G�ȤơGh�Ȥ���20~40 & s�Ȥ���0~100 & v�Ȥ���100~255
		mask=CardBoardColor;

		//erode
		Mat element=getStructuringElement(MORPH_RECT,Size(g_erode_size,g_erode_size));
		Mat erode_mask;
		erode(mask,erode_mask,element);

		//dilate
		Mat opening_mask; //erode and dilate
		dilate(erode_mask,opening_mask,element);

		//==canny==//
		Mat canny_output;
		//Mat blur_mask;
		//blur(opening_mask,blur_mask,Size(20,20));
		Canny(opening_mask,canny_output,3,9,3);

		//==find countours==//
		Mat contour_out;
		vector<Vec4i> g_vHierarchy;
		vector<vector<Point>> g_vCountours;
		findContours(canny_output,g_vCountours,g_vHierarchy,RETR_EXTERNAL,CHAIN_APPROX_NONE);//RETR_EXTERNAL �u���~�h RETR_LIST������ CHAIN_APPROX_NONE�Ҧ������I CHAIN_APPROX_SIMPLE�u�����I



		//==find minima area rectangel==//
		Mat dst=src.clone();
		RotatedRect rRect;
		
		Point2f vertices[4]={0};
		char text[100];

		for(int c=0;c<g_vCountours.size();c++)
		{			
			rRect=minAreaRect(g_vCountours[c]);
			rRect.points(vertices);

			for (int i = 0; i < 4; i++)
			{
				circle(dst,vertices[i],6,Scalar(0,0,200),2);//��X�����I
				line(dst, vertices[i], vertices[(i+1)%4], Scalar(0,200,0),3);//�N4�����I�s�u
				sprintf_s(text, "(%3.2f,%3.2f)", vertices[i].x, vertices[i].y);//�е��|�����y��

				putText(dst, text, Point(vertices[i].x,vertices[i].y), FONT_HERSHEY_PLAIN, 1, CV_RGB(0,255,0));
			}
		}
		//==draw contours ot dst
		//Mat drawing=Mat::zeros(canny_output.size(),CV_8UC3);
		//Mat dst=src.clone();
		//

		//for(int i=0;i<g_vCountours.size();i++)
		//{
		//	//Scalar color=Scalar(g_rng.uniform(0,255),g_rng.uniform(0,255),g_rng.uniform(0,255));
		//	Scalar color=Scalar(0,255,0);
		//	drawContours(dst,g_vCountours,i,color,5,8,g_vHierarchy,0,Point());
		//}

		//show
		imshow("src",src);
		imshow(MASK_WINDOW,mask);//show mask
		imshow(ERODE_MASK_WINDOW,erode_mask);
		imshow(OPENING_MASK_WINDOW,opening_mask);
		//imshow("blur_mask",blur_mask);
		imshow("canny output",canny_output);
		imshow("dst",dst);//show���G
		waitKey(30);
	}


   

	return 0;
}

//mask=r+r2+g+b;//�������G�Ȥƹϲ֥[�_�ӴN�ܦ��B�n 
//src.copyTo(dst,mask); //�N��Ϥ��g�ѾB�n�L�o��A�o�쵲�Gdst

