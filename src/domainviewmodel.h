/**
 * Copyright (c) 2014 Ralph Janke. All rights reserved.
 *
 * Author: Ralph Janke virtnosis@jankeconsulting.ca
 */

#ifndef SRC_DOMAINVIEWMODEL_H_
#define SRC_DOMAINVIEWMODEL_H_

#include <QStandardItemModel>
#include <QVariant>
#include "./domainitem.h"
#include "./hypervisoritem.h"

/**
 * @brief The DomainViewModel class
 */
class DomainViewModel : public QStandardItemModel
{
    Q_OBJECT
 public:
    explicit DomainViewModel(QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    void connectHypervisor(const QModelIndex &index);
    void disconnectHypervisor(const QModelIndex &index);
    Hypervisor hypervisor(const int row) const;
    bool indexIsHypervisor(const QModelIndex &index) const;
    bool indexIsDomain(const QModelIndex &index) const;

    /**
     * @brief contains the possible roles for the item in the model
     */
    enum datarole {
        domainTypeRole = Qt::UserRole + 100,
        domainStateRole = Qt::UserRole + 101,
        domainHypervisorRole = Qt::UserRole + 102,
        domainDomainRole = Qt::UserRole + 103,
        hypervisorConnectedRole = Qt::UserRole + 104,
    };

    /**
     * @brief contains the possible types of the item in the model
     */
    enum datatype {
        typeHypervisor,
        typeDomain,
    };

 signals:

 public slots:

 private:
    Hypervisor hypervisor(const QModelIndex &index) const;
    void setHypervisor(const QModelIndex &index, Hypervisor *hypervisor);
    Domain domain(const QModelIndex &index) const;
};

#endif  // SRC_DOMAINVIEWMODEL_H_
