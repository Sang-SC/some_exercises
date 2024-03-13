#include "line_follower/level1.hpp"


/*
* 函数功能：显示用的函数
*/
void level1(Mat srcImg)
{
    std::string title = "1、原图，未处理";
    moveWindow(title, 100, 100);
    setWindowProperty(title, WND_PROP_TOPMOST, 1);  // 设置窗口置顶
    imshow(title, srcImg);

    title = "2、添加高斯噪声";
    srcImg = addGuassNoise(srcImg);
    moveWindow(title, 700, 100);
    imshow(title, srcImg);

    title = "3、均值滤波消除高斯噪声";
    srcImg = meanFilter(srcImg);
    moveWindow(title, 100, 750);
    imshow(title, srcImg);

    title = "4、仿射变换";
    srcImg = perspectiveTransform1(srcImg);
    moveWindow(title, 700, 750);
    imshow(title, srcImg);
    srcImg = perspectiveTransform2(srcImg);

    title = "5、转灰度图";
    srcImg = colorToGray(srcImg);
    moveWindow(title, 100, 1400);
    imshow(title, srcImg);
    
    title = "6、Canny边缘检测";
    srcImg = cannyEdgeDetect(srcImg);
    moveWindow(title, 700, 1400);
    imshow(title, srcImg);
    waitKey(1);
}


/*
* 函数功能：为彩色图片添加高斯噪声
*/
Mat addGuassNoise(Mat srcImg)
{
    Mat noiseSrcImg = srcImg;
    Mat guassNoise = Mat::zeros(noiseSrcImg.size(), noiseSrcImg.type());
    randn(guassNoise, (5, 5, 5), (10, 10, 10));  //生成噪声图像
    Mat guassNoiseImg;
    add(guassNoise, noiseSrcImg, guassNoiseImg);
    return guassNoiseImg;
}


/*
* 函数功能：进行均值滤波
*/
Mat meanFilter(Mat srcImg)
{
    // Mat noiseImg = srcImg;
    // Mat filterImg = noiseImg.clone();
    Mat filterImg;
    GaussianBlur(srcImg, filterImg, Size(3, 3), 0);
    // for (int i = 1; i < noiseImg.rows - 1; i++) {
    //     for (int j = 1; j < noiseImg.cols - 1; j++) {
    //         for (int k = 0; k < 3; k++) {
    //             filterImg.at<Vec3b>(i, j)[k] = saturate_cast<uchar>((noiseImg.at<Vec3b>(i - 1, j - 1)[k] + noiseImg.at<Vec3b>(i - 1, j)[k] 
    //                                                                + noiseImg.at<Vec3b>(i - 1, j + 1)[k] + noiseImg.at<Vec3b>(i, j - 1)[k] 
    //                                                                + noiseImg.at<Vec3b>(i, j)[k] + noiseImg.at<Vec3b>(i, j + 1)[k]
    //                                                                + noiseImg.at<Vec3b>(i + 1, j - 1)[k] + noiseImg.at<Vec3b>(i + 1, j)[k] 
    //                                                                + noiseImg.at<Vec3b>(i + 1, j + 1)[k]) / 9);
    //         }
    //     }
    // }
    return filterImg;
}


/*
* 函数功能：将 选择的彩色图片 进行透视变换，此处为将一幅正常图片变换到某个角度
*/
Mat perspectiveTransform1(Mat srcImg)
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

    return dstPerspective;
}


/*
* 函数功能：将 选择的彩色图片 进行透视变换，此处为将刚才变换完的，再变换回去
*/
Mat perspectiveTransform2(Mat srcImg)
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

    return dstPerspective;
}


/*
* 函数功能：将滤波后的彩色图转灰度图
*/
Mat colorToGray(Mat srcImg)
{
    Mat grayImg;
    cvtColor(srcImg, grayImg, COLOR_BGR2GRAY);  // 此处一开始填的 CV_BGR2GRAY ，但 VSCode 报错，找不到标识符，应该是新版本改成 COLOR_BGR2GRAY
    return grayImg;
}


/*
* 函数功能：将灰度图进行Canny边缘检测
*/
Mat cannyEdgeDetect(Mat srcImg)
{
    Mat cannyEdgeImg;
    Canny(srcImg, cannyEdgeImg, 50, 150);
    return cannyEdgeImg;
}