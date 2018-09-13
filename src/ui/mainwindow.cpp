#include <QMenuBar>
#include <QMenu>
#include <QMessageBox>
#include <QDebug>
#include <QWidget>
#include <QMenuBar>

#include "ui/mainwindow.h"
#include "ui/mainwidget.h"

MainWindow::MainWindow(QWidget *parent)
    :QMainWindow(parent)
    , main_widget_(Q_NULLPTR)
    , actions_()
{
    setWindowTitle("GLWarp Configurator");

    initCentralWidget();
    initActions();
    initMenuBar();
}

MainWindow::~MainWindow()
{}

void MainWindow::toggleFullScreen(bool enable) {
    if(enable)
        showFullScreen();
    else
        showMaximized();
}

void MainWindow::initCentralWidget() {
    main_widget_ = new MainWidget(this);
    setCentralWidget(main_widget_);
}

void MainWindow::initActions()
{
    QList<QAction*> window_actions;

    window_actions.append(new QAction(tr("Full Screen"), this));
    window_actions.back()->setToolTip(tr("Show application in Full Screen mode"));
    window_actions.back()->setCheckable(true);
    window_actions.back()->setShortcut(QKeySequence(tr("Ctrl+Shift+f")));

    connect(window_actions.back(), &QAction::toggled,
            this, &MainWindow::toggleFullScreen);

    actions_[tr("Window")] = window_actions;
}

void MainWindow::initMenuBar()
{
    menuBar()->addActions(main_widget_->getMenu()->actions());

    foreach (const QString &menu_name, actions_.keys()) {
        QMenu* new_menu = menuBar()->addMenu(menu_name);
        foreach (QAction *act, actions_[menu_name]) {
            new_menu->addAction(act);
        }
    }

}

