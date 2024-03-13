#include "line_follower/level2.hpp"

/*
* 函数功能：调用其他模块，确定中间点
*/
Point level2(Mat img)
{
    
    Mat img_tem = ImageProcessFun(img);
    return FindPoints(img_tem);
}



/*
* 函数功能：找到中间点
*/
Point FindPoints(Mat img){
    Point left_point, right_point, middle_point;// 左上一点，右下一点，求出中点
    int flag = 1;// 点数
    // 找出左边线
    while (flag <= 4){
        for (int i = 0; i < 64; i++){
            // 在前块中查找左边线的点
            if (flag == 1){
                if((int)img.at<uchar>(0, i) == 255){
                    left_point.x = i;
                    left_point.y = 0;
                    cout<<"left_point="<<left_point<<endl;
                    flag = 4;
                    break;
                }
            } 
            // 在中块中查找左边线的点
            else if (flag == 2){
                if((int)img.at<uchar>(9, i) == 255){
                    left_point.x = i;
                    left_point.y = 9;
                    cout<<"left_point="<<left_point<<endl;
                    flag = 4;
                    break;
                }
            }
            else{
                cout<<"已超出范围"<<endl;
                flag = 6;
                break;
            }        
        }
        flag++;
    }  
    // 找到左边线后求解右边线
    for (int i = 63; i >= 0; i--){
        if((int)img.at<uchar>(23, i) == 255){
            right_point.x = i;
            right_point.y = 23;
            cout<<"right_point="<<right_point<<endl;
            break;
        }
    }
        
    //计算出中间点
    middle_point.x = left_point.x + (right_point.x - left_point.x)/2;
    middle_point.y = 12;//固定中点的高
    cout<<"middle_point="<<middle_point<<endl;
    line(img, Point(32,0), Point(32,24), Scalar(255));// 在感兴趣区域中绘制中心线
    line(img, middle_point, middle_point, Scalar(255),5);// 画出中心点
    return middle_point;
}


/*
* 函数功能：对图像进行Canny边缘检测
*/
Mat ImageProcessFun(Mat img_rgb){
    Mat img_gray, img;
    // img_rgb(Range(40, 64),Range(0, 64)).copyTo(img_gray); //选择感兴趣区域
    // img_gray = img_rgb(cv::Range(img.rows-20, img.rows),cv::Range(img.cols-20, img.cols));
    // img_rgb(Range(40, 64),Range(0, 64)).copyTo(img_gray); //选择感兴趣区域
    img_rgb(Range(40, 64),Range(0, 64)).copyTo(img_rgb);
    img_gray = img_rgb;
    
    cvtColor(img_gray, img_gray, COLOR_BGR2GRAY);// 将图像转换为灰度图像
    threshold(img_gray, img, 100, 255, THRESH_BINARY);// 对感兴趣区域进行二值化操作
    Canny(img, img, 20, 40);// 由于仿真没有额外复杂环境，使用Canny边缘检测
    return img;
}