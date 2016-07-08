#ifndef POPUP_H
#define POPUP_H



#include <QDialog>
#include <QWidget>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QLabel>
#include <QPointF>
#include <QDebug>
#include <QStandardItemModel>
#include <QList>
#include <QFileDialog>
#include <QXmlStreamWriter>
#include <QMessageBox>
#include <QVector>
#include <QGraphicsScene>
#include <QTreeWidget>
#include <AnnotatorLib/Annotation.h>
#include <AnnotatorLib/Session.h>



namespace Ui {
class Popup;
}

static QVector< QVector<QString> > StartObjetcsList;
//static int iNr;

class Popup : public QDialog
{
    Q_OBJECT

public:
    explicit Popup(QWidget *parent = 0);
    ~Popup();


    bool isEditItem;
    QString itemName;
    static QString fileName;
    QString videoFilePath;
    QFileInfoList ImagesFilePaths;
    bool isImages;

    void setData(QPointF UL, QPointF LR, QString id, QString Name, QColor borderColor);

    void CreateXMLFile(long fileNr, long EndFrame);

    void setFileName(QString FileName);
    void setCurrFrame(long FrameNr);
    void setCurrFrame(long FrameNr, QString filename);

    void update(long fileNr);

    void showItem();
    void parseData(QString ULString, QString LRString, double arr[]);

    void updateItem(long start, long end);
    void showPopUp();

    void removeItem(QString id);
    bool isRAMEmpty();
    void removeRAM();

    void setSession(AnnotatorLib::Session *session);

private slots:
    void on_btnPopClose_clicked();

    void on_btnPopSave_clicked();

    void on_btnPopAddAtr_clicked();

    void setNewWidget(const QString &labelText, const QString &typeText);

    void on_btnPopLayoutReset_clicked();

private:
    Ui::Popup *ui;

    QVBoxLayout *layout;
    QWidget * contents;

    QList<QLineEdit*> widgetList;

    QPointF pointUL;
    QPointF pointLR;

    long iNr;

    QString Projectpath;
    QString FilePath;
    static QString dataPath;
    static QString dataDirPath;
    QXmlStreamReader Rxml;

    static long frameNr;
    QString ItemID;
    QColor ItemColor;

    bool notFound;
    long startFr;
    long endFr;
    long startFrame;
    long endFrame;
    bool inRAM;
    double *dataArray;


    long previousStart;
    long previousEnd;
    long splitterFrame;
    QString relatedID;

    bool isSplitter;
    bool isLayoutEmpty;
    QString numbering;
    long startItem;
    long endItem;

    AnnotatorLib::Session *session = nullptr;
    void putInRAM(QString x, QString y, int id, QString borderColor);
    long getObjectData(QString FileName, double array[]);
    QString removeFileName(QString Projectpath);
    void editObject(QString FileName, QString ID, QString TagName, QString Value);


    void setDataXML(long EndFrame);
    void insertSEQ(QString FileName, QString ID, QString Start, QString End);


    void editItem();
    long getItemData(QString ID, long &startOut, long &endOut);
    void removeItemFromData(QString ID);
    bool hasSplitter(QString ID, long frameNr);
    void clearLayout(QLayout* layout);


signals:
    void removeLastItem();
    void saveIDsettings();
    void paintItem(QPointF point1, QPointF point2, int id, QColor borderColor);

    void newAnnotation(AnnotatorLib::Annotation *annotation);
};

#endif // POPUP_H
