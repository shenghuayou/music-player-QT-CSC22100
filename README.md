# music-player-QT-CSC22100 #
Our project was to create a music player called qTunes, which mimics an earlier version of iTunes, which had the usage of the cover flow to view album art. In our program, the user is able to select the directory of their choice which has the music files and load it into the program. After loading the music into the program, it will place the music and their respective tags into a table and different panels that allow the user to sort the music. In addition, the cover flow will be enabled, where the album art of the respective music files will be shown. 

## Group Members: ##
Name: Victor Fung

e-mail: victor.fung122@gmail.com

Name: Shenghua You

e-mail: shenghuayou@gmail.com

Name: Fioger Shahallori

e-mail: fioger95@live.com

## qTunes - Things needed to have/access all the features of our program. ##
1. In the MainWindow.cpp, the user needs to fix the paths to the icons that control 
   the music.
   I have placed the following comment on top of each section, so the user needs to use 
   find //**CHANGE THIS TO THE PATH OF THE IMAGE** 
   located in the s_play, s_next, s_previous, s_pause_play, createWidgets
   without this, the buttons will not be visible to the user and the functions cannot be accessed.

2. In v2.pro, the user needs to fix the taglib paths to the local path in which their taglib is installed,
   the files are libtag.so, libtag.so.1, libtag.so.1.14.0, all 3 files should be located under 
   /taglib-1.9.1/build/taglib/

3. Our qTunes uses Qt (version 5.4) with OpenGL, which can be downloaded from http://www.qt.io/download-open-source/#section-2
   Our qTunes uses Taglib (version 1.9.1) which can be downloaded from http://taglib.github.io/
   Taglib has dependencies on cmake which can be downloaded from http://www.cmake.org/download/

4. Operating systems our program works on that we used MAC OS X 10.9.4, Ubuntu 14.04

## Instructions on running qTunes on newly installed Ubuntu 14.04 ##
1. Get the Ubuntu iso from http://www.ubuntu.com/download/desktop

2. Get an virtual machine to set up the Ubuntu OS
   A free VM that we used was VMware Player from https://my.vmware.com/web/vmware/downloads
   the download is located under Desktop & End-User Computing.

3. Set up the virtual environment using the Ubuntu iso and set up username, password, etc (Ubuntu will guide you)

4. Installing a compiler and basic essentials for your newly installed Ubuntu.
   This is a great guide to set-up Qt on your Ubuntu along with basic essentials
   http://wiki.qt.io/Install_Qt_5_on_Ubuntu
   **NOTE:The website shows Qt 5.0 rather than 5.4, please install Qt 5.4 instead.**

5. After following the guide and setting up Qt with OpenGL and other basic essentials. 
   The next step is to install Taglib and its dependencies (cmake)

6. The following link is instructions to set up Taglib, so first we will set up its dependences
   cmake, please follow this guide: http://www.linuxfromscratch.org/blfs/view/svn/general/cmake.html
   **NOTE: As this is a newly installed version of Ubuntu, you will (most likely) run into other missing
           libraries that cmake requires, please install the missing libraries, if you run into issues of 
           a missing library, please send an e-mail to a person listed above**
   **NOTE: To install as "root" user, type sudo in the front of the command, i.e. sudo make install**

7. After setting up cmake, now we have to set up Taglib, the following link is also a guide on how to set up 
   Taglib: http://www.linuxfromscratch.org/blfs/view/svn/multimedia/taglib.html
   **NOTE: To install as "root" user, type sudo in the front of the command, i.e. sudo make install**

8. After successfully setting up Qt, Taglib and their dependencies, the next step to fix a couple paths
   in our MainWindow.cpp, the user needs to fix the paths to the icons that control the music.
   I have placed the following comment on top of each section, so the user needs to use 
   find //**CHANGE THIS TO THE PATH OF THE IMAGE** 
   located in the s_play, s_next, s_previous, s_pause_play, createWidgets
   without this, the buttons will not be visible to the user and the corresponding functions cannot be accessed.

9. In v2.pro, the user needs to fix the taglib paths to the local path in which their taglib is installed,
   the files are libtag.so, libtag.so.1, libtag.so.1.14.0, all 3 files should be located under 
   /taglib-1.9.1/build/taglib/, without this the program is unable to extract the metadata from the mp3 songs.

10. All that is left is to compile our program, if you open our program with Qt-Creator, press Ctrl-R
    to run the program.
    
## Demo images ##
<img src=https://github.com/shenghuayou/music-player-QT-CSC22100/blob/master/demo_image/d1.png />

<img src=https://github.com/shenghuayou/music-player-QT-CSC22100/blob/master/demo_image/d2.png />

<img src=https://github.com/shenghuayou/music-player-QT-CSC22100/blob/master/demo_image/d3.png />
