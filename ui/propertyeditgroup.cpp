#include <utility>

#include "ui/propertyeditgroup.h"

PropertyEditGroup::PropertyEditGroup(const QJsonObject &config, QWidget *parent)
    :QWidget(parent)
{
    initLayout(config);
}


QMap<QString, QMap<QString, double>> PropertyEditGroup::getValues() const {
    QMap<QString, QMap<QString, double>> map;
    for(auto pair: _ui_elements.toStdMap()) {
        map[pair.first] = pair.second->getValues();
    }
    return map;
}

QJsonObject PropertyEditGroup::getValuesJson() const {
    QJsonObject obj;
    for(auto pair: _ui_elements.toStdMap()) {
        obj[pair.first] = pair.second->getValuesJson();
    }
    return obj;
}

void PropertyEditGroup::initLayout(const QJsonObject & config) {

    setContentsMargins(0,0,0,0);

    // create new root layout
    QVBoxLayout *root = new QVBoxLayout;
    root->setSpacing(0);
    root->setAlignment(Qt::AlignRight);
    root->setContentsMargins(0,0,0,0);

    // add groups
    QJsonArray groups = config["groups"].toArray();
    for(auto group : groups) {
        initEditGroup(group.toObject(), root);
    }

    // set layout
    setLayout(root);
}

void PropertyEditGroup::initEditGroup(const QJsonObject &config, QBoxLayout *layout) {
    QString title = config["title"].toString();
    QString attribute = config["attribute"].toString();

    QGroupBox *group_box = new QGroupBox;
    group_box->setContentsMargins(0,0,0,0);
    group_box->setTitle(title);

    PropertyEdit *edit = new PropertyEdit(config, this);
    group_box->setLayout(edit->layout());

    _ui_elements[attribute] = edit;

    layout->addWidget(group_box);
}
