#include <opencv2/opencv.hpp>
#include <iostream>
#define useMouse "Mouse Event"

using namespace cv;
using namespace std;


typedef struct _PointInfo
{
    int x;
    int y;
    int sad;
}PointInfo;

typedef struct Locationxy {
    int p1;
    int p2;
}Locationxy;

static Locationxy L_xy[1]; //Ŭ���� ��ǥ ������ ����

void GrayScale(Mat image, Mat& image_) {
    int h = image.rows;
    int w = image.cols;
    float fGray = 0.0f;
    float chBlue, chGreen, chRed;
    Mat bgr[3];
    split(image, bgr);

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            chBlue = (float)(image.at<Vec3b>(y, x)[0]);
            chGreen = (float)(image.at<Vec3b>(y, x)[1]);
            chRed = (float)(image.at<Vec3b>(y, x)[2]);
            fGray = 0.2126f * chRed + 0.7152f * chGreen + 0.0722f * chBlue;

            if (fGray < 0.0) fGray = 0.0f;
            if (fGray > 255.0) fGray = 255.0f;

            bgr[0].at<uchar>(y, x) = (int)fGray;
        }
    }
    bgr[0].copyTo(image_);
}

vector<Point> save_point;
int Th;
int Tw;

void onMouseEvent(int event, int x, int y, int flags, void* dstImage)
{
    Mat mouseImage = *(Mat*)dstImage;

    switch (event)
    {
        // ���콺 ���� Ŭ�� �̺�Ʈ
    case cv::EVENT_LBUTTONDOWN:
        L_xy[0].p1 = x;
        L_xy[0].p2 = y;
        break;
    }

    //���� Ŭ���� �� ǥ��
    for (int i = 0; i < save_point.size(); i++) {
        if (L_xy[0].p1 >= save_point[i].x && L_xy[0].p1 < save_point[i].x + Tw && L_xy[0].p2 >= save_point[i].y && L_xy[0].p2 < save_point[i].y + Th) {
            circle(mouseImage, Point(save_point[i].x+Tw/2, save_point[i].y+Th/2), sqrt(pow(Tw/2,2)+pow(Th/2,2)), Scalar(255, 0, 0), 3, 8, 0);
        }
    }

    imshow(useMouse, mouseImage);
}


void TemplateMatch(Mat& imgsrc, Mat& imgtemp, vector<PointInfo>& point) {
    int Sh = imgsrc.rows;
    int Sw = imgsrc.cols;
    Th = imgtemp.rows;
    Tw = imgtemp.cols;


    int point_X = 0;
    int point_Y = 0;
    int min_value = 99999;



    // ���� �̹��� ��ĵ
    for (int Ori_y = 0; Ori_y < Sh - Th; Ori_y++)
    {
        uchar* dataOri = imgsrc.data;

        for (int Ori_x = 0; Ori_x < Sw - Tw; Ori_x++)
        {

            int sum_of_diff = 0;

            //���ø� �̹��� ��ĵ
            for (int Temp_y = 0; Temp_y < Th; Temp_y++)
            {
                uchar* dataTemp = imgtemp.data;

                for (int Temp_x = 0; Temp_x < Tw; Temp_x++)
                {

                    int original_pixel = dataOri[(Ori_y + Temp_y) * Sw + (Ori_x + Temp_x)];
                    int template_pixel = dataTemp[Temp_y * Tw + Temp_x];

                    sum_of_diff += abs(original_pixel - template_pixel);
                }
            }


            // �ּ� SAD ���� ã��  
            if (min_value > sum_of_diff)
            {
                min_value = sum_of_diff;
                point_Y = Ori_y;
                point_X = Ori_x;
            }

            PointInfo pointInfo = { Ori_x, Ori_y, sum_of_diff };
            point.push_back(pointInfo);

        }
    }


    //���� ������ ������ ����
    for (int i = 0; i < point.size(); i++)
    {
        if (abs(min_value - point[i].sad) < 100) {

            save_point.push_back(Point(point[i].x, point[i].y));

        }
    }

}

