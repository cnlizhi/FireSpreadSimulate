#include "QFSSWidget.h"
#include "ui_QFSSWidget.h"
#include "CRSImage.h"
#include "CImgProcess.h"
#include <QString>
#include <QFileDialog>
#include <QErrorMessage>
#include <QMessageBox>

Widget::Widget(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowTitle("Fire Spread Simulate");
    this->resize(742, 800);
    m_OpenFile_Btn = new QPushButton("OpenFile");
    m_Clear_Btn = new QPushButton("Clear");
    m_Area_Btn = new QPushButton("Area");
    m_OpenFile_Btn->resize(this->width(), 50);
    m_Clear_Btn->resize(this->width(), 50);
    m_Area_Btn->resize(this->width(), 50);
    connect(m_OpenFile_Btn, SIGNAL(clicked(bool)), this, SLOT(_OpenFile()));
    connect(m_Clear_Btn, SIGNAL(clicked(bool)), this, SLOT(_Clear()));
    connect(m_Area_Btn, SIGNAL(clicked(bool)), this, SLOT(_Area()));
    m_Label = new QLabel(this);
    m_Label->setPixmap(QPixmap::fromImage(m_Image));
    m_Label->resize(this->width(), this->height() - 150);
    m_Layout = new QVBoxLayout(this);
    m_Layout->addWidget(m_OpenFile_Btn);
    m_Layout->addWidget(m_Clear_Btn);
    m_Layout->addWidget(m_Area_Btn);
    m_Layout->addWidget(m_Label);
    m_Layout->addStretch();
    m_OpenFile_Btn->setEnabled(true);
    m_Clear_Btn->setEnabled(false);
    m_Area_Btn->setEnabled(false);
    m_isOpen = false;
}

Widget::~Widget()
{
    delete ui;
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    if (m_isOpen)
    {
        QPoint Pos = event->pos();
        QPoint LabelPos = m_Label->pos();
        ImgProcess.SetFire(Pos.x() - LabelPos.x(), Pos.y() - LabelPos.y());
        m_Image = ImgProcess.Display();
        m_Label->setPixmap(QPixmap::fromImage(m_Image));
        m_Label->show();
    }
    else
    {
        Q_UNUSED(event);
    }
}

void Widget::_OpenFile()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Please Choose Image."), "D:", tr("图片文件(*hdr *img)"));
    if (!RSImage.OpenImage(path))
    {
        QErrorMessage* Dialog = new QErrorMessage(this);
        Dialog->setWindowTitle(tr("Error！"));
        Dialog->showMessage(tr("Open Image Failed."));
    }
    else
    {
        if (!ImgProcess.Set(RSImage))
        {
            QErrorMessage* Dialog = new QErrorMessage(this);
            Dialog->setWindowTitle(tr("Error！"));
            Dialog->showMessage(tr("Set Image Process Failed."));
        }
        m_Image = ImgProcess.Display();
        m_Label->setPixmap(QPixmap::fromImage(m_Image));
        m_OpenFile_Btn->setEnabled(false);
        m_Clear_Btn->setEnabled(true);
        m_Area_Btn->setEnabled(true);
    }
    m_isOpen = true;
}

void Widget::_Clear()
{
    ImgProcess.ReSet();
    if (!ImgProcess.Set(RSImage))
    {
        QErrorMessage* Dialog = new QErrorMessage(this);
        Dialog->setWindowTitle(tr("Error！"));
        Dialog->showMessage(tr("Set Image Process Failed."));
    }
    m_Image = ImgProcess.Display();
    m_Label->setPixmap(QPixmap::fromImage(m_Image));
    m_Label->show();
}

void Widget::_Area()
{
    QMessageBox::about(this, "Spread Area", QString::number(ImgProcess.GetArea()));
}
