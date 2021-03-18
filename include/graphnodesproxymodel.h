#ifndef GRAPHNODESPROXYMODEL_H
#define GRAPHNODESPROXYMODEL_H

#include <QAbstractListModel>
#include "graphlayout.h"

class GraphNodesProxyModel : public QAbstractListModel
{
public:
    enum class Roles : int
    {
        Data = Qt::UserRole + 1,
        XPosition,
        YPosition
    };

    GraphNodesProxyModel(GraphLayout *parentModel);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

private slots:
    void onModelUpdated();
    void onPositionUpdated(int index);

private:
    GraphLayout *parentModel;
};

#endif // GRAPHNODESPROXYMODEL_H
