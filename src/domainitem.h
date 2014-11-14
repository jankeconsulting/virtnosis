/**
 * Copyright (c) 2014 Ralph Janke. All rights reserved.
 *
 * Author: Ralph Janke virtnosis@jankeconsulting.ca
 */

#ifndef SRC_DOMAINITEM_H_
#define SRC_DOMAINITEM_H_

#include <QStandardItem>
#include "./hypervisor.h"

/**
 * @brief The DomainItem class
 */
class DomainItem : public QStandardItem
{
 public:
    explicit DomainItem(const QString &text);

 private:
};

#endif  // SRC_DOMAINITEM_H_
