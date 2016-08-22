#include "owngraphicscene.h"
#include "graphic/owngraphicscene.h"
#include "gui/newobjectdialog.h"

#include <algorithm>

#include <QDebug>

OwnGraphicScene::OwnGraphicScene(Popup *parent):
    isDrawn(false),
    isLeftPressed(false),
    isItemMove(false)
{
    this->setParent(parent);
    popup = parent;

    //connect(popup, SIGNAL(removeLastItem()), this, SLOT(removeLastItem()));
}


/**
 * set background image to scene
*/
void OwnGraphicScene::addBackgroundImg(QImage img){
    image=img;
}


/**
 * draw scene background
*/
void OwnGraphicScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    QGraphicsScene::drawBackground(painter, rect);
    painter->drawImage(0,0,image);
}


/**
 * draw geom_item by mouse pressed & moving
*/
void OwnGraphicScene::rectDraw(QPointF pt1,QPointF pt2)
{
    QPointF ptMin;
    QPointF ptMax;


    if(pt1.x()<pt2.x()){
        ptMin.setX(pt1.x());
        ptMax.setX(pt2.x());
    }
    else if(pt1.x()>pt2.x()){
        ptMin.setX(pt2.x());
        ptMax.setX(pt1.x());
    }else{
        ptMin.setX(pt1.x());
        ptMax.setX(pt2.x()+1);
    }
    if(pt1.y()<pt2.y()){
        ptMin.setY(pt1.y());
        ptMax.setY(pt2.y());
    }
    else if(pt1.y()>pt2.y()){
        ptMin.setY(pt2.y());
        ptMax.setY(pt1.y());
    }else{
        ptMin.setY(pt1.y());
        ptMax.setY(pt2.y()+1);
    }
    this->removeItem(rectangle);

    itemColor.setRgb(qrand()% (255 + 1) ,qrand()% (255 + 1),qrand()% (255 + 1));
    rectangle = new OwnQGraphicsItem(ptMin.x(),ptMin.y(),ptMax.x()-ptMin.x(),ptMax.y()-ptMin.y(),itemColor);

    this->addItem(rectangle);
    this->update();
}


/**
 * draw geom_item from file/RAM
*/
void OwnGraphicScene::setItem(QPointF point_Top, QPointF point_Bottom, int id, QColor borderColor)
{
    QPointF ptMin;
    QPointF ptMax;


    if(point_Top.x()<point_Bottom.x()){
        ptMin.setX(point_Top.x());
        ptMax.setX(point_Bottom.x());
    }
    else if(point_Top.x()>point_Bottom.x()){
        ptMin.setX(point_Bottom.x());
        ptMax.setX(point_Top.x());
    }else{
        ptMin.setX(point_Top.x());
        ptMax.setX(point_Bottom.x()+1);
    }
    if(point_Top.y()<point_Bottom.y()){
        ptMin.setY(point_Top.y());
        ptMax.setY(point_Bottom.y());
    }
    else if(point_Top.y()>point_Bottom.y()){
        ptMin.setY(point_Bottom.y());
        ptMax.setY(point_Top.y());
    }else{
        ptMin.setY(point_Top.y());
        ptMax.setY(point_Bottom.y()+1);
    }


    rectangle = new OwnQGraphicsItem(resetCoordinate(ptMin).x(),resetCoordinate(ptMin).y(),resetCoordinate(ptMax).x()-resetCoordinate(ptMin).x(),resetCoordinate(ptMax).y()-resetCoordinate(ptMin).y(), borderColor);

    rectangle->setID(id);
    ItemsList.append(rectangle);  //insert rectangle to the list
    this->addItem(rectangle);
    this->update();
}


/**
 * mouse Press: get pressed point
 *              pause the video if is playing
 *              remove geom_item if right click
 *              activat drawmode if left click on No item
*/
void OwnGraphicScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    //video Pause, to paint item.
    emit on_btnPause_clicked();

    if(event->button() == Qt::LeftButton) isLeftPressed = true;

    if(itemAt(event->scenePos(),QTransform()) == 0)
    {
        if(event->button() == Qt::LeftButton){
             //get x,y coordinate
             point1 = (event->scenePos());

             //point1 = mapToGlobal(event->pos());
             rectangle = (OwnQGraphicsItem*)this->addRect(point1.x(),point1.y(),0,0,QPen(),QBrush());
             //set flag on press
             drawMode = DrawON;
         }
    }
    else
    {
        drawMode = DrawOFF;
        if(event->button() == Qt::RightButton && !isLeftPressed)
        {
            //TODO: I removed this, because we don't want the rect to disappear when right-clicking.

            //selectItem;
//            point1 = (event->scenePos());

//            QTransform transform;
//            rectangle = (OwnQGraphicsItem*)this->itemAt(point1,transform);
//            if(rectangle!=0){
//                this->removeItem(rectangle);
//                this->update();
//                ItemsList.removeOne(rectangle);
//                ///qDebug()<<"# of Rects: "<<ItemsList.size();
//            }
        } else if(event->button() == Qt::LeftButton)
        {
            this->clearSelection();     //single selection mode
        }
    }

    QGraphicsScene::mousePressEvent(event);
}


