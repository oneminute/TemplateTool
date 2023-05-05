#include "MatchResultItem.h"

#include <QPainter>
#include <QPen>

MatchResultItem::MatchResultItem(QGraphicsItem* parent)
    : QGraphicsItem(parent)
    , m_score(1.0)
    , m_scoreThreshold(0.5)
    , m_angleDegrees(0)
{

}

MatchResultItem::~MatchResultItem()
{

}

QRectF MatchResultItem::boundingRect() const
{
    return m_boundingRect;
}

void MatchResultItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    QColor color;
    if (m_score >= m_scoreThreshold)
    {
        color = Qt::green;
    }
    else
    {
        color = Qt::red;
    }

    QPen pen(color, 1);
    pen.setCosmetic(true);
    painter->setPen(pen);
    painter->drawRect(m_boundingRect);
    QString scoreText = QString("%1").arg(m_score, 0, 'g', 2);
    QTextOption textOpt;
    textOpt.setAlignment(Qt::AlignCenter);
    painter->drawText(m_boundingRect, scoreText, textOpt);
}


