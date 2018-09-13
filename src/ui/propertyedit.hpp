#ifndef PROPERTYEDIT_HPP
#define PROPERTYEDIT_HPP

#include <QWidget>
#include <QJsonObject>
#include <QJsonArray>
#include <QFormLayout>
#include <QDebug>
#include <QDoubleSpinBox>

/**
 * @brief The PropertyEdit class creates a specified amount of QSpinboxes as well as a Label naming the properties they change.
 *
 * Details on the configuraion structure can be found within the Configurator class.
 */
class PropertyEdit : public QWidget
{
        Q_OBJECT
    public:
        explicit PropertyEdit(const QJsonObject &config, QWidget *parent = Q_NULLPTR);

        /**
         * @brief Returns a json object of the components values.
         * @return QJsonObject containing the set value
         */
        QJsonObject toJson() const;

        /**
         * @brief Initializes the component from json configuration.
         * @param values
         */
        void fromJson(const QJsonObject &values);

        /**
         * @brief Returns the attribute set in the ui configuraion
         * @return
         */
        QString getAttribute() const;

    signals:

    public slots:

    private:
        // qt initializes
        void initDefaultLayout();
        void initLayout(const QJsonObject &config);
        void initLayoutElement(const QJsonObject &config, QFormLayout *layout);
    private:
        QString _attribute;
        QMap<QString, QDoubleSpinBox*> _ui_elements;
};

#endif // PROPERTYEDIT_HPP
