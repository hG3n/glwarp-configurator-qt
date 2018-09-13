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
        /**
         * @brief Toggle between native fullscreen and window mode
         */
        void toggleFullScreen(bool);

    private:
        /**
         * @brief Initializes the windows central widget
         */
        void initCentralWidget();

        /**
         * @brief Initialize actions available in the applications menu bar
         */
        void initActions();

        /**
         * @brief Initialize and create the menu bar
         */
        void initMenuBar();

    private:
        MainWidget* main_widget_;
        QMap<QString, QList<QAction*>> actions_;
};

#endif
