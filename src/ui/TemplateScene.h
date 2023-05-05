#pragma once

#include <QGraphicsScene>

class MatchResultItem;

class TemplateScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit TemplateScene(QObject* parent = nullptr);
    ~TemplateScene();

    void setImage(const QImage& image);

    void clearResults();

    void addMatchResultItem(MatchResultItem* resultItem);

    QList<MatchResultItem*> results() const { return m_resultItems; }

private:
    QGraphicsPixmapItem* m_pixmapItem;
    QList<MatchResultItem*> m_resultItems;
};
