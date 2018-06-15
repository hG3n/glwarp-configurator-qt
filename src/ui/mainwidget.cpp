#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <fstream>

#include "mainwidget.h"
#include "mainwindow.h"
#include "config.h"
#include "propertyeditgroup.h"
#include "model/simulation.h"

#include <QMenu>
#include <QFile>
#include <QFileDialog>
#include <QDir>

MainWidget::MainWidget(MainWindow *mw)
    : main_window_(mw)
    , gl_widget_(0)
    , gl_warp_widget_(0)
    , configurator_(0)
    , simulation_()
    , config_ ()
    , model_config_(0)
    , ui_config_()

{
    loadModelConfig(true);
    loadUiConfig(true);
    initWidgets();
    initLayout();
    initActions();
    initMenu();
    initSimulation();
    setUiValues(config_.getModelConfigJson());

    // todo: reimplement this
    // get scene pointer
    //    _simulation.updateScene(_glWidget->getScene());
}


QMenu * MainWidget::getMenu() {
    return main_menu_;
}


void MainWidget::onValueUpdate(QJsonObject new_values) {
    // get configs
    QJsonObject projector_obj = new_values["projector"].toObject();
    DomeProjectorConfig projector_config = DomeProjectorConfig::fromJson(projector_obj);

    QJsonObject mirror_obj = new_values["mirror"].toObject();
    SphereConfig mirror_config = SphereConfig::fromJson(mirror_obj);

    QJsonObject dome_obj = new_values["dome"].toObject();
    SphereConfig dome_config = SphereConfig::fromJson(dome_obj);

    // create model config
    ModelConfig *config = new ModelConfig;
    config->dome_projector = projector_config;
    config->dome = dome_config;
    config->mirror = mirror_config;

    // get new simulation values and update gl
    simulation_.updateFromConfig(config);
    delete config;
    simulation_.runCalculations();
    Scene s = simulation_.getCurrentScene();
    gl_widget_->setScene(s);

    // update the warp view as well
    std::vector<QVector3D> mesh_coords = simulation_.getTransformationMesh();
    std::vector<QVector3D> tex_coords = simulation_.getTextureCoords();
    gl_warp_widget_->updateValues(mesh_coords, tex_coords);
}

void MainWidget::onOpenTransformationView() {
    std::vector<QVector3D> mesh_coords = simulation_.getTransformationMesh();
    std::vector<QVector3D> tex_coords = simulation_.getTextureCoords();

    gl_warp_widget_->updateValues(mesh_coords, tex_coords);
    gl_warp_widget_->show();
}


void MainWidget::onLoadCustomConfig() {
    loadModelConfig(false);
    setUiValues(config_.getModelConfigJson());
    initSimulation();
}

void MainWidget::onSaveModelConfig() {
    QJsonObject current_values = configurator_->getValues();
    QJsonDocument document(current_values);

    QDir app_path = Config::getApplicationPath();

    QString filepath = QFileDialog::getSaveFileName( nullptr, tr("Save current transformation"), app_path.absoluteFilePath("model.json"));

    qDebug().nospace() << Q_FUNC_INFO << " :" << __LINE__;
    qDebug() << "  >" << filepath;

    if(filepath != "") {
        QFile json_file(filepath);
        if(json_file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text)){
            json_file.write(document.toJson());
        } else {
            qDebug() << "Error: Saving file" << filepath << "was not possible!";
        }
    } else {
        qDebug() << "Error: No file specified";
    }
}

void MainWidget::loadModelConfig(bool default_config) {
    if(default_config) {
        if(config_.loadModelConfig(":configs/model.json"))
            model_config_ = config_.getModelConfig();
    } else {
        QDir app_root_dir = Config::getApplicationPath();
        QString config_file = QFileDialog::getOpenFileName(nullptr, tr("Load Model Config"), app_root_dir.absoluteFilePath("model.json"));

        if(config_.loadModelConfig(config_file))
            model_config_ = config_.getModelConfig();
        else
            qDebug() << "Error: loading non default Model config";
    }
}

void MainWidget::loadUiConfig(bool default_config) {
    if(default_config) {
        if(config_.loadUiConfig(":configs/ui.json"))
            ui_config_ = config_.getUiConfig();
    } else {
        QDir app_root_dir = Config::getApplicationPath();
        QString config_file = QFileDialog::getOpenFileName(nullptr, tr("Load UI Config"), app_root_dir.absoluteFilePath("ui.json"));

        if(config_.loadModelConfig(config_file))
            model_config_ = config_.getModelConfig();
        else
            qDebug() << "Error: loading non default Ui config";
    }
}

void MainWidget:: loadConfigs() {
    config_ = Config();

    if(config_.loadModelConfig(":/configs/model.json")) {
        model_config_ = config_.getModelConfig();
    } else {
        qDebug() << "Error laoding model config!";
    }

    if(config_.loadUiConfig(":/configs/ui.json")) {
        ui_config_ = config_.getUiConfig();
    } else {
        qDebug() << "Error loading UI config!";
    }
}

void MainWidget::setUiValues(QJsonObject model_config) {
    configurator_->setValues(model_config);
}

