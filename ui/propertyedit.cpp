#include "propertyedit.hpp"

PropertyEdit::PropertyEdit(const QJsonObject &config, QWidget *parent)
    : QWidget(parent)
{
    initLayout(config);
}

QJsonObject PropertyEdit::toJson() const {
    QJsonObject obj;
    for(auto pair: _ui_elements.toStdMap()) {
        obj[pair.first] = pair.second->value();
    }
    return obj;
}

void PropertyEdit::fromJson (const QJsonObject &values) {
    for(auto key: values.keys()) {
        double value = values[key].toDouble();

        if(_ui_elements.contains(key))
            _ui_elements[key]->setValue(value);
    }
}

QString PropertyEdit::getAttribute() const {
    return _attribute;
}

void PropertyEdit::initDefaultLayout() {
    QFormLayout *form = new QFormLayout;
    setLayout(form);
}

void PropertyEdit::initLayout(const QJsonObject &config) {

    setContentsMargins(0,0,0,0);

    // create new root layout
    QFormLayout *form = new QFormLayout;
    form->setFormAlignment(Qt::AlignRight);

    _attribute = config["attribute"].toString();

    QJsonArray element_array = config["elements"].toArray();
    for(auto array_item: element_array) {
        initLayoutElement(array_item.toObject(), form);
    }

    setLayout(form);
}

void PropertyEdit::initLayoutElement(const QJsonObject &config, QFormLayout *layout) {

    QString title = config["title"].toString();
    double precision = config["precision"].toDouble();
    QString attribute = config["attribute"].toString();
    double min_value = config["min"].toDouble();
    double max_value = config["max"].toDouble();

    QDoubleSpinBox *spinbox = new QDoubleSpinBox(this);
    spinbox->setSingleStep(precision);
    spinbox->setMinimum(min_value);
    spinbox->setMaximum(max_value);

    _ui_elements[attribute] = spinbox;

    layout->addRow(title, spinbox);
}

