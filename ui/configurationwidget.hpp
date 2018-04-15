#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <QWidget>

#include <QObject>
#include <QSlider>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QPushButton>
#include <QLineEdit>
#include <QDesktopWidget>
#include <QApplication>
#include <QMessageBox>
#include <QDoubleSpinBox>
#include <QGroupBox>
#include <QLabel>
#include <QScrollArea>
#include <QJsonObject>
#include <QJsonArray>
#include <QVector>

#include "projectorconfigwidget.hpp"
#include "ui/vector3edit.hpp"

class ConfigurationWidget : public QWidget
{
        Q_OBJECT
    public:
        explicit ConfigurationWidget(QWidget *parent = 0);

    signals:

    public slots:
//        void onRecalculateButtonPressed();

    private:

       void initLayout();
         /**
         * @brief initProjectorUi
         * @param root
         */
        void initProjectorUi(QVBoxLayout *root);

        /**
         * @brief initMirrorUi
         * @param root
         */
        void initMirrorUi(QVBoxLayout *root);

        /**
         * @brief initDomeUi
         * @param root
         */
        void initDomeUi(QVBoxLayout *root);

        /**
         * @brief createButton
         * @param title
         * @return
         */
        QPushButton* createButton(QString const & name,QString const & title);

        /**
         * @brief createSpinBox
         * @return
         */
        QDoubleSpinBox* createSpinBox(double step_size);

    private:
        ProjectorConfigWidget *_projector_config_widget;

        Vector3Edit *_mirror_pos_edit;
        QDoubleSpinBox *_mirror_radius_edit;

        Vector3Edit *_dome_pos_edit;
        QDoubleSpinBox *_dome_radius_edit;

        QPushButton *_recalculate_btn;
};

#endif // CONFIGURATION_HPP
