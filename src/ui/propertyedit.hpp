#ifndef PROPERTYEDIT_HPP
#define PROPERTYEDIT_HPP

#include <QWidget>
#include <QJsonObject>
#include <QJsonArray>
#include <QFormLayout>
#include <QDebug>
#include <QDoubleSpinBox>

class PropertyEdit : public QWidget
{
        Q_OBJECT
    public:
        explicit PropertyEdit(const QJsonObject &config, QWidget *parent = 0);

        QJsonObject toJson() const;
        void fromJson(const QJsonObject &values);

        QString getAttribute() const;
    signals:

    public slots:

    private:
        void initDefaultLayout();
        void initLayout(const QJsonObject &config);
        void initLayoutElement(const QJsonObject &config, QFormLayout *layout);
    private:
        QString _attribute;
        QMap<QString, QDoubleSpinBox*> _ui_elements;

};

#endif // PROPERTYEDIT_HPP
