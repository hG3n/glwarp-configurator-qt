#ifndef GROUPCONFIGURATORWIDGET_HPP
#define GROUPCONFIGURATORWIDGET_HPP

#include <QWidget>
#include <QGroupBox>
#include <QJsonArray>
#include <QJsonObject>

#include <QPushButton>

#include "ui/propertyeditgroup.h"

class Configurator : public QWidget
{
        Q_OBJECT
    public:
        explicit Configurator(const QJsonObject &config, QWidget *parent = 0);

        QJsonObject getValues() const;
        void setValues(const QJsonObject& values);

    signals:
        void valueUpdate(QJsonObject new_values);

    public slots:
        void onConfirm();

    private:
        /**
         * @brief initLayout
         * @param config
         */
        void initLayout(const QJsonObject &config);

        /**
         * @brief initLayoutGroup
         * @param config
         * @param layout
         */
        void initLayoutGroup(const QJsonObject &config, QBoxLayout*layout);

    private:
        QString _attribute;

        QMap<QString, PropertyEditGroup*> _ui_elements;

        QPushButton *_confirm_button;
};

#endif // GROUPCONFIGURATORWIDGET_HPP