void MainWidget::initSimulation() {
    simulation_ = Simulation();
    simulation_.initialize(config_.getModelConfig());

    simulation_.runCalculations();
    Scene s = simulation_.getCurrentScene();

    gl_widget_->setScene(s);
}

QVector3D MainWidget::vec3fromJson(const QJsonObject &object) const{
    return QVector3D(object["x"].toDouble(), object["y"].toDouble(), object["z"].toDouble());
}

void MainWidget::saveTransformationValues() const {

    auto mesh = simulation_.getTransformationMesh();
    auto tex = simulation_.getTextureCoords();

    QString filename =  QFileDialog::getSaveFileName(nullptr, tr("Save Transformation files"), tr("foo"));
    if(filename =="") {
        qDebug().nospace() << Q_FUNC_INFO << " :" << __LINE__;
        qDebug() << "  >" << "no filename";
    } else {
        qDebug().nospace() << Q_FUNC_INFO << " :" << __LINE__;
        qDebug() << "  >" << filename;

        QString tex_filename = filename + ".tex";
        QFile tex_file(tex_filename);
        QTextStream tex_stream(&tex_file);

        if(tex_file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text))
            for(auto i : tex)
                tex_stream << i.x() << " " << i.y() << " " << i.z() << "\n";

        tex_file.close();

        QString mesh_filename = filename + ".mesh";
        QFile mesh_file(mesh_filename);
        QTextStream mesh_stream(&mesh_file);
        if(mesh_file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text))
            for(auto i : mesh)
                mesh_stream << i.x() << " " << i.y() << " " << i.z() << "\n";

        mesh_file.close();
        qDebug() << "mesh saved!";
    }


}

void MainWidget::initWidgets() {
    gl_widget_ = new GLWidget(this);
    configurator_ = new Configurator(ui_config_, this);
    gl_warp_widget_ = new GLWarpWidget();

    connect(configurator_, &Configurator::valueUpdate,
            this, &MainWidget::onValueUpdate);

}

void MainWidget::initLayout() {
    setContentsMargins(0,0,0,0);

    QHBoxLayout *root = new QHBoxLayout;
    root->setContentsMargins(0,0,0,0);
    root->setAlignment(Qt::AlignRight);
    root->setSpacing(0);
    root->addWidget(gl_widget_);

    QScrollArea *scrollarea = new QScrollArea;
    scrollarea->setFixedWidth(250);
    scrollarea->setAlignment(Qt::AlignCenter);
    scrollarea->setContentsMargins(0,0,0,0);
    scrollarea->setWidget(configurator_);

    root->addWidget(scrollarea);
    setLayout(root);
}

void MainWidget::initActions() {

    actions_["Load Config"] = new QAction(tr("Load Config"), this);
    actions_["Load Config"]->setToolTip(tr("Loads an existing config file."));
    actions_["Load Config"]->setShortcut(QKeySequence("Ctrl+O"));

    actions_["Save Config"] = new QAction(tr("Save Config"), this);
    actions_["Save Config"]->setToolTip(tr("Saves the current config."));
    actions_["Save Config"]->setShortcut(QKeySequence("Ctrl+S"));

    actions_["Save Transformations"] = new QAction(tr("Save Transformations"), this);
    actions_["Save Transformations"]->setToolTip(tr("Saves the calculated transformations."));
    actions_["Save Transformations"]->setShortcut(QKeySequence("Ctrl+Shift+S"));

    actions_["Run Simulation"] = new QAction(tr("Run Simulation"), this);
    actions_["Run Simulation"]->setToolTip(tr("Simulates according to the made settings."));
    actions_["Run Simulation"]->setShortcut(QKeySequence("Ctrl+R"));

    actions_["Open Transformation View"] = new QAction(tr("Open Transformation View"), this);
    actions_["Open Transformation View"]->setToolTip(tr("Opens a separate window showing the current warping results."));
    actions_["Open Transformation View"]->setShortcut(QKeySequence("Ctrl+T"));

    // connections
    connect(actions_["Load Config"], &QAction::triggered,
            this, &MainWidget::onLoadCustomConfig);

    connect(actions_["Save Config"], &QAction::triggered,
            this, &MainWidget::onSaveModelConfig);

    connect(actions_["Save Transformations"], &QAction::triggered,
            this, &MainWidget::saveTransformationValues);

    connect(actions_["Run Simulation"], &QAction::triggered,
            configurator_, &Configurator::onConfirm);

    connect(actions_["Open Transformation View"], &QAction::triggered,
            this, &MainWidget::onOpenTransformationView);

}

void MainWidget::initMenu() {
    main_menu_ = new QMenu("GLWarp Configurator");

    QMenu* file_menu = main_menu_->addMenu(tr("File"));
    file_menu->addAction(actions_["Load Config"]);
    file_menu->addAction(actions_["Save Config"]);
    file_menu->addAction(actions_["Save Transformations"]);

    QMenu* edit_menu = main_menu_->addMenu(tr("Edit"));
    edit_menu->addAction(actions_["Run Simulation"]);

    main_menu_->addMenu(file_menu);
    main_menu_->addMenu(edit_menu);
}
