#include "popup.h"
#include "ui_popup.h"
#include "newwidget.h"

#include <AnnotatorLib/Commands/NewAnnotation.h>
#include <AnnotatorLib/Commands/NewObject.h>
#include "plugins/pluginloader.h"


Popup::Popup(QWidget *parent) : QDialog(parent),
    ui(new Ui::Popup),
    previousStart(-1),
    previousEnd(-1),
    splitterFrame(-1),
    relatedID(-1)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::SplashScreen);
    this->setAttribute(Qt::WA_TranslucentBackground);



    //to set new Widget
    contents = new QWidget;
    layout = new QVBoxLayout(contents);

}

Popup::~Popup()
{
    delete contents;
    delete ui;
}

/**
 * 	-	set project path
 *
*/
QString Popup::fileName;
QString Popup::dataPath;
QString Popup::dataDirPath;
void Popup::setFileName(QString FileName)
{
    this->Projectpath = FileName;

    //data path
    dataPath = Projectpath + "/data.xml";
}


/**
 * 	-	set current frame number
 *
*/
long Popup::frameNr;
void Popup::setCurrFrame(long FrameNr)
{
    frameNr = FrameNr;
}

void Popup::setCurrFrame(long FrameNr, QString filename)
{
    frameNr = FrameNr;
    videoFilePath = filename;
}


/**
 * 	-	close dialog and reset setting
 *
*/
void Popup::on_btnPopClose_clicked()
{
    if(isEditItem){
        isEditItem = false;
        this->close();
        return;
    }

    emit removeLastItem();

    ui->lEObject->setText("");

    for(int i = 0; i < widgetList.count(); i++)
        widgetList[i]->setText("");

    this->close();
}


/**
 * 	-	put item into RAM, while it is not completely saved
 *
*/
void Popup::putInRAM(QString x, QString y, int id, QString borderColor)
{
    //save start objects in list
    double arr[5];
    parseData(x, y, arr);
    arr[4] = id;  //put id in 4 index


    QVector<QString> object;
    object.push_back(QString::number(arr[0]));
    object.push_back(QString::number(arr[1]));
    object.push_back(QString::number(arr[2]));
    object.push_back(QString::number(arr[3]));
    object.push_back(QString::number(arr[4]));
    object.push_back(QString::number(startFrame));
    object.push_back(borderColor);

    StartObjetcsList.push_back(object);
}


/**
 * 	-	save button, set init setting
 *
*/
void Popup::on_btnPopSave_clicked()
{
    if(isEditItem){
        isEditItem = false;
        this->close();
        return;
    }

    AnnotatorLib::Commands::NewObject * nO = new AnnotatorLib::Commands::NewObject(ui->lEObject->text().toStdString(), session);
    session->execute(nO);
    float width = pointLR.x() - pointUL.x();
    float height = pointLR.y()- pointUL.y();

    float x = pointUL.x() + width / 2;
    float y = pointUL.y() + height / 2;

    AnnotatorLib::Frame *frame = session->getFrame(frameNr);

    AnnotatorLib::Commands::NewAnnotation * nA = new AnnotatorLib::Commands::NewAnnotation(nO->getObject(), frame, x, y
                                      , width/2, height/2, this->session, false);

    session->execute(nA);

    Annotator::Plugin * plugin = Annotator::PluginLoader::getInstance().getCurrent();
    plugin->setObject(nO->getObject());
    plugin->setLastAnnotation(nA->getAnnotation());

    //get the start frame number of item.
    startFr = frameNr;
    endFr = -1;

    //get the start frame number of the currently sequence.
    startFrame = frameNr;
    endFrame = -1; //-1 means no END Frame was defined


    //save item in RAM
    putInRAM(ui->lE_UL->text(), ui->lE_LR->text(),ItemID.toInt(), ItemColor.name());

    emit saveIDsettings();

    emit removeLastItem();

    this->close();
}





/**
 * 	-	show add_attribute dialog
 *
*/
void Popup::on_btnPopAddAtr_clicked()
{
    NewWidget *newWidget = new NewWidget;
    newWidget->setModal(true);
    connect(newWidget, SIGNAL(setNewWidget(const QString, const QString)), this, SLOT(setNewWidget(const QString, const QString)));
    newWidget->exec();
}


/**
 * 	-	set new_widget into layout
 *
*/
void Popup::setNewWidget(const QString &labelText, const QString &typeText)
{

    QLineEdit *myLineEdit = new QLineEdit(this);
    myLineEdit->setFixedSize(253,26);
    myLineEdit->setStyleSheet("background-color: rgb(255, 255, 255);");
    myLineEdit->setPlaceholderText(labelText);
    myLineEdit->setObjectName(labelText);
    myLineEdit->setToolTip(labelText);

    layout->addWidget(myLineEdit);
    this->ui->scrollArea->setWidget(contents);

    widgetList.append(myLineEdit);
    ///qDebug() << widgetList.count();
}

