#include "ui/glwarpwidget.h"

#include <vector>
#include <math.h>

#include <QCoreApplication>
#include <QPoint>

GLWarpWidget::GLWarpWidget(QWidget *parent)
    : QOpenGLWidget(parent),
      _shader_program(0)
{}

GLWarpWidget::~GLWarpWidget()
{
    cleanup();
}

QSize GLWarpWidget::minimumSizeHint() const
{
    return QSize(640, 480);
}

QSize GLWarpWidget::sizeHint() const
{
    return QSize(1400, 1800);
}

void GLWarpWidget::updateValues(std::vector<QVector3D> mesh_values,
                                std::vector<QVector3D> uv_values) {

    qDebug().nospace() << Q_FUNC_INFO << " :" << __LINE__;
    qDebug() << "  >" << "values received";
    qDebug() << "  >" << "number of mesh values:" <<mesh_values.size();
    qDebug() << "  >" << "mesh meta:" << mesh_values.back();
    qDebug() << "  >" << "number of uv values:" <<uv_values.size();
    qDebug() << "  >" << "uv meta:" << uv_values.back();

    int triangle_count;
    std::vector<GLfloat> mesh;
    if(setup_vertices(mesh_values, &mesh, &triangle_count)) {
        qDebug() << "  >" << "Mesh setup succeeded!";
    } else {
        qDebug().nospace() << Q_FUNC_INFO << " :" << __LINE__;
        qDebug() << "  >" << "Error in setting up the transformation mesh";
    }

    std::vector<GLfloat> uv_coords;
    if(setup_tex_coords(uv_values, &uv_coords)) {
        qDebug() << "  >" << "Texture coord setup succeeded!";
    } else {
        qDebug().nospace() << Q_FUNC_INFO << " :" << __LINE__;
        qDebug() << "  >" << "Error in setting up texture coordinates";
    }

    _mesh_vbo.create();
    _mesh_vbo.bind();
    _mesh_vbo.allocate(mesh.data(), mesh.size() * sizeof(GLfloat));

    _texture_vbo.create();
    _texture_vbo.bind();
    _texture_vbo.allocate(uv_coords.data(), uv_coords.size() * sizeof(GLfloat));
}

void GLWarpWidget::cleanup()
{
    makeCurrent();
    delete _shader_program;
    _shader_program = 0;
    doneCurrent();
}

void GLWarpWidget::keyPressEvent(QKeyEvent *event) {

    int key = event->key();

    switch (key) {
    case Qt::Key_Escape:
        close(); break;
    case Qt::Key_Q:
        close(); break;
    case Qt::Key_F:
//        setWindowFlags(Qt::WindowStaysOnTopHint);
//        setWindowFlags(Qt::FramelessWindowHint);
        setWindowState(Qt::WindowFullScreen);
        break;
    }

}

void GLWarpWidget::initShader() {
    // init shader
    _shader_program = new QOpenGLShaderProgram;
    _shader_program->addShaderFromSourceFile(QOpenGLShader::Vertex,  ":/shaders/simple-warp.vert" );
    _shader_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/simple-warp.frag" );
    _shader_program->bindAttributeLocation("vertex_position_modelspace", 0);
    _shader_program->bindAttributeLocation("vertex_uv", 1);
    _shader_program->link();

    // get uniform locations
    _shader_program->bind();
    _mvp_location = _shader_program->uniformLocation("mvp");
    _shader_program->release();
}


QOpenGLBuffer GLWarpWidget::createBuffer(std::vector<GLfloat> buffer_data) {
    QOpenGLBuffer buffer;
    buffer.create();
    buffer.bind();
    buffer.allocate(buffer_data.data(), buffer_data.size() * sizeof(GLfloat));
    return buffer;
}


void GLWarpWidget::initView() {
    // setup projection matrix
    _projection_mat.setToIdentity();
    _projection_mat.perspective(90.0f, 16.0f/9.0f, 0.1f, 100.0f);

    // setup model matrix
    _model_mat.setToIdentity();

    // setup view matrix
    _view_mat.setToIdentity();
    _view_mat.lookAt(QVector3D(0.0f, 1.8f, 4.5f),
                     QVector3D(0.0f, 1.0f, 0.0f),
                     QVector3D(0.0f, 1.0f, 0.0f));
}


