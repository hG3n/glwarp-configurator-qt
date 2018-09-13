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

/**
 * @brief The PropertyEditGroup class wraps n PropertyEdits according to the specifies configuration.
 *
 * Details on the configuraion structure can be found within the Configurator class
 */
class PropertyEditGroup : public QWidget
{
        Q_OBJECT
    public:
        explicit PropertyEditGroup(const QJsonObject &config, QWidget *parent = Q_NULLPTR);

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
        * @brief Initializes the Widgets layout.
        * @param config
        */
        void initLayout(const QJsonObject &config);

        /**
        * @brief Initializes a single edit group.
        *
        * Parses config attributes and creates a PropertyEdit Widget matching the given json description.
        * Adds the new element to the specified layout.
        *
        * @param config
        * @param layout
        */
        void initEditGroup(const QJsonObject &config, QBoxLayout *layout);

    private:
        QString _attribute;
        QMap<QString, PropertyEdit*> _ui_elements;

};

#endif // CONFIGURATION_HPP
