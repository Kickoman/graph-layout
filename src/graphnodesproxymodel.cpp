#include "graphnodesproxymodel.h"

GraphNodesProxyModel::GraphNodesProxyModel(GraphLayout *parentModel)
    : parentModel(parentModel)
{
    connect(parentModel, &GraphLayout::positionsUpdated, this, &GraphNodesProxyModel::onModelUpdated);
    connect(parentModel, &GraphLayout::positionUpdated, this, &GraphNodesProxyModel::onPositionUpdated);
}

int GraphNodesProxyModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return parentModel->nodesCount();
}

QVariant GraphNodesProxyModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid())
    {
        switch (static_cast<Roles>(role))
        {
            case Roles::Data:       return parentModel->node(index.row());
            case Roles::XPosition:  return parentModel->getNodeXPosition(index.row());
            case Roles::YPosition:  return parentModel->getNodeYPosition(index.row());
        }
    }
    return QVariant();
}

QHash<int, QByteArray> GraphNodesProxyModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(static_cast<int>(Roles::Data), "data");
    roles.insert(static_cast<int>(Roles::XPosition), "x");
    roles.insert(static_cast<int>(Roles::YPosition), "y");
    return roles;
}

Qt::ItemFlags GraphNodesProxyModel::flags(const QModelIndex &index) const
{
    return QAbstractListModel::flags(index)/* | Qt::ItemFlag::ItemIsEditable*/;
}

bool GraphNodesProxyModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    const auto _role = static_cast<Roles>(role);
    if (_role == Roles::XPosition)
    {
        auto x_position = parentModel->getNodeXPosition(index.row());
        if (!qFuzzyCompare(x_position, value.toDouble()))
        {
            parentModel->setNodeXPosition(index.row(), value.toDouble());
            emit dataChanged(index, index, {role});
            return true;
        }
    }
    if (_role == Roles::YPosition)
    {
        auto y_position = parentModel->getNodeYPosition(index.row());
        if (!qFuzzyCompare(y_position, value.toDouble()))
        {
            parentModel->setNodeYPosition(index.row(), value.toDouble());
            emit dataChanged(index, index, {role});
            return true;
        }
    }
    return false;
}

void GraphNodesProxyModel::onModelUpdated()
{
    auto startIndex = createIndex(0, 0);
    auto finishIndex = createIndex(parentModel->nodesCount() - 1, 0);
    emit dataChanged(startIndex, finishIndex);
}

void GraphNodesProxyModel::onPositionUpdated(int index)
{
    emit dataChanged(createIndex(index, 0), createIndex(index, 0),
                     QVector<int>() << static_cast<int>(Roles::XPosition)
                                    << static_cast<int>(Roles::YPosition));
}
