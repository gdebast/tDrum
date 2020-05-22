#include "DrumWidget/drumui_drummainwindow.h"

#include <QApplication>


int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    DrumUI::DrumMainWindow mainWindow;
    mainWindow.show();
    int end =  a.exec();
    mainWindow.end();
    return end;

}
