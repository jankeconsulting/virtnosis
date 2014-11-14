/**
 * Copyright (c) 2014 Ralph Janke. All rights reserved.
 *
 * Author: Ralph Janke virtnosis@jankeconsulting.ca
 */

#ifndef HYPERVISOR_H
#define HYPERVISOR_H

#include <QObject>
#include <QMetaType>
#include <QString>
#include "libvirt/libvirt.h"
#include "libvirt/virterror.h"
#include "domain.h"

/**
 * @brief The Hypervisor class
 */
class Hypervisor : public QObject
{
    Q_OBJECT

public:
    explicit Hypervisor(QObject *parent = 0);
    Hypervisor(const Hypervisor& hypervisor);
    explicit Hypervisor(QString host, QString user, int port = 22, QString protocol = "qemu+ssh", QString path = "system", QObject *parent = 0);
    ~Hypervisor();
    void write(QDataStream &out) const;
    void read(QDataStream &in);

    QString uri();
    static QString uri(QString host, QString account, int port, QString protocol = "qemu+ssh", QString path = "system");
    QString name();
    virConnectPtr connection();
    void disconnect();
    int alive();
    QList<Domain *> domains();
    void setAutoConnect(bool enable);
    bool autoConnect();


private:
    QString protocol;
    QString account;
    QString host;
    int port;
    QString path;
    int status;
    bool auto_connect;

    virConnectPtr m_connection;
    ulong m_libVersion;

    ulong version();
    ulong libVersion();
    QString capabilities();
};

Q_DECLARE_METATYPE(Hypervisor)

#ifndef QT_NO_DATASTREAM
QDataStream &operator<<(QDataStream &out, const Hypervisor &hypervisor);
QDataStream &operator>>(QDataStream &in, Hypervisor &hypervisor);
#endif

#endif // HYPERVISOR_H