void GLWarpWidget::initializeGL()
{
    // In this example the widget's corresponding top-level window can change
    // several times during the widget's lifetime. Whenever this happens, the
    // QOpenGLWidget's associated context is destroyed and a new one is created.
    // Therefore we have to be prepared to clean up the resources on the
    // aboutToBeDestroyed() signal, instead of the destructor. The emission of
    // the signal will be followed by an invocation of initializeGL() where we
    // can recreate all resources.
    connect(context(), &QOpenGLContext::aboutToBeDestroyed,
            this, &GLWarpWidget::cleanup);

    initializeOpenGLFunctions();

    glClearColor(0, 0, 0, 0);
    glPointSize(3.0f);

    // create vao for whatever reason
    // still, apparently nothing'll ever work without at least creating it
    // so there you are gl, have fun
    _vao.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&_vao);

    // load the final testing texture
    _texture = new QOpenGLTexture(QImage(":/textures/radialgrid.png"));

    // further inits
    initShader();
    initView();
}


void GLWarpWidget::renderElement(QOpenGLBuffer vertex_buffer, QOpenGLBuffer tex_buffer, const QMatrix4x4 &mvp) {

    _shader_program->setUniformValue(_mvp_location, mvp);


    vertex_buffer.bind();
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    vertex_buffer.release();

    tex_buffer.bind();
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    tex_buffer.release();

    _texture->bind();

    glDrawArrays(GL_TRIANGLES, 0, 3);
}


void GLWarpWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    QOpenGLVertexArrayObject::Binder vao_binder(&_vao);

    // calculate
    _mvp = _projection_mat * _view_mat * _model_mat;
    _shader_program->bind();

    renderElement(_mesh_vbo, _texture_vbo, _mvp);

    _shader_program->release();

    vao_binder.release();
}


void GLWarpWidget::resizeGL(int w, int h)
{
    glViewport(0,0,w, h);
    _projection_mat.setToIdentity();
    _projection_mat.perspective(70.0f, GLfloat(w) / h, 0.01f, 100.0f);
}


bool GLWarpWidget::setup_vertices(std::vector<QVector3D> mesh_values, std::vector<GLfloat> *mesh, int *triangle_count){

    // get meta information about calculated
    auto ring_count = (int) mesh_values.back().x();
    auto ring_elements_count = (int) mesh_values.back().y();
    auto point_count = (int) mesh_values.back().z();

    qDebug().nospace() << Q_FUNC_INFO << " :" << __LINE__;
    qDebug() << "  >" << "circle count: " << ring_count << " points_per_circle: " << ring_elements_count;

    // remove meta info
    mesh_values.pop_back();

    //mapVecToRange(&mesh);
    qDebug() << "  >" << "Number of verices:" << mesh_values.size() << " actual number of vertices:" << point_count;
    if ((int)mesh_values.size() != point_count) {
        qDebug().nospace() << Q_FUNC_INFO << " :" << __LINE__;
        qDebug() << "  >" << "Error: Number of mesh points do not match!";
        return false;
    }

    // IF HAGEN SWITCHES HIS ZERO COORDS AGAIN JUST SLAP HIM IN THE FACE
    // AND EPHRAIM AS WELL, JUST BECAUSE
    // for (int i = 0; i < mesh.size(); ++i) {
    //    mesh[i].y = mesh[i].z;
    //    mesh[i].z = 0.0f;
    // }

    // create mesh

    std::vector<QVector3D> mesh_vec;
    for (int circle_idx = 0; circle_idx < ring_count; ++circle_idx) {
        if (circle_idx == 0) {
            for (int t = 1; t < ring_elements_count + 1; ++t) {
                // start triangles around center
                //std::cout << 0 << " " << t << " " << 1 + (t % points_per_circle) << std::endl;

                int i1 = 0;
                int i2 = t;
                int i3 = 1 + (t % ring_elements_count);
                mesh_vec.push_back(mesh_values[i1]);
                mesh_vec.push_back(mesh_values[i2]);
                mesh_vec.push_back(mesh_values[i3]);
                *triangle_count += 1;
            }
        } else {
            int start_point = circle_idx * ring_elements_count - (ring_elements_count - 1);
            for (int idx = 0; idx < ring_elements_count; ++idx) {
                int i1 = start_point + idx;
                int i2 = start_point + idx + ring_elements_count;
                int i3 = start_point + (idx + 1) % ring_elements_count;
                mesh_vec.push_back(mesh_values[i1]);
                mesh_vec.push_back(mesh_values[i2]);
                mesh_vec.push_back(mesh_values[i3]);
                //std::cout << i1<< " " << i2 << " " << i3 << std::endl;
                int i4 = start_point + (idx + 1) % ring_elements_count;
                int i5 = start_point + idx + ring_elements_count;
                int i6 = start_point + ((idx + 1) % ring_elements_count) + ring_elements_count;
                mesh_vec.push_back(mesh_values[i4]);
                mesh_vec.push_back(mesh_values[i5]);
                mesh_vec.push_back(mesh_values[i6]);
                //std::cout << i4 << " " << i5<< " " << i6 << std::endl;
                *triangle_count += 2;
            }
        }
    }

    // convert to std::vector<GLfloat>
    for( auto i : mesh_vec) {
        GLfloat x = i.x();
        GLfloat y = i.y();
        GLfloat z = i.z();

        mesh->push_back(x);
        mesh->push_back(y);
        mesh->push_back(z);
    }


    return true;

//    // create buffers
//    GLuint vertex_buffer;
//    glGenBuffers(1, &vertex_buffer);
//    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
//    glBufferData(GL_ARRAY_BUFFER, mesh_vec.size() * sizeof(glm::vec3), &mesh_vec[0], GL_STATIC_DRAW);
//    return vertex_buffer;
}


