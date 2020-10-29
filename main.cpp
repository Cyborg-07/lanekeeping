#include <iostream>
#include "AVL.h"


using namespace avl;
using namespace std;
using namespace atl;

int main() {

    //считываем изображение
    Image img;
    LoadImage("/home/anvar/Downloads/images/img1.jpg",false, img);
    
    // переводим в формат hsv (пока еще его не использовал)
    Image hsv_img;
    RgbToHsv(img, hsv_img);

    Image bw_img;
    
    //перевод изображения в ЧБ
    AverageChannels(img, NIL,bw_img);
    SaveImageToJpeg(bw_img,"/home/anvar/Downloads/bw_img", NIL, false);

    //1) обработка с помощью выравнивания гистограммы (пока еще не уверен, что это лучший способ обработки картинки)
    Image clahe_img;
    EqualizeImageHistogram(bw_img, NIL, 0.01, 0.001, clahe_img);
    SaveImageToJpeg(clahe_img,"/home/anvar/Downloads/clahe_img", NIL, false);

    //2) обработка вторым способом (пока еще не подобрал хорошие коэффициенты)
    NormalizeLocalBrightness_Gauss(bw_img,NIL,5.0f,NIL,128.0f,1,bw_img);
    NormalizeLocalContrast(bw_img, NIL, 128.0f, 90.0f, 1, 0.1f, bw_img); // выдает ошибку, хз почему
    SaveImageToJpeg(bw_img,"/home/anvar/Downloads/norm_img", NIL, false);


    // из обработанного изображения с помощью трешхолда выделяем маску, например, белый цвет
    Image mask_img;
    ThresholdImage(clahe_img, NIL, 220.0f, 255.0f, 0.0f, mask_img);
    SaveImageToJpeg(mask_img,"/home/anvar/Downloads/mask_img", NIL, false);



    return 0;
}
