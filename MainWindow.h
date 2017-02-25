
// ======================================================================
// IMPROC: Image Processing Software Package
// Copyright (C) 2015 by George Wolberg
//
// MainWindow.h - Main Window widget class
//
// Written by: George Wolberg, 2015
// ======================================================================

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "coverflow.h"
#include <QtWidgets>
#include <QMediaPlayer>
#include <QDialog>
#include <QPushButton>
#include <QOpenGLWidget>
#include <QtWidgets>
#include <QDialog>
#include <QFileInfo>
#include <QWidget>
#include <QMediaPlayer>
#include <QMediaObject>
#include <QMediaMetaData>
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
#include <QAudioRecorder>
#include <QAudioEncoderSettings>
#include <QAudioProbe>

///////////////////////////////////////////////////////////////////////////////
///
/// \class MainWindow
/// \brief IMPROC main window.
///
/// The IMPROC main window consists of three main areas: a tree view
/// in the top-left corner, an information view directly below the
/// tree view, and a frame array that takes up the entire right side
/// of the screen.
///
///////////////////////////////////////////////////////////////////////////////

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    //! Constructor.
    MainWindow(QString);

    //! Destructor.
    ~MainWindow();

    //Public Function
    static void sentfromOpengl(QString);
    void traverseDirs (QString);
    static void sendtoOpengl();
    static void empty();
    void s_load2  ();
    void keyPressEvent(QKeyEvent *e);

public slots:
    // slots
    void s_visualizer();
    void s_load   ();
    void s_panel1(QListWidgetItem*);
    void s_panel2(QListWidgetItem*);
    void s_panel3(QListWidgetItem*);
    void s_panel4(QListWidgetItem*);
    void s_play  (QTableWidgetItem*);
    void s_about ();
    void s_pause_play();
    void s_next();
    void s_previous();
    void s_counter();
    void s_coverflow();


private:
    void createActions();
    void createMenus  ();
    void createWidgets();
    void createLayouts();
    void initLists	  ();
    void redrawLists  (QListWidgetItem *, int);
    void redrawLists2 (QString , int);
    void setSizes	  (QSplitter *, int, int, int);
    void getCoverArt(char*);
//    void keyPressEvent(QKeyEvent *e);

    //private variable
    MainWindow *dialog;

    // actions
    QAction		*m_loadAction;
    QAction		*m_quitAction;
    QAction		*m_aboutAction;
    QAction     *m_findAction;

    // menus
    QMenu		*m_fileMenu;
    QMenu		*m_helpMenu;
    QMenu       *m_musicMenu;

    //Splitter
    QSplitter	*m_mainSplit;
    QSplitter	*m_leftSplit;
    QSplitter	*m_rightSplit;
    QSplitter   *m_OpenGLSplit;
    QSplitter	*m_visualizerSplit;
    QSplitter	*m_coverflowSplit;
    //List Widget
    QListWidget *m_panel2;
    QListWidget *m_panel[3];

    //Table Widget
    QTableWidget	*m_table;

    //Progress
    QProgressDialog	*m_progressBar;

    //Line Edit
    QLineEdit *m_lineEdit2;

    //String
    QString m_directory;
    QString m_findText;

    //Slider
    QSlider *m_slider;

    //Label
    QLabel *m_cover;
    QLabel *m_time;
    QLabel *m_playingsong;
    QLabel *m_labelSide[2];
    QLabel *m_label[3];
    QLabel *m_searchlabel;
    QLabel *m_searchlabel2;

    //Push Buttons
    QPushButton	*m_stop;
    QPushButton *m_play_pause;
    QPushButton *m_previous;
    QPushButton *m_next;
    QPushButton *m_coverflow;
    QPushButton *m_visualizer;
    //Timer
    QTimer      *m_timer;

    // string lists
    QStringList	   m_listGenre;
    QStringList	   m_listArtist;
    QStringList	   m_listAlbum;
    QList<QStringList> m_listSongs;

    //Global Variable
    int load_control=1;
};

#endif //MAINWINDOW_H
