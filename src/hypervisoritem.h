/**
 * Copyright (c) 2014 Ralph Janke. All rights reserved.
 *
 * Author: Ralph Janke virtnosis@jankeconsulting.ca
 */

#ifndef SRC_HYPERVISORITEM_H_
#define SRC_HYPERVISORITEM_H_

#include <QStandardItem>
#include "./hypervisor.h"
#include "./domainviewmodel.h"

/**
 * @brief The HypervisorItem class
 */
class HypervisorItem : public QStandardItem
{

 public:
    explicit HypervisorItem(Hypervisor *hypervisor);
    void addDomainsFromHypervisor(Hypervisor *hypervisor);    
    void removeDomainsFromItem(Hypervisor *hypervisor);

 private:
};

#endif  // SRC_HYPERVISORITEM_H_