bool GLWarpWidget::setup_tex_coords(std::vector<QVector3D> uv_coord_values, std::vector<GLfloat> *uv_coords){

    // get meta information about calculated
    auto ring_count = (int) uv_coord_values.back().x();
    auto ring_elements_count = (int) uv_coord_values.back().y();
    auto point_count = (int) uv_coord_values.back().z();

    // remove meta info
    uv_coord_values.pop_back();

    // IF HAGEN SWITCHES HIS ZERO COORDS AGAIN JUST SLAP HIM IN THE FACE
    // for (int i = 0; i < uv_coords.size(); ++i) {
    //     uv_coords[i].y = uv_coords[i].z;
    //     uv_coords[i].z = 0.0f;
    // }

    qDebug().nospace() << Q_FUNC_INFO << " :" << __LINE__;
    qDebug() << "  >" << "red size: " << uv_coord_values.size() << " read point count: " << point_count;

    std::vector<QVector3D> uv_vec;
    for (int circle_idx = 0; circle_idx < ring_count; ++circle_idx) {
        if (circle_idx == 0) {
            for (int t = 1; t < ring_elements_count + 1; ++t) {
                // start triangles around center
                //std::cout << 0 << " " << t << " " << 1 + (t % points_per_circle) << std::endl;
                int i1 = 0;
                int i2 = t;
                int i3 = 1 + (t % ring_elements_count);

                //float u = mapToRange(uv_coords[i1].x, min_pos_x, max_pos_x, 0.0f, 1.0f);
                float u = uv_coord_values[i1].x();
                float v = uv_coord_values[i1].y();
                uv_vec.push_back(QVector2D(u, v));

                u = uv_coord_values[i2].x();
                v = uv_coord_values[i2].y();
                uv_vec.push_back(QVector2D(u, v));

                u = uv_coord_values[i3].x();
                v = uv_coord_values[i3].y();
                uv_vec.push_back(QVector2D(u, v));
            }
        } else {
            int start_point = circle_idx * ring_elements_count - (ring_elements_count - 1);
            for (int idx = 0; idx < ring_elements_count; ++idx) {
                int i1 = start_point + idx;
                int i2 = start_point + idx + ring_elements_count;
                int i3 = start_point + (idx + 1) % ring_elements_count;
                float u = uv_coord_values[i1].x();
                float v = uv_coord_values[i1].y();
                uv_vec.push_back(QVector2D(u, v));

                u = uv_coord_values[i2].x();
                v = uv_coord_values[i2].y();
                uv_vec.push_back(QVector2D(u, v));

                u = uv_coord_values[i3].x();
                v = uv_coord_values[i3].y();
                uv_vec.push_back(QVector2D(u, v));

                //std::cout << i1<< " " << i2 << " " << i3 << std::endl;
                int i4 = start_point + (idx + 1) % ring_elements_count;
                int i5 = start_point + idx + ring_elements_count;
                int i6 = start_point + ((idx + 1) % ring_elements_count) + ring_elements_count;
                u = uv_coord_values[i4].x();
                v = uv_coord_values[i4].y();
                uv_vec.push_back(QVector2D(u, v));

                u = uv_coord_values[i5].x();
                v = uv_coord_values[i5].y();
                uv_vec.push_back(QVector2D(u, v));

                u = uv_coord_values[i6].x();
                v = uv_coord_values[i6].y();
                uv_vec.push_back(QVector2D(u, v));
                //std::cout << i4 << " " << i5<< " " << i6 << std::endl;
            }
        }
    }


    for( auto i : uv_vec) {
        GLfloat x = i.x();
        GLfloat y = i.y();
        GLfloat z = i.z();

        uv_coords->push_back(x);
        uv_coords->push_back(y);
        uv_coords->push_back(z);
    }

    return true;

//    GLuint uv_buffer;
//    glGenBuffers(1, &uv_buffer);
//    glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
//    glBufferData(GL_ARRAY_BUFFER, uv_coords.size() * sizeof(glm::vec2), &uv_coords[0], GL_STATIC_DRAW);
//    return uv_buffer;
}
