
//MainWindow.cpp
#include "MainWindow.h"
#include "coverflow.h"
#include "visualizer.h"
using namespace std;


enum {TITLE, TRACK, TIME, ARTIST, ALBUM, GENRE, PATH};
const int COLS = PATH;
bool caseInsensitive(const QString &s1, const QString &s2)
{
    return s1.toLower() < s2.toLower();
}
int m_timevalue;
int m_slidertime=0;
char *cr;
int CurrentSongIndex;
int initLists_counter = 0;
int CoverState=1;
QString mw_ps;
QMediaPlayer *player = new QMediaPlayer;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::MainWindow:
//
// Constructor. Initialize user-interface elements.
//
MainWindow::MainWindow (QString program)
    : m_directory(".")
{
    // setup GUI with actions, menus, widgets, and layouts
    createActions(); // create actions for each menu item
    createMenus  (); // create menus and associate actions
    createWidgets(); // create window widgets
    createLayouts(); // create widget layouts
    s_load2();
    initLists();
    // set main window titlebar
    QString copyright = "Copyright (C) 2015 by George Wolberg";
    QString version   = "Version 1.0";
    QString title   =  QString("%1   (%2)         %3")
            .arg(program).arg(version).arg(copyright);
    setWindowTitle(title);

    // set central widget and default size
    setCentralWidget(m_mainSplit);
    setMinimumSize(400, 300);
    resize(800, 600);
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::~MainWindow:
//
// Destructor. Save settings.
//
MainWindow::~MainWindow() {}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::createActions:
//
// Create actions to associate with menu and toolbar selection.
//
void MainWindow::createActions()
{
    m_loadAction = new QAction("&Load Music Folder", this);
    m_loadAction->setShortcut(tr("Ctrl+L"));
    connect(m_loadAction, SIGNAL(triggered()), this, SLOT(s_load()));
    m_quitAction = new QAction("&Quit", this);
    m_quitAction->setShortcut(tr("Ctrl+Q"));
    connect(m_quitAction, SIGNAL(triggered()), this, SLOT(close()));
    m_aboutAction = new QAction("&About", this);
    m_aboutAction->setShortcut(tr("Ctrl+A"));
    connect(m_aboutAction, SIGNAL(triggered()), this, SLOT(s_about()));

}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::createMenus:
//
// Create menus and install in menubar.
//
void MainWindow::createMenus()
{
    m_fileMenu = menuBar()->addMenu("&File");
    m_fileMenu->addAction(m_loadAction);
    m_fileMenu->addAction(m_quitAction);
    m_helpMenu = menuBar()->addMenu("&Help");
    m_helpMenu->addAction(m_aboutAction);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::createWidgets:
//
// Create widgets to put in the layout.
//
void MainWindow::createWidgets()
{
    // initialize splitters
    m_mainSplit  = new QSplitter(this);
    m_OpenGLSplit = new QSplitter(Qt::Vertical, m_mainSplit);
    m_leftSplit  = new QSplitter(Qt::Vertical, m_mainSplit);
    m_rightSplit = new QSplitter(Qt::Vertical, m_mainSplit);
    m_coverflowSplit = new QSplitter(Qt::Vertical, m_OpenGLSplit);
    m_visualizerSplit =new QSplitter(Qt::Vertical, m_OpenGLSplit);

    // init labels on left side of main splitter
    for(int i=0; i<1; i++)
    {
        m_labelSide[i] = new QLabel(QString("Label%1").arg(i));
        m_labelSide[i]->setAlignment(Qt::AlignCenter);
    }


    // initialize label on right side of main splitter
    for(int i=0; i<3; i++)
    {
        // make label widget with centered text and sunken panels
        m_label[i] = new QLabel;
        m_label[i]->setAlignment(Qt::AlignCenter);
        m_label[i]->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    }

    // initialize label text
    m_label[0]->setText("<b>Genre<\b>" );
    m_label[1]->setText("<b>Artist<\b>");
    m_label[2]->setText("<b>Album<\b>" );

    // initialize list widgets: genre, artist, album
    for(int i=0; i<3; i++)
        m_panel[i] = new QListWidget;


    // initialize table widget: complete song data
    m_table = new QTableWidget(0, COLS);
    QHeaderView *header = new QHeaderView(Qt::Horizontal,m_table);
    m_table->setHorizontalHeader(header);
    m_table->setHorizontalHeaderLabels(QStringList() <<
                                       "Title" << "Track" << "Time" << "Artist" << "Album" << "Genre");
    m_table->setAlternatingRowColors(1);
    m_table->setShowGrid(1);
    m_table->setEditTriggers (QAbstractItemView::NoEditTriggers);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);

    m_searchlabel = new QLabel(tr("Search By:"));
    m_searchlabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    m_searchlabel2 = new QLabel(tr("Specify:"));
    m_searchlabel2->setFrameStyle(QFrame::Panel | QFrame::Sunken);

    m_panel2 = new QListWidget;
    m_lineEdit2 = new QLineEdit;


    // add widgets to the splitters
    m_play_pause = new QPushButton;
    m_previous = new QPushButton;
    m_next = new QPushButton;
    m_coverflow= new QPushButton("coverflow");
    m_visualizer= new QPushButton("visualizer");
    //setup icons
    QPixmap pixplay("/Users/youshenghua/Desktop/musicplayer/Icons/play.png");
    QPixmap pixnext("/Users/youshenghua/Desktop/musicplayer/Icons/next.png");
    QPixmap pixprevious("/Users/youshenghua/Desktop/musicplayer/Icons/previous.png");
    //QPixmap pixplay("/home/vfung000/Desktop/musicplayer/Icons/play.png");
    //QPixmap pixnext("/home/vfung000/Desktop/musicplayer/Icons/next.png");
    //QPixmap pixprevious("/home/vfung000/Desktop/musicplayer/Icons/previous.png");
    //QPixmap pixplay("/home/fioger/Desktop/musicplayer/Icons/play.png");
    //QPixmap pixnext("/home/fioger/Desktop/musicplayer/Icons/next.png");
    //QPixmap pixprevious("/home/fioger/Desktop/musicplayer/Icons/previous.png");

    m_play_pause->setIcon(pixplay);
    m_play_pause->setIconSize(pixplay.rect().size());
    m_play_pause->setFixedSize(pixplay.rect().size());

    m_next->setIcon(pixnext);
    m_next->setIconSize(pixnext.rect().size());
    m_next->setFixedSize(pixnext.rect().size());

    m_previous->setIcon(pixprevious);
    m_previous->setIconSize(pixprevious.rect().size());
    m_previous->setFixedSize(pixprevious.rect().size());

    //setup cover label
    m_cover= new QLabel();
    m_cover->setMaximumSize(130,130);
    m_cover->setMinimumSize(130,130);

    //create layouts
    m_time= new QLabel(QString("0:00"));
    m_playingsong= new QLabel(QString("         "));
    m_slider= new QSlider(Qt::Horizontal);
    m_slider->setFixedWidth(140);

    //setup timer
    m_timer=new QTimer(this);
    m_timer->start(1000);

    // init signal/slot connections
    connect(m_panel[0], SIGNAL(itemClicked(QListWidgetItem*)),
            this,    SLOT(s_panel1   (QListWidgetItem*)));
    connect(m_panel[1], SIGNAL(itemClicked(QListWidgetItem*)),
            this,    SLOT(s_panel2   (QListWidgetItem*)));
    connect(m_panel[2], SIGNAL(itemClicked(QListWidgetItem*)),
            this,    SLOT(s_panel3   (QListWidgetItem*)));
    connect(m_table, SIGNAL(itemDoubleClicked(QTableWidgetItem*)),
            this,    SLOT(s_play   (QTableWidgetItem*)));
    connect(m_panel2, SIGNAL(itemClicked(QListWidgetItem*)),
            this, SLOT(s_panel4 (QListWidgetItem*)));
    connect(m_play_pause,SIGNAL(clicked()),
            this, SLOT(s_pause_play()));
    connect(m_next,SIGNAL(clicked()),
            this, SLOT(s_next()));
    connect(m_previous,SIGNAL(clicked()),
            this, SLOT(s_previous()));
    connect(m_slider,SIGNAL(valueChanged(int)),
            m_time,SLOT(setNum(int)));
    connect(m_coverflow,SIGNAL(clicked()),
            this,SLOT(s_coverflow()));
    connect(m_visualizer,SIGNAL(clicked()),
            this,SLOT(s_visualizer()));
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::createLayouts:
//
// Create layouts for widgets.
//
void MainWindow::createLayouts()
{
    // create a 2-row, 3-column grid layout, where row0 and
    // row1 consist of labels and list widgets, respectively.

    QWidget     *widget1 = new QWidget(this);
    QWidget     *widget2 = new QWidget(this);
    QWidget     *widget3 = new QWidget(this);
    QWidget     *widget4 = new QWidget(this);

    QGridLayout *upperrightgrid   = new QGridLayout(widget1);
    upperrightgrid->addWidget(m_label[0], 0, 0);
    upperrightgrid->addWidget(m_label[1], 0, 1);
    upperrightgrid->addWidget(m_label[2], 0, 2);
    upperrightgrid->addWidget(m_panel[0], 1, 0);
    upperrightgrid->addWidget(m_panel[1], 1, 1);
    upperrightgrid->addWidget(m_panel[2], 1, 2);

    QGridLayout *lowerrightgrid   = new QGridLayout(widget2);
    lowerrightgrid->addWidget(m_table, 0, 0);

    QGridLayout *upperleftgrid = new QGridLayout(widget3);
    upperleftgrid->addWidget(m_playingsong, 0, 0);
    upperleftgrid->addWidget(m_play_pause, 0, 2);
    upperleftgrid->addWidget(m_next, 0, 3);
    upperleftgrid->addWidget(m_previous, 0, 1);
    upperleftgrid->addWidget(m_time, 1, 0);
    upperleftgrid->addWidget(m_cover, 3, 0);
    upperleftgrid->addWidget(m_coverflow,4,0);
    upperleftgrid->addWidget(m_visualizer,5,0);
    upperleftgrid->addWidget(m_slider, 2, 0);

    QGridLayout *lowerleftgrid = new QGridLayout(widget4);
    lowerleftgrid->addWidget(m_searchlabel, 0, 0);
    lowerleftgrid->addWidget(m_panel2, 0, 1);
    lowerleftgrid->addWidget(m_searchlabel2, 1, 0);
    lowerleftgrid->addWidget(m_lineEdit2, 1, 1);

    m_leftSplit ->addWidget(widget3 );
    m_leftSplit ->addWidget(widget4 );
    m_rightSplit->addWidget(widget1 );
    m_rightSplit->addWidget(widget2 );

    m_OpenGLSplit->setHidden(true);

}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::initLists:
//
// Populate lists with data (first time).
//
void MainWindow::initLists()
{
    // error checking
    if(m_listSongs.isEmpty()) return;

    if (initLists_counter == 0)
    {
        //TITLE, TRACK, TIME, ARTIST, ALBUM, GENRE
        for (int i = 0; i < 1; i++)
        {
            if (i == 0)
            {
                m_panel2->addItem("Title");
                m_panel2->addItem("Track");
                m_panel2->addItem("Time");
                m_panel2->addItem("Artist");
                m_panel2->addItem("Album");
                m_panel2->addItem("Genre");
            }
            else
                break;
        }
        initLists_counter++;
    }

    // create separate lists for genres, artists, and albums
    for(int i=0; i<m_listSongs.size(); i++)
    {
        m_listGenre  << m_listSongs[i][GENRE ];
        m_listArtist << m_listSongs[i][ARTIST];
        m_listAlbum << m_listSongs[i][ALBUM];
    }

    // sort each list
    qStableSort(m_listGenre .begin(), m_listGenre .end(), caseInsensitive);
    qStableSort(m_listArtist.begin(), m_listArtist.end(), caseInsensitive);
    qStableSort(m_listAlbum .begin(), m_listAlbum .end(), caseInsensitive);

    // add each list to list widgets, filtering out repeated strings
    for(int i=0; i<m_listGenre.size(); i+=m_listGenre.count(m_listGenre[i]))
        m_panel[0]->addItem(m_listGenre [i]);
    for(int i=0; i<m_listArtist.size(); i+=m_listArtist.count(m_listArtist[i]))
        m_panel[1]->addItem(m_listArtist[i]);
    for(int i=0; i<m_listAlbum.size(); i+=m_listAlbum.count(m_listAlbum[i]))
        m_panel[2]->addItem(m_listAlbum[i]);
    // copy data to table widget
    QTableWidgetItem *item[COLS];
    for(int i=0; i<m_listSongs.size(); i++)
    {
        m_table->insertRow(i);

        for(int j=0; j<COLS; j++)
        {
            item[j] = new QTableWidgetItem;
            item[j]->setText(m_listSongs[i][j]);
            item[j]->setTextAlignment(Qt::AlignCenter);
            m_table->setItem(i, j, item[j]);
        }
    }

}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::redrawLists:
//
// Re-populate lists with data matching item's text in field x.
//

void MainWindow::redrawLists(QListWidgetItem *listItem, int x)
{
    m_table->setRowCount(0);
    // copy data to table widget
    for(int i=0,row=0; i<m_listSongs.size(); i++)
    {

        // skip rows whose field doesn't match text
        if(m_listSongs[i][x] != listItem->text())
            continue;
        if(m_listSongs[i][x] == listItem->text())
        {
            m_table->insertRow(row);
            QTableWidgetItem *item[COLS];
            for(int j=0; j<COLS; j++)
            {
                item[j] = new QTableWidgetItem;
                item[j]->setText(m_listSongs[i][j]);
                item[j]->setTextAlignment(Qt::AlignCenter);
                m_table->setItem(row,j,item[j]);
            }
            // increment table row index (row <= i)
            row++;
        }
    }
}

void MainWindow::redrawLists2(QString listItem, int x)
{
    m_table->setRowCount(0);
    // copy data to table widget
    for(int i=0,row=0; i<m_listSongs.size(); i++)
    {
        QString tableElements = m_listSongs[i][x];
        // skip rows whose field doesn't match text
        if(tableElements.contains(listItem, Qt::CaseInsensitive))
        {
            m_table->insertRow(row);
            QTableWidgetItem *item[COLS];
            for(int j=0; j<COLS; j++)
            {
                item[j] = new QTableWidgetItem;
                item[j]->setText(m_listSongs[i][j]);
                item[j]->setTextAlignment(Qt::AlignCenter);
                m_table->setItem(row,j,item[j]);
            }
            // increment table row index (row <= i)
            row++;
        }
        else
            continue;
    }
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::traverseDirs:
//
// Traverse all subdirectories and collect filenames into m_listSongs.
//
void MainWindow::traverseDirs(QString path)
{
    QString  key, val;
    QStringList list;

    // init listDirs with subdirectories of path
    QDir dir(path);
    dir.setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);
    QFileInfoList listDirs = dir.entryInfoList();

    // init listFiles with all *.mp3 files in path
    QDir files(path);
    files.setFilter(QDir::Files);
    files.setNameFilters(QStringList("*.mp3"));
    QFileInfoList listFiles = files.entryInfoList();
    m_progressBar->setMaximum(listFiles.size());

    for(int i=0; i < listFiles.size(); i++)
    {
        // init list with default values: ""
        for(int j=0; j<=COLS; j++)
            list.insert(j,"");


        // store file pathname into 0th position in list
        QFileInfo fileInfo = listFiles.at(i);
        list.replace(PATH, fileInfo.filePath());
        QByteArray QBA= fileInfo.filePath().toLatin1();
        cr=QBA.data();
        TagLib::FileName Tpath(cr);
        TagLib::FileRef Tref(Tpath);

        // process all song tags
        for(int j=0; j<COLS; j++)
        {
            // get tag key and value
            if(j==TITLE)
            {
                key = "title";
                val = Tref.tag()->title().toCString();
            }

            else if(j==GENRE)
            {
                key ="genre";
                val=Tref.tag()->genre().toCString();
            }

            else if(j==ARTIST)
            {
                key="artist";
                val=Tref.tag()->artist().toCString();
            }

            else if(j==TIME)
            {
                key="time";
                int second=Tref.audioProperties()->length();
                int minute=second/60;
                int leftSecond=second%60;
                if(leftSecond<10)
                    val=QString::number(minute)+":0"+QString::number(leftSecond);

                else
                    val=QString::number(minute)+":"+QString::number(leftSecond);
            }

            else if(j==TRACK)
            {
                key="track";
                val=QString::number(Tref.tag()->track());
            }

            else if(j==ALBUM)
            {
                key="album";
                val=Tref.tag()->album().toCString();
            }

            if(val.isEmpty()  ) val = "Unknown";

            // store tag value in proper position in list
            if(key == "genre" ) list.replace(GENRE , val);
            if(key == "artist") list.replace(ARTIST, val);
            if(key == "album" ) list.replace(ALBUM , val);
            if(key == "title" ) list.replace(TITLE , val);
            if(key == "track" ) list.replace(TRACK , val);
            if(key == "time"  ) list.replace(TIME  , val);
        }

        // append list (song data) into songlist m_listSongs;
        // uninitialized fields are empty strings
        m_listSongs << list;
    }
    // recursively descend through all subdirectories
    for(int i=0; i<listDirs.size(); i++)
    {
        QFileInfo fileInfo = listDirs.at(i);
        traverseDirs( fileInfo.filePath() );
    }
    return;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::sentfromOpengl
//
// Gets the path from OpenGL, coverflow.cpp
//
void MainWindow::sentfromOpengl(QString path)
{
    mw_ps = path;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::empty()
//
// Empty function
//
void MainWindow::empty()
{

}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::s_load:
//
// Slot function for File|Load
//

void MainWindow::s_load()
{
    //create the coverflow widget and visualizer widget
    if(load_control==1)
    {
        //Create GL Widget
        QWidget     *currentGLWidget = new MyGlWidget;
        QWidget     *currentGLWidget2 = new Visualizer;
        QWidget     *widget5 = new QWidget(this);
        QWidget     *widget6 = new QWidget(this);

        //Place the GL Widgets in the respective splitters
        QGridLayout *covergrid = new QGridLayout(widget5);
        covergrid->addWidget(currentGLWidget);
        QGridLayout *visualgrid = new QGridLayout(widget6);
        visualgrid->addWidget(currentGLWidget2);
        m_coverflowSplit->addWidget(widget5 );
        m_visualizerSplit->addWidget(widget6 );

        //hide the coversplitters
        m_OpenGLSplit->setHidden(true);

        //Call s_load2()
        s_load2();

        load_control++;
    }
    else
    {
        MyGlWidget o;
        s_load2();
    }
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::s_load2()
//
// Slot function for File|load
//
void MainWindow::s_load2()
{
    m_directory = mw_ps;
    // init progress bar
    m_progressBar = new QProgressDialog(this);
    m_progressBar->setWindowTitle("Updating");
    m_progressBar->setFixedSize(300,100);
    m_progressBar->setCancelButtonText("Cancel");
    traverseDirs(m_directory);
    initLists();
    m_progressBar->close();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::s_panel1:
//
// Slot function to adjust data if an item in panel1 (genre) is selected.
//
void MainWindow::s_panel1(QListWidgetItem *item)
{
    // sort remaining two panels for artists and albums
    qStableSort(m_listArtist.begin(), m_listArtist.end(), caseInsensitive);
    qStableSort(m_listAlbum .begin(), m_listAlbum .end(), caseInsensitive);
    redrawLists(item, GENRE);\
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::s_panel2:
//
// Slot function to adjust data if an item in panel2 (artist) is selected.
//

void MainWindow::s_panel2(QListWidgetItem *item)
{
    // sort remaining panel for albums
    qStableSort(m_listAlbum.begin(), m_listAlbum.end(), caseInsensitive);
    qStableSort(m_listGenre.begin(), m_listGenre.end(), caseInsensitive);
    redrawLists(item, ARTIST);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::s_panel3:
//
// Slot function to adjust data if an item in panel3 (album) is selected.
//
void MainWindow::s_panel3(QListWidgetItem *item)
{
    // sort remaining panel for albums
    qStableSort(m_listArtist.begin(), m_listArtist.end(), caseInsensitive);
    qStableSort(m_listGenre.begin(), m_listGenre.end(), caseInsensitive);
    redrawLists(item, ALBUM);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::s_panel4
//
// Slot functionn to adjust data if an item in panel4 is selected
//
void MainWindow::s_panel4(QListWidgetItem *item)
{
    QString text = m_lineEdit2->text();
    int worth = 0;
    for(int j = 0; j < 5; j++)
    {
        if (item->text() == "Title")
            worth = 0;
        if (item->text() == "Track")
            worth = 1;
        if (item->text() == "Time")
            worth = 2;
        if (item->text() == "Artist")
            worth = 3;
        if (item->text() == "Album")
            worth = 4;
        if (item->text() == "Genre")
            worth = 5;
    }
    redrawLists2(text, worth);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::s_about:
//
// Slot function for Help|About
//
void MainWindow::s_about()
{
    QMessageBox::about(this, "About qTunes",
                       "<center> qTunes 1.0 </center> \
                       <center> by Shenghua You, Victor Fung, Fioger Shahollari, 2015 </center>");
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::s_play:
//
// Slot function to play an mp3 song.
//
//

void MainWindow::s_pause_play()
{
    QPixmap pixplay("/Users/youshenghua/Desktop/musicplayer/Icons/play.png");
    QPixmap pixpause("/Users/youshenghua/Desktop/musicplayer/Icons/pause.png");
    //QPixmap pixplay("/home/vfung000/Desktop/musicplayer/Icons/play.png");
    //QPixmap pixpause("/home/vfung000/Desktop/musicplayer/Icons/pause.png");
    //QPixmap pixplay("/home/fioger/Desktop/musicplayer/Icons/play.png");
    //QPixmap pixpause("/home/fioger/Desktop/musicplayer/Icons/pause.png");
    if(player->state()==QMediaPlayer::PlayingState)
    {
        player->pause();
        m_play_pause->setIcon(pixplay);
        m_play_pause->setIconSize(pixplay.rect().size());
        m_play_pause->setFixedSize(pixplay.rect().size());
        m_timer->stop();
    }
    else if(player->state()==QMediaPlayer::PausedState)
    {
        player->play();
        m_play_pause->setIcon(pixpause);
        m_play_pause->setIconSize(pixpause.rect().size());
        m_play_pause->setFixedSize(pixpause.rect().size());
        m_timer->start();
    }
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::s_previous()
//
// Slot function to play the previous mp3 song in the table
//
void MainWindow::s_previous()
{
    //setup icon,slider time and timer
    m_slidertime=0;
    QPixmap pixpause("/Users/youshenghua/Desktop/musicplayer/Icons/pause.png");
    //QPixmap pixpause("/home/vfung000/Desktop/musicplayer/Icons/pause.png");
    //QPixmap pixpause("/home/fioger/Desktop/musicplayer/Icons/pause.png");
    m_timer->disconnect(m_timer,SIGNAL(timeout()),this,SLOT(s_counter()));
    connect(m_timer,SIGNAL(timeout()),this,SLOT(s_counter()));
    m_timer->start();
    if(player->state()==QMediaPlayer::PausedState)
    {
        m_play_pause->setIcon(pixpause);
        m_play_pause->setIconSize(pixpause.rect().size());
        m_play_pause->setFixedSize(pixpause.rect().size());
        //m_play_pause->move(50,40);
    }

    if(CurrentSongIndex>0)
    {
        player->setMedia(QUrl::fromLocalFile(m_listSongs[CurrentSongIndex-1][PATH].toLatin1().data()));
        player->play();
        CurrentSongIndex--;
    }
    else if(CurrentSongIndex==0)
    {
        player->setMedia(QUrl::fromLocalFile(m_listSongs[m_listSongs.size()-1][PATH].toLatin1().data()));
        player->play();
        CurrentSongIndex=m_listSongs.size()-1;
    }

    //setup title
    m_playingsong->setText(m_listSongs[CurrentSongIndex][TITLE]);

    //get timevalue
    QByteArray QBA= m_listSongs[CurrentSongIndex][PATH].toLatin1();
    cr=QBA.data();
    TagLib::FileName Tpath(cr);
    TagLib::FileRef Tref(Tpath);

    m_slider->setMaximum(Tref.audioProperties()->length());
    m_timevalue=Tref.audioProperties()->length();

    //Get the selected song path and pass it to tglib
    TagLib::MPEG::File mpegFile(cr);
    //Check to make sure that it has an ID3v2 tag
    TagLib::ID3v2::Tag *id3v2tag = mpegFile.ID3v2Tag();
    //Get the list of frames for a specific frame type
    TagLib::ID3v2::FrameList Frame  = id3v2tag->frameList("APIC");
    //Constructs an empty picture frame.
    TagLib::ID3v2::AttachedPictureFrame *PicFrame ;
    if (id3v2tag)
    {
        if (!Frame.isEmpty())
        {
            for(TagLib::ID3v2::FrameList::ConstIterator it = Frame.begin(); it != Frame.end(); ++it)
            {
                //Get the image(front) from the mp3 file
                PicFrame = static_cast<TagLib::ID3v2::AttachedPictureFrame *>(Frame.front()) ;
                //Create a Qimage
                QImage coverimage;
                //Get the data from PicFrame and put it into coverimage
                coverimage.loadFromData((const uchar*)PicFrame->picture().data(),PicFrame->picture().size());
                //Place the image in QImage into m_cover
                m_cover->setPixmap(QPixmap::fromImage(coverimage));
                m_cover->show();
            }
        }
    }

}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::s_next()
//
// Slot function to play the next mp3 song in the table
//
void MainWindow::s_next()
{
    //setup icon,slider time and timer
    m_slidertime=0;
    QPixmap pixpause("/Users/youshenghua/Desktop/musicplayer/Icons/pause.png");
    //QPixmap pixpause("/home/vfung000/Desktop/musicplayer/Icons/pause.png");
    //QPixmap pixpause("/home/fioger/Desktop/musicplayer/Icons/pause.png");
    m_timer->disconnect(m_timer,SIGNAL(timeout()),this,SLOT(s_counter()));
    connect(m_timer,SIGNAL(timeout()),this,SLOT(s_counter()));
    m_timer->start();
    if(player->state()==QMediaPlayer::PausedState)
    {
        m_play_pause->setIcon(pixpause);
        m_play_pause->setIconSize(pixpause.rect().size());
        m_play_pause->setFixedSize(pixpause.rect().size());
    }

    if(CurrentSongIndex<m_listSongs.size()-1)
    {
        player->setMedia(QUrl::fromLocalFile(m_listSongs[CurrentSongIndex+1][PATH].toLatin1().data()));
        player->play();
        CurrentSongIndex++;
    }
    else if(CurrentSongIndex>=m_listSongs.size()-1)
    {
        player->setMedia(QUrl::fromLocalFile(m_listSongs[0][PATH].toLatin1().data()));
        player->play();
        CurrentSongIndex=0;
    }

    //setup title
    m_playingsong->setText(m_listSongs[CurrentSongIndex][TITLE]);
    //get timevalue
    QByteArray QBA= m_listSongs[CurrentSongIndex][PATH].toLatin1();
    cr=QBA.data();
    TagLib::FileName Tpath(cr);
    TagLib::FileRef Tref(Tpath);
    m_slider->setMaximum(Tref.audioProperties()->length());
    m_timevalue=Tref.audioProperties()->length();
    TagLib::MPEG::File mpegFile(cr);
    TagLib::ID3v2::Tag *id3v2tag = mpegFile.ID3v2Tag();
    TagLib::ID3v2::FrameList Frame  = id3v2tag->frameList("APIC");
    TagLib::ID3v2::AttachedPictureFrame *PicFrame ;
    if (id3v2tag)
    {
        if (!Frame.isEmpty())
        {
            for(TagLib::ID3v2::FrameList::ConstIterator it = Frame.begin(); it != Frame.end(); ++it)
            {
                PicFrame = static_cast<TagLib::ID3v2::AttachedPictureFrame *>(Frame.front()) ;
                QImage coverimage;
                coverimage.loadFromData((const uchar*)PicFrame->picture().data(),PicFrame->picture().size());
                m_cover->setPixmap(QPixmap::fromImage(coverimage));
                m_cover->show();
            }
        }
    }

}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::s_counter()
//
// Function to keep track of current time on the song
//
void MainWindow::s_counter()
{
    if(player->state()==QMediaPlayer::PlayingState)
    {
        m_slider->setValue(m_slidertime);
        m_slidertime++;
    }
    else if(player->state()==QMediaPlayer::StoppedState)
        s_next();

    QString T;
    m_timevalue--;
    int minute=m_timevalue/60;
    int leftSecond=m_timevalue%60;

    if(leftSecond<10)
    {
        T=QString::number(minute)+":0"+QString::number(leftSecond);
        m_time->setText(T);
    }
    else
    {
        T=QString::number(minute)+":"+QString::number(leftSecond);
        m_time->setText(T);
    }

}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::s_play()
//
// Function to play the selected mp3 song
//
void MainWindow::s_play(QTableWidgetItem *item)
{
    QPixmap pixpause("/Users/youshenghua/Desktop/musicplayer/Icons/pause.png");
    //QPixmap pixpause("/home/vfung000/Desktop/musicplayer/Icons/pause.png");
    //QPixmap pixpause("/home/fioger/Desktop/musicplayer/Icons/pause.png");

    for(int i=0; i<m_listSongs.size(); i++)
    {
        if(m_listSongs[i][TITLE] != item->text()) continue;
        player->setMedia(QUrl::fromLocalFile(m_listSongs[i][PATH].toLatin1().data()));
        player->play();
        CurrentSongIndex=i;
    }

    //set current song and icons
    m_play_pause->setIcon(pixpause);
    m_play_pause->setIconSize(pixpause.rect().size());
    m_play_pause->setFixedSize(pixpause.rect().size());
    m_time->setText(m_listSongs[CurrentSongIndex][TIME]);
    m_playingsong->setText(m_listSongs[CurrentSongIndex][TITLE]);

    //get timevalue
    QByteArray QBA= m_listSongs[CurrentSongIndex][PATH].toLatin1();
    cr=QBA.data();
    TagLib::FileName Tpath(cr);
    TagLib::FileRef Tref(Tpath);
    m_slider->setMaximum(Tref.audioProperties()->length());
    m_timevalue=Tref.audioProperties()->length();
    //setup timer
    m_timer->disconnect(m_timer,SIGNAL(timeout()),this,SLOT(s_counter()));
    connect(m_timer,SIGNAL(timeout()),this,SLOT(s_counter()));
    //set slidertime to 0
    m_slidertime=0;

    TagLib::MPEG::File mpegFile(cr);
    TagLib::ID3v2::Tag *id3v2tag = mpegFile.ID3v2Tag();
    TagLib::ID3v2::FrameList Frame  = id3v2tag->frameList("APIC");
    TagLib::ID3v2::AttachedPictureFrame *PicFrame ;
    if (id3v2tag)
    {
        if (!Frame.isEmpty())
        {
            for(TagLib::ID3v2::FrameList::ConstIterator it = Frame.begin(); it != Frame.end(); ++it)
            {
                PicFrame = static_cast<TagLib::ID3v2::AttachedPictureFrame *>(Frame.front()) ;
                QImage coverimage;
                coverimage.loadFromData((const uchar*)PicFrame->picture().data(),PicFrame->picture().size());
                m_cover->setPixmap(QPixmap::fromImage(coverimage));
                m_cover->show();
            }
        }
    }
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::setSizes()
//
// Function to control the sizes of the splitters that contains the widgets
//
void MainWindow::setSizes(QSplitter *splitter, int size1, int size2, int size3)
{
    QList<int> sizes;
    sizes.append(size1);
    sizes.append(size2);
    sizes.append(size3);

    splitter->setSizes(sizes);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::s_coverflow()
//
// Function to control which splitters will be shown
//
void MainWindow::s_coverflow()
{
    /*Cover State 1 shows initial layout (Main Window)
      Cover State 2 shows Coverflow
      Cover State 3 shows Visualizer*/
    //False means show
    //True means hide
    //This will show the coverflow
    if(CoverState==1|| CoverState==3)
    {
        //Show the OpenGLSplit
        m_OpenGLSplit->setHidden(false);
        //Hide the MainWindow (right side)
        m_rightSplit->setHidden(true);
        //Hide the visualizer
        m_visualizerSplit->setHidden(true);
        //Show the coverflow
        m_coverflowSplit->setHidden(false);
        setSizes(m_mainSplit, (int)(width ()*.8), (int)(width ()*.2),(int)(width ()*0));
        CoverState=2;
    }
    //This will show initial Layout
    else if(CoverState==2)
    {
        m_rightSplit->setHidden(false);
        m_OpenGLSplit->setHidden(true);
        setSizes(m_mainSplit, (int)(width ()*0), (int)(width ()*.2),(int)(width ()*.8));
        CoverState=1;
    }
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::s_visualizer()
//
// Function to control which splitters will be shown
//
void MainWindow::s_visualizer()
{

    /*Cover State 1 shows initial layout (Main Window)
      Cover State 2 shows Coverflow
      Cover State 3 shows Visualizer*/
    //False means show
    //True means hide
    //This will show the visualizer
    if(CoverState==1 ||CoverState==2)
    {
        //Show the OpenGLSplit
        m_OpenGLSplit->setHidden(false);
        //Hide the right split
        m_rightSplit->setHidden(true);
        m_coverflowSplit->setHidden(true);
        m_visualizerSplit->setHidden(false);
        setSizes(m_mainSplit, (int)(width ()*.8), (int)(width ()*.2),(int)(width ()*0));
        CoverState=3;
    }
    //This will go back to initial layout (Main Window)
    else if(CoverState==3)
    {
        //Show the right side of Main Window
        m_rightSplit->setHidden(false);
        //Hide the coverflow
        m_OpenGLSplit->setHidden(true);
        setSizes(m_mainSplit, (int)(width ()*0), (int)(width ()*.2),(int)(width ()*.8));
        CoverState=1;
    }
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MainWindow::keyPressEvent()
//
// Function that sends the left/right signal to the coverflow to move the covers
//
void MainWindow::keyPressEvent(QKeyEvent *e)
{

    if(e->key()==Qt::Key_Right)
        MyGlWidget::MoveRight();
    else if(e->key()==Qt::Key_Left)
        MyGlWidget::MoveLeft();
}
