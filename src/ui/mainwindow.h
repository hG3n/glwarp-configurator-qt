#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QAction>

#include "mainwidget.h"

class MainWidget;

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        MainWindow(QWidget *parent = 0);
        ~MainWindow();

    private slots:
        void toggleFullScreen(bool);

    private:
        void initCentralWidget();
        void initActions();
        void initMenuBar();

        MainWidget* main_widget_;
        QMap<QString, QList<QAction*> > actions_;
};

#endif
