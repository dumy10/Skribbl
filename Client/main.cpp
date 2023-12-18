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
//#include <QtWidgets/QApplication>
//#include "Game.h" // Include the header for your game window class
//
//int main(int argc, char* argv[])
//{
//    QApplication a(argc, argv);
//    std::string username = "exampleUsername"; // Replace with actual username
//    int playerIndex = 1; // Replace with the actual player index
//    bool isOwner = false; // Set based on whether the player is the owner
//
//    Game w(username, playerIndex, isOwner); // Create an instance of your game window class with parameters
//    w.show(); // Show the game window
//    return a.exec(); // Start the event loop
//}
