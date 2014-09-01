/**
 * Copyright (c) 2014 Ralph Janke. All rights reserved.
 *
 * Author: Ralph Janke virtnosis@jankeconsulting.ca
 */

#ifndef DOMAINITEM_H
#define DOMAINITEM_H

#include <QStandardItem>

class DomainItem : public QStandardItem
{

public:
    explicit DomainItem(const QString &text);

    enum datarole {
                   domainNameRole = Qt::UserRole + 100,
                   domainStateRole = Qt::UserRole + 101,
                   domainIconRole = Qt::UserRole + 102
                  };

private:

};

#endif // DOMAINITEM_H
