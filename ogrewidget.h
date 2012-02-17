#ifndef OGREWIDGET_H
#define OGREWIDGET_H

#include <Ogre.h>
#include <OgreConfigFile.h>
#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>
#include <QtGui>
#include <QTime>
#include <iostream>

#ifdef Q_WS_X11
  #include <QtGui/QX11Info>
#endif

#include <QDebug>

class OgreWidget : public QWidget
{
    Q_OBJECT

public:
    OgreWidget(QWidget *parent = 0);
    ~OgreWidget();

public slots:
    void setBackgroundColor(QColor c);

protected:
    virtual void moveEvent(QMoveEvent *e);
    virtual QPaintEngine* paintEngine() const;
    virtual void paintEvent(QPaintEvent *e);
    virtual void resizeEvent(QResizeEvent *e);
    virtual void showEvent(QShowEvent *e);
    virtual void createScene();

private:
    virtual void createOgreRoot();
    virtual void renderFrame();
    void initOgreSystem();

private:
    Ogre::SceneNode *ogreCamNode;
    Ogre::SceneNode *ogreCamNodeLook;
    Ogre::Root          *ogreRoot;
    Ogre::SceneManager *ogreSceneManager;
    Ogre::RenderWindow *ogreRenderWindow;
    Ogre::Viewport      *ogreViewport;
    Ogre::Camera        *ogreCamera;

signals:


};

#endif // OGREWIDGET_H
