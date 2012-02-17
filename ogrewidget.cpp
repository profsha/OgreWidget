#include "ogrewidget.h"
#include <QDebug>

OgreWidget::OgreWidget(QWidget *parent)
:QWidget(parent),
ogreRoot(0), ogreSceneManager(0), ogreRenderWindow(0), ogreViewport(0),
ogreCamera(0)
{
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_PaintOnScreen);
    setMinimumSize(240,240);
}

OgreWidget::~OgreWidget()
{
    if(ogreRenderWindow)
    {
        ogreRenderWindow->removeAllViewports();
    }

    if(ogreRoot)
    {
        ogreRoot->detachRenderTarget(ogreRenderWindow);

        if(ogreSceneManager)
        {
            ogreRoot->destroySceneManager(ogreSceneManager);
        }
    }

    delete ogreRoot;
}

void OgreWidget::setBackgroundColor(QColor c)
{
    if(ogreViewport)
    {
        Ogre::ColourValue ogreColour;
        ogreColour.setAsARGB(c.rgba());
        ogreViewport->setBackgroundColour(ogreColour);
    }
}

void OgreWidget::moveEvent(QMoveEvent *e)
{
    QWidget::moveEvent(e);

    if(e->isAccepted() && ogreRenderWindow)
    {
        ogreRenderWindow->windowMovedOrResized();
        update();
    }
}

QPaintEngine *OgreWidget:: paintEngine() const
{
    return 0;
}

void OgreWidget::paintEvent(QPaintEvent *e)
{
        ogreRoot->_fireFrameStarted();
        ogreRenderWindow->update();
        ogreRoot->_fireFrameRenderingQueued();
        ogreRoot->_fireFrameEnded();
    renderFrame();
    e->accept();
}

void OgreWidget::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);

    if(e->isAccepted())
    {
        const QSize &newSize = e->size();
        if(ogreRenderWindow)
        {
            ogreRenderWindow->resize(newSize.width(), newSize.height());
            ogreRenderWindow->windowMovedOrResized();
        }
        if(ogreCamera)
        {
            Ogre::Real aspectRatio = Ogre::Real(newSize.width()) / Ogre::Real(newSize.height());
            ogreCamera->setAspectRatio(aspectRatio);
        }
    }
}

void OgreWidget::showEvent(QShowEvent *e)
{
    if(!ogreRoot)
    {
        createOgreRoot();
        initOgreSystem();
    }

    QWidget::showEvent(e);
}

void OgreWidget::initOgreSystem()
{




    Ogre::NameValuePairList viewConfig;
    Ogre::String widgethandle;

#if defined(Q_WS_WIN)
      //positive integer for W32 (HWND handle) - According to Ogre Docs
      widgethandle += Ogre::StringConverter::toString((unsigned long)(this->parentWidget()->winId()));
      viewConfig["parentWindowHandle"] = widgethandle;

#endif

#if defined(Q_WS_X11)
                //poslong:posint:poslong:poslong (display*:screen:windowHandle:XVisualInfo*) for GLX - According to Ogre Docs
    QX11Info info = x11Info();


                widgethandle  = Ogre::StringConverter::toString((unsigned long)(info.display()));
                widgethandle += ":";
                widgethandle += Ogre::StringConverter::toString((unsigned int)(info.screen()));
                widgethandle += ":";
                widgethandle += Ogre::StringConverter::toString((unsigned long)(this->parentWidget()->winId()));
                //widgethandle += ":";
                //widgethandle += Ogre::StringConverter::toString((unsigned long)(info.visual()));

                viewConfig["parentWindowHandle"] = widgethandle;
#endif


//#if defined(Q_WS_MAC)
//
//      widgethandle = Ogre::StringConverter::toString((unsigned int)(winId()));
//      viewConfig["macAPI"] = "cocoa";
//      viewConfig["macAPICocoaUseNSView"] = "true";
//
//#endif
    ogreRenderWindow = ogreRoot->createRenderWindow("Ogre rendering window",
               this->width(), this->height(), false, &viewConfig);

    ogreRenderWindow->setActive(true);
#if defined(Q_WS_X11)
     WId ogreWinId = 0x0;
     ogreRenderWindow->getCustomAttribute( "WINDOW", &ogreWinId );

     assert( ogreWinId );

     this->create( ogreWinId );
#endif
     createScene();
     ogreRenderWindow->reposition(x(), y());

}

void OgreWidget::createScene()
{
    Ogre::Vector3 lightdir(0.55, -0.3, 0.75);
    lightdir.normalise();

    Ogre::Light* light = ogreSceneManager->createLight("tstLight");
    light->setType(Ogre::Light::LT_DIRECTIONAL);
    light->setDirection(lightdir);
    light->setDiffuseColour(Ogre::ColourValue::White);
    light->setSpecularColour(Ogre::ColourValue(0.4, 0.4, 0.4));
    ogreSceneManager->setAmbientLight(Ogre::ColourValue(0.2, 0.2, 0.2));
    ogreCamera = ogreSceneManager->createCamera("myCamera");
    ogreCamera->setPosition(Ogre::Vector3(0, 0, 0));
    ogreCamera->setNearClipDistance(0.1);
    ogreCamera->setFarClipDistance(50000);

if (ogreRoot->getRenderSystem()->getCapabilities()->hasCapability(Ogre::RSC_INFINITE_FAR_PLANE))
    {
        ogreCamera->setFarClipDistance(0);   // enable infinite far clip distance if we can
    }
ogreViewport = ogreRenderWindow->addViewport(ogreCamera);
setBackgroundColor(QColor(100,100,100));

ogreCamera->setAspectRatio(Ogre::Real(this->parentWidget()->width()) / Ogre::Real(this->parentWidget()->height()));
}

void OgreWidget::renderFrame()
{

}

void OgreWidget::createOgreRoot()
{
    ogreRoot= new Ogre::Root();

    ogreRoot->loadPlugin("RenderSystem_GL");
    Ogre::RenderSystemList::const_iterator renderers = ogreRoot->getAvailableRenderers().begin();
      while(renderers != ogreRoot->getAvailableRenderers().end())
      {
          Ogre::String rName = (*renderers)->getName();
            if (rName == "OpenGL Rendering Subsystem")
                break;
            renderers++;
      }

      Ogre::RenderSystem *renderSystem = *renderers;
    ogreRoot->setRenderSystem(renderSystem);

    if(ogreRoot->getRenderSystem()== NULL)
    {
        delete ogreRoot;
        qApp->quit();
    }
    if(!ogreRoot->restoreConfig())
        ogreRoot->showConfigDialog();
    ogreRoot->saveConfig();
    ogreRoot->initialise(false);
     ogreSceneManager = ogreRoot->createSceneManager(Ogre::ST_GENERIC);
}
