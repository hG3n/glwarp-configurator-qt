#include "ui/propertyeditgroup.h"

PropertyEditGroup::PropertyEditGroup(const QJsonObject &config, QWidget *parent)
    :QWidget(parent)
{
    initLayout(config);
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
    double precision = config["precision"].toDouble();

    QWidget *edit = new PropertyEdit(config, this);

    QGroupBox *group_box = new QGroupBox;
    group_box->setContentsMargins(0,0,0,0);
    group_box->setTitle(title);
    group_box->setLayout(edit->layout());

    _ui_elements[attribute] = edit;

    layout->addWidget(group_box);
}
