#pragma once

#include <QGraphicsObject>
#include <QGraphicsItem>

class MatchResultItem : public QGraphicsItem
{
public:
    explicit MatchResultItem(QGraphicsItem* parent = nullptr);
    ~MatchResultItem();

    void setBoundingRect(const QRectF& boundingRect) { m_boundingRect = boundingRect; }
    virtual QRectF boundingRect() const override;

    qreal score() const { return m_score; }
    void setScore(qreal score) { m_score = score; }

    qreal scoreThreshold() const { return m_scoreThreshold; }
    void setScoreThreshold(qreal scoreThreshold) { m_scoreThreshold = scoreThreshold; }

    qreal angleDegrees() const { return m_angleDegrees; }
    void setAngleDegrees(qreal angleDegrees) { m_angleDegrees = angleDegrees; }

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

private:
    QRectF m_boundingRect;
    qreal m_score;
    qreal m_scoreThreshold;
    qreal m_angleDegrees;

};
