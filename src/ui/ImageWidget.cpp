#include "ImageWidget.h"

#include <QPainter>

ImageWidget::ImageWidget(QGraphicsObject* parent)
    : QGraphicsObject(parent)
{

}

ImageWidget::~ImageWidget()
{

}

void ImageWidget::setImage(QImage& image)
{
    m_image = image;
}

void ImageWidget::addMatchUnit(const MatchUnit& unit)
{
    m_units.append(unit);
}

void ImageWidget::clearMatchUnit()
{
    m_units.clear();
}

QRectF ImageWidget::boundingRect() const
{
    return m_image.rect();
}

void ImageWidget::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    painter->drawImage(m_image.rect(), m_image);
}


