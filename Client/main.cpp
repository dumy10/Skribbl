#include "LoginForm.h"
#include <QtWidgets/QApplication>

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
    LoginForm w;
    w.show();
    return a.exec();
}

