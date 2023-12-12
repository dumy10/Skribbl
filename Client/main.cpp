#include "LoginForm.h"
#include <QtWidgets/QApplication>
#include "Game.h" 
#include "ServerConnectForm.h"
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
    ServerConnectForm w;
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


