#include <iostream>
#include "AVL.h"

using namespace avl;
using namespace std;
using namespace atl;
using namespace avs;

auto LinesArray(Array<Line2D>& lines)
{
    auto arr1 = Array<Conditional<Line2D>>(lines.Size()); //Array<Line2D>
    for(int i=0;i<arr1.Size();++i)
        arr1[i] = Conditional<Line2D>(lines[i]);
    return Conditional<Array<Conditional<Line2D>>>(arr1);
}


int main() {

    Image img;
    LoadImage("/home/anvar/Downloads/images/img1.jpg",false, img);

    Image hsv_img;
    RgbToHsv(img, hsv_img);

    Image bw_img;

    //EqualizeImageHistogram(img, atl::NIL ,0.8, 0.8, img2);

    //DetectLinePeak(img, avl::LinePeakDetectionMethod::MaximalPixel, 128.0f, );


    //SelectChannel(img, NIL, 2, img2);

    AverageChannels(img, NIL,bw_img);

    SaveImageToJpeg(bw_img,"/home/anvar/Downloads/bw_img", NIL, false);

    Image clahe_img;
    EqualizeImageHistogram(bw_img, NIL, 0.05, 0.35, clahe_img);
    SaveImageToJpeg(clahe_img,"/home/anvar/Downloads/clahe_img", NIL, false);


    //Image norm_img;
    //NormalizeLocalBrightness_Gauss(bw_img,NIL,5.0f,NIL,160.0f,0.7,norm_img);
    //NormalizeLocalContrast(bw_img, NIL, 128.0f, 90.0f, 1, 0.1f, norm_img);
    //SaveImageToJpeg(norm_img,"/home/anvar/Downloads/norm_img", NIL, false);

    Image mask_img;
    ThresholdImage(clahe_img, NIL, 150.0f, 230.0f, 0.0f, mask_img);
    SaveImageToJpeg(mask_img,"/home/anvar/Downloads/mask_img", NIL, false);

    Array<Line2D> lines;
    Array<float> scores;
    Image vizualized_img, score_img;


//    DetectLines(clahe_img, NIL, 1.0f, 20.0f, 20.0f, 20.f, 10.0f, lines, scores, vizualized_img, score_img);

    int H = clahe_img.Height();
    int W = clahe_img.Width();

    Rectangle2D r({0,H/2},0,W,H/2);
    Region down_half;
    CreateRectangleRegion(r,NIL,W,H,down_half,NIL);

    DetectLines(clahe_img, down_half, 1.0f, 20.0f, 40.0f, 40.f, 10.0f, lines, scores);

    //SaveImageToJpeg(vizualized_img,"/home/anvar/Downloads/vizualized_img", NIL, false);
    //SaveImageToJpeg(score_img,"/home/anvar/Downloads/score_img", NIL, false);




    Image lines_img;
    Pixel pix;
    AvsFilter_MakePixel(1,0,0,1,pix);
    DrawingStyle drwstl;
    drwstl.thickness = 5;
    drwstl.filled = true;
    drwstl.opacity = 1;
    //drwstl.DrawingMode(DrawingMode::HighQuality);
    //=(DrawingMode: HighQuality Opacity: 1.0f Thickness: 1.0f Filled: False PointShape: NIL PointSize: 1.0f)
    DrawLines_SingleColor(img, LinesArray(lines), NIL, pix , drwstl, true, lines_img);
    SaveImageToJpeg(lines_img,"/home/anvar/Downloads/lines_img", NIL, false);


    return 0;
}
