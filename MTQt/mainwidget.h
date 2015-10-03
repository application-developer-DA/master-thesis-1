#pragma once

#include "frameextractor.h"

#include <QWidget>

#include <memory>


class GLWidget;

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget* parent = nullptr);
    ~MainWidget();

private:
    GLWidget* m_glWidget;
    std::unique_ptr<FrameExtractor> m_frameExtractor;
};

