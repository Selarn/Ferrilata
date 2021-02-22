#include "src/Controllers/client.h"
#include "src/Controllers/server.h"

#include <QApplication>

int main( int argc, char *argv[] ) {
    QApplication a( argc, argv );
    Server server(
        2314        // port
        );
    Client c1(
        "127.0.0.1", // server ip
        2314         // port
        );
    Client c2(
        "127.0.0.1", // server ip
        2314         // port
        );
    return a.exec();
}
