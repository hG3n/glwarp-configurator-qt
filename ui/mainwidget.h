#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QKeyEvent>
#include <QWindow>

#include "glwidget.h"
#include "propertyeditgroup.h"

#include "mainwidget.h"
#include "mainwindow.h"

#include "model/simulation.h"
#include "model/dome_projector.hpp"

#include "config.h"
#include "ui/propertyedit.hpp"
#include "ui/glwidget.h"
#include "ui/glwarpwidget.h"
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

    public slots:
        void keyPressEvent(QKeyEvent *event);

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

        /**
         * @brief initUiValues
         */
        void setUiValues(QJsonObject model_config);

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
        GLWarpWidget *_glWarpWidget;
        Configurator *_configurator;

        MainWindow *_mainWindow;
        Simulation _simulation;
        Config _config;

        ModelConfig *_model_config;
        QJsonObject _ui_config;
};

#endif
