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
        QMap<QString, double> getValues() const;
        QJsonObject getValuesJson() const;
    signals:

    public slots:

    private:
        void initDefaultLayout();
        void initLayout(const QJsonObject &config);
        void initLayoutElement(const QJsonObject &config, QFormLayout *layout);
    private:
        QMap<QString, QDoubleSpinBox*> _ui_elements;

};

#endif // PROPERTYEDIT_HPP
