#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <vector>

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QMatrix4x4>

#include "model/simulation.h"

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
        Q_OBJECT

    public:
        /**
         * @brief GLWidget
         * @param parent
         */
        GLWidget(QWidget *parent = 0);

        /**
         * d'tor
         */
        ~GLWidget();

        /**
         * @brief minimumSizeHint
         * @return
         */
        QSize minimumSizeHint() const override;

        /**
         * @brief sizeHint
         * @return
         */
        QSize sizeHint() const override;

        void setScene(Scene scene);

    public slots:
        void cleanup();

    signals:
        void updateScene(Scene scene);

    protected:
        /**
         * @brief initializeGL
         */
        void initializeGL() override;

        /**
         * @brief paintGL
         */
        void paintGL() override;

        /**
         * @brief resizeGL
         * @param width
         * @param height
         */
        void resizeGL(int width, int height) override;

        void mousePressEvent(QMouseEvent *event) override;

        void mouseMoveEvent(QMouseEvent *event) override;

    private:
        void setYRotation(int angle);
        /**
         * @brief initShader
         */
        void initShader();

        /**
         * @brief initVBOs
         */
        void initVBOs();

        /**
         * @brief initView
         */
        void initView();

        QOpenGLBuffer createBuffer(std::vector<GLfloat> buffer_data);


        /**
         * @brief renderElement
         * @param vertex_buffer
         * @param color_buffer
         * @param mvp
         */
        void renderElement(QOpenGLBuffer vertex_buffer, QOpenGLBuffer color_buffer, const QMatrix4x4 &mvp);

    private:
        QOpenGLShaderProgram *_shader_program;
        int _mvp_location;

        std::vector<GLfloat> _point;
        std::vector<GLfloat> _point_blue;
        std::vector<GLfloat> _point_green  ;
        std::vector<GLfloat> _point_red    ;
        std::vector<GLfloat> _point_yellow ;
        std::vector<GLfloat> _point_white  ;
        std::vector<GLfloat> _point_grey   ;

        QOpenGLVertexArrayObject _vao;
        QOpenGLBuffer _point_vbo;
        QMap<QString, QOpenGLBuffer> _point_colors;

        // matrices
        QMatrix4x4 _projection_mat;
        QMatrix4x4 _view_mat;
        QMatrix4x4 _model_mat;
        QMatrix4x4 _mvp;

        // scene
        Scene _scene;
        bool _running;

        int _x_rotation;
        int _y_rotation;
        int _z_rotation;
        QPoint _last_position;
};

#endif
