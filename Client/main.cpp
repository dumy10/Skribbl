#include <QtWidgets/QApplication>
#include "ServerConnectForm.h"

/*
Use to connect to server: 127.0.0.1
Use to login:
username: abcabc
username: abc
password: Abcabc1
(make sure server is running)
*/

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    ServerConnectForm w;
    w.show();
    return a.exec();
}
