#include "LoginForm.h"
#include <QtWidgets/QApplication>
#include "Game.h" 

/*
Use to login:
username: abcabc
username: abc
password: Abcabc1
(make sure server is running)
*/

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    LoginForm w;
    w.show();
    return a.exec();
}

//int main(int argc, char* argv[])
//{
//    QApplication a(argc, argv);
//    Game w;
//    w.show();
//    return a.exec();
//}


