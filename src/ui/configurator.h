#ifndef GROUPCONFIGURATORWIDGET_HPP
#define GROUPCONFIGURATORWIDGET_HPP

#include <QWidget>
#include <QGroupBox>
#include <QJsonArray>
#include <QJsonObject>

#include <QPushButton>

#include "ui/propertyeditgroup.h"

/**
 * @brief The Configurator class creates a mondular user interfaces consisting of ui groups with Qt style spin boxes.
 *
 * A user interface configuraion file has the following basic layout, starting
 * off with a <string> specifying the Confirm buttons label, as well as an array
 * containing groups of ui elements to be created.
 *
 * {
 *  "confirm_message" : "Recalculate",
 *  "ui" : [...]
 * }
 *
 * Each major group gets a title as well as a key attribute matching the key specified
 * within the respective model configuration. The key is important for the main application
 * to build a correspondence between user interface and model configuraion. Furthermore,
 * to-be-created ui groups are given as attributes of a json object. This is done in order
 * to maintain the order of groups specified in the json file within the application.
 *
 * {
 *   "title": "Projector",
 *   "attribute": "projector",
 *   "groups": { ... },
 * }
 *
 * Groups are specified by simple creating a named json object. Containing a title, again
 * another key to identify the attribute it changes. Important is, that the objects name
 * as well as the string inclueded within the 'attrbute' field MUST match. Finally each
 * group contains an array which specifies the amount of values to be captured. For
 * each element of the 'elements' array, the applicaion will create a simple QSpinbox.
 *
 * {
 *  "position": {
 *      "title": "Position",
 *      "attribute" : "position",
 *      "elements" : [...],
 * }
 *
 * Each Element to be created needs several attributes in order to function properly.
 * First the Spinboxes title, as well as the attribute it captures. In addition to that
 * the minimum, maximum and the precision need to be specified
 *
 * {
 *     "title": "X",
 *     "attribute": "x",
 *     "precision": 0.01,
 *     "min": -2.0,
 *     "max": 2.0
 * },
 */
class Configurator : public QWidget
{
        Q_OBJECT
    public:
        explicit Configurator(const QJsonObject &config, QWidget *parent = Q_NULLPTR);


        /**
         * @brief Returns the set values of all nested UI-elements as json object.
         * @return
         */
        QJsonObject getValues() const;

        /**
         * @brief Set values from given json object.
         *
         * Information on the json structure can be found at this classes main description *
         *
         * @param values specifies a json object containing information on
         * which UI-element should be set to with the given value
         */
        void setValues(const QJsonObject& values);

    signals:
        /**
         * @brief Emits a signal that new values are available.
         * @param new_values
         */
        void valueUpdate(QJsonObject new_values);

    public slots:

        /**
         * @brief Slot connected to button press event.
         * Emits the valueUpdate signal
         */
        void onConfirm();

    private:
        /**
         * @brief Initializes the user interface specified by the passed configuration.
         *
         * Information on the json structure can be found at this classes main description *
         *
         * @param config
         */
        void initUI(const QJsonObject &config);

        /**
         * @brief Initializes a layout group from given json object.
         *
         * Again...
         * Information on the json structure can be found at this classes main description *
         *
         * @param config specifies the configuration to be used.
         * @param layout specifies what layout the Group should be created in.
         */
        void initLayoutGroup(const QJsonObject &config, QBoxLayout*layout);

    private:
        QString attribute_;
        QMap<QString, PropertyEditGroup*> ui_elements_;

        // ui
        QPushButton *confirm_button_;
};

#endif // GROUPCONFIGURATORWIDGET_HPP
