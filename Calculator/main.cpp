#include <QApplication>
#include "msdwidget.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    msdwidget window;
    window.setWindowTitle("Expression Calculator");
    window.show();

    return app.exec();
}
