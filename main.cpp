#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>

using namespace cv;
using namespace std;

Mat image, image_gray, image_blur, image_rect, image_rect2, image_test;
int y_n;
int x2;
int x3;
int y2;
int y3;

static void onClick(int event, int x, int y, int flags, void* param) //bude uživateli ukazovat jaké souřadnice má bod na kterém je myš pokud na bod klikne
{
    if (event==1)
        {
            cout << "souradnice x = " << x << "; souradnice y = " << y << endl;
        }
}

int main ()
{
    //načtení obrázku
    image = imread("obrazek.bmp");
    if (image.empty())
    {
        cout << "Soubor nenalezen." << endl;
        return -1;
    }

    cvtColor(image, image_gray, COLOR_BGR2GRAY); //převede do odstínů šedé
    GaussianBlur(image_gray, image_blur, Size(3, 3), 0); //vzhlazení pomocí blur, medianBlur i bilateralFilter ničí hrany (parametry 3.3 zvoleny aby byla zachována co nejvyšší přesnost)


    //zobrazí obrázky po převedení do spektra šedé a po aplikaci vyhlazení
    imshow("Obrázek černobíle", image_gray);
    imshow("Gaussovo vyhlazování", image_blur);
    waitKey(0);

    //výběr oblasti o kterou se zajímáme (zobrazí souřadnice bodu na který klikne uživatel miší) a nechá uživatele zapsat souřadnice dvou bodů oblasti s vyšetřovanou plochou (levý horní bod a pravý dolní bod)
loop:
    image_test = Mat();
    image_blur.copyTo(image_test);

    imshow("Označte oblast hledané plochy:", image_blur);
    setMouseCallback("Označte oblast hledané plochy:", onClick, 0);
    waitKey(0);

    cout << "Zapište souřadnici x levého horního bodu: "; cin >> x2;
    cout << "Zapište souřadnici y levého horního bodu: "; cin >> y2;
    cout << "Zapište souřadnici x pravého dolního bodu: "; cin >> x3;
    cout << "Zapište souřadnici y pravého dolního bodu: "; cin >> y3;

    //vytvoření obdélníku s hledanou oblastí k nahlédnutí zda se vytvořil správně
    int thickness = 2;
    Point p1(x2,y2);
    Point p2(x3,y3);
    rectangle(image_test,p1,p2, Scalar(255,255,255), thickness, LINE_8);
    imshow("Obrázek s vyznačenou oblastí zájmu: ",image_test);
    waitKey(0);

    //ptá se uživatele zda souhlasí s vybranou oblastí a případně ho vrací do kroku vypisování souřadnic
    cout << "Pokud souhlasíte se zvoleným obdélníkem napište 1 jinak vypište 0: "; cin >> y_n;
    if (y_n == 0)
    {
        goto loop;
    }

    //vyříznutí vyšetřovaného obdélníku z obrázku
    image_rect = image_blur(Range(y2,y3), Range(x2,x3));
    imshow("Vyznačená obdélníková oblast", image_rect);
    waitKey(0);

    //dále aplikace algoritmu na hledání hran v danném rozsahu
    //kontrola uzavřenosti oblasti (případně návrat do předchozího kroku s nižším kontrasten...bylo by možné využít toho co měli v projektu naši předchůdci a nechat uživatele mrknout kdy už to najde celou oblast a pak ho nechat zadat požadovaný kontrast případně umožnit změnu hledaného kontrastu jen v urřité podoblasti)
    //zobrazení nalezené oblasti
    //výpočet plochy této oblasti
    return 0;
}
