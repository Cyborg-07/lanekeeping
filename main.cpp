#include <iostream>
#include "AVL.h"

using namespace avl;
using namespace std;
using namespace atl;
using namespace avs;
using namespace KernelShape;

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

    AverageChannels(img, NIL,bw_img);
    SaveImageToJpeg(bw_img,"/home/anvar/Downloads/bw_img", NIL, false);

    Image clahe_img;
    EqualizeImageHistogram(bw_img, NIL, 0.05, 0.35, clahe_img);
    SaveImageToJpeg(clahe_img,"/home/anvar/Downloads/clahe_img", NIL, false);


    //Image norm_img;
    //NormalizeLocalBrightness_Gauss(bw_img,NIL,5.0f,NIL,160.0f,0.7,norm_img);
    //NormalizeLocalContrast(bw_img, NIL, 128.0f, 90.0f, 1, 0.1f, norm_img);
    //SaveImageToJpeg(norm_img,"/home/anvar/Downloads/norm_img", NIL, false);


    Array<Line2D> lines;
    Array<float> scores;
    Image vizualized_img, score_img;


//    DetectLines(clahe_img, NIL, 1.0f, 20.0f, 20.0f, 20.f, 10.0f, lines, scores, vizualized_img, score_img);

    int H = clahe_img.Height();
    int W = clahe_img.Width();

    Rectangle2D r({0,H/2},0,W,H/2);
    Region down_half;
    CreateRectangleRegion(r,NIL,W,H,down_half,NIL);



    ///// сглаживание изображения
    Image smooth_img;
    DilateImage(bw_img, down_half, NIL, NIL, Ellipse, 5, NIL, smooth_img);
    SaveImageToJpeg(smooth_img,"/home/anvar/Downloads/smooth_img", NIL, false);

    /// строим маску
    Image mask_img;
    ThresholdImage(smooth_img, NIL, 180.0f, 255.0f, 70.0f, mask_img);
    SaveImageToJpeg(mask_img,"/home/anvar/Downloads/mask_img", NIL, false);


    ////// убираем маленькие яркие точки с помощью OpenImage

    Image open_img;
    OpenImage(mask_img, NIL, NIL, NIL, Ellipse, 10, 6, open_img);
    SaveImageToJpeg(open_img,"/home/anvar/Downloads/open_img", NIL, false);


    //SaveImageToJpeg(vizualized_img,"/home/anvar/Downloads/vizualized_img", NIL, false);
    //SaveImageToJpeg(score_img,"/home/anvar/Downloads/score_img", NIL, false);

    /// DETECTING LINES
    DetectLines(open_img, down_half, 1.0f, 10.0f, 30.0f, 30.f, 30.0f, lines, scores);

    
    /// рисуем
    Image lines_img;
    Pixel pix;
    AvsFilter_MakePixel(1,0,0,1,pix);
    DrawingStyle drwstl;
    drwstl.thickness = 5;
    drwstl.filled = true;
    drwstl.opacity = 1;
    DrawLines_SingleColor(img, LinesArray(lines), NIL, pix , drwstl, true, lines_img);
    SaveImageToJpeg(lines_img,"/home/anvar/Downloads/lines_img", NIL, false);


    return 0;
}
