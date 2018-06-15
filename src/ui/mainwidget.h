#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QKeyEvent>
#include <QWindow>
#include <QMenu>

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

class MainWindow;

class MainWidget : public QWidget
{
        Q_OBJECT

    public:
        /**
         * @brief MainWidget
         * @param mw
         */
        MainWidget(MainWindow *mw);

        QMenu *getMenu();
    private slots:
        /**
         * @brief onValueUpdate
         * @param new_values
         */
        void onValueUpdate(QJsonObject new_values);

    public slots:
        void onOpenTransformationView();
        void onLoadCustomConfig();
        void onSaveModelConfig();

    private:

        QVector3D vec3fromJson(const QJsonObject &object) const;

        void loadConfigs();
        void loadModelConfig(bool default_config);
        void loadUiConfig(bool default_config);
        void setUiValues(QJsonObject model_config);
        void initSimulation();
        void saveTransformationValues() const;

        // inits
        void initWidgets();
        void initLayout();
        void initActions();
        void initMenu();

    private:
        MainWindow *main_window_;

        // widgets
        GLWidget *gl_widget_;
        GLWarpWidget *gl_warp_widget_;
        Configurator *configurator_;

        Simulation simulation_;
        Config config_;
        ModelConfig *model_config_;
        QJsonObject ui_config_;

        QMenu *main_menu_;
        QMap<QString, QAction*> actions_;

};

#endif
