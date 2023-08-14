#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "classa.h"
#include "classb.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private slots:
    void on_dataBtn_clicked();
    void on_sigBtn_clicked();
    void on_threadBtn_clicked();
    void on_memBtn_clicked();
private:
    Ui::Widget *ui;
    ClassA mClassA;
    ClassB mClassB;
};
#endif // WIDGET_H