int main()
{

    Mat img = imread("image1.jpg", IMREAD_COLOR);
    Mat imgSrc;

    GrayScale(img, imgSrc);//�׷��̽����� �ۼ��Լ� ���
   // cvtColor(img_color, img_original, COLOR_BGR2GRAY);

    Mat imgTemp = imread("p2.jpg", IMREAD_COLOR);
    Mat imgTempSrc;

    GrayScale(imgTemp, imgTempSrc);

    imshow(useMouse, img);
    setMouseCallback(useMouse, onMouseEvent, (void*)&img);
    //img.copyTo(img_color);

    vector<PointInfo> point;
    TemplateMatch(imgSrc, imgTempSrc, point);
    //=======================ù��° ����==========================

    Mat img2 = imread("image1.jpg", IMREAD_COLOR);
    Mat imgSrc2;

    GrayScale(img2, imgSrc2);

    Mat imgTemp2 = imread("p3.jpg", IMREAD_COLOR);
    Mat imgTempSrc2;

    GrayScale(imgTemp2, imgTempSrc2);

    imshow(useMouse, img2);
    setMouseCallback(useMouse, onMouseEvent, (void*)&img2);

    vector<PointInfo>point2;
    TemplateMatch(imgSrc2, imgTempSrc2, point2);
    //=========================�ι��� ����=====================


    Mat img3 = imread("image1.jpg", IMREAD_COLOR);
    Mat imgSrc3;

    GrayScale(img3, imgSrc3);//�׷��̽����� �ۼ��Լ� ���
   // cvtColor(img_color, img_original, COLOR_BGR2GRAY);

    Mat imgTemp3 = imread("p5.jpg", IMREAD_COLOR);
    Mat imgTempSrc3;

    GrayScale(imgTemp3, imgTempSrc3);

    imshow(useMouse, img3);
    setMouseCallback(useMouse, onMouseEvent, (void*)&img3);
    //img.copyTo(img_color);

    vector<PointInfo> point3;
    TemplateMatch(imgSrc3, imgTempSrc3, point3);
    //=======================����° ����==========================

    Mat img4 = imread("image1.jpg", IMREAD_COLOR);
    Mat imgSrc4;

    GrayScale(img4, imgSrc4);//�׷��̽����� �ۼ��Լ� ���
   // cvtColor(img_color, img_original, COLOR_BGR2GRAY);

    Mat imgTemp4 = imread("p4.jpg", IMREAD_COLOR);
    Mat imgTempSrc4;

    GrayScale(imgTemp4, imgTempSrc4);

    imshow(useMouse, img4);
    setMouseCallback(useMouse, onMouseEvent, (void*)&img4);
    //img.copyTo(img_color);

    vector<PointInfo> point4;
    TemplateMatch(imgSrc4, imgTempSrc4, point4);
    //=======================�׹�° ����==========================

    Mat img5 = imread("image1.jpg", IMREAD_COLOR);
    Mat imgSrc5;

    GrayScale(img5, imgSrc5);//�׷��̽����� �ۼ��Լ� ���
   // cvtColor(img_color, img_original, COLOR_BGR2GRAY);

    Mat imgTemp5 = imread("p1.jpg", IMREAD_COLOR);
    Mat imgTempSrc5;

    GrayScale(imgTemp5, imgTempSrc5);

    imshow(useMouse, img5);
    setMouseCallback(useMouse, onMouseEvent, (void*)&img5);
    //img.copyTo(img_color);

    vector<PointInfo> point5;
    TemplateMatch(imgSrc5, imgTempSrc5, point5);
    //=======================�ټ���° ����==========================



    //���� �׸� ũ�� Ű��!
    Mat dst = imread("find.jpg", IMREAD_COLOR);
   // resize(imgTemp, dst, Size(50, 50), 0, 0, cv::INTER_LINEAR);
    imshow("Let's find!", dst);

    waitKey(0);

}