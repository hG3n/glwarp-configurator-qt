#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <vector>

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>

#include "model/simulation.h"

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
        Q_OBJECT

    public:
        GLWidget(QWidget *parent = Q_NULLPTR);
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

        /**
         * @brief Sets the current scene.
         * @param scene
         */
        void setScene(Scene scene);

    public slots:
        void cleanup();

    signals:
        /**
         * @brief Updates the scene.
         * @param scene
         */
        void updateScene(Scene scene);

    protected:
        /**
         * @brief Initialize OpenGL context.
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
         * @brief Initializes Shaders.
         */
        void initShader();

        /**
         * @brief Initialize Vertext Buffer Objects
         */
        void initVBOs();

        /**
         * @brief initView
         */
        void initView();

        /**
         * @brief Create buffer.
         * @param buffer_data
         * @return
         */
        QOpenGLBuffer createBuffer(std::vector<GLfloat> buffer_data);

        /**
         * @brief Renders a single vertex in the scene as quad.
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
        std::vector<GLfloat> _point_green;
        std::vector<GLfloat> _point_red;
        std::vector<GLfloat> _point_yellow;
        std::vector<GLfloat> _point_white;
        std::vector<GLfloat> _point_grey;

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
