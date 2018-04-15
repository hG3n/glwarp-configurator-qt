#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollArea>


QT_BEGIN_NAMESPACE
class QSlider;
class QPushButton;
class QLineEdit;
class QDoubleSpinBox;
QT_END_NAMESPACE

class GLWidget;
class MainWindow;

#include "glwidget.h"
#include "propertyeditgroup.h"

#include "mainwidget.h"
#include "mainwindow.h"

#include "model/simulation.h"
#include "model/dome_projector.hpp"

#include "config.h"
#include "ui/vector3edit.hpp"
#include "ui/propertyedit.hpp"
#include "ui/glwidget.h"
#include "ui/configurator.h"

class MainWidget : public QWidget
{
        Q_OBJECT

    public:
        MainWidget(MainWindow *mw);


    private slots:
//        void onRecalculateButtonPressed();

    private:
        /**
         * @brief initLayout
         */
        void initLayout();

        void loadConfigs();

//        /**
//         * @brief initUiValues
//         */
//        void setUiValues(ModelConfig *model_config);

        /**
         * @brief initSimulation
         */
        void initSimulation();

//        /**
//         * @brief createButton
//         * @param title
//         * @return
//         */
//        QPushButton* createButton(QString const & name,QString const & title);

//        /**
//         * @brief createSpinBox
//         * @return
//         */
//        QDoubleSpinBox* createSpinBox(double step_size);

    private:
        GLWidget *_glWidget;
        Configurator *_configurator;

        MainWindow *_mainWindow;
        Simulation _simulation;
        Config _config;

        ModelConfig *_model_config;
        QJsonObject _ui_config;
};

#endif
