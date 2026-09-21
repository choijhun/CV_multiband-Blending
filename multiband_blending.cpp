#include<iostream>
#include<algorithm>
#include<vector>
#include<cmath>
#include <opencv2/core.hpp>
#include<opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
using namespace cv;
using namespace std;

#ifdef _DEBUG
#pragma comment(lib,"opencv_world490d.lib")
#else
#pragma comment(lib,"opencv_world490.lib")
#endif

std::vector<Mat> buildLaplacian(const Mat img, int depth = 8) {
    vector<Mat>pyr(depth);
    pyr[0] = img.clone();
    Mat dst, tmp, low;
    for (int i = 0; i < depth-1; i++) {
        pyrDown(pyr[i], pyr[i + 1]);
        pyrUp(pyr[i + 1], tmp,pyr[i].size());
        pyr[i] = pyr[i] - tmp;
    }
    return pyr;
}
std::vector<Mat> buildGaussian(const Mat img, int depth = 8) {
    vector<Mat>pyr(depth);
    pyr[0] = img.clone();
    Mat dst, tmp, low;
    for (int i = 0; i < depth - 1; i++) {
        pyrDown(pyr[i], pyr[i + 1]);     
    }
    return pyr;
}
Mat reconstruct(const vector<Mat>& pyr) {
    Mat ret;
    int n = pyr.size();
    ret = pyr[n - 1];
    for (int i = n - 2; i >= 0;i--) {
        pyrUp(ret, ret, pyr[i].size());
        ret += pyr[i];
    }
    return ret;
}

int main(void)
{
    Mat apple = imread("C:/Users/choij/Desktop/burt_apple.png");    
    Mat orange = imread("C:/Users/choij/Desktop/burt_orange.png");
    Mat mask = imread("C:/Users/choij/Desktop/burt_mask.png");
    apple.convertTo(apple, CV_32F, 1 / 255.f);
    orange.convertTo(orange, CV_32F, 1 / 255.f);
    mask.convertTo(mask, CV_32F, 1 / 255.f);
    auto apple_pyr = buildLaplacian(apple);
    auto orange_pyr = buildLaplacian(orange);
    
    Mat mask2 = Scalar(1, 1, 1) - mask;
    auto mask_pyr = buildGaussian(mask);
    auto mask2_pyr = buildGaussian(mask2);
   
    vector<Mat> added(apple_pyr.size());

    for (int i = 0; i < apple_pyr.size(); i++) {
        Mat m_apple, m_orange;
        multiply(apple_pyr[i], mask_pyr[i], m_apple);
        multiply(orange_pyr[i], mask2_pyr[i], m_orange);
        added[i] = m_apple + m_orange;
    }

    Mat result = reconstruct(added);

    imshow("apple", result);
    waitKey();
       
}
