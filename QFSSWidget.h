#ifndef QFSSWIDGET_H
#define QFSSWIDGET_H

#include "CRSImage.h"
#include "CImgProcess.h"
#include <QWidget>
#include <QPushButton>
#include <QLayout>
#include <QImage>
#include <QLabel>
#include <QMouseEvent>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
protected:
    void mousePressEvent(QMouseEvent *event);
private slots:
    void _OpenFile();
    void _Clear();
    void _Area();
private:
    Ui::Widget *ui;
    CRSImage RSImage;
    CImgProcess ImgProcess;
    QVBoxLayout* m_Layout;
    QPushButton* m_OpenFile_Btn;
    QPushButton* m_Clear_Btn;
    QPushButton* m_Area_Btn;
    QLabel* m_Label;
    QImage m_Image;
    bool m_isOpen;
};

#endif // QFSSWIDGET_H
