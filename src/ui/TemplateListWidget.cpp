#include "TemplateListWidget.h"

#include <QBoxLayout>

TemplateListWidget::TemplateListWidget(QWidget* parent)
    : QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout;
    m_title = new QLabel;
    m_label = new QLabel;
    m_label->setMaximumSize(256, 256);
    layout->addWidget(m_title);
    layout->addWidget(m_label);
    layout->setStretch(0, 0);
    layout->setStretch(1, 1);

    setLayout(layout);
}

TemplateListWidget::~TemplateListWidget()
{
}

void TemplateListWidget::setTitle(const QString& title)
{
    m_title->setText(title);
}

QString TemplateListWidget::title() const
{
    return m_title->text();
}

void TemplateListWidget::setImage(const QImage& image)
{
    m_label->setPixmap(QPixmap::fromImage(image));
}

QImage TemplateListWidget::image() const
{
    return m_label->pixmap()->toImage();
}

void TemplateListWidget::setFileInfo(const QFileInfo& fileInfo)
{
    m_fileInfo = fileInfo;
}

QFileInfo TemplateListWidget::fileInfo() const
{
    return m_fileInfo;
}
