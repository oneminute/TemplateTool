#pragma once

#include <QGraphicsObject>
#include <QImage>
#include <QPixmap>
#include <QRectF>

struct MatchUnit
{
    QRectF bounding;
    qreal angle;
    qreal score;
};

class ImageWidget : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit ImageWidget(QGraphicsObject* parent = nullptr);
    ~ImageWidget();

    void setImage(QImage& image);

    void setPixmap(QPixmap& pixmap);

    void addMatchUnit(const MatchUnit& unit);

    void clearMatchUnit();

    virtual QRectF boundingRect() const override;

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;


private:
    QImage m_image;
    QList<MatchUnit> m_units;

    Q_DISABLE_COPY(ImageWidget)
};
