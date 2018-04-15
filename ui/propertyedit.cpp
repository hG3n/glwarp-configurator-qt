#include "propertyedit.hpp"

PropertyEdit::PropertyEdit(QWidget *parent)
    : QWidget(parent)
{

    if(!_config.isEmpty()) {
        initLayout();
    }

}

PropertyEdit::PropertyEdit(const QJsonObject &config, QWidget *parent)
    : QWidget(parent)
    , _config(config)
{
    if(!_config.isEmpty()) {
        initLayout();
    }
}

QMap<QString, double> PropertyEdit::getValues() const {
    QMap<QString, double> map;
    for(auto pair: _ui_elements.toStdMap()) {
        map[pair.first] = pair.second->value();
    }
    return map;
}

void PropertyEdit::initDefaultLayout() {
    QFormLayout *form = new QFormLayout;
    setLayout(form);
}

void PropertyEdit::initLayout() {
    QFormLayout *form = new QFormLayout;

    if(_config.contains("elements") && _config["elements"].isArray()) {

        QJsonArray element_array = _config["elements"].toArray();

        for(auto array_item: element_array) {
            QJsonObject element = array_item.toObject();
            qDebug() << "element" << element;
            QString title = element["title"].toString();
            QString attribute = element["attribute"].toString();
            double precision = element["precision"].toDouble();

            QDoubleSpinBox *spinbox = new QDoubleSpinBox(this);
            spinbox->setSingleStep(precision);

            qDebug() << "attribute" << attribute;
            _ui_elements[attribute] = spinbox;

            form->addRow(title, spinbox);
        }
    }

    setLayout(form);
}

