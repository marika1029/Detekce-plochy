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
#include <fstream> //načítání, ukládání a práce se soubory
#include <cstdlib>
#include <cstring>

using namespace cv;
using namespace std;


//definice všech používaných proměnných
Mat image, image_gray, image_blur, image_rect, image_rect2, image_test, edges, hrany,hrany_closed;

int y_n;
int x2;
int x3;
int y2;
int y3;

vector<int> index1;

int minKontrast = 0;
const int max_minKontrast = 255;
const int ratio1 = 1;
const int kernel_size = 3;

vector<int> index2;
vector<int> index3;
vector<int> index4;
vector<int> index5;
int y_n2;
int y_n3;
int y_n4;
int y_n5;





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
    return y2, y3, x2, x3;
}





//detekuje hrany v obrázku
static void CannyDetekce(int , void* )
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
            //cout << "souradnice x = " << x << "; souradnice y = " << y << endl;
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
    int pocetni_matice[hrany.rows][hrany.cols] = {0};
    pocetni_matice[y5][x5] = 1;

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
        if(y_horni < hrany.rows)
            {
                ++y_horni;
            }
        if(y_dolni > 1)
            {
                --y_dolni;
            }
        if(x_horni < hrany.cols)
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
                if(pocetni_matice[yi][xi]==1)
                {
                    if(xi < hrany.cols-1)
                    {
                        if(hrany.at<Vec3b>(Point(xi+1,yi)).val[0]==0 && hrany.at<Vec3b>(Point(xi+1,yi)).val[1]==0 && hrany.at<Vec3b>(Point(xi+1,yi)).val[2]==0)
                        {
                            pocetni_matice[yi][xi+1]=1;
                        }
                    }
                    if(xi > 1)
                    {
                        if(hrany.at<Vec3b>(Point(xi-1,yi)).val[0]==0 && hrany.at<Vec3b>(Point(xi-1,yi)).val[1]==0 && hrany.at<Vec3b>(Point(xi-1,yi)).val[2]==0)
                        {
                            pocetni_matice[yi][xi-1]=1;
                        }
                    }
                    if(yi < hrany.rows-1)
                    {
                        if(hrany.at<Vec3b>(Point(xi,yi+1)).val[0]==0 && hrany.at<Vec3b>(Point(xi,yi+1)).val[1]==0 && hrany.at<Vec3b>(Point(xi,yi+1)).val[2]==0)
                        {
                            pocetni_matice[yi+1][xi]=1;
                        }
                    }
                    if(yi > 1)
                    {
                        if(hrany.at<Vec3b>(Point(xi,yi-1)).val[0]==0 && hrany.at<Vec3b>(Point(xi,yi-1)).val[1]==0 && hrany.at<Vec3b>(Point(xi,yi-1)).val[2]==0)
                        {
                            pocetni_matice[yi-1][xi]=1;
                        }
                    }
                }
            }
        }
    }while(x_dolni > 1 || y_dolni > 1 || x_horni < hrany.cols || y_horni < hrany.rows);

    for(int i=1;i<hrany.cols;++i)
    {
        for(int j=1;j<hrany.rows;++j)
        {
            if(pocetni_matice[j][i]==1)
            {
                ++pocitadlo;
                hrany.at<Vec3b>(Point(i,j)).val[0]=0;
                hrany.at<Vec3b>(Point(i,j)).val[1]=230;
                hrany.at<Vec3b>(Point(i,j)).val[2]=255;
            }
        }
    }

    ofstream Ukladani("maticeplochy.txt");
    ofstream Ukladani2("maticeplochy2.txt");
    for(int i=1;i<hrany.cols;++i)
    {
        for(int j=1;j<hrany.rows;++j)
        {
            //Ukladani << pocetni_matice[j][i] <<"; ";
            //Ukladani << hrany.at<Vec3b>(i,j) << ";";
            //Ukladani2 << pocetni_matice[j][i] <<"; ";
            //Ukladani2 << "[" << i << ";" << j << "]";
        }
    }

    plocha = pocitadlo/(meritko*meritko);

    return plocha;
    //return pocitadlo;
}




static void onClickHrany(int event, int x, int y, int flags, void* param)
{
    if (event==1)
        {
            //cout << "souradnice x = " << x << "; souradnice y = " << y << endl;
            index4.push_back(x);
            index4.push_back(y);
        }
}





