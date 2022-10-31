#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

Mat img, img_gray, img_blur;

static void onMouse(int event, int x, int y, int flags, void* param) //bude uživateli ukazovat jaké souřadnice má bod na kterém je myš
    {
        if () //chci tam dát podmínku že souřadnice vypíše jen když tou myší klikne (jinak by to vypisovalo pořát)
            {
                cout << x <<" " << y << endl;
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
    setMouseCallback("Označte oblast hledané plochy:", onMouse, 0);
    waitKey(0);

    //nechá uživatele zapsat souřadnice krajních bodů obdélníku ve kterém je hledaná oblast (možné vylepšení - zapsání těchto souřadnic rovnou pomocí klikání na body ve snímku)
    int x1;
    int x2;
    int x3;
    int x4;
    int y1;
    int y2;
    int y3;
    int y4;
    cout << "Zadejte souradnici x prvního rohoveho bodu cilove oblasti: ", cin >> x1;
    cout << "Zadejte souradnici y prvního rohoveho bodu cilove oblasti: ", cin >> y1;
    cout << "Zadejte souradnici x druheho rohoveho bodu cilove oblasti: ", cin >> x2;
    cout << "Zadejte souradnici y druheho rohoveho bodu cilove oblasti: ", cin >> y2;
    cout << "Zadejte souradnici x tretiho rohoveho bodu cilove oblasti: ", cin >> x3;
    cout << "Zadejte souradnici y tretiho rohoveho bodu cilove oblasti: ", cin >> y3;
    cout << "Zadejte souradnici x ctvrteho rohoveho bodu cilove oblasti: ", cin >> x4;
    cout << "Zadejte souradnici y ctvrteho rohoveho bodu cilove oblasti: ", cin >> y4;
    waitKey(0);

    //vytvoření obdélníku s hledanou oblastí
    //dále aplikace algoritmu na hledání hran v danném rozsahu
    //kontrola uzavřenosti oblasti (případně návrat do předchozího kroku s nižším kontrasten...bylo by možné využít toho co měli v projektu naši předchůdci a nechat uživatele mrknout kdy už to najde celou oblast a pak ho nechat zadat požadovaný kontrast případně umožnit změnu hledaného kontrastu jen v urřité podoblasti)
    //zobrazení nalezené oblasti
    //výpočet plochy této oblasti
    return 0;
}
