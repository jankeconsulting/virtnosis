/**
 * Copyright (c) 2014 Ralph Janke. All rights reserved.
 *
 * Author: Ralph Janke virtnosis@jankeconsulting.ca
 */

#ifndef DOMAINVIEWITEMDELEGATE_H
#define DOMAINVIEWITEMDELEGATE_H

#include <QAbstractItemDelegate>

class DomainViewItemDelegate : public QAbstractItemDelegate
{
    Q_OBJECT
public:
    explicit DomainViewItemDelegate(QObject *parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const;

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index ) const;

signals:

public slots:

};

#endif // DOMAINVIEWITEMDELEGATE_H
