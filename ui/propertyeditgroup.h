#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP
#include <utility>

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
#include <QDebug>


#include "ui/propertyedit.hpp"

class PropertyEditGroup : public QWidget
{
        Q_OBJECT
    public:
        explicit PropertyEditGroup(const QJsonObject &config, QWidget *parent = 0);

        QMap<QString, QMap<QString, double>> getValues() const;
        QJsonObject getValuesJson() const;

        void setValues();
    signals:

    public slots:

    private:

        /**
        * @brief initLayout
        * @param config
        */
        void initLayout(const QJsonObject &config);

        /**
        * @brief initEditGroup
        * @param config
        * @param layout
        */
        void initEditGroup(const QJsonObject &config, QBoxLayout *layout);

    private:
        QMap<QString, PropertyEdit*> _ui_elements;

};

#endif // CONFIGURATION_HPP
