/**
 * Copyright (c) 2014 Ralph Janke. All rights reserved.
 *
 * Author: Ralph Janke virtnosis@jankeconsulting.ca
 */

#include "domainviewitemdelegate.h"
#include <QDebug>

/**
 * @brief constructs DomainViewItemDelegate object
 * @param parent - parent object
 */
DomainViewItemDelegate::DomainViewItemDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

/**
 * @brief paints an item in the view
 * @param painter - pointer to painter used
 * @param option - style options
 * @param index - index of the item that is painted
 *
 * Reimplemented method that creates the layout for the items displayed
 * in the view
 */
void DomainViewItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
//    Todo: Add stats for domains
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

    painter->setFont(stateFont);
    if(index.data(DomainViewModel::domainTypeRole) == DomainViewModel::typeDomain) {
        QIcon icon = stateIcon(index.data(DomainViewModel::domainStateRole));
        QSize iconsize = icon.actualSize(option.decorationSize);
        domainIconRect.setRight(iconsize.width()+30);
        domainIconRect.setTop(domainIconRect.top()+5);
        painter->drawPixmap(QPoint(domainIconRect.left(),domainIconRect.top()),icon.pixmap(iconsize.width()*2,iconsize.height()*2));
        domainStateRect.setLeft(domainStateRect.left()+iconsize.width()*2+5);
        domainNameRect.setLeft(domainNameRect.left()+iconsize.width()*2+5);
        painter->drawText(domainStateRect, stateText(index.data(DomainViewModel::domainStateRole)));
    } else if(index.data(DomainViewModel::domainTypeRole) == DomainViewModel::typeHypervisor) {
        QIcon icon = connectionIcon(index.data(DomainViewModel::hypervisorConnectedRole));
        QSize iconsize = icon.actualSize(option.decorationSize);
        domainIconRect.setRight(iconsize.width()+30);
        domainIconRect.setTop(domainIconRect.top()+5);
        painter->drawPixmap(QPoint(domainIconRect.left(),domainIconRect.top()),icon.pixmap(iconsize.width()*2,iconsize.height()*2));
        domainStateRect.setLeft(domainStateRect.left()+iconsize.width()*2+5);
        domainNameRect.setLeft(domainNameRect.left()+iconsize.width()*2+5);
        painter->drawText(domainStateRect, connectionText(index.data(DomainViewModel::hypervisorConnectedRole)));
    }

    painter->setFont(font);
    painter->drawText(domainNameRect, qvariant_cast<QString>(index.data()));

    painter->restore();
}

/**
 * @brief provides hint for sizing the item in view
 * @param option - style options
 * @param index - index of the item which size is needed
 * @return
 *
 * Reimplemented methods that helps to choose the right size for
 * the item display in view
 *
 * Currently option and index are not used for the calculation of the item
 */
QSize DomainViewItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

    QFont font = QApplication::font();
    QFontMetrics fm(font);

    return(QSize(32, fm.height()*2 + 8 ));
}

/**
 * @brief provides textual respresentation of state
 * @param state
 * @return string that contains the state in textual representation
 */
QString DomainViewItemDelegate::stateText(const QVariant state) const
{
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

/**
 * @brief provides icon that represents the given state
 * @param state
 * @return icon that represents the given state
 */
QIcon DomainViewItemDelegate::stateIcon(const QVariant state) const
{
    switch(state.toInt()) {
        case VIR_DOMAIN_NOSTATE: return QIcon();
        case VIR_DOMAIN_RUNNING: return QIcon("://icons/hicolor/32x32/status/state_running.png");
        case VIR_DOMAIN_BLOCKED: return QIcon();
        case VIR_DOMAIN_PAUSED: return QIcon("://icons/hicolor/32x32/status/state_paused.png");
        case VIR_DOMAIN_SHUTDOWN: return QIcon();
        case VIR_DOMAIN_SHUTOFF: return QIcon("://icons/hicolor/32x32/status/state_shutoff.png");
        case VIR_DOMAIN_CRASHED: return QIcon();
        case VIR_DOMAIN_PMSUSPENDED: return QIcon();
    }
    return QIcon();

}

/**
 * @brief provides the text for the connection state of a hypervisor state
 * @param state
 * @return text that represents the connection state
 */
QString DomainViewItemDelegate::connectionText(const QVariant state) const
{
    switch(state.toInt()) {
        case 1: return QString(tr("Connected"));
        case 0: return QString(tr("Not Connected"));
    }
    return QString(tr("Error"));
}

/**
 * @brief provides the icon for the connection state of a hypervisor state
 * @param state
 * @return icon that represents the connection state
 */
QIcon DomainViewItemDelegate::connectionIcon(const QVariant state) const
{
    switch(state.toInt()) {
        case 1: return QIcon("://icons/hicolor/32x32/status/connected.png");
        case 0: return QIcon("://icons/hicolor/32x32/status/disconnected.png");
    }
    return QIcon();
}
