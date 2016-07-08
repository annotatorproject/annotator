#include "owngraphicview.h"




OwnGraphicView::OwnGraphicView(QGraphicsScene *scene, QWidget *parent):
    QGraphicsView(scene, parent)
{
    //make some initialization to give graphicsview transparence color and scene
    ownScene = (OwnGraphicScene*)scene;
    this->setScene(ownScene);
    this->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    this->setRenderHint(QPainter::Antialiasing);

    this->setStyleSheet("QGraphicsView { background-color : transparent; color : blue; }");
    this->sizePolicy().setHorizontalPolicy(QSizePolicy::Ignored);
    this->sizePolicy().setVerticalPolicy(QSizePolicy::Ignored);
    this->setAlignment(Qt::AlignCenter);

    //this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

/**
 * set background image to scene
*/
void OwnGraphicView::setBackground(QImage background){
    ownScene->addBackgroundImg(background);
}

/**
 * set new width and height to scene
*/
void OwnGraphicView::updateSceneSize(int width, int height){
    ownScene->setSceneRect(0,0,width,height);
}


/**
 * resize geo_item if graphicsview was resized
*/
void OwnGraphicView::resizeEvent(QResizeEvent *event)
{
    this->fitInView(this->sceneRect(),Qt::IgnoreAspectRatio);
    QGraphicsView::resizeEvent(event);
}
