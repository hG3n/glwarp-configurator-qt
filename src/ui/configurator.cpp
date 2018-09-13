#include "ui/configurator.h"

Configurator::Configurator(const QJsonObject &config, QWidget *parent)
    : QWidget(parent)
    , attribute_()
    , ui_elements_()
    , confirm_button_(Q_NULLPTR)
{
    initUI(config);
}

QJsonObject Configurator::getValues() const{
    QJsonObject obj;
    for(auto pair: ui_elements_.toStdMap()) {
        obj[pair.first] = pair.second->toJson();
    }
    return obj;
}

void Configurator::setValues(const QJsonObject &values) {

    for(auto key: values.keys()) {
        ui_elements_[key]->fromJson(values[key].toObject());
    }
}

void Configurator::onConfirm() {
    QJsonObject values = getValues();
    emit valueUpdate(values);
}

void Configurator::initUI(const QJsonObject &config) {

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
    confirm_button_ = new QPushButton(confirm_message, this);
    connect(confirm_button_, &QPushButton::pressed,
            this, &Configurator::onConfirm);

    root->addWidget(confirm_button_);

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

    ui_elements_[attribute] = edit;

    layout->addWidget(group);
}

