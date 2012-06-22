#include "gui/RenderViewWidget.h"
//--
#include "imageio/ImageTypes.h"
#include "utils/Assert.h"
//--
#include <iostream>


void RenderViewWidget::initShaders(){

    Assert(QGLContext::currentContext() != 0);

    //First, check if we can even use OpenGL shader programs
    const bool canUseShaders = QGLShaderProgram::hasOpenGLShaderPrograms();
    Assert(canUseShaders);

    //Fragment shader(s)
    QGLShader* shader = new QGLShader(QGLShader::Fragment, this);
    const bool ok1 = shader->compileSourceCode(
        "//Simple Vertex Shader" \
        "\n"\
        "uniform sampler2D sampler;\n"\
        "\n"\
        "void main(void)\n" \
        "{\n" \
        "//    gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n" \
        "\n"\
        "    vec4 c = texture2D(sampler, gl_TexCoord[0].st);\n" \
        "    gl_FragColor = vec4(c.x, c.y, c.z, 1.0);\n"\
        "}\n" \
    );
    std::cout << shader->log().toStdString() << std::endl;
    Assert(ok1);
    hdrFragShaders.push_back(shader);


    //Vertex shader
    hdrVertShader = new QGLShader(QGLShader::Vertex, this);
    const bool ok2 = hdrVertShader->compileSourceCode(
        "//Simple fragment shader\n"\
        "\n"\
        "\n"\
        "void main(void){\n"\
        "    gl_TexCoord[0] = gl_MultiTexCoord0;\n"\
        "    gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;\n"\
        "}\n"\
        "\n"\
    );
    std::cout << hdrVertShader->log().toStdString() << std::endl;
    Assert(ok2);
    hdrFragShaders.push_back(hdrVertShader);
}


void makeIm(HDRImage* im){
    im->fill(0.0f);

    for(int x = 0; x < im->width(); x++){
        for(int y = 0; y < im->height(); y++){
            float* r = (*im)(x,y,0);
            float* g = (*im)(x,y,1);
            float* b = (*im)(x,y,2);

            if(x == y){
                *r = 1.0f;
                *g = 0.0f;
                *b = 0.0f;
            }else{
                float pv = (((float)x) / ((float)im->width())) * 2.0f * 3.14159f;
                *r = sinf(pv);
                *g = cosf(pv * 0.5f);
                *b = tanf(pv * 0.1f);

                *r = std::max<float>(*r, 0.0f);
                *g = std::max<float>(*g, 0.0f);
                *b = std::max<float>(*b, 0.0f);
                *r = *b = *g = 0.0f;
            }
        }
    }
}


void RenderViewWidget::resizeGL(int width, int height){
    std::cout << "Resize w,h = (" << width << ", " << height << ")" << std::endl;
    w = width;
    h = height;
    draw();
}

RenderViewWidget::RenderViewWidget(QWidget *parent) :
    QGLWidget(
        QGLFormat(QGL::SampleBuffers | QGL::AlphaChannel),
        parent
    ),
    hdrShaderIdx(0)
{
    makeCurrent();
    initShaders();

    //Make sure we have what we need...
    if(! QGLFramebufferObject::hasOpenGLFramebufferBlit()){
        std::cerr << "Missing OpenGL Blit!" << std::endl;
        exit(1);
    }

    //Setup opengl settings
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    //TODO: Turn OFF depth test!
    //TODO: What frame buffer size?
    texture_fbo = new QGLFramebufferObject(512, 512);
}

RenderViewWidget::~RenderViewWidget(){
    delete texture_fbo; texture_fbo = NULL;
    /*
    for(size_t i = 0; i < hdrFragShaders.size(); i++){
        delete hdrFragShaders[i]; hdrFragShaders[i] = NULL;
    }
    delete hdrVertShader; hdrVertShader = NULL;
    */
}


void RenderViewWidget::draw(){

    //Setup shaders
    Assert(hdrShaderIdx >= 0 && ((size_t)hdrShaderIdx) < hdrFragShaders.size());
//    QGLFormat format;
//    QGLContext context(format);
//    QGLShaderProgram program(&context);
    QGLShaderProgram program(QGLContext::currentContext());
    program.addShader( hdrFragShaders[hdrShaderIdx] );
    program.link();
    program.bind();


    //Draw

    QPainter p(this);
    saveGLState();

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    //Texture setup
    glBindTexture(GL_TEXTURE_2D, texture_fbo->texture() );
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_MULTISAMPLE);


    //TODO: HERE IS WHERE WE SHOULD GET THE IMAGE BUFFER
    int wim = 128;
    int him = 128;
    HDRImage* im = new HDRImage(wim,him);
    makeIm(im);
    float* buf = im->getPixelBuf();
    glTexImage2D(GL_TEXTURE_2D, 0,
        GL_RGB, //Internal format
        wim,
        him,
        0,
        GL_RGB,
        GL_FLOAT,
        (void*) buf
    );
    delete im;




    //Full screen quad code from here:
    //http://www.michimichbeck.de/wordpress/tag/full-screen-quad/
    const float ww = 1;
    const float hh = 1;
    const float z = -1;

    glRasterPos2i(0,0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1,1,-1,1, 1.0, 40.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0,0,w,h);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBegin (GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-ww, -hh, z);


    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(ww, -hh, z);


    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(ww, hh , z);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-ww, hh, z );
    glEnd();


    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    restoreGLState();
}


void RenderViewWidget::saveGLState(){
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
}

void RenderViewWidget::restoreGLState(){
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glPopAttrib();
}

void RenderViewWidget::paintEvent(QPaintEvent* pe){}
void RenderViewWidget::mousePressEvent(QMouseEvent* me){}
void RenderViewWidget::mouseDoubleClickEvent(QMouseEvent* me){}
void RenderViewWidget::mouseMoveEvent(QMouseEvent* me){}
void RenderViewWidget::timerEvent(QTimerEvent* te){}
void RenderViewWidget::wheelEvent(QWheelEvent* we){}



