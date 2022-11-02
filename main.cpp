#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

Mat img, img_gray, img_blur, obd;

static void onClick(int event, int x, int y, int flags, void* param) //bude uživateli ukazovat jaké souřadnice má bod na kterém je myš
{
    if (event==1)
        {
            cout << "souradnice x = " << x << "; souradnice y = " << y << endl;
        }
}

int main ()
{
    //načtení obrázku
    img = imread("obrazek.bmp");
    if (img.empty())
    {
        cout << "Soubor nenalezen." << endl;
        return -1;
    }

    cvtColor(img, img_gray, COLOR_BGR2GRAY); //převede do odstínů šedé
    GaussianBlur(img_gray, img_blur, Size(3, 3), 0); //vzhlazení pomocí blur, medianBlur i bilateralFilter ničí hrany (parametry 3.3 zvoleny aby byla zachována co nejvyšší přesnost)

    //zobrazí obrázky po převedení do spektra šedé a po aplikaci vyhlazení
    imshow("Obrázek černobíle", img_gray);
    imshow("Gaussovo vyhlazování", img_blur);
    waitKey(0);

    //výběr oblasti o kterou se zajímáme (zobrazí souřadnice bodu na který klikne uživatel miší)
    imshow("Označte oblast hledané plochy:", img_blur);
    setMouseCallback("Označte oblast hledané plochy:", onClick, 0);
    waitKey(0);

    int x1;
    int x2;
    int y1;
    int y2;

    cout << "Zapište souřadnici x levého horního bodu: "; cin >> x1;
    cout << "Zapište souřadnici y levého horního bodu: "; cin >> y1;
    cout << "Zapište souřadnici x pravého dolního bodu: "; cin >> x2;
    cout << "Zapište souřadnici y pravého dolního bodu: "; cin >> y2;

    //vytvoření obdélníku s hledanou oblastí

    obd = img(Rect(x1,y1,x2,y2));
    imshow("Vyznačená obdélníková oblast", obd);
    waitKey(0);

    //dále aplikace algoritmu na hledání hran v danném rozsahu
    //kontrola uzavřenosti oblasti (případně návrat do předchozího kroku s nižším kontrasten...bylo by možné využít toho co měli v projektu naši předchůdci a nechat uživatele mrknout kdy už to najde celou oblast a pak ho nechat zadat požadovaný kontrast případně umožnit změnu hledaného kontrastu jen v urřité podoblasti)
    //zobrazení nalezené oblasti
    //výpočet plochy této oblasti
    return 0;
}
