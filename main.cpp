#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <vector>
#include <string.h>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

Mat image, image_gray, image_blur, image_rect, image_rect2, image_test;

int y_n;
int x2;
int x3;
int y2;
int y3;

vector<int> index1;

static void onClick(int event, int x, int y, int flags, void* param) //bude uživateli ukazovat jaké souřadnice má bod na kterém je myš pokud na bod klikne
{
    if (event==1)
        {
            cout << "souradnice x = " << x << "; souradnice y = " << y << endl;
            index1.push_back(x);
            index1.push_back(y);
        }
}
int indexuj(vector<int> index2) //převede jakékoliv dva nakliknuté opoziční body na potřebné souřadnice pro další funkce
{
    if(index2[index2.size()-4]<index2[index2.size()-2] && index2[index2.size()-3]>index2[index2.size()-1])
    {
        y3 = index2[index2.size()-1];
        x3 = index2[index2.size()-2];
        y2 = index2[index2.size()-3];
        x2 = index2[index2.size()-4];
    }

    if(index2[index2.size()-4]>index2[index2.size()-2] && index2[index2.size()-3]<index2[index2.size()-1])
    {
        y3 = index2[index2.size()-3];
        x3 = index2[index2.size()-4];
        y2 = index2[index2.size()-1];
        x2 = index2[index2.size()-2];
    }

    if(index2[index2.size()-4]<index2[index2.size()-2] && index2[index2.size()-3]<index2[index2.size()-1])
    {
        y3 = index2[index2.size()-3];
        x3 = index2[index2.size()-2];
        y2 = index2[index2.size()-1];
        x2 = index2[index2.size()-4];
    }

    if(index2[index2.size()-4]>index2[index2.size()-2] && index2[index2.size()-3]>index2[index2.size()-1])
    {
        y3 = index2[index2.size()-1];
        x3 = index2[index2.size()-4];
        y2 = index2[index2.size()-3];
        x2 = index2[index2.size()-2];
    }
    return y2,y3,x2,x3;
}

int main ()
{
    //načtení obrázku
    image = imread("obrazek.png");
    if (image.empty())
    {
        cout << "Soubor nenalezen." << endl;
        return -1;
    }

    cvtColor(image, image_gray, COLOR_BGR2GRAY); //převede do odstínů šedé
    GaussianBlur(image_gray, image_blur, Size(3, 3), 0); //vzhlazení pomocí blur, medianBlur i bilateralFilter ničí hrany (parametry 3.3 zvoleny aby byla zachována co nejvyšší přesnost)


    //zobrazí obrázky po převedení do spektra šedé a po aplikaci vyhlazení
//    namedWindow("Originální obrázek",WINDOW_NORMAL);
//    imshow("Originální obrázek",image);
//    resizeWindow("Originální obrázek",1200,900);

//    namedWindow("Obrázek černobíle",WINDOW_NORMAL);
//    imshow("Obrázek černobíle", image_gray);
//    resizeWindow("Obrázek černobíle",1200,900);

//    namedWindow("Gaussovo vyhlazování",WINDOW_NORMAL);
//    imshow("Gaussovo vyhlazování", image_blur);
//    resizeWindow("Gaussovo vyhlazování",1200,900);
//    waitKey(0);

    //výběr oblasti o kterou se zajímáme (zobrazí souřadnice bodu na který klikne uživatel miší) a nechá uživatele zapsat souřadnice dvou bodů oblasti s vyšetřovanou plochou (levý horní bod a pravý dolní bod)
loop:
    image_test = Mat();
    image_blur.copyTo(image_test);

    namedWindow("Označte oblast hledané plochy:",WINDOW_NORMAL);
    imshow("Označte oblast hledané plochy:", image_blur);
    resizeWindow("Označte oblast hledané plochy:",1500,1000);
    setMouseCallback("Označte oblast hledané plochy:", onClick, 0);
    waitKey(0);

    indexuj(index1);
    //vytvoření obdélníku s hledanou oblastí k nahlédnutí zda se vytvořil správně
    int thickness = 4;
    Point p1(x2,y2);
    Point p2(x3,y3);
    rectangle(image_test,p1,p2, Scalar(255,255,255), thickness, LINE_8);
    namedWindow("Obrázek s vyznačenou oblastí zájmu: ",WINDOW_NORMAL);
    imshow("Obrázek s vyznačenou oblastí zájmu: ",image_test);
    resizeWindow("Obrázek s vyznačenou oblastí zájmu: ",1500,1000);
    waitKey(0);

    //ptá se uživatele zda souhlasí s vybranou oblastí a případně ho vrací do kroku vypisování souřadnic
    cout << "Pokud souhlasíte se zvoleným obdélníkem napište 1 jinak vypište 0: "; cin >> y_n;
    if (y_n == 0)
    {
        goto loop;
    }

    //vyříznutí vyšetřovaného obdélníku z obrázku

    image_rect = image_blur(Range(y3,y2), Range(x2,x3));
    namedWindow("Vyznačená obdélníková oblast (odstíny šedé)",WINDOW_NORMAL);
    imshow("Vyznačená obdélníková oblast (odstíny šedé)", image_rect);
    resizeWindow("Vyznačená obdélníková oblast (odstíny šedé)",800,800);

    image_rect2 = image(Range(y3,y2), Range(x2,x3));
    namedWindow("Vyznačená obdélníková oblast",WINDOW_NORMAL);
    imshow("Vyznačená obdélníková oblast", image_rect2);
    resizeWindow("Vyznačená obdélníková oblast",800,800);
    waitKey(0);

    //dále aplikace algoritmu na hledání hran v danném rozsahu
    //kontrola uzavřenosti oblasti (případně návrat do předchozího kroku s nižším kontrasten...bylo by možné využít toho co měli v projektu naši předchůdci a nechat uživatele mrknout kdy už to najde celou oblast a pak ho nechat zadat požadovaný kontrast případně umožnit změnu hledaného kontrastu jen v urřité podoblasti)
    //zobrazení nalezené oblasti
    //výpočet plochy této oblasti
    return 0;
}
