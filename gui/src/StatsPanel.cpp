#include "StatsPanel.h"
//--
#include <QLabel>
#include <QHBoxLayout>
#include <QProgressBar>
//--
#include "LayoutConstants.h"


StatsPanel::StatsPanel(QWidget* parent) : 
    QWidget(parent)
{
    QHBoxLayout* layout = new QHBoxLayout();

    QProgressBar* pbar = new QProgressBar(this);
    pbar->setRange(0,100);
    layout->addWidget(pbar);

    QLabel* rps = new QLabel(this);
    rps->setText("X rays/second");
    layout->addWidget(rps);

    setLayout(layout);
}



