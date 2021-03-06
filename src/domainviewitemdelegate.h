/**
 * Copyright (c) 2014 Ralph Janke. All rights reserved.
 *
 * Author: Ralph Janke virtnosis@jankeconsulting.ca
 */

#ifndef SRC_DOMAINVIEWITEMDELEGATE_H_
#define SRC_DOMAINVIEWITEMDELEGATE_H_

#include <QStyledItemDelegate>
#include <QPainter>
#include <QApplication>
#include "./domainitem.h"
#include "./domainviewmodel.h"

/**
 * @brief The DomainViewItemDelegate class
 */
class DomainViewItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
 public:
    explicit DomainViewItemDelegate(QObject *parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const;

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const;

 signals:

 public slots:

 private:
    QString stateText(const QVariant state) const;
    QIcon stateIcon(const QVariant state) const;
    QString connectionText(const QVariant state) const;
    QIcon connectionIcon(const QVariant state) const;
};

#endif  // SRC_DOMAINVIEWITEMDELEGATE_H_