/**
 * setData	-	store the current data into the correct field
 *
 * @param UL	-	Upper Left point
 *        LR	-	Low Right point
 *        id	-	item ID
 *        Name	-	item Name
 *        borderColor - geom. object border color
 *
 */
void Popup::setData(QPointF UL, QPointF LR, QString id, QString Name, QColor borderColor)
{
    QPointF ptMin;
    QPointF ptMax;


    if(UL.x()<LR.x()){
        ptMin.setX(UL.x());
        ptMax.setX(LR.x());
    }
    else if(UL.x()>LR.x()){
        ptMin.setX(LR.x());
        ptMax.setX(UL.x());
    }else{
        ptMin.setX(UL.x());
        ptMax.setX(LR.x()+1);
    }
    if(UL.y()<LR.y()){
        ptMin.setY(UL.y());
        ptMax.setY(LR.y());
    }
    else if(UL.y()>LR.y()){
        ptMin.setY(LR.y());
        ptMax.setY(UL.y());
    }else{
        ptMin.setY(UL.y());
        ptMax.setY(LR.y()+1);
    }

    //pointUL = QPointF(ptMin.x(), ptMin.y());
    //pointLR = QPointF(ptMax.x(), ptMax.y());

    pointUL = UL;
    pointLR = LR;

    ItemID = id;
    ItemColor = borderColor;

    if (!Name.isNull()){
        ui->lEObject->setText(Name);
    }
    ui->lE_UL->setText("UL:(" + QString::number(ptMin.x()) + ", " + QString::number(ptMin.y()) + ")");
    ui->lE_LR->setText("LR:(" + QString::number(ptMax.x()) + ", " + QString::number(ptMax.y()) + ")");
}






/**
 * 	-	remove file name: remove last part from file path
 *
*/
QString Popup::removeFileName(QString Projectpath)
{
    //remove filename from path
    QStringList list = Projectpath.split( "/" );
    Projectpath = Projectpath.left(Projectpath.length() - (list.value(list.length()-1).length()+1));
    return Projectpath;
}


/**
 * 	-	clearLayout: clear Layout
 *      remove all child of layout
 *
*/
void Popup::clearLayout(QLayout *layout)
{
    QLayoutItem* child;
    while(layout->count()!=0)
    {
        child = layout->takeAt(0);
        if(child->layout() != 0)
        {
            clearLayout(child->layout());
        }
        else if(child->widget() != 0)
        {
            delete child->widget();
        }

        delete child;
    }
}




/**
 * 	-	parse data: get coordinate of object
 *
*/
void Popup::parseData(QString ULString, QString LRString, double arr[])
{
    //get x and y value
    QStringList list = ULString.split( "(" );
    ULString = list.value( list.length() - 1 );
    list = ULString.split(",");
    //set x value
    arr[0] = list.value(0).toDouble();

    ULString = list.value( list.length() - 1 );
    list = ULString.split(")");
    //set y value
    arr[1] = list.value(0).toDouble();


    //get w and h value
    list = LRString.split( "(" );
    LRString = list.value( list.length() - 1 );
    list = LRString.split(",");
    //set w value
    arr[2] = list.value(0).toDouble(); //list.value(0).toDouble() - arr[0];

    LRString = list.value( list.length() - 1 );
    list = LRString.split(")");
    //set h value
    arr[3] = list.value(0).toDouble(); //list.value(0).toDouble() - arr[1];

}


/**
 * 	-	show Popup: to edit item
 *
*/
void Popup::showPopUp()
{
    //get item data, (Name...)
    //FilePath = Projectpath + ("/"+ QString::number(frameNr) +".xml");
    FilePath = Projectpath + ("/"+ QString("%1").arg(frameNr, 8, 10, QChar('0')) +".xml");


    ui->lEObject->setText(itemName);
    isEditItem = true;
    this->exec();
}





/**
 * 	-	isRAMEmpty: check if RAM is empty
 *
*/
bool Popup::isRAMEmpty()
{
    if(StartObjetcsList.isEmpty())
        return true;

    return false;
}


/**
 * 	-	removeRAM: clead RAM
 *
*/
void Popup::removeRAM()
{
    StartObjetcsList.clear();
}

void Popup::setSession(AnnotatorLib::Session *session)
{
    this->session = session;
}


/**
 * 	-	PopLayoutReset: reset layout
 *
*/
void Popup::on_btnPopLayoutReset_clicked()
{
    clearLayout(layout);
    widgetList.clear();
}
