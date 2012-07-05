#include <iostream>
//--
#include <qapplication.h>
//--
#include "gui/gui.h"
//--
#include "imageio/ImageTypes.h"
#include "utils/Assert.h"
#include "imageio/ImageIO.h"

int main(int argc, char** argv){
    QApplication app(argc, argv);
    GUI gui;

    //Initially draw the scene once
    QMetaObject::invokeMethod(gui.sceneView, "draw", Qt::QueuedConnection);
    gui.show();
    return app.exec();
}
