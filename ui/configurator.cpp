#include "ui/configurator.h"

Configurator::Configurator(const QJsonObject &config, QWidget *parent) : QWidget(parent)
{
    initLayout(config);
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

    // set layout
    setLayout(root);
}

void Configurator::initLayoutGroup(const QJsonObject &config, QBoxLayout *layout) {

    PropertyEditGroup *edit = new PropertyEditGroup(config, this);

    QString title = config["title"].toString();
    QGroupBox * group = new QGroupBox;
    group->setContentsMargins(0,0,0,0);
    group->setTitle(title);
    group->setLayout(edit->layout());

    layout->addWidget(group);
}

