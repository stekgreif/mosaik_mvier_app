#include "MosaikMiniApp.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon( QIcon(":/icon.png") );

    QFile file(":/stylesheet.qss");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        a.setStyleSheet(file.readAll());
        file.close();
    }

    MosaikMiniApp w;
    w.show();

    return a.exec();
}



