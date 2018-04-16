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
#include "ui/propertyedit.hpp"
#include "ui/glwidget.h"
#include "ui/configurator.h"

class MainWidget : public QWidget
{
        Q_OBJECT

    public:
        /**
         * @brief MainWidget
         * @param mw
         */
        MainWidget(MainWindow *mw);

    private slots:
        /**
         * @brief onValueUpdate
         * @param new_values
         */
        void onValueUpdate(QJsonObject new_values);

    private:
        /**
         * @brief initLayout
         */
        void initLayout();

        /**
         * @brief loadConfigs
         * @return
         */
        void loadConfigs();

//        /**
//         * @brief initUiValues
//         */
//        void setUiValues(ModelConfig *model_config);

        /**
         * @brief initSimulation
         */
        void initSimulation();

        /**
         * @brief vec3fromJson
         * @return
         */
        QVector3D vec3fromJson(const QJsonObject &object) const;

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
