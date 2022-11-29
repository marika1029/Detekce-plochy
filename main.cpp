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
#include <opencv2/core/matx.hpp>
#include <fstream>

using namespace cv;
using namespace std;

Mat image, image_gray, image_blur, image_rect, image_rect2, image_test, edges, hrany;

int y_n;
int x2;
int x3;
int y2;
int y3;

vector<int> index1;

int minKontrast = 0;
const int max_minKontrast = 300;
const int ratio1 = 0.2;
const int kernel_size = 3;

vector<int> index2;
vector<int> index3;
int y_n2;

//bude uživateli ukazovat jaké souřadnice má bod na kterém je myš pokud na bod klikne
static void onClick(int event, int x, int y, int flags, void* param)
{
    if (event==1)
        {
            //cout << "souradnice x = " << x << "; souradnice y = " << y << endl;
            index1.push_back(x);
            index1.push_back(y);
        }
}

//převede jakékoliv dva opoziční nakliknuté rohové body na potřebné souřadnice pro další funkce
int indexuj(vector<int> index2)
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

//detekuje hrany v obrázku
static void CannyDetekce(int, void*)
{
    Canny( image_rect, image_rect2, minKontrast, minKontrast*ratio1, kernel_size );
    hrany = Scalar::all(0);
    image_rect.copyTo( hrany, image_rect2);
    imshow( "Nalezené hrany", hrany );
}

//bude uživateli ukazovat jaké souřadnice má bod na kterém je myš pokud na bod klikne
static void onClickPlocha(int event, int x, int y, int flags, void* param)
{
    if (event==1)
        {
            cout << "souradnice x = " << x << "; souradnice y = " << y << endl;
            index2.push_back(x);
            index2.push_back(y);
        }
}

static void onClickMeritko(int event, int x, int y, int flags, void* param)
{
    if (event==1)
        {
            //cout << "souradnice x = " << x << "; souradnice y = " << y << endl;
            index3.push_back(x);
            index3.push_back(y);
        }
}

//funkce pro výpočet obsahu plochy
float plocha(vector<int> index2,Mat hrany,vector<int> index3)
{
    int pocitadlo = 0;
    int x5 = index2[index2.size()-2];
    int y5 = index2[index2.size()-1];
    int pocetni_matice[hrany.cols][hrany.rows] = {0};
    pocetni_matice[x5][y5] = 1;

    int y_horni = y5;
    int y_dolni = y5;
    int x_horni = x5;
    int x_dolni = x5;

    int x_meritko = index3[index3.size()-2] - index3[index3.size()-4];
    int y_meritko = index3[index3.size()-1] - index3[index3.size()-3];
    float meritko = sqrt((x_meritko*x_meritko)+(y_meritko*y_meritko));

    float plocha;

    do
    {
        if(y_horni < hrany.rows-1)
            {
                ++y_horni;
            }
        if(y_dolni > 1)
            {
                --y_dolni;
            }
        if(x_horni < hrany.cols-1)
            {
                ++x_horni;
            }
        if(x_dolni > 1)
            {
                --x_dolni;
            }

        for(int xi=x_dolni;xi<x_horni;++xi)
        {
            for(int yi=y_dolni;yi<y_horni;++yi)
            {
                if(pocetni_matice[xi][yi]==1)
                {
                    if(xi < hrany.cols-1)
                    {
                        if(hrany.at<Vec3b>(Point(xi+1,yi))[0]<30 && hrany.at<Vec3b>(Point(xi+1,yi))[1]<30 && hrany.at<Vec3b>(Point(xi+1,yi))[2]<30)
                        {
                            pocetni_matice[xi+1][yi]=1;
                        }
                    }
                    if(xi > 1)
                    {
                        if(hrany.at<Vec3b>(Point(xi-1,yi))[0]<30 && hrany.at<Vec3b>(Point(xi-1,yi))[1]<30 && hrany.at<Vec3b>(Point(xi-1,yi))[2]<30)
                        {
                            pocetni_matice[xi-1][yi]=1;
                        }
                    }
                    if(yi < hrany.rows-1)
                    {
                        if(hrany.at<Vec3b>(Point(xi,yi+1))[0]<30 && hrany.at<Vec3b>(Point(xi,yi+1))[1]<30 && hrany.at<Vec3b>(Point(xi,yi+1))[2]<30)
                        {
                            pocetni_matice[xi][yi+1]=1;
                        }
                    }
                    if(yi > 1)
                    {
                        if(hrany.at<Vec3b>(Point(xi,yi-1))[0]<30 && hrany.at<Vec3b>(Point(xi,yi-1))[1]<30 && hrany.at<Vec3b>(Point(xi,yi-1))[2]<30)
                        {
                            pocetni_matice[xi][yi-1]=1;
                        }
                    }
                }
            }
        }
    }while(x_dolni > 1 || y_dolni > 1 || x_horni < hrany.cols-1 || y_horni < hrany.rows-1);

    for(int i=1;i<hrany.cols-1;++i)
    {
        for(int j=1;j<hrany.rows-1;++j)
        {
            if(pocetni_matice[i][j]==1)
            {
                ++pocitadlo;
            }
        }
    }

    ofstream Ukladani("maticeplochy.txt");
    for(int i=1;i<hrany.cols-1;++i)
    {
        for(int j=1;j<hrany.rows-1;++j)
        {
            Ukladani << pocetni_matice[i][j] <<"; ";
            if(j==hrany.rows-1)
            {
                Ukladani << "\n";
            }
        }
    }

    plocha = pocitadlo/(meritko*meritko);

    return plocha;
}

