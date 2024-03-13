#include <opencv2/opencv.hpp>

using namespace cv;

Mat srcImg, grayImg;
Mat guassNoise, guassNoiseImg;


/*
* 函数功能：打开一张彩色图片
*/
void readColorImage()
{
    srcImg = imread("/home/ssc/image_process/src/line_follower/src/road2.png");
    // srcImg = imread("/home/ssc/image_process/src/line_follower/src/test.jpg");
    imshow("srcImg", srcImg);
}


/*
* 函数功能：将 选择的彩色图片 进行透视变换，此处为将一幅正常图片变换到某个角度
* 并更新彩色图片
*/
void perspectiveTransform1()
{
    Mat srcPerspective, dstPerspective;
    srcPerspective = srcImg;

    // srcPoint[4]代表在原图框定的4个点，dstPoint[4]是变换后的4个点
    Point2f srcPoint[4], dstPoint[4];
    srcPoint[0] = Point2f(0, 0);
    srcPoint[1] = Point2f(srcPerspective.cols - 1, 0);
    srcPoint[2] = Point2f(0, srcPerspective.rows - 1);
    srcPoint[3] = Point2f(srcPerspective.cols - 1, srcPerspective.rows - 1);
    dstPoint[0] = Point2f(50, 50);
    dstPoint[1] = Point2f(srcPerspective.cols - 150, 0);
    dstPoint[2] = Point2f(0, srcPerspective.rows - 150);
    dstPoint[3] = Point2f(srcPerspective.cols - 100, srcPerspective.rows - 100);
    Mat transMatrix = getPerspectiveTransform(srcPoint, dstPoint);
    warpPerspective(srcPerspective, dstPerspective, transMatrix, srcPerspective.size());

    imshow("dstPerspective13", dstPerspective);
    // imwrite("/home/ssc/image_process/src/line_follower/src/Lena_per.jpg", dstPerspective);
    
    srcImg = dstPerspective;
}


/*
* 函数功能：为彩色图片添加高斯噪声
* 并更新彩色图片
*/
void addGuassNoise()
{
    Mat noiseSrcImg = srcImg;
    guassNoise = Mat::zeros(noiseSrcImg.size(), noiseSrcImg.type());
    randn(guassNoise, (5, 5, 5), (10, 10, 10));  //生成噪声图像
    // imshow("guassNoise", guassNoise);
    add(guassNoise, noiseSrcImg, guassNoiseImg);
    imshow("guassNoiseImg", guassNoiseImg);

    srcImg = guassNoiseImg;
}


/*
* 函数功能：将 选择的彩色图片 进行透视变换，此处为将刚才变换完的，再变换回去
* 并更新彩色图片
*/
void perspectiveTransform2()
{
    Mat srcPerspective, dstPerspective;
    srcPerspective = srcImg;

    // srcPoint[4]代表在原图框定的4个点，dstPoint[4]是变换后的4个点
    Point2f srcPoint[4], dstPoint[4];
    srcPoint[0] = Point2f(50, 50);
    srcPoint[1] = Point2f(srcPerspective.cols - 150, 0);
    srcPoint[2] = Point2f(0, srcPerspective.rows - 150);
    srcPoint[3] = Point2f(srcPerspective.cols - 100, srcPerspective.rows - 100);
    dstPoint[0] = Point2f(0, 0);
    dstPoint[1] = Point2f(srcPerspective.cols - 1, 0);
    dstPoint[2] = Point2f(0, srcPerspective.rows - 1);
    dstPoint[3] = Point2f(srcPerspective.cols - 1, srcPerspective.rows - 1);
    Mat transMatrix = getPerspectiveTransform(srcPoint, dstPoint);
    warpPerspective(srcPerspective, dstPerspective, transMatrix, srcPerspective.size());

    imshow("dstPerspective2", dstPerspective);
    
    srcImg = dstPerspective;
}


/*
* 函数功能：将 添加完噪声的彩色图片 进行均值滤波
* 并更新彩色图片
*/
void meanFilter()
{
    Mat noiseImg = srcImg;
    Mat filterImg = noiseImg.clone();
    for (int i = 1; i < noiseImg.rows - 1; i++) {
        for (int j = 1; j < noiseImg.cols - 1; j++) {
            for (int k = 0; k < 3; k++) {
                filterImg.at<Vec3b>(i, j)[k] = saturate_cast<uchar>((noiseImg.at<Vec3b>(i - 1, j - 1)[k] + noiseImg.at<Vec3b>(i - 1, j)[k] 
                                                                   + noiseImg.at<Vec3b>(i - 1, j + 1)[k] + noiseImg.at<Vec3b>(i, j - 1)[k] 
                                                                   + noiseImg.at<Vec3b>(i, j)[k] + noiseImg.at<Vec3b>(i, j + 1)[k]
                                                                   + noiseImg.at<Vec3b>(i + 1, j - 1)[k] + noiseImg.at<Vec3b>(i + 1, j)[k] 
                                                                   + noiseImg.at<Vec3b>(i + 1, j + 1)[k]) / 9);
            }
        }
    }
    imshow("meanFilterImg", filterImg);
    srcImg = filterImg;
}


/*
* 函数功能：将滤波后的彩色图转灰度图
*/
void colorToGray()
{
    cvtColor(srcImg, grayImg, COLOR_BGR2GRAY);  // 此处一开始填的 CV_BGR2GRAY ，但 VSCode 报错，找不到标识符，应该是新版本改成 COLOR_BGR2GRAY
    imshow("grayImg", grayImg);
}


/*
* 函数功能：将灰度图进行Canny边缘检测
*/
void cannyEdgeDetect()
{
    Mat cannyEdgeImg;
    Canny(grayImg, cannyEdgeImg, 50, 150);
    imshow("cannyEdgeImg", cannyEdgeImg);
}


/*
* 函数功能：主函数
*/
int main()
{
    //----------------------首先模拟摄像机采集到的图片----------------------
    readColorImage();                  // 读入图片
    perspectiveTransform1();           // 给图片添加某个角度
    addGuassNoise();                   // 给图片添加高斯噪声，从而模拟出实际采集的图片（有角度，也有高斯噪声）

    //----------------------之后对摄像机采集到的图片进行处理----------------------
    perspectiveTransform2();           // 首先矫正采集到的图片
    meanFilter();                      // 其次进行滤波
    colorToGray();                     // 之后转灰度图，用于后续处理
    cannyEdgeDetect();                 // 之后利用灰度图进行 Canny 边缘检测
    waitKey(0);
    return 0;
}