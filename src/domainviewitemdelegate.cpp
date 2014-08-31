/**
 * Copyright (c) 2014 Ralph Janke. All rights reserved.
 *
 * Author: Ralph Janke virtnosis@jankeconsulting.ca
 */

#include "domainviewitemdelegate.h"

DomainViewItemDelegate::DomainViewItemDelegate(QObject *parent) :
    QAbstractItemDelegate(parent)
{
}

void DomainViewItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

}

QSize DomainViewItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{

}
