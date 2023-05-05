#include "TemplateScene.h"

#include <QGraphicsItem>

#include "MatchResultItem.h"

TemplateScene::TemplateScene(QObject* parent)
    : QGraphicsScene(parent)
    , m_pixmapItem(nullptr)
{
}

TemplateScene::~TemplateScene()
{
}

void TemplateScene::setImage(const QImage& image)
{
    if (m_pixmapItem)
    {
        m_pixmapItem->setPixmap(QPixmap::fromImage(image));
    }
    else
    {
        m_pixmapItem = addPixmap(QPixmap::fromImage(image));
    }
}

void TemplateScene::clearResults()
{
    for (MatchResultItem* item : m_resultItems)
    {
        removeItem(item);
    }
    m_resultItems.clear();
}

void TemplateScene::addMatchResultItem(MatchResultItem* resultItem)
{
    addItem(resultItem);
    m_resultItems.append(resultItem);
}
