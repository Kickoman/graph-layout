#include "graphlineproxymodel.h"

GraphLineProxyModel::GraphLineProxyModel(GraphLayout *parentModel)
    : parentModel(parentModel)
{
    connect(parentModel, &GraphLayout::positionsUpdated, this, &GraphLineProxyModel::onModelUpdated);
    connect(parentModel, &GraphLayout::positionUpdated, this, &GraphLineProxyModel::onPositionUpdated);
}

int GraphLineProxyModel::rowCount(const QModelIndex & /*parent*/) const
{
    return parentModel->edgesCount();
}

QVariant GraphLineProxyModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid())
    {
        auto edge = parentModel->edge(index.row());

        switch (static_cast<Roles>(role))
        {
            case Roles::StartXPosition:  return parentModel->getNodeXPosition(edge.first);
            case Roles::StartYPosition:  return parentModel->getNodeYPosition(edge.first);
            case Roles::FinishXPosition: return parentModel->getNodeXPosition(edge.second);
            case Roles::FinishYPosition: return parentModel->getNodeYPosition(edge.second);
            case Roles::StartNodeIndex:  return edge.first;
            case Roles::FinishNodeIndex: return edge.second;
        }
    }
    return QVariant();
}

QHash<int, QByteArray> GraphLineProxyModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(static_cast<int>(Roles::StartXPosition), "startX");
    roles.insert(static_cast<int>(Roles::StartYPosition), "startY");
    roles.insert(static_cast<int>(Roles::FinishXPosition), "finishX");
    roles.insert(static_cast<int>(Roles::FinishYPosition), "finishY");
    roles.insert(static_cast<int>(Roles::StartNodeIndex), "startNodeIndex");
    roles.insert(static_cast<int>(Roles::FinishNodeIndex), "finishNodeIndex");
    return roles;
}

Qt::ItemFlags GraphLineProxyModel::flags(const QModelIndex &index) const
{
    return QAbstractListModel::flags(index) | Qt::ItemFlag::ItemIsEditable;
}

void GraphLineProxyModel::onModelUpdated()
{
    auto startIndex = createIndex(0, 0);
    auto finishIndex = createIndex(parentModel->edgesCount() - 1, 0);
    emit dataChanged(startIndex, finishIndex);
}

void GraphLineProxyModel::onPositionUpdated(int index)
{
    const auto cnt = parentModel->edgesCount();
    for (int i = 0; i < cnt; ++i)
    {
        auto edge = parentModel->edge(i);
        if (edge.first != index && edge.second != index) continue;
        emit dataChanged(createIndex(i, 0), createIndex(i, 0),
                         QVector<int>() << static_cast<int>(Roles::StartXPosition)
                                        << static_cast<int>(Roles::StartYPosition)
                                        << static_cast<int>(Roles::FinishXPosition)
                                        << static_cast<int>(Roles::FinishYPosition)
                         );
    }
}
