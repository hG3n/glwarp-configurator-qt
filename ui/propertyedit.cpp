#include "propertyedit.hpp"


PropertyEdit::PropertyEdit(const QJsonObject &config, QWidget *parent)
    : QWidget(parent)
{
    initLayout(config);
}

QMap<QString, double> PropertyEdit::getValues() const {
    QMap<QString, double> map;
    for(auto pair: _ui_elements.toStdMap()) {
        map[pair.first] = pair.second->value();
    }
    return map;
}

QJsonObject PropertyEdit::getValuesJson() const {
    QJsonObject obj;
    for(auto pair: _ui_elements.toStdMap()) {
        obj[pair.first] = pair.second->value();
    }
    return obj;
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

    QJsonArray element_array = config["elements"].toArray();
    for(auto array_item: element_array) {
        initLayoutElement(array_item.toObject(), form);
    }

    setLayout(form);
}

void PropertyEdit::initLayoutElement(const QJsonObject &config, QFormLayout *layout) {

    QString title = config["title"].toString();
    QString attribute = config["attribute"].toString();
    double precision = config["precision"].toDouble();

    QDoubleSpinBox *spinbox = new QDoubleSpinBox(this);
    spinbox->setSingleStep(precision);

    _ui_elements[attribute] = spinbox;

    layout->addRow(title, spinbox);
}

