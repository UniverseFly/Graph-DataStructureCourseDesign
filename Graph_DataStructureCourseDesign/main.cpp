#include <QApplication>
#include "ApplicationWindow.h"

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    QApplication::setApplicationName("图的遍历");
    QApplication::setWindowIcon(QIcon(":/Assets/PrettyVertex.png"));

    ApplicationWindow window;
    window.show();

    return QApplication::exec();
}
