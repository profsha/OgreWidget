#include <QtGui/QApplication>
#include "ogrewidget.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QWidget window;

    window.resize(800, 600);
    window.setWindowTitle("Simple example");
    OgreWidget* ogreWidget = new OgreWidget();

    QPushButton* push=new QPushButton("test");
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(ogreWidget);
    layout->addWidget(push);

    window.setLayout(layout);
    window.show();
    QTimer timer;
    QObject::connect(&timer,SIGNAL(timeout()),ogreWidget,SLOT(repaint()));
    timer.start(10);
    
    return a.exec();
}
