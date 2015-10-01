#include "glwidget.h"
#include "mainwidget.h"

#include <QVBoxLayout>

#include <algorithm>
#include <fstream>

MainWidget::MainWidget(QWidget* parent)
    : QWidget(parent)
    , m_glWidget(new GLWidget(this))
    , m_frameExtractor(*m_glWidget, "/Users/daniel/Movies/20150909_111119.mp4")
{
    m_glWidget->setFocusPolicy(Qt::StrongFocus);
    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(m_glWidget);
    setLayout(layout);
    resize(640, 480);
}

MainWidget::~MainWidget()
{
}
