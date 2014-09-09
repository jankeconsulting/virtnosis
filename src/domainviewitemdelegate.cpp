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
    painter->save();

    if (option.state & QStyle::State_Selected)
        painter->fillRect(option.rect, option.palette.highlight());

    QFont font = QApplication::font();
    QFontMetrics fm(font);
    QFont stateFont = QApplication::font();
    font.setBold(true);
    stateFont.setItalic(true);

    QRect domainNameRect = option.rect;
    QRect domainStateRect = option.rect;
    QRect domainIconRect = option.rect;

    domainNameRect.setTop(domainNameRect.top()+5);
    domainNameRect.setBottom(domainNameRect.top()+fm.height());

    domainStateRect.setTop(domainNameRect.bottom()+2);

    painter->setFont(font);
//    painter->drawText(domainNameRect, qvariant_cast<QString>(index.data(headerTextRole)));
    painter->drawText(domainNameRect, qvariant_cast<QString>(index.data()));

    painter->setFont(stateFont);
    painter->drawText(domainStateRect, stateText(index.data(DomainViewModel::domainStateRole)));

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

QString DomainViewItemDelegate::stateText(const QVariant state) const
{
    if(state.toString() == "") return ""; // for Hypervisor row
    switch(state.toInt()) {
        case 0: return QString("No State");
        case 1: return QString("Running");
        case 2: return QString("Blocked");
        case 3: return QString("Paused");
        case 4: return QString("Shutdown");
        case 5: return QString("Shutoff");
        case 6: return QString("Crashed");
        case 7: return QString("Power Management Suspended");
    }
    return "Error";
}
