/*#include <QtWidgets/QApplication>
#include "ServerConnectForm.h"

/*
Use to connect to server: 127.0.0.1
Use to login:
username: abcabc
username: abc
password: Abcabc1
(make sure server is running)


int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    ServerConnectForm w;
    w.show();
    return a.exec();
}*/
#include <QtWidgets/QApplication>
#include "Game.h" 

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    std::string username = "exampleUsername";
    int playerIndex = 1; 
    bool isOwner = false;

    Game w(username, playerIndex, isOwner);
    w.show();
    return a.exec();
}
