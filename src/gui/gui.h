#ifndef QT_GUI_H
#define QT_GUI_H

#include <qwidget.h>
//--
#include "gui/StatsPanel.h"
#include "LayoutConstants.h"
#include "gui/RenderViewWidget.h"

class GUI : public QWidget{
    Q_OBJECT
public:
    GUI(QWidget* parent = 0);
    virtual ~GUI();

public:
    StatsPanel* stats;
    RenderViewWidget* sceneView;
};


#endif //QT_GUI_H
