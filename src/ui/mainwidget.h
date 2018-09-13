#ifndef MAIN_WIDGET_H
#define MAIN_WIDGET_H

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
         * @brief Gets called once new values are available from the manipulation UI
         * @param new_values
         */
        void onValueUpdate(QJsonObject new_values);

    public slots:
        /**
         * @brief Opens the transformation view in a separate window to view a simulation
         * of the current set parameters
         *
         */
        void onOpenTransformationView();

        /**
         * @brief This funciton either loads the default simluation and UI values or opens
         * a file dialog from which a custom configuration file can be specified
         */
        void onLoadCustomConfig();

        /**
         * @brief onSaveModelConfig
         */
        void onSaveModelConfig();

    private:

        QVector3D vec3fromJson(const QJsonObject &object) const;

        /**
         * @brief Loads the UI and model config from file
         */
        void loadConfigs();

        /**
         * @brief Loads Model config
         * @param default_config specifies whether the loaded config is
         * the default or a custom one
         */
        void loadModelConfig(bool default_config);

        /**
         * @brief Loads User interface configuration
         * @param default_config
         */
        void loadUiConfig(bool default_config);

        /**
         * @brief Notifies the configuration UI to set its values
         * @param model_config
         */
        void setUiValues(QJsonObject model_config);

        /**
         * @brief Initializes Simulation
         */
        void initSimulation();

        /**
         * @brief Opens a file dialog to specify a file to save the current configuration to
         */
        void saveTransformationValues() const;

        // qt inits
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

#endif // MAIN_WIDGET_H

