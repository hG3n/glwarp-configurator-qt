#include "ui/configurator.h"

Configurator::Configurator(const QJsonObject &config, QWidget *parent) : QWidget(parent)
{
    initLayout(config);
}

QJsonObject Configurator::getValues() const{
    QJsonObject obj;
    for(auto pair: _ui_elements.toStdMap()) {
        obj[pair.first] = pair.second->toJson();
    }
    return obj;
}

void Configurator::setValues(const QJsonObject &values) {

    for(auto key: values.keys()) {
//        qDebug() << "new element" << key;
//        //        qDebug() << "ui element" << _ui_elements[key]->
        _ui_elements[key]->fromJson(values[key].toObject());
    }
}

void Configurator::onConfirm() {
    QJsonObject values = getValues();
    emit valueUpdate(values);
}

void Configurator::initLayout(const QJsonObject &config) {

    setContentsMargins(0,0,0,0);

    // create new root layout
    QVBoxLayout *root = new QVBoxLayout;
    root->setSpacing(0);
    root->setAlignment(Qt::AlignRight);
    root->setContentsMargins(0,0,0,0);

    QJsonArray ui_groups = config["ui"].toArray();
    for(auto group: ui_groups) {
        initLayoutGroup(group.toObject(), root);
    }

    QString confirm_message = config["confirm_message"].toString();
    _confirm_button = new QPushButton(confirm_message, this);
    connect(_confirm_button, &QPushButton::pressed,
            this, &Configurator::onConfirm);

    root->addWidget(_confirm_button);

    // set layout
    setLayout(root);
}

void Configurator::initLayoutGroup(const QJsonObject &config, QBoxLayout *layout) {

    PropertyEditGroup *edit = new PropertyEditGroup(config, this);

    QString title = config["title"].toString();
    QString attribute = config["attribute"].toString();

    QGroupBox * group = new QGroupBox;
    group->setContentsMargins(0,0,0,0);
    group->setTitle(title);
    group->setLayout(edit->layout());

    _ui_elements[attribute] = edit;

    layout->addWidget(group);
}

