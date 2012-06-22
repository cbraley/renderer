#ifndef RENDER_VIEW_WIDGET_H
#define RENDER_VIEW_WIDGET_H

#include <vector>
//--
#include <QGLWidget>
#include <QtOpenGL>
#include <QImage>
#include <QTimeLine>
#include <QGLShader>
#include <QGLShaderProgram>

class RenderViewWidget : public QGLWidget{
    Q_OBJECT
public:

    RenderViewWidget(QWidget *parent);
    virtual ~RenderViewWidget();

    void saveGLState();
    void restoreGLState();

    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void timerEvent(QTimerEvent *);
    void wheelEvent(QWheelEvent *);

    typedef struct ToneMapper{
        std::string algoName;
        std::vector<std::string> paramNames;
        std::vector<float> paramMins;
        std::vector<float> paramMaxs;
    }ToneMapper;

public slots:
    void draw();

private:
    int w,h; //Width and height
    QGLFramebufferObject *texture_fbo; //texture we render the scene to
    std::vector<QGLShader*> hdrFragShaders; //list of HDR fragment(pixel) shaders
    int hdrShaderIdx; //Which shader are we currently using?
    QGLShader* hdrVertShader; //Single vertex shader used
    void initShaders();

protected:
    virtual void resizeGL(int width, int height);
};

#endif //RENDER_VIEW_WIDGET_H
