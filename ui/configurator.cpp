#include "ui/configurator.h"

Configurator::Configurator(const QJsonObject &config, QWidget *parent) : QWidget(parent)
{
    initLayout(config);
}

void Configurator::getValues() const{

    for(auto pair: _ui_elements.toStdMap()) {
        qDebug() << pair.second->getValuesJson();
    }

}

void Configurator::onConfirm() {
    for(auto pair: _ui_elements.toStdMap()) {
        pair.second->getValues();
    }
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

    connect( _confirm_button, &QPushButton::released,
             [=] () {

        getValues();
             }
    );


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

