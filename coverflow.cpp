#include "coverflow.h"
#include "MainWindow.h"
using namespace std;

int counter = 0;
int m_recordCount = 0;
int ww, hh;
static float m_move=0;
float m_animate=0;
float m_movespeed=0.02;
static int MoveState=0;
float centerX=2;
float centerY=-1;
float centerZ=0;
unsigned char  *texData;
int global_holder = 0;
QString ps;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MyGlWidget::MyGlWidget()
//
// Constructor
//
MyGlWidget::MyGlWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    initRecords();
    m_move=m_recordCount/2;
    m_animate=m_move;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MyGlWidget::~MyGlWidget()
//
// Deconstructor
//
MyGlWidget::~MyGlWidget()
{

}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MyGlWidget::traverseDirs()
//
// Traverse through the directories of the selected path
//
void MyGlWidget::traverseDirs (QString path, int holder)
{
     QFileInfo fileInfo;
     // init listDirs with subdirectories of path
     QDir dir(path);
     dir.setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);
     QFileInfoList listDirs = dir.entryInfoList();

     // init listFiles with all *.ppm files in path
     QDir files(path);
     files.setFilter(QDir::Files);
     files.setNameFilters(QStringList("*.mp3"));
     QFileInfoList listFiles = files.entryInfoList();
     QByteArray cp = path.toLatin1().data();
     char *filepath = cp.data();
     char imageDir[100];
     strcpy(imageDir, filepath);
     int imageFilenameSize = listFiles.size();
     char *imageFilename[imageFilenameSize];
     m_recordCount += listFiles.size();
     if (counter == 0)
     {
         m_records = (Record*) malloc(sizeof(Record) * m_recordCount);
         counter++;
     }
     else if (counter > 0)
     {
         tmp_records = (Record*) realloc(m_records, sizeof(Record) * m_recordCount);
         if (tmp_records == NULL)
         {
             free(tmp_records);
         }
         else if (tmp_records != NULL)
         {
             m_records = tmp_records;
         }
     }
     for(int i = 0; i < listFiles.size(); i++)
     {
         fileInfo = listFiles.at(i);
         QString filename(fileInfo.fileName());
         QByteArray ba = filename.toLatin1().data();
         char *file2 = ba.data();
         imageFilename[holder] = file2;
         m_records[holder].width  = 5;
         m_records[holder].height = 5;
         // save image filenames
         sprintf(m_records[holder].imageFilename, "%s/%s", imageDir, imageFilename[holder % m_recordCount]);
        holder++;
         global_holder++;
     }
     for(int i=0; i<listDirs.size(); i++)
     {
         QFileInfo fileInfo = listDirs.at(i);
         traverseDirs( fileInfo.filePath(), holder);
     }
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MyGlWidget::initRecords:
//
// Initialize data structure of records.
//
void MyGlWidget::initRecords()
{
    s_load();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MyGlWidget::s_load()
//
// Selects the path and passes it to traverseDirs and MainWindow::traverseDirs
//
void MyGlWidget::s_load()
{
    counter = 0;
    // open a file dialog box
    QFileDialog *fd = new QFileDialog;
    fd->setFileMode(QFileDialog::Directory);
    ps = fd->getExistingDirectory(0, "Select Folder", m_directory,
                                         QFileDialog::ShowDirsOnly |
                                         QFileDialog::DontResolveSymlinks);
    if (ps == NULL)
    {
        return;
    }
    m_progressBar = new QProgressDialog(this);
    m_progressBar->setWindowTitle("Updating");
    m_progressBar->setFixedSize(300,100);
    m_progressBar->setCancelButtonText("Cancel");
    traverseDirs(ps, global_holder);
    MainWindow::sentfromOpengl(ps);
    m_progressBar->close();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MyGlWidget::empty()
//
// An empty function
//
void MyGlWidget::empty()
{

}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// initGL:
//
// GL initialization routine before display loop.
//

void MyGlWidget::initializeGL()
{
    glEnable(GL_DEPTH_TEST);
    glClearColor(0,0,0,0);

    // load textures
    glEnable(GL_TEXTURE_2D);
    int		width, height;
    unsigned char  *texData;
    for(int i=0; i<m_recordCount; i++)
    {
        readMP3(m_records[i].imageFilename, width, height, texData);
        glGenTextures  (1, &m_records[i].texId);
        glBindTexture  (GL_TEXTURE_2D,  m_records[i].texId);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D   (GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGBA,
                        GL_UNSIGNED_BYTE, texData);
    }
    glDisable(GL_TEXTURE_2D);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// display:
//
// Display handler routine.
//

void MyGlWidget::paintGL()
{
    // clear color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // draw all records
    drawRecords();


    // update the screen
    update();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// reshape:
//
// Reshape handler routine. Called after reshaping window.
//
void MyGlWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (float) width/height, 2, 100);
    gluLookAt(0,0,1,0,0,0,0,1,0);
    glMatrixMode(GL_MODELVIEW);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MyGlWidget::drawRecords()
//
// Draws the records and allows the animation of the covers in the widget
//
void MyGlWidget::drawRecords()
{
    int middleItem=m_move;

    //animation
    //initial MoveState= 0
    //Move right MoveState=2
    //Move left MoveState=1
    //m_move is index of middle record, user controls m_move value
    //m_animate value will get close to m_move value
    if(MoveState==2 && m_animate>m_move)
    {
        m_animate=m_animate-m_movespeed;
    }
    else if(MoveState==1 && m_animate<m_move)
    {
        m_animate=m_animate+m_movespeed;
    }

    // center scene
    glTranslatef(0, -m_records[0].height/150.0,-6);
    // leftmost record position
    glPushMatrix();
    glTranslatef(-m_animate-1, 0, 0);
    for(int i= 0; i<m_recordCount; i++)
    {
        //init layout
        if(MoveState==0)
        {
            if(middleItem==i)
            {
                glTranslatef(1, 0, 0);
                continue;
            }
        }
        // for move
        else
        {
            if(middleItem==i || middleItem-1==i ||middleItem+1==i)
            {
                glTranslatef(1, 0, 0);
                continue;
            }
        }
        //move covers up
        glTranslatef(1, 2, 0);
        drawRecord(&m_records[i]);
        glTranslatef(0, -2, 0);
    }
    glPopMatrix();

    //transformation for MoveState==MoveRight-------------------------------
    if(MoveState==2)
    {
        //animate from left 2 to left 1 (-2,2,0) -> (-1,2,0)
        if(m_move!=0)
        {
            glPushMatrix();
            glTranslatef(-1*(m_animate-m_move)-1, 2, 0);
            drawRecord(&m_records[middleItem-1]);
            glPopMatrix();
        }
        //animate from left to middle (-1,2,0) -> (2,-1,0)  0->90
        glPushMatrix();
        glRotatef(90*(1-m_move+m_animate),0,-1,0);
        glTranslatef(2-m_animate+m_move, 3*(m_animate-m_move)-1,0);
        drawRecord(&m_records[middleItem]);
        glPopMatrix();

        //animate from middle to right (2,-1,0) -> (1,2,0)  rotate from 270->360
        glPushMatrix();
        glRotatef(270+90*(1-m_move+m_animate),0,-1,0);
        glTranslatef(1+m_animate-m_move, 2-3*(m_animate-m_move), 0);
        drawRecord(&m_records[middleItem+1]);
        glPopMatrix();
    }


    //transformation for MoveState==MoveLeft-------------------------------
    else if(MoveState==1)
    {
        //middle to left (2,-1,0) -> (1,2,0)
        glPushMatrix();
        glRotatef(180+90*(m_move-m_animate),0,1,0);
        glTranslatef(1+m_move-m_animate, 2-3*(m_move-m_animate), 0);
        drawRecord(&m_records[middleItem-1]);
        glPopMatrix();
        //left to middle   (1,2,0) ->(2,-1,0)
        glPushMatrix();
        glRotatef(90*(1-m_move+m_animate),0,-1,0);
        glTranslatef(2-m_move+m_animate, -1+3*(m_move-m_animate), 0);
        drawRecord(&m_records[middleItem]);
        glPopMatrix();
        //left 2 to left 1 (2,2,0) -> (1,2,0)
        if(m_move!=m_recordCount-1)
        {
            glPushMatrix();
            glTranslatef(1+m_move-m_animate, 2, 0);
            drawRecord(&m_records[middleItem+1]);
            glTranslatef(m_animate-m_move, -2, 0); //for animation
            glPopMatrix();
        }
    }
    //
    else if(MoveState==0)
    {
        glPushMatrix();
        glRotatef(90,0,1,0);
        glTranslatef(-2, -1, 0);
        drawRecord(&m_records[middleItem]);
        glPopMatrix();
    }

}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// drawRecord:
//
// Draw one record.
//
void MyGlWidget::drawRecord(Record *record)
{
    // enable texture mapping and bind image to polygon
    glRotatef(90,0,1,0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, record->texId);

    // draw filled polygon
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glColor3f(.8, .8, .7);
    glBegin(GL_QUADS);
    glTexCoord2f(1, 1);	glVertex3f(1, 1, 0);
    glTexCoord2f(0, 1);	glVertex3f(-1, 1 ,0);
    glTexCoord2f(0, 0);	glVertex3f(-1, -1, 0);
    glTexCoord2f(1, 0);	glVertex3f(1, -1, 0);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_LINE_SMOOTH);
    glColor3f(.8, 0.3, 1);
    glBegin(GL_QUADS);
    glVertex3f(-1, -1, 0);
    glVertex3f(1, -1 ,0);
    glVertex3f(1, 1, 0);
    glVertex3f(-1, 1, 0);
    glEnd();
    glDisable(GL_LINE_SMOOTH);
    glRotatef(90,0,-1,0);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MyGlWidget::MoveRight()
//
// Move the covers right
//
void MyGlWidget::MoveRight()
{
    if(m_move==0)
        return;
    m_move--;
    MoveState=2;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MyGlWidget::MoveLeft()
//
// Move the covers left
//
void MyGlWidget::MoveLeft()
{
    if(m_move == m_recordCount-1)
        return;
    m_move++;
    MoveState=1;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MyGlWidget::quit()
//
// Free allocated data
//
void MyGlWidget::quit()
{
    free(m_records);
    free(tmp_records);
    exit(0);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MyGlWidget::readMP3
//
// Get the image from the mp3 file and pass the width, height and image back to initializeGL()
//
int  MyGlWidget::readMP3(char *file, int &width, int &height, unsigned char* &image)
{
    //Get the selected song path and pass it to tglib
    TagLib::MPEG::File mpegFile(file);
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
                //Convert QImage into an image that OpenGL can understand
                QImage GL_formatted_image = QGLWidget::convertToGLFormat(coverimage);
                //Error check
                if( GL_formatted_image.isNull() )
                {
                    qDebug() << "Error there";
                    return 1;
                }
                //Get the width, height and image data and return it to initializerGL()
                width =  GL_formatted_image.width();
                height = GL_formatted_image.height();
                //For the RGBA
                image = new unsigned char[width*height*4];
                image = GL_formatted_image.bits();
            }
        }
        else
            qDebug() << "No image";
    }
    return 1;
}
