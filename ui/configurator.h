#ifndef GROUPCONFIGURATORWIDGET_HPP
#define GROUPCONFIGURATORWIDGET_HPP

#include <QWidget>
#include <QGroupBox>
#include <QJsonArray>
#include <QJsonObject>

#include "ui/propertyeditgroup.h"

class Configurator : public QWidget
{
        Q_OBJECT
    public:
        explicit Configurator(const QJsonObject &config, QWidget *parent = 0);

    signals:

    public slots:

    private:
        /**
         * @brief initLayout
         * @param config
         */
        void initLayout(const QJsonObject &config);

        void initLayoutGroup(const QJsonObject &config, QBoxLayout*layout);
    private:
        QMap<QString, PropertyEditGroup> _ui_elements;

};

#endif // GROUPCONFIGURATORWIDGET_HPP
