#ifndef GLWARPWIDGET_H
#define GLWARPWIDGET_H

#include <vector>

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>

#include <QKeyEvent>

#include <QMatrix4x4>
#include <QVector3D>
#include <QVector2D>

#include <QImage>

#include "model/simulation.h"

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class GLWarpWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
        Q_OBJECT

    public:
        /**
         * @brief GLWidget
         * @param parent
         */
        GLWarpWidget(QWidget *parent = Q_NULLPTR);

        /**
         * d'tor
         */
        ~GLWarpWidget();

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
         * @brief Update the mesh and uv_values
         * @param mesh_values
         * @param uv_values
         */
        void updateValues(std::vector<QVector3D> mesh_values, std::vector<QVector3D> uv_values);

    public slots:
        void cleanup();
        void keyPressEvent(QKeyEvent *event) override;

    signals:

    protected:
        /**
         * @brief Initializes the Qt-OpenGL context
         *
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

    private:
        /**
         * @brief initShader
         */
        void initShader();

        /**
         * @brief initView
         */
        void initView();

        /**
         * @brief createBuffer
         * @param buffer_data
         * @return
         */
        QOpenGLBuffer createBuffer(std::vector<GLfloat> buffer_data);

        /**
         * @brief renderElement
         * @param vertex_buffer
         * @param color_buffer
         * @param mvp
         */
        void renderElement(QOpenGLBuffer vertex_buffer, QOpenGLBuffer color_buffer, const QMatrix4x4 &mvp);

        /**
         * @brief Calculates the final warping mesh from given points
         * @param mesh
         * @param triangle_count
         * @return
         */
        bool setup_vertices(std::vector<QVector3D> mesh_values, std::vector<GLfloat> *mesh, int *triangle_count);

        /**
         * @brief Calculates the final uv coordinates used for warping
         * @param uv_coord_values
         * @param uv_coords
         * @return
         */
        bool setup_tex_coords(std::vector<QVector3D> uv_coord_values, std::vector<GLfloat> *uv_coords);

    private:
        QOpenGLShaderProgram *_shader_program;
        int _mvp_location;

        std::vector<GLfloat> _mesh;
        std::vector<GLfloat> _uv_values;

        QOpenGLVertexArrayObject _vao;
        QOpenGLBuffer _point_vbo;
        QOpenGLBuffer _mesh_vbo;
        QOpenGLBuffer _texture_vbo;

        QOpenGLTexture *_texture;

        // matrices
        QMatrix4x4 _projection_mat;
        QMatrix4x4 _view_mat;
        QMatrix4x4 _model_mat;
        QMatrix4x4 _mvp;

        // scene
        Scene _scene;
        bool _running;

        QPoint _last_position;
};

#endif // GLWARPWIDGET_H