int main ()
{
    //načtení obrázku
    image = imread("obrazek3.png");
    if (image.empty())
    {
        cout << "Soubor nenalezen." << endl;
        return -1;
    }

    cvtColor(image, image_gray, COLOR_BGR2GRAY); //převede do odstínů šedé
    GaussianBlur(image_gray, image_blur, Size(3, 3), 0); //vzhlazení pomocí blur, medianBlur i bilateralFilter ničí hrany (parametry 3.3 zvoleny aby byla zachována co nejvyšší přesnost)

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
    //namedWindow("Vyznačená obdélníková oblast (odstíny šedé)",WINDOW_NORMAL);
    //imshow("Vyznačená obdélníková oblast (odstíny šedé)", image_rect);
    //resizeWindow("Vyznačená obdélníková oblast (odstíny šedé)",800,800);

    image_rect2 = image(Range(y3,y2), Range(x2,x3));
    //namedWindow("Vyznačená obdélníková oblast",WINDOW_NORMAL);
    //imshow("Vyznačená obdélníková oblast", image_rect2);
    //resizeWindow("Vyznačená obdélníková oblast",800,800);
    //waitKey(0);

    //vvykreslí okno s posuvnou lištou a černobílím obrázkem s detekovanými hranami (přepočet probíhá v sekci CannyThreshold)
    hrany.create(image_rect.size(),image_rect.type());
    namedWindow( "Nalezené hrany", WINDOW_NORMAL);
    resizeWindow("Nalezené hrany",1500,1000);
    createTrackbar( "Min Threshold:", "Nalezené hrany", &minKontrast, max_minKontrast, CannyDetekce);
    CannyDetekce(0, 0);
    waitKey(0);

    //smyčka pro označení oblasti pro výpočet plochy
    loop2:
        image_test = Mat();
        hrany.copyTo(image_test); //inicializace obrásků, aby neosahovali dříve označené body + vytvoření kopie abyhom si nezničili originál

        //vykreslí nalezené hrany a umožní uživateli označit kliknutím hledanou plochu
        namedWindow("Klikněte do hledané oblasti:",WINDOW_NORMAL);
        imshow("Klikněte do hledané oblasti:", hrany);
        resizeWindow("Klikněte do hledané oblasti:",1500,1000);
        setMouseCallback("Klikněte do hledané oblasti:", onClickPlocha, 0);
        waitKey(0);

        //vykreslí označený bod
        Point p3(index2[index2.size()-2],index2[index2.size()-1]);
        Point p4(index2[index2.size()-2],index2[index2.size()-1]);
        rectangle(image_test,p3,p4, Scalar(255,255,255), thickness, LINE_8);
        namedWindow("Obrázek s vyznačeným bodem v hledané oblasti: ",WINDOW_NORMAL);
        imshow("Obrázek s vyznačeným bodem v hledané oblasti: ",image_test);
        resizeWindow("Obrázek s vyznačeným bodem v hledané oblasti: ",1500,1000);
        waitKey(0);

        namedWindow("Vyznačte měřítko o velikosti 1: ",WINDOW_NORMAL);
        imshow("Vyznačte měřítko o velikosti 1: ",hrany);
        resizeWindow("Vyznačte měřítko o velikosti 1: ",1500,1000);
        setMouseCallback("Vyznačte měřítko o velikosti 1: ", onClickMeritko, 0);
        waitKey(0);

        //dotaz na uživatele zda je se svým výběrem spokojen
        cout << "Pokud souhlasíte se zvolenou plochou napište 1 jinak vypište 0: "; cin >> y_n2;
        if (y_n2 == 0)
        {
            goto loop2;
        }

    //výpočet obsahu plochy
    namedWindow("Hrany",WINDOW_NORMAL);
    imshow("Hrany",hrany);
    resizeWindow("Hrany",1500,1000);
    cout << "Plocha je: " << plocha(index2,hrany,index3);
    waitKey(0);

    return 0;
}
