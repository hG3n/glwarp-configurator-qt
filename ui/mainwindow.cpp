#include <QMenuBar>
#include <QMenu>
#include <QMessageBox>
#include <QDebug>
#include <QWidget>

#include "ui/mainwindow.h"
#include "ui/mainwidget.h"

MainWindow::MainWindow()
{
//    QMenuBar *menuBar = new QMenuBar;
//    QMenu *menuWindow = menuBar->addMenu(tr("&Window"));

//    setMenuBar(menuBar);

    _window = new MainWidget(this);
    setCentralWidget(_window);
}
