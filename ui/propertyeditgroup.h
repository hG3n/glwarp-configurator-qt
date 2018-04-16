#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP
#include <utility>

#include <QWidget>
#include <QObject>

#include <QVBoxLayout>
#include <QGroupBox>
#include <QJsonObject>
#include <QVector>
#include <QDebug>

#include "ui/propertyedit.hpp"

class PropertyEditGroup : public QWidget
{
        Q_OBJECT
    public:
        explicit PropertyEditGroup(const QJsonObject &config, QWidget *parent = 0);

        /**
         * @brief Returns json containing values of all child elements
         * @return
         */
        QJsonObject toJson() const;

        /**
         * @brief Set child values from Json
         * @param values
         */
        void fromJson(const QJsonObject &values);

        /**
         * @brief Returns groups attribute
         * @return
         */
        QString getAttribute() const;

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
        QString _attribute;
        QMap<QString, PropertyEdit*> _ui_elements;

};

#endif // CONFIGURATION_HPP
