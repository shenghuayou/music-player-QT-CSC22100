#ifndef VISUALIZER
#define VISUALIZER

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <time.h>
#include <stdlib.h>


class Visualizer : public QOpenGLWidget
{
    Q_OBJECT

protected slots:
    void paintGL() Q_DECL_OVERRIDE;
    void initializeGL() Q_DECL_OVERRIDE;
    void resizeGL(int width, int height) Q_DECL_OVERRIDE;

public:
    ~Visualizer();
    explicit Visualizer(QWidget *parent = 0);

    //Public Functions
    void DrawRectangle(float);
    void DrawRectangles();

private:
    int height;
};

#endif // VISUALIZER