static void onClickHranypridej(int event, int x, int y, int flags, void* param)
{
    if (event==1)
        {
            //cout << "souradnice x = " << x << "; souradnice y = " << y << endl;
            index5.push_back(x);
            index5.push_back(y);
        }
}



void odstranhranu(Mat hrany,vector<int> index4)
{
    vector<int> fronta = {index4[index4.size()-2],index4[index4.size()-1]};

        for(int i=0;i<fronta.size()/2;++i)
        {
            int x6=fronta[2*i];
            int y6=fronta[2*i+1];

            if(hrany_closed.at<Vec3b>(Point(x6,y6)).val[0]!=0 && hrany_closed.at<Vec3b>(Point(x6,y6)).val[1]!=0 && hrany_closed.at<Vec3b>(Point(x6,y6)).val[2]!=0)
            {
                hrany_closed.at<Vec3b>(Point(x6,y6)).val[0]=0;
                hrany_closed.at<Vec3b>(Point(x6,y6)).val[1]=0;
                hrany_closed.at<Vec3b>(Point(x6,y6)).val[2]=0;

                if(hrany_closed.at<Vec3b>(Point(x6+1,y6)).val[0]!=0 && hrany_closed.at<Vec3b>(Point(x6+1,y6)).val[1]!=0 && hrany_closed.at<Vec3b>(Point(x6+1,y6)).val[2]!=0)
                {
                    fronta.push_back(x6+1);
                    fronta.push_back(y6);
                }
                if(hrany_closed.at<Vec3b>(Point(x6-1,y6)).val[0]!=0 && hrany_closed.at<Vec3b>(Point(x6-1,y6)).val[1]!=0 && hrany_closed.at<Vec3b>(Point(x6-1,y6)).val[2]!=0)
                {
                    fronta.push_back(x6-1);
                    fronta.push_back(y6);
                }
                if(hrany_closed.at<Vec3b>(Point(x6,y6+1)).val[0]!=0 && hrany_closed.at<Vec3b>(Point(x6,y6+1)).val[1]!=0 && hrany_closed.at<Vec3b>(Point(x6,y6+1)).val[2]!=0)
                {
                    fronta.push_back(x6);
                    fronta.push_back(y6+1);
                }
                if(hrany_closed.at<Vec3b>(Point(x6,y6-1)).val[0]!=0 && hrany_closed.at<Vec3b>(Point(x6,y6-1)).val[1]!=0 && hrany_closed.at<Vec3b>(Point(x6,y6-1)).val[2]!=0)
                {
                    fronta.push_back(x6);
                    fronta.push_back(y6-1);
                }
                if(hrany_closed.at<Vec3b>(Point(x6-1,y6-1)).val[0]!=0 && hrany_closed.at<Vec3b>(Point(x6-1,y6-1)).val[1]!=0 && hrany_closed.at<Vec3b>(Point(x6-1,y6-1)).val[2]!=0)
                {
                    fronta.push_back(x6-1);
                    fronta.push_back(y6-1);
                }
                if(hrany_closed.at<Vec3b>(Point(x6+1,y6-1)).val[0]!=0 && hrany_closed.at<Vec3b>(Point(x6+1,y6-1)).val[1]!=0 && hrany_closed.at<Vec3b>(Point(x6+1,y6-1)).val[2]!=0)
                {
                    fronta.push_back(x6+1);
                    fronta.push_back(y6-1);
                }
                if(hrany_closed.at<Vec3b>(Point(x6+1,y6+1)).val[0]!=0 && hrany_closed.at<Vec3b>(Point(x6+1,y6+1)).val[1]!=0 && hrany_closed.at<Vec3b>(Point(x6+1,y6+1)).val[2]!=0)
                {
                    fronta.push_back(x6+1);
                    fronta.push_back(y6+1);
                }
                if(hrany_closed.at<Vec3b>(Point(x6-1,y6+1)).val[0]!=0 && hrany_closed.at<Vec3b>(Point(x6-1,y6+1)).val[1]!=0 && hrany_closed.at<Vec3b>(Point(x6-1,y6+1)).val[2]!=0)
                {
                    fronta.push_back(x6-1);
                    fronta.push_back(y6+1);
                }
            }
            if(hrany_closed.at<Vec3b>(Point(x6,y6)).val[0]==100 && hrany_closed.at<Vec3b>(Point(x6,y6)).val[1]==0 && hrany_closed.at<Vec3b>(Point(x6,y6)).val[2]==255)
            {
                hrany_closed.at<Vec3b>(Point(x6,y6)).val[0]=0;
                hrany_closed.at<Vec3b>(Point(x6,y6)).val[1]=0;
                hrany_closed.at<Vec3b>(Point(x6,y6)).val[2]=0;

                if(hrany_closed.at<Vec3b>(Point(x6+1,y6)).val[0]==100 && hrany_closed.at<Vec3b>(Point(x6+1,y6)).val[1]==0 && hrany_closed.at<Vec3b>(Point(x6+1,y6)).val[2]==255)
                {
                    fronta.push_back(x6+1);
                    fronta.push_back(y6);
                }
                if(hrany_closed.at<Vec3b>(Point(x6-1,y6)).val[0]==100 && hrany_closed.at<Vec3b>(Point(x6-1,y6)).val[1]==0 && hrany_closed.at<Vec3b>(Point(x6-1,y6)).val[2]==255)
                {
                    fronta.push_back(x6-1);
                    fronta.push_back(y6);
                }
                if(hrany_closed.at<Vec3b>(Point(x6,y6+1)).val[0]==100 && hrany_closed.at<Vec3b>(Point(x6,y6+1)).val[1]==0 && hrany_closed.at<Vec3b>(Point(x6,y6+1)).val[2]==255)
                {
                    fronta.push_back(x6);
                    fronta.push_back(y6+1);
                }
                if(hrany_closed.at<Vec3b>(Point(x6,y6-1)).val[0]==100 && hrany_closed.at<Vec3b>(Point(x6,y6-1)).val[1]==0 && hrany_closed.at<Vec3b>(Point(x6,y6-1)).val[2]==255)
                {
                    fronta.push_back(x6);
                    fronta.push_back(y6-1);
                }
                if(hrany_closed.at<Vec3b>(Point(x6-1,y6-1)).val[0]==100 && hrany_closed.at<Vec3b>(Point(x6-1,y6-1)).val[1]==0 && hrany_closed.at<Vec3b>(Point(x6-1,y6-1)).val[2]==255)
                {
                    fronta.push_back(x6-1);
                    fronta.push_back(y6-1);
                }
                if(hrany_closed.at<Vec3b>(Point(x6+1,y6-1)).val[0]==100 && hrany_closed.at<Vec3b>(Point(x6+1,y6-1)).val[1]==0 && hrany_closed.at<Vec3b>(Point(x6+1,y6-1)).val[2]==255)
                {
                    fronta.push_back(x6+1);
                    fronta.push_back(y6-1);
                }
                if(hrany_closed.at<Vec3b>(Point(x6+1,y6+1)).val[0]==100 && hrany_closed.at<Vec3b>(Point(x6+1,y6+1)).val[1]==0 && hrany_closed.at<Vec3b>(Point(x6+1,y6+1)).val[2]==255)
                {
                    fronta.push_back(x6+1);
                    fronta.push_back(y6+1);
                }
                if(hrany_closed.at<Vec3b>(Point(x6-1,y6+1)).val[0]==100 && hrany_closed.at<Vec3b>(Point(x6-1,y6+1)).val[1]==0 && hrany_closed.at<Vec3b>(Point(x6-1,y6+1)).val[2]==255)
                {
                    fronta.push_back(x6-1);
                    fronta.push_back(y6+1);
                }
            }
            if(hrany_closed.at<Vec3b>(Point(x6,y6)).val[0]==250 && hrany_closed.at<Vec3b>(Point(x6,y6)).val[1]==150 && hrany_closed.at<Vec3b>(Point(x6,y6)).val[2]==0)
            {
                hrany_closed.at<Vec3b>(Point(x6,y6)).val[0]=0;
                hrany_closed.at<Vec3b>(Point(x6,y6)).val[1]=0;
                hrany_closed.at<Vec3b>(Point(x6,y6)).val[2]=0;

                if(hrany_closed.at<Vec3b>(Point(x6+1,y6)).val[0]==250 && hrany_closed.at<Vec3b>(Point(x6+1,y6)).val[1]==150 && hrany_closed.at<Vec3b>(Point(x6+1,y6)).val[2]==0)
                {
                    fronta.push_back(x6+1);
                    fronta.push_back(y6);
                }
                if(hrany_closed.at<Vec3b>(Point(x6-1,y6)).val[0]==250 && hrany_closed.at<Vec3b>(Point(x6-1,y6)).val[1]==150 && hrany_closed.at<Vec3b>(Point(x6-1,y6)).val[2]==0)
                {
                    fronta.push_back(x6-1);
                    fronta.push_back(y6);
                }
                if(hrany_closed.at<Vec3b>(Point(x6,y6+1)).val[0]==250 && hrany_closed.at<Vec3b>(Point(x6,y6+1)).val[1]==150 && hrany_closed.at<Vec3b>(Point(x6,y6+1)).val[2]==0)
                {
                    fronta.push_back(x6);
                    fronta.push_back(y6+1);
                }
                if(hrany_closed.at<Vec3b>(Point(x6,y6-1)).val[0]==250 && hrany_closed.at<Vec3b>(Point(x6,y6-1)).val[1]==150 && hrany_closed.at<Vec3b>(Point(x6,y6-1)).val[2]==0)
                {
                    fronta.push_back(x6);
                    fronta.push_back(y6-1);
                }
                if(hrany_closed.at<Vec3b>(Point(x6-1,y6-1)).val[0]==250 && hrany_closed.at<Vec3b>(Point(x6-1,y6-1)).val[1]==150 && hrany_closed.at<Vec3b>(Point(x6-1,y6-1)).val[2]==0)
                {
                    fronta.push_back(x6-1);
                    fronta.push_back(y6-1);
                }
                if(hrany_closed.at<Vec3b>(Point(x6+1,y6-1)).val[0]==250 && hrany_closed.at<Vec3b>(Point(x6+1,y6-1)).val[1]==150 && hrany_closed.at<Vec3b>(Point(x6+1,y6-1)).val[2]==0)
                {
                    fronta.push_back(x6+1);
                    fronta.push_back(y6-1);
                }
                if(hrany_closed.at<Vec3b>(Point(x6+1,y6+1)).val[0]==250 && hrany_closed.at<Vec3b>(Point(x6+1,y6+1)).val[1]==150 && hrany_closed.at<Vec3b>(Point(x6+1,y6+1)).val[2]==0)
                {
                    fronta.push_back(x6+1);
                    fronta.push_back(y6+1);
                }
                if(hrany_closed.at<Vec3b>(Point(x6-1,y6+1)).val[0]==250 && hrany_closed.at<Vec3b>(Point(x6-1,y6+1)).val[1]==150 && hrany_closed.at<Vec3b>(Point(x6-1,y6+1)).val[2]==0)
                {
                    fronta.push_back(x6-1);
                    fronta.push_back(y6+1);
                }
            }
        }
}





