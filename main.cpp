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


#pragma region norm
    Image img;
    LoadImage("/home/anvar/Downloads/images/img2.jpg",false, img);

    int H = img.Height();
    int W = img.Width();

    //Rectangle2D r({0,H/2},0,W,H/2);
    Rectangle2D r({W*0.2f,H/2},0,W*0.6f,H/2*0.9f);
    Region down_half;
    CreateRectangleRegion(r,NIL,W,H,down_half,NIL);


    Image bw_img;
    AverageChannels(img, NIL,bw_img);
    SaveImageToJpeg(bw_img,"/home/anvar/Downloads/img0_bw", NIL, false);

    Image clahe_img;
    EqualizeImageHistogram(bw_img, NIL, 0.05, 0.35, clahe_img);
    SaveImageToJpeg(clahe_img,"/home/anvar/Downloads/img1_clahe", NIL, false);

#pragma endregion norm
    // сглаживание изображения
    Image smooth_img;
    DilateImage(bw_img, down_half, NIL, NIL, Ellipse, 3, NIL, smooth_img);
    SaveImageToJpeg(smooth_img,"/home/anvar/Downloads/img2_smooth", NIL, false);


    //Detecting edges

    Image outedges_img;
    Image canny_img;


    Image hsl_img;
    RgbToHsl(img, hsl_img);

    Image yellow_img, white_img, yw_img;

//TODO поменять коэффициенты
    ThresholdImage_HSx(hsl_img, down_half, HSxColorModel::Type::HSL, 0, 255, 160, 255, 140, 255, 2, yellow_img);
    ThresholdImage_HSx(hsl_img, down_half, HSxColorModel::Type::HSL, 0, 255, 0, 210, 170, 255, 2, white_img);


    AddImages(yellow_img, white_img, down_half, 1.0f, yw_img);
    SaveImageToJpeg(yw_img, "/home/anvar/Downloads/img3_yw_mask", NIL, false);

    //Умножение чб клахе на бело-желтое изображение
    Image final_mask;
    MultiplyImages(yw_img, clahe_img, down_half, 1.0f, final_mask);
    SaveImageToJpeg(final_mask,"/home/anvar/Downloads/img4_final_mask", NIL, false);

    Image smooth_mask;
    DilateImage(final_mask, down_half, NIL, NIL, Ellipse, 1, NIL, smooth_mask);
    //SmoothImage_Gauss(final_mask, down_half, 1.0f, 1.0f, 2.0f, smooth_mask);
    SaveImageToJpeg(smooth_mask,"/home/anvar/Downloads/img5_smooth_mask", NIL, false);

    Image final_img;
    DetectEdges(final_mask, down_half, avl::EdgeFilter::Canny, 2.0f, NIL, 15.0f, 5.0f, 0.0f, 1, outedges_img, canny_img);
    SaveImageToJpeg(outedges_img,"/home/anvar/Downloads/img6_outedges", NIL, false);



    Array<Line2D> lines;
    Array<float> scores;

    // рисуем
    Image lines_img;
    Pixel pix;
    AvsFilter_MakePixel(255,0,0,255,pix);
    DrawingStyle drwstl;
    drwstl.thickness = 5;
    drwstl.filled = true;
    drwstl.opacity = 1;

    //auto lines_ = LinesArray(lines);
   // DetectLines(outedges_img, down_half, 0.5f, 20.0f, 40.0f, 20.0f, 20.0f, lines, scores);
    DetectLines(outedges_img, down_half, 0.5f, 10.0f, 40.0f, 10.0f, 0.0f, lines, scores);

    //lines[0].a -=0.01;

    DrawLines_SingleColor(outedges_img, LinesArray(lines), NIL, pix , drwstl, true, lines_img);
    SaveImageToJpeg(lines_img,"/home/anvar/Downloads/img7_lines", NIL, false);
    DrawLines_SingleColor(img, LinesArray(lines), NIL, pix , drwstl, true, img);
    SaveImageToJpeg(img,"/home/anvar/Downloads/img8_lines", NIL, false);


    lines[0].a +=10;

    return 0;
}
