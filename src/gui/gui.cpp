#include "gui/gui.h"
//--
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QToolBox>

static const char* WINDOW_TITLE = "Colin's Renderer";

GUI::~GUI(){
    delete stats; stats = NULL;
    delete sceneView; sceneView = NULL;
}

GUI::GUI(QWidget* parent) : 
    QWidget(parent), stats(NULL), sceneView(NULL)
{
    //Create each component
    stats     = new StatsPanel(this);
    sceneView = new RenderViewWidget(this);

    //TODO: Make real stuff here
    QPushButton* button1 = new QPushButton(this); button1->setText("Re-Render");
    connect(button1, SIGNAL(clicked()), sceneView, SLOT(draw()));

    QPushButton* button2 = new QPushButton(this); button2->setText("Button 2");
    QPushButton* button3 = new QPushButton(this); button3->setText("Button 3");
    QPushButton* button4 = new QPushButton(this); button4->setText("Button 4");
    QPushButton* button5 = new QPushButton(this); button5->setText("Button 5");

    QToolBox* tb = new QToolBox(this);
    tb->addItem(button1, "Rendering");
    tb->insertItem(0, button2, "Rendering 2");
    tb->addItem(button3, "Tone Mapping");
    tb->addItem(button4, "Tone Mapping");
    tb->addItem(button5, "Parallelization");

    //Left layout = vertical box layout
    //upper component = view panel
    //lower component = statstics
    QVBoxLayout* leftLayout = new QVBoxLayout();
    leftLayout->addWidget(sceneView, 100); //OpenGL viewer
    leftLayout->addWidget(stats);     //Stats Panel

    //Right layout = Vertical layout with a bunch of controls
    QVBoxLayout* controlsLayout = new QVBoxLayout();
    controlsLayout->addWidget(tb);

    //Main layout is a vbox layout
    QHBoxLayout* mainLayout = new QHBoxLayout();
    mainLayout->addLayout(leftLayout, 100);
    mainLayout->addLayout(controlsLayout);
    setLayout(mainLayout);

    setWindowTitle(WINDOW_TITLE);

}
