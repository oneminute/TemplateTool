#pragma once

#include <QFileInfo>
#include <QLabel>
#include <QImage>
#include <QWidget>

class TemplateListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TemplateListWidget(QWidget* parent = nullptr);
    ~TemplateListWidget();

    void setTitle(const QString& title);
    QString title() const;

    void setImage(const QImage& image);
    QImage image() const;

    void setFileInfo(const QFileInfo& fileInfo);
    QFileInfo fileInfo() const;

private:
    QLabel* m_title;
    QLabel* m_label;
    QFileInfo m_fileInfo;
};
