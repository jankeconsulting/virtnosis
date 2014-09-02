/**
 * Copyright (c) 2014 Ralph Janke. All rights reserved.
 *
 * Author: Ralph Janke virtnosis@jankeconsulting.ca
 */

#include "domainviewitemdelegate.h"
#include <QDebug>

DomainViewItemDelegate::DomainViewItemDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

void DomainViewItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    qDebug() << "DomainViewItemDelegate::paint: index:" << index;

    /*
    QStyledItemDelegate::paint(painter,option,index);

    painter->save();
    */

    if (option.state & QStyle::State_Selected)
        painter->fillRect(option.rect, option.palette.highlight());

    QFont font = QApplication::font();
    QFontMetrics fm(font);

    QRect domainNameRect = option.rect;
    QRect domainStateRect = option.rect;
    QRect domainIconRect = option.rect;

    domainNameRect.setTop(domainNameRect.top()+5);
    domainNameRect.setBottom(domainNameRect.top()+fm.height());

    domainStateRect.setTop(domainNameRect.bottom()+2);

//    painter->drawText(domainNameRect, qvariant_cast<QString>(index.data(headerTextRole)));
    painter->drawText(domainNameRect, qvariant_cast<QString>(index.data()));
    painter->drawText(domainStateRect, qvariant_cast<QString>(index.data(DomainItem::domainStateRole)));

    painter->restore();
}

QSize DomainViewItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

    QFont font = QApplication::font();
    QFontMetrics fm(font);

    return(QSize(100, fm.height()*2 + 8 ));
}
