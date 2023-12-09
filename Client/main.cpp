#include "LoginForm.h"
#include <QtWidgets/QApplication>
#include "Menu.h"
/*
Use to login:
username: abcabc
username: abc
password: Abcabc1
(make sure server is running)
*/

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Menu w;
    w.show();
    return a.exec();
}

