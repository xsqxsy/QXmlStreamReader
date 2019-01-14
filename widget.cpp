#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#define cout qDebug() << "[" << __LINE__ << "]"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowTitle("xml reader");
    resize(800, 600);

    QStringList header;
    header << "entry" << "pages";
    ui->treeWidget->setHeaderLabels(header);

    QHeaderView* view = ui->treeWidget->header();
    view->setSectionResizeMode(QHeaderView::ResizeToContents);


    readFile("../QXmlStreamReader/sample.xml");
}

Widget::~Widget()
{
    delete ui;
}

bool Widget::readFile(const QString &fileName)
{
    bool ret = true;

    QFile file(fileName);
    bool isOk = file.open(QIODevice::Text | QIODevice::ReadOnly);
    if(isOk)
    {
        reader.setDevice(&file);
        while(!reader.atEnd())
        {
            if(reader.isStartElement())
            {
                if("bookindex" == reader.name())
                {
                    readBookindexElement();
                }
                else
                {
                    reader.raiseError("xml document's root element is not bookindex!");
                }
                break;
            }
            else
            {
                reader.readNext();
            }
        }
        file.close();
    }
    else
    {
        cout << "failed to open file!";
        ret = false;
    }

    return ret;
}

void Widget::readBookindexElement()
{
    reader.readNext();
    while(!reader.atEnd())
    {
        if(reader.isEndElement())
        {
            break;
        }

        if(reader.isStartElement())
        {
            if("entry" == reader.name())
            {
                readEntryElement(ui->treeWidget->invisibleRootItem());
            }
            else
            {
                skipUnknownElement();
            }
        }
        reader.readNext();
    }

}

void Widget::readEntryElement(QTreeWidgetItem *parent)
{
    QTreeWidgetItem* item = new QTreeWidgetItem(parent);
    item->setText(0, reader.attributes().value("term").toString());

    reader.readNext();
    while(!reader.atEnd())
    {
        if(reader.isEndElement())
        {
            break;
        }
        if(reader.isStartElement())
        {
            if("entry" == reader.name())
            {
                readEntryElement(item);
            }
            else if("page" == reader.name())
            {
                readPageElement(item);
            }
            else
            {
                skipUnknownElement();
            }
        }

        reader.readNext();

    }
}

void Widget::readPageElement(QTreeWidgetItem *parent)
{
    QString page = reader.readElementText();

    QString allPages = parent->text(1);
    if (!allPages.isEmpty())
    {
        allPages += ", ";
    }
    allPages += page;
    parent->setText(1, allPages);
}

void Widget::skipUnknownElement()
{

}
