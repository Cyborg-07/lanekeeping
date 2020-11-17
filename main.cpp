#include "AVL.h"
#include <iostream>

using namespace std;

atl::Conditional<atl::Array<atl::Conditional<avl::Line2D>>> LinesArray(atl::Array<avl::Line2D>&);
avl::Region CreateROI(avl::Image&);
void SaveIMG(avl::Image, atl::File);

avl::Image LoadImage(atl::File);
avl::Image YWMask(avl::Image& img, avl::Region& region);

avl::Image BWImage(avl::Image img)
{
    avl::Image bw_img;
    AverageChannels(img, atl::NIL, bw_img);
    SaveIMG(bw_img, "/home/anvar/Downloads/img0_bw");
    return bw_img;
}
avl::Image ClaheImage(avl::Image& bw_img,avl::Region& region)
{
    avl::Image clahe_img;
    EqualizeImageHistogram(bw_img, atl::NIL, 0.05, 0.35, clahe_img);
    SaveIMG(clahe_img, "/home/anvar/Downloads/img1_clahe");

    return clahe_img;
}

avl::Image Multiply(avl::Image& a, avl::Image& b, avl::Region region)
{
    avl::Image result;
    MultiplyImages(a, b, region, 1.0f, result);
    SaveIMG(result, "/home/anvar/Downloads/img4_final_mask");
    return result;
}

int main() {
    avl::Image src_img(LoadImage("/home/anvar/Downloads/images/img2.jpg"));

    avl::Region downHalf = CreateROI(src_img);

    avl::Image yw_mask = YWMask(src_img, downHalf);

    auto bw_img = BWImage(src_img);
    auto clahe_img = ClaheImage(bw_img, downHalf);

    avl::Image final_mask = Multiply(yw_mask, clahe_img, downHalf);


    avl::Image smooth_mask;
    DilateImage(final_mask, downHalf, atl::NIL, atl::NIL, avl::KernelShape::Ellipse, 1, atl::NIL, smooth_mask);
    SaveIMG(smooth_mask, "/home/anvar/Downloads/img5_smooth_mask");

    avl::Image final_img;
    avl::Image outedges_img;
    avl::Image canny_img;
    DetectEdges(final_mask, downHalf, avl::EdgeFilter::Canny, 2.0f, atl::NIL, 15.0f, 5.0f, 0.0f, 1, outedges_img, canny_img);
    //DetectEdges(final_mask, downHalf, avl::EdgeFilter::Canny, 2.5, NIL, 20.0f, 30.0f, 1.0f, 1, outedges_img, canny_img);
    SaveIMG(outedges_img, "/home/anvar/Downloads/img6_outedges");


    atl::Array<avl::Line2D> lines;
    atl::Array<float> scores;

    // рисуем
    avl::Image lines_img;
    avl::Pixel pix;
    avs::AvsFilter_MakePixel(255,0,0,255,pix);
    avl::DrawingStyle drwstl;
    drwstl.thickness = 5;
    drwstl.filled = true;
    drwstl.opacity = 1;

    //auto lines_ = LinesArray(lines);
    //DetectLines(outedges_img, downHalf, 0.5f, 20.0f, 90.0f, 20.0f, 20.0f, lines, scores);
    DetectLines(outedges_img, downHalf, 1, 1, 10, 10.0f, 20, lines, scores);
    //DetectLines(outedges_img, downHalf, 0.5f, 20.0f, 60.0f, 40.0f, 10.0f, lines, scores);

    atl::Array<avl::Line2D> new_lines(10);


    for (int i=0; i<lines.Size(); i++)
    {
        if ((abs(lines[i].a/lines[i].b) > 0.95f) && (abs(lines[i].a/lines[i].b) < 1.3f) )
        {
            cout <<"tan = "<< abs(lines[i].a/lines[i].b) << endl;
            new_lines.Insert(i, lines[i]);
        }
    }


    //avs::DrawLines_SingleColor(outedges_img, LinesArray(new_lines), atl::NIL, pix , drwstl, true, lines_img);
    avs::DrawLines_SingleColor(outedges_img, LinesArray(lines), atl::NIL, pix , drwstl, true, lines_img);
    SaveIMG(lines_img, "/home/anvar/Downloads/img7_lines");
    avs::DrawLines_SingleColor(src_img, LinesArray(new_lines), atl::NIL, pix , drwstl, true, src_img);
    SaveIMG(src_img, "/home/anvar/Downloads/img8_lines");

    return 0;
}

avl::Region CreateROI(avl::Image& image) {
    int H = image.Height();
    int W = image.Width();
    avl::Point2D origin({W*0.2f,H/2.0f});
    float width = W * 0.6f;
    float heigth = H / 2.0f * 0.9f;
    avl::Rectangle2D r(origin,0, width, heigth);
    avl::Region down_half;
    CreateRectangleRegion(r,atl::NIL,W,H,down_half,atl::NIL);

    return down_half;
}

void SaveIMG(avl::Image img, atl::File path)
{
    bool inIgnoreErrors = false;
    atl::Optional<int> quality = 100;
    avl::SaveImageToJpeg(img, path, quality, inIgnoreErrors);
}

avl::Image YWMask(avl::Image& img, avl::Region& region)
{
    avl::Image hsl_img;
    RgbToHsl(img, hsl_img);

    avl::Image yellow_img, white_img, yw_img;

//TODO поменять коэффициенты
    ThresholdImage_HSx(hsl_img, region, avl::HSxColorModel::Type::HSL, 0, 255, 160, 255, 140, 255, 2, yellow_img);
    ThresholdImage_HSx(hsl_img, region, avl::HSxColorModel::Type::HSL, 0, 255, 0, 210, 170, 255, 2, white_img);

    AddImages(yellow_img, white_img, region, 1.0f, yw_img);
    //SaveImageToJpeg(yw_img, "/home/anvar/Downloads/img3_yw_mask", atl::NIL, false);
    SaveIMG(yw_img, "/home/anvar/Downloads/img3_yw_mask");

    return yw_img;

}

avl::Image LoadImage(atl::File file) {
    avl::Image img;
    LoadImage(file,false, img);

    return img;
}

atl::Conditional<atl::Array<atl::Conditional<avl::Line2D>>> LinesArray(atl::Array<avl::Line2D>& lines)
{
    auto arr1 = atl::Array<atl::Conditional<avl::Line2D>>(lines.Size()); //Array<Line2D>
    for(int i=0;i<arr1.Size();++i)
        arr1[i] = atl::Conditional<avl::Line2D>(lines[i]);
    return atl::Conditional<atl::Array<atl::Conditional<avl::Line2D>>>(arr1);
}

