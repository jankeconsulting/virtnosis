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
    painter->drawText(domainNameRect, qvariant_cast<QString>(index.data()));

    painter->setFont(stateFont);
    if(index.data(DomainViewModel::domainTypeRole) == DomainViewModel::typeDomain) {
        painter->drawText(domainStateRect, stateText(index.data(DomainViewModel::domainStateRole)));
    } else if(index.data(DomainViewModel::domainTypeRole) == DomainViewModel::typeHypervisor) {
        painter->drawText(domainStateRect, connectionText(index.data(DomainViewModel::hypervisorConnectedRole)));
    }

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
        case VIR_DOMAIN_NOSTATE: return QString(tr("No State"));
        case VIR_DOMAIN_RUNNING: return QString(tr("Running"));
        case VIR_DOMAIN_BLOCKED: return QString(tr("Blocked"));
        case VIR_DOMAIN_PAUSED: return QString(tr("Paused"));
        case VIR_DOMAIN_SHUTDOWN: return QString(tr("Shutdown"));
        case VIR_DOMAIN_SHUTOFF: return QString(tr("Shutoff"));
        case VIR_DOMAIN_CRASHED: return QString(tr("Crashed"));
        case VIR_DOMAIN_PMSUSPENDED: return QString(tr("Power Management Suspended"));
    }
    return QString(tr("Error"));
}

QString DomainViewItemDelegate::connectionText(const QVariant state) const
{
    switch(state.toInt()) {
        case 1: return QString(tr("Connected"));
        case 0: return QString(tr("Not Connected"));
    }
    return QString(tr("Error"));
}
