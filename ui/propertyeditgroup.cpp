#include <utility>

#include "ui/propertyeditgroup.h"

PropertyEditGroup::PropertyEditGroup(const QJsonObject &config, QWidget *parent)
    :QWidget(parent)
{
    initLayout(config);
}

QJsonObject PropertyEditGroup::toJson() const {
    QJsonObject obj;
    for(auto pair: _ui_elements.toStdMap()) {
        obj[pair.first] = pair.second->toJson();
    }
    return obj;
}

void PropertyEditGroup::fromJson(const QJsonObject &values) {
    for(auto key : values.keys()) {
        _ui_elements[key]->fromJson(values[key].toObject());
    }
}

QString PropertyEditGroup::getAttribute() const {
    return _attribute;
}

void PropertyEditGroup::initLayout(const QJsonObject & config) {

    setContentsMargins(0,0,0,0);

    // create new root layout
    QVBoxLayout *root = new QVBoxLayout;
    root->setSpacing(0);
    root->setAlignment(Qt::AlignRight);
    root->setContentsMargins(0,0,0,0);

    _attribute = config["attribute"].toString();

    // add groups
    QJsonObject groups = config["groups"].toObject();
    for(auto group : groups) {
        qDebug() << group;
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
