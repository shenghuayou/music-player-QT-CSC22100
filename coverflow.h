#ifndef COVERFLOW_H
#define COVERFLOW_H

#include "MainWindow.h"
#include <QPushButton>
#include <QOpenGLWidget>
#include <QtWidgets>
#include <QDialog>
#include <QFileInfo>
#include <QImage>
#include <QGLWidget>
#include <QOpenGLFunctions>
#include <iostream>
#include <fstream>
#include <cmath>
#include <cassert>
#include <cstring>
#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/fileref.h>
#include <QProgressBar>
#include <taglib/mpeg/id3v2/id3v2tag.h>
#include <taglib/mpeg/mpegfile.h>
#include <taglib/mpeg/id3v2/id3v2frame.h>
#include <taglib/mpeg/id3v2/id3v2header.h>
#include <taglib/mpeg/id3v2/frames/attachedpictureframe.h>
#include <cstdio>
#include <string.h>
#include <fstream>
#include <QImage>
#include <QtWidgets>
#include <QDebug>
#include <glu.h>

class QOpenGLContext;
class MyGlWidget : public QOpenGLWidget
{
    Q_OBJECT

    typedef struct {
        int		width;
        int		height;
        GLuint		texId;
        char		imageFilename[512];
    } Record;


protected slots:
    void paintGL() Q_DECL_OVERRIDE;
    void initializeGL() Q_DECL_OVERRIDE;
    void resizeGL(int width, int height) Q_DECL_OVERRIDE;

public:
    explicit MyGlWidget(QWidget *parent = 0);
    ~MyGlWidget();

    //Public Function
    static void MoveRight();
    static void MoveLeft();
    void empty();
    void initRecords	();
    void drawRecord		(Record *record);
    void drawRecords	();
    void quit		();
    void traverseDirs (QString, int);
    int  readMP3		(char*, int&, int&, unsigned char *&);
    void s_load();

    //String
    QString m_directory;

    //Progress Dialog
    QProgressDialog	*m_progressBar;

    //FileInfoList
    QFileInfoList listDirs ;

    //Structure
    Record  *m_records;
    Record  *tmp_records;

};

#endif // COVERFLOW_H