void pridejhranu(Mat hrany_closed,vector<int> index4)
{
    int x6 = index4[index4.size()-2];
    int y6 = index4[index4.size()-1];
    int x7 = index4[index4.size()-4];
    int y7 = index4[index4.size()-3];

    Point p1(x6,y6);
    Point p2(x7,y7);

    line(hrany_closed,p1,p2,Scalar(100,0,255),1,1,0);
}




void uzavri(Mat hrany,vector<int> index2)
{
    int pocitadlo = 0;
    int x5 = index2[index2.size()-2];
    int y5 = index2[index2.size()-1];
    int pocetni_matice[hrany.rows][hrany.cols] = {0};
    pocetni_matice[y5][x5] = 1;

    int y_horni = y5;
    int y_dolni = y5;
    int x_horni = x5;
    int x_dolni = x5;
    vector<int> fronta;
    vector<int> konce;

    do
    {
        if(y_horni < hrany.rows)
            {
                ++y_horni;
            }
        if(y_dolni > 1)
            {
                --y_dolni;
            }
        if(x_horni < hrany.cols)
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
                if(pocetni_matice[yi][xi]==1)
                {
                    if(xi < hrany.cols-1)
                    {
                        if(hrany.at<Vec3b>(Point(xi+1,yi)).val[0]==0 && hrany.at<Vec3b>(Point(xi+1,yi)).val[1]==0 && hrany.at<Vec3b>(Point(xi+1,yi)).val[2]==0)
                        {
                            pocetni_matice[yi][xi+1]=1;
                        }
                    }
                    if(xi > 1)
                    {
                        if(hrany.at<Vec3b>(Point(xi-1,yi)).val[0]==0 && hrany.at<Vec3b>(Point(xi-1,yi)).val[1]==0 && hrany.at<Vec3b>(Point(xi-1,yi)).val[2]==0)
                        {
                            pocetni_matice[yi][xi-1]=1;
                        }
                    }
                    if(yi < hrany.rows-1)
                    {
                        if(hrany.at<Vec3b>(Point(xi,yi+1)).val[0]==0 && hrany.at<Vec3b>(Point(xi,yi+1)).val[1]==0 && hrany.at<Vec3b>(Point(xi,yi+1)).val[2]==0)
                        {
                            pocetni_matice[yi+1][xi]=1;
                        }
                    }
                    if(yi > 1)
                    {
                        if(hrany.at<Vec3b>(Point(xi,yi-1)).val[0]==0 && hrany.at<Vec3b>(Point(xi,yi-1)).val[1]==0 && hrany.at<Vec3b>(Point(xi,yi-1)).val[2]==0)
                        {
                            pocetni_matice[yi-1][xi]=1;
                        }
                    }
                }
            }
        }
    }while(x_dolni > 1 || y_dolni > 1 || x_horni < hrany.cols || y_horni < hrany.rows);

    for(int i = 2;i<hrany.cols-1;++i)
    {
        for(int j = 2;j<hrany.rows-1;++j)
        {
            if(pocetni_matice[j][i]==1 && (pocetni_matice[j+1][i]==0||pocetni_matice[j-1][i]==0||pocetni_matice[j][i+1]==0||pocetni_matice[j][i-1]==0||pocetni_matice[j+1][i+1]==0||pocetni_matice[j+1][i-1]==0||pocetni_matice[j-1][i-1]==0||pocetni_matice[j-1][i+1]==0))
            {
                if(hrany.at<Vec3b>(Point(i+1,j)).val[0]!=0 || hrany.at<Vec3b>(Point(i+1,j)).val[1]!=0 || hrany.at<Vec3b>(Point(i+1,j)).val[2]!=0)
                {
                    pocetni_matice[j][i+1]=2;
                }
                if(hrany.at<Vec3b>(Point(i-1,j)).val[0]!=0 || hrany.at<Vec3b>(Point(i-1,j)).val[1]!=0 || hrany.at<Vec3b>(Point(i-1,j)).val[2]!=0)
                {
                    pocetni_matice[j][i-1]=2;
                }
                if(hrany.at<Vec3b>(Point(i,j+1)).val[0]!=0 || hrany.at<Vec3b>(Point(i,j+1)).val[1]!=0 || hrany.at<Vec3b>(Point(i,j+1)).val[2]!=0)
                {
                    pocetni_matice[j+1][i]=2;
                }
                if(hrany.at<Vec3b>(Point(i,j-1)).val[0]!=0 || hrany.at<Vec3b>(Point(i,j-1)).val[1]!=0 || hrany.at<Vec3b>(Point(i,j-1)).val[2]!=0)
                {
                    pocetni_matice[j-1][i]=2;
                }
                if(hrany.at<Vec3b>(Point(i+1,j-1)).val[0]!=0 || hrany.at<Vec3b>(Point(i+1,j-1)).val[1]!=0 || hrany.at<Vec3b>(Point(i+1,j-1)).val[2]!=0)
                {
                    pocetni_matice[j-1][i+1]=2;
                }
                if(hrany.at<Vec3b>(Point(i-1,j-1)).val[0]!=0 || hrany.at<Vec3b>(Point(i-1,j-1)).val[1]!=0 || hrany.at<Vec3b>(Point(i-1,j-1)).val[2]!=0)
                {
                    pocetni_matice[j-1][i-1]=2;
                }
                if(hrany.at<Vec3b>(Point(i+1,j+1)).val[0]!=0 || hrany.at<Vec3b>(Point(i+1,j+1)).val[1]!=0 || hrany.at<Vec3b>(Point(i+1,j+1)).val[2]!=0)
                {
                    pocetni_matice[j+1][i+1]=2;
                }
                if(hrany.at<Vec3b>(Point(i-1,j+1)).val[0]!=0 || hrany.at<Vec3b>(Point(i-1,j+1)).val[1]!=0 || hrany.at<Vec3b>(Point(i-1,j+1)).val[2]!=0)
                {
                    pocetni_matice[j+1][i-1]=2;
                }
            }
        }
    }

    int pocitadl;






    for(int i = 2;i<hrany.cols-1;++i)
    {
        for(int j = 2;j<hrany.rows-1;++j)
        {
            if(pocetni_matice[j][i]==2)
            {
                fronta.push_back(i);
                fronta.push_back(j);
                ++pocitadl;
                if(pocetni_matice[j+1][i]==2)
                {
                    fronta.push_back(i);
                    fronta.push_back(j+1);
                    ++pocitadl;
                }
                if(pocetni_matice[j-1][i]==2)
                {
                    fronta.push_back(i);
                    fronta.push_back(j-1);
                    ++pocitadl;
                }
                if(pocetni_matice[j][i+1]==2)
                {
                    fronta.push_back(i+1);
                    fronta.push_back(j);
                    ++pocitadl;
                }
                if(pocetni_matice[j][i-1]==2)
                {
                    fronta.push_back(i-1);
                    fronta.push_back(j);
                    ++pocitadl;
                }
                if(pocetni_matice[j-1][i-1]==2)
                {
                    fronta.push_back(i-1);
                    fronta.push_back(j-1);
                    ++pocitadl;
                }
                if(pocetni_matice[j-1][i+1]==2)
                {
                    fronta.push_back(i+1);
                    fronta.push_back(j-1);
                    ++pocitadl;
                }
                if(pocetni_matice[j+1][i+1]==2)
                {
                    fronta.push_back(i+1);
                    fronta.push_back(j+1);
                    ++pocitadl;
                }
                if(pocetni_matice[j+1][i-1]==2)
                {
                    fronta.push_back(i-1);
                    fronta.push_back(j+1);
                    ++pocitadl;
                }
            }
        }
    }

    for(int i = 0;i < fronta.size()/2;++i)
    {
        int sousedi = 0;
        int y = fronta[2*i+1];
        int x = fronta[2*i];

        if(pocetni_matice[y+1][x]==2)
        {
            ++sousedi;
        }
        if(pocetni_matice[y-1][x]==2)
        {
            ++sousedi;
        }
        if(pocetni_matice[y][x+1]==2)
        {
            ++sousedi;
        }
        if(pocetni_matice[y][x-1]==2)
        {
            ++sousedi;
        }
        if(pocetni_matice[y+1][x+1]==2)
        {
            ++sousedi;
        }
        if(pocetni_matice[y+1][x-1]==2)
        {
            ++sousedi;
        }
        if(pocetni_matice[y-1][x-1]==2)
        {
            ++sousedi;
        }
        if(pocetni_matice[y-1][x+1]==2)
        {
            ++sousedi;
        }
        if(sousedi<3)
        {
            konce.push_back(x);
            konce.push_back(y);
        }
    }


    //najde nejblizsi konce a pospojuje je
    for(int i = 0;i < konce.size()/2;++i)
    {
        for(int j = 0;j < konce.size()/2;++j)
        {
            if((abs(konce[2*i]-konce[2*j])<4) && (abs(konce[2*i+1]-konce[2*j+1])<4))
            {
                Point p1(konce[2*i],konce[2*i+1]);
                Point p2(konce[2*j],konce[2*j+1]);
                line(hrany,p1,p2,Scalar(255,150,0),1,1,0);
            }
        }
    }
    //zjisti jestli se to uzavrelo a kdyz ne vrati na zacatek



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
    image_rect2 = image(Range(y3,y2), Range(x2,x3));

    //vykreslí okno s posuvnou lištou a černobílým obrázkem s detekovanými hranami (přepočet probíhá v sekci CannyThreshold)
    hrany.create(image_rect.size(),image_rect.type());
    namedWindow( "Nalezené hrany", WINDOW_NORMAL);
    resizeWindow("Nalezené hrany",1500,1000);
    createTrackbar( "Min Threshold:", "Nalezené hrany", &minKontrast, max_minKontrast, CannyDetekce);
    CannyDetekce(0, 0);
    waitKey(0);





    //smyčka pro označení oblasti pro výpočet plochy
    loop2:
        image_test = Mat();
        hrany.copyTo(image_test); //inicializace obrázků, aby neosahovaly dříve označené body + vytvoření kopie abychom si nezničili originál

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
        imshow("Vyznačte měřítko o velikosti 1: ",image);
        resizeWindow("Vyznačte měřítko o velikosti 1: ",1500,1000);
        setMouseCallback("Vyznačte měřítko o velikosti 1: ", onClickMeritko, 0);
        waitKey(0);

        //dotaz na uživatele zda je se svým výběrem spokojen
        cout << "Pokud souhlasíte se zvolenou plochou napište 1 jinak vypište 0: "; cin >> y_n2;
        if (y_n2 == 0)
        {
            goto loop2;
        }






    cvtColor(hrany,hrany,COLOR_GRAY2BGR);


    //návrh na uzavření plochy
    hrany.copyTo(hrany_closed);
    do
    {
        namedWindow("Odstranění nechtěných hran před automatickým doplněním:",WINDOW_NORMAL);
        imshow("Odstranění nechtěných hran před automatickým doplněním:",hrany_closed);
        resizeWindow("Odstranění nechtěných hran před automatickým doplněním:",1500,1000);
        setMouseCallback("Odstranění nechtěných hran před automatickým doplněním:", onClickHrany, 0);
        waitKey(0);

        odstranhranu(hrany_closed,index4);

        cout << "Pokud chcete vymazat další hranu napište 0 jinak vypište 1: "; cin >> y_n5;
    }while(y_n5==0);


        uzavri(hrany_closed,index2);

    //ukáže se uživateli
        namedWindow("Návrh uzavření plochy:",WINDOW_NORMAL);
        imshow("Návrh uzavření plochy:",hrany_closed);
        resizeWindow("Návrh uzavření plochy:",900,900);

        namedWindow("Návrh uzavření plochy",WINDOW_NORMAL);
        imshow("Návrh uzavření plochy",hrany);
        resizeWindow("Návrh uzavření plochy",900,900);
        waitKey(0);

        //zeptá se jestli se mu to takhle líbí
        cout << "Pokud souhlasíte s navrhnutým uzavřením plochy napište 1 jinak vypište 0: "; cin >> y_n4;
        if (y_n4 == 0)
        {
            loop4:
                do
                {//když ne, naklikat konce, které byly spojeny špatně...segmenty dodané námi barevně...klikne na ten co se mu nelíbí (nejbližší segment ke kliknutí)...ten se vymaže...klikne na koncové body které chce spojit (asi přímka nebo polynomiální spojení)
                    namedWindow("Odstranění nechtěných hran:",WINDOW_NORMAL);
                    imshow("Odstranění nechtěných hran:",hrany_closed);
                    resizeWindow("Odstranění nechtěných hran:",1500,1000);
                    setMouseCallback("Odstranění nechtěných hran:", onClickHrany, 0);
                    waitKey(0);

                    odstranhranu(hrany_closed,index4);

                    cout << "Pokud chcete vymazat další hranu napište 0 jinak vypište 1: "; cin >> y_n5;
                }while(y_n5==0);

                do
                {//když ne, naklikat konce, které byly spojeny špatně...segmenty dodané námi barevně...klikne na ten co se mu nelíbí (nejbližší segment ke kliknutí)...ten se vymaže...klikne na koncové body které chce spojit (asi přímka nebo polynomiální spojení)
                    namedWindow("Přidání hran:",WINDOW_NORMAL);
                    imshow("Přidání hran:",hrany_closed);
                    resizeWindow("Přidání hran:",1500,1000);
                    setMouseCallback("Přidání hran:", onClickHranypridej, 0);
                    waitKey(0);

                    pridejhranu(hrany_closed,index5);

                    cout << "Pokud chcete vymazat další hranu napište 0 jinak vypište 1: "; cin >> y_n5;
                }while(y_n5==0);


            namedWindow("Návrh uzavření plochy:",WINDOW_NORMAL);
            imshow("Návrh uzavření plochy:",hrany_closed);
            resizeWindow("Návrh uzavření plochy:",1500,1000);
            waitKey(0);

            cout << "Pokud souhlasíte s aktuálním návrhem uzavření plochy napište 1 jinak vypište 0: "; cin >> y_n3;
            if (y_n3 == 0)
            {
                goto loop4;
            }
        }


    //asi by šlo i odstranit nadbytečné hrany
    cout << "Plocha je: " << plocha(index2,hrany_closed,index3);
    imwrite("hrany.png",hrany_closed);

    namedWindow("Vyznačená vypočtená plocha:",WINDOW_NORMAL);
    imshow("Vyznačená vypočtená plocha:",hrany_closed);
    resizeWindow("Vyznačená vypočtená plocha:",1500,1000);
    waitKey(0);


    return 0;
}
