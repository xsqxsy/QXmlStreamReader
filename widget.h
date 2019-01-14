#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QFile>
#include <QXmlStreamReader>
#include <QTreeWidgetItem>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    bool readFile(const QString& fileName);

private:
    void readBookindexElement();
    void readEntryElement(QTreeWidgetItem* parent);
    void readPageElement(QTreeWidgetItem* parent);
    void skipUnknownElement();

private:
    Ui::Widget *ui;
    QXmlStreamReader reader;

};

#endif // WIDGET_H
