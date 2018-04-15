#include <math.h>

#include <QMouseEvent>
#include <QOpenGLShaderProgram>
#include <QCoreApplication>

#include "ui/glwidget.h"

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent),
      _shader_program(0)
{
    _point =  {0.0f,0.0f,0.0f};
    _point_blue   = {0.0f, 0.0f, 1.0f};
    _point_green  = {0.0f, 1.0f, 0.0f};
    _point_red    = {1.0f, 0.0f, 0.0f};
    _point_yellow = {1.0f, 1.0f, 0.0f};
    _point_white  = {1.0f, 1.0f, 1.0f};
    _point_grey   = {0.5f, 0.5f, 0.5f};

    _scene = new Scene;
}

GLWidget::~GLWidget()
{
    cleanup();
}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(640, 480);
}

QSize GLWidget::sizeHint() const
{
    return QSize(1400, 1800);
}

void GLWidget::setScene(Scene *scene) {
    _scene = scene;
}

Scene* GLWidget::getScene() const {
    return _scene;
}

void GLWidget::cleanup()
{
    makeCurrent();
    delete _shader_program;
    _shader_program = 0;
    doneCurrent();
}


void GLWidget::initShader() {
    // init shader
    _shader_program = new QOpenGLShaderProgram;
    _shader_program->addShaderFromSourceFile(QOpenGLShader::Vertex,  ":/shaders/simple.vert" );
    _shader_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/simple.frag" );
    _shader_program->bindAttributeLocation("vertex_position_modelspace", 0);
    _shader_program->bindAttributeLocation("vertex_color", 1);
    _shader_program->link();

    // get uniform locations
    _shader_program->bind();
    _mvp_location = _shader_program->uniformLocation("mvp");
    _shader_program->release();

}


void GLWidget::initVBOs() {

    // create vao
    _vao.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&_vao);

    // create vbo
    _point_vbo.create();
    _point_vbo.bind();
    _point_vbo.allocate(_point.data(), _point.size() * sizeof(GLfloat));

    // create color vbos
    _point_colors["blue"] = createBuffer(_point_blue   );
    _point_colors["green"] = createBuffer(_point_green  );
    _point_colors["red"] = createBuffer(_point_red    );
    _point_colors["yellow"] = createBuffer(_point_yellow );
    _point_colors["white"] = createBuffer(_point_white  );
    _point_colors["grey"] = createBuffer(_point_grey   );


}

QOpenGLBuffer GLWidget::createBuffer(std::vector<GLfloat> buffer_data) {
    QOpenGLBuffer buffer;
    buffer.create();
    buffer.bind();
    buffer.allocate(buffer_data.data(), buffer_data.size() * sizeof(GLfloat));
    return buffer;
}


void GLWidget::initView() {
    // setup projection matrix
    _projection_mat.setToIdentity();
    _projection_mat.perspective(90.0f, 16.0f/9.0f, 0.1f, 100.0f);

    // setup model matrix
    _model_mat.setToIdentity();

    // setup view matrix
    _view_mat.setToIdentity();
    _view_mat.lookAt(QVector3D(0.0f, 1.8f, -3.5f),
                     QVector3D(0.0f, 1.0f, 0.0f),
                     QVector3D(0.0f, 1.0f, 0.0f));
}


void GLWidget::initializeGL()
{
    // In this example the widget's corresponding top-level window can change
    // several times during the widget's lifetime. Whenever this happens, the
    // QOpenGLWidget's associated context is destroyed and a new one is created.
    // Therefore we have to be prepared to clean up the resources on the
    // aboutToBeDestroyed() signal, instead of the destructor. The emission of
    // the signal will be followed by an invocation of initializeGL() where we
    // can recreate all resources.
    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &GLWidget::cleanup);

    initializeOpenGLFunctions();

    glClearColor(0, 0, 0, 0);
    glPointSize(2.0f);

    initShader();
    initVBOs();
    initView();
}


void GLWidget::renderElement(QOpenGLBuffer vertex_buffer, QOpenGLBuffer color_buffer, QMatrix4x4 mvp) {

    _shader_program->setUniformValue(_mvp_location, mvp);

    vertex_buffer.bind();
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    vertex_buffer.release();

    color_buffer.bind();
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    color_buffer.release();

    glDrawArrays(GL_POINTS, 0, 3);
}


void GLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    QOpenGLVertexArrayObject::Binder vao_binder(&_vao);

    // calculate
    _mvp = _projection_mat * _view_mat * _model_mat;
    _shader_program->bind();

    // render scene elements
    if(_scene) {
        for(auto point: _scene->dome_vertices) {
            QMatrix4x4 current_mvp(_mvp);
            current_mvp.translate(point);
            renderElement(_point_vbo, _point_colors["grey"], current_mvp);
        }

        for(auto point: _scene->sample_grid) {
            QMatrix4x4 current_mvp(_mvp);
            current_mvp.translate(point);
            renderElement(_point_vbo, _point_colors["blue"], current_mvp);
        }

        for(auto point: _scene->first_hits) {
            QMatrix4x4 current_mvp(_mvp);
            current_mvp.translate(point);
            renderElement(_point_vbo, _point_colors["green"], current_mvp);
        }

        for(auto point: _scene->second_hits) {
            QMatrix4x4 current_mvp(_mvp);
            current_mvp.translate(point);
            renderElement(_point_vbo, _point_colors["red"], current_mvp);
        }

        for(auto pair: _scene->near_corners) {
            QMatrix4x4 current_mvp(_mvp);
            current_mvp.translate(pair.second);
            renderElement(_point_vbo, _point_colors["yellow"], current_mvp);
        }

        for(auto pair: _scene->far_corners) {
            QMatrix4x4 current_mvp(_mvp);
            current_mvp.translate(pair.second);
            renderElement(_point_vbo, _point_colors["yellow"], current_mvp);
        }
    }

    _shader_program->release();

    vao_binder.release();
}


void GLWidget::resizeGL(int w, int h)
{
    glViewport(0,0,w, h);
    _projection_mat.setToIdentity();
    _projection_mat.perspective(90.0f, GLfloat(w) / h, 0.01f, 100.0f);
}
