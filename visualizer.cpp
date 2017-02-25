
#include "visualizer.h"
#include "MainWindow.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Visualizer::Visualizer
//
// Contructor
//
Visualizer::Visualizer(QWidget *parent)
    : QOpenGLWidget(parent)
{
    srand(time(NULL));
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Visualizer::~Visualizer
//
// Deconstructor
//
Visualizer::~Visualizer()
{

}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Visualizer::paintGL()
//
// Protected function from OpenGL to draw
//
void Visualizer::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    //Call DrawRectangles()
    DrawRectangles();
    update();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Visualizer::initializerGL()
//
// Protected function from OpenGl
//
void Visualizer::initializeGL()
{

}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Visualizer::resizeGL()
//
// Protected function from resizeGL
//
void Visualizer::resizeGL(int width, int height)
{

    glViewport(0, 0, width, height);
    glLoadIdentity();

}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Visualizer::DrawRectangles()
//
// Function to draw each column of the visualizer and
// use the RNG the give each column a certain height
//
void Visualizer::DrawRectangles()
{
    float v1 = (rand()%100)/10;
    float v2 = (rand()%100)/10;
    float v3 = (rand()%100)/10;
    float v4 = (rand()%100)/10;
    float v5 = (rand()%100)/10;
    float v6 = (rand()%100)/10;
    float v7 = (rand()%100)/10;
    float v8 = (rand()%100)/10;
    float v9 = (rand()%100)/10;
    float v10 = (rand()%100)/10;
    float v11 = (rand()%100)/10;
    float v12 = (rand()%100)/10;

    glTranslatef(-0.8,-0.5,0);

    //1st bar - All the way on the left
    glPushMatrix();
    DrawRectangle(v1/10);
    glPopMatrix();

    glTranslatef(0.15,0,0);

    //2nd bar
    glPushMatrix();
    DrawRectangle(v2/10);
    glPopMatrix();

    glTranslatef(0.15,0,0);

    //3rd bar
    glPushMatrix();
    DrawRectangle(v3/10);
    glPopMatrix();

    glTranslatef(0.15,0,0);

    //4th bar
    glPushMatrix();
    DrawRectangle(v4/10);
    glPopMatrix();

    glTranslatef(0.15,0,0);

    //5th bar
    glPushMatrix();
    DrawRectangle(v5/10);
    glPopMatrix();

    glTranslatef(0.15,0,0);

    //6th bar
    glPushMatrix();
    DrawRectangle(v6/10);
    glPopMatrix();

    glTranslatef(0.15,0,0);

    //7th bar
    glPushMatrix();
    DrawRectangle(v7/10);
    glPopMatrix();

    glTranslatef(0.15,0,0);

    //8th bar
    glPushMatrix();
    DrawRectangle(v8/10);
    glPopMatrix();

    glTranslatef(0.15,0,0);

    //9th bar
    glPushMatrix();
    DrawRectangle(v9/10);
    glPopMatrix();

    glTranslatef(0.15,0,0);

    //10th bar
    glPushMatrix();
    DrawRectangle(v10/10);
    glPopMatrix();

    glTranslatef(0.15,0,0);

    //11th bar
    glPushMatrix();
    DrawRectangle(v11/10);
    glPopMatrix();

    glTranslatef(0.15,0,0);

    //12th bar
    glPushMatrix();
    DrawRectangle(v12/10);
    glPopMatrix();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Visualizer::DrawRectangle()
//
// Draw one rectangle
//
void Visualizer::DrawRectangle(float health)
{
    float barHeight = 1.0/(float)15;
    glBegin(GL_QUADS);
    int theme_selector = rand()%10;
    if (theme_selector <= 5)
    {
        for (float i = 0; i < health; i += (barHeight))
        {
            glColor3f(0,0,0);
            glVertex2f(-0.05,i);
            glVertex2f(0.05,i);
            glColor3f(1,1,1);
            glVertex2f(0.05,i+barHeight);
            glVertex2f(-0.05,i+barHeight);
        }
        glEnd();
    }
    else if (theme_selector > 5)
    {
        for (float i = 0; i < health; i += (barHeight))
        {
            glColor3f(1,0,0);
            glVertex2f(-0.05,i);
            glVertex2f(0.05,i);
            glColor3f(0,0,0);
            glVertex2f(0.05,i+barHeight);
            glVertex2f(-0.05,i+barHeight);
        }
        glEnd();
    }
}
