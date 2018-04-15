#ifndef POSITIONEDIT_HPP
#define POSITIONEDIT_HPP

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QVector3D>
#include <QFormLayout>
#include <QGroupBox>

class Vector3Edit : public QWidget
{
        Q_OBJECT
    public:
        explicit Vector3Edit(QWidget *parent = 0);
        explicit Vector3Edit(double step_size, QWidget *parent = 0);

        /**
         * @brief setStepSize
         * @param step_size
         */
        void setStepSize(double step_size);

        /**
         * @brief setValue
         * @param value
         */
        void setValue(QVector3D const& value);

        /**
         * @brief getPosition
         * @return
         */
        QVector3D getValue() const;

    private:
        void initLayout(double step_size);

    private:
        QDoubleSpinBox *_x_edit;
        QDoubleSpinBox *_y_edit;
        QDoubleSpinBox *_z_edit;
};

#endif // POSITIONEDIT_HPP
