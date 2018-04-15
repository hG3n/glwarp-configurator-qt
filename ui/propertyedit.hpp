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
        explicit PropertyEdit(QWidget *parent = 0);
        explicit PropertyEdit(const QJsonObject &config, QWidget *parent = 0);

        QMap<QString, double> getValues() const;
    signals:

    public slots:

    private:
        void initDefaultLayout();
        void initLayout();
    private:
        QJsonObject _config;
        QMap<QString, QDoubleSpinBox*> _ui_elements;

};

#endif // PROPERTYEDIT_HPP
