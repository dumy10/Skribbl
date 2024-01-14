#include <QtWidgets/QApplication>
#include "ServerConnectForm.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    ServerConnectForm w;
    w.show();
    return a.exec();
}