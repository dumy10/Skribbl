#include "LoginForm.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginForm w;
    w.show();
    return a.exec();
}
//#include "Lobby.h"
//#include <QtWidgets/QApplication>
//
//int main(int argc, char* argv[])
//{
//    QApplication a(argc, argv);
//
//    Lobby w;
//    w.show();
//    return a.exec(); 
//}