/**
 * remove last drawed geom_item
*/
void OwnGraphicScene::removeLastItem()
{

    QTransform transform;
    //calculate the average Point
    qreal x = (point1.x()<= point2.x()) ? point1.x()+((point2.x()-point1.x())/2) : point2.x() + ((point1.x()-point2.x())/2);
    qreal y = (point1.y()<= point2.y()) ? point1.y()+((point2.y()-point1.y())/2) : point2.y() + ((point1.y()-point2.y())/2);

    QPointF removePoint;
    removePoint.setX(x);
    removePoint.setY(y);

    rectangle = (OwnQGraphicsItem*)this->itemAt(removePoint,transform);

    if(rectangle!=0){
        this->removeItem(rectangle);
        this->update();
        ItemsList.removeOne(rectangle);
        ///qDebug()<<"# of Rects: "<<ItemsList.size();
    }
}




/**
 * mouse move: if drawmode active, draw geom_item
*/
void OwnGraphicScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton){
        if(drawMode==DrawON){
            point2 = (event->scenePos()).toPoint();
            rectDraw(point1,point2);
            this->isDrawn = true;
            isItemMove = false;
        }else{
            isItemMove = true;
        }
    }

    QGraphicsScene::mouseMoveEvent(event);
}


/**
 * mouse release: if drawmode active, set geom_item data & show popup
 *                if geom_item is moving, set geom data to update coordinate
*/
void OwnGraphicScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button()==Qt::LeftButton){
        isLeftPressed = false;
        if(drawMode==DrawON){

            if(isDrawn){
                NewObjectDialog newObjectDialog(session);
                QPointF upperLeft = adjustCoordinate(point1);
                QPointF lowerRight =  adjustCoordinate(point2);
                float x = std::min(upperLeft.x(), lowerRight.x());
                float y = std::min(upperLeft.y(), lowerRight.y());
                float w = std::max(upperLeft.x(), lowerRight.x()) - x;
                float h = std::max(upperLeft.y(), lowerRight.y()) - y;

                newObjectDialog.setDimenstions(x, y, w, h);
                newObjectDialog.setFrame(currentFrame);
                newObjectDialog.move(event->scenePos().x(),event->scenePos().y());
                newObjectDialog.exec();

                ItemsList.append(rectangle);


                isDrawn = false;

            }


        }else if(isItemMove){
            //if item is moved, save data into xml file or update new position
            if(!ItemsList.isEmpty()){
                popup->setData(adjustCoordinate(rectangle->getX((OwnQGraphicsItem*)this->selectedItems().last())),
                               adjustCoordinate(rectangle->getY((OwnQGraphicsItem*)this->selectedItems().last())),
                               rectangle->getID((OwnQGraphicsItem*)this->selectedItems().last()),
                               NULL,
                               rectangle->getColor((OwnQGraphicsItem*)this->selectedItems().last()));


                isItemMove = false;
                ///qDebug()<<"# of Rects: "<<ItemsList.size();

            }
        }


        drawMode = DrawOFF;
    }

    QGraphicsScene::mouseReleaseEvent(event);
}


/**
 * mouse doubleClick: get geom_item data and show popup to edit geom_item.
*/
void OwnGraphicScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event)
{
    if(itemAt(event->scenePos(),QTransform()) != 0){
        QGraphicsScene::mouseDoubleClickEvent(event);
    }

}


/**
 * clear List_items
*/
void OwnGraphicScene::clearItemsList()
{
    ItemsList.clear();
}


/**
 * remove geom_item at pressed point
*/
OwnQGraphicsItem *OwnGraphicScene::getItemByID(QString ID)
{
    for(int i = 0; i < ItemsList.count(); i++){
        if(((OwnQGraphicsItem *)ItemsList.at(i))->idText->toPlainText() == ID){
            return (OwnQGraphicsItem *)ItemsList.at(i);
        }
    }
    return NULL;
}



/**
 * edit geom_item: get geom_item data and show popup
*/
void OwnGraphicScene::editItem(OwnQGraphicsItem *rectangle)
{
    this->clearSelection();
    rectangle->setSelected(true);

    rectangle->selectItem(rectangle);
    rectangle->diselectAll();
}

void OwnGraphicScene::setCurrentFrame(int frame)
{
    this->currentFrame = frame;
}

void OwnGraphicScene::setSession(AnnotatorLib::Session *session)
{
    this->session = session;
}


/**
 * get the next free id from config file
*/
int OwnGraphicScene::loadSettings()
{
    QSettings settings(id_File, QSettings::NativeFormat);

    if(settings.contains("Last_ID")){
        QString sID = settings.value("Last_ID", "").toString();
        return sID.toInt();
    }
    return 1;
}

/**
 * save the next free id in config file
*/
void OwnGraphicScene::saveIDsettings()
{
    QSettings settings(id_File, QSettings::NativeFormat);
    QString sID = QString::number(loadSettings() + 1);
    settings.setValue("Last_ID", sID);
}


/**
 * adjustCoordinate: to calculate the correct coordinate of image/video on scene.
 * @param: pt1: QPoint the coordinate on scene (mouse Position)
 *         pt2: QPoint the coordinate of image/video Frame
 *         pt3: QPoint the coordinate of scene (lower Right)(width, height)
 *
*/
QPoint OwnGraphicScene::adjustCoordinate(QPointF MousePos)
{
    return QPoint(MousePos.x(), MousePos.y());
    // I commented out because there was an offset. Without it works.
    //return QPoint(((MousePos.x() * inputCoordinate.x()) / this->width()),((MousePos.y() * inputCoordinate.y())  / this->height()));
}


QPointF OwnGraphicScene::resetCoordinate(QPointF RectPos)
{
    return QPointF(((RectPos.x() * this->width()) / inputCoordinate.x()),((RectPos.y() * this->height())  / inputCoordinate.y()));
}
