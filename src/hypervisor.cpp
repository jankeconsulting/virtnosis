/**
 * Copyright (c) 2014 Ralph Janke. All rights reserved.
 *
 * Author: Ralph Janke virtnosis@jankeconsulting.ca
 */

#include "hypervisor.h"
#include <QDebug>

Hypervisor::Hypervisor(QObject *parent) :
    QObject(parent),
    m_connection(0),
    m_libVersion(0),
    auto_connect(false)
{
    this->host = "";
    this->account = "";
    this->port = 22;
    this->protocol = "qemu+ssh";
    this->path = "system";
}

Hypervisor::Hypervisor(const Hypervisor &hypervisor) :
    QObject(),
    m_connection(0),
    m_libVersion(0),
    auto_connect(false)
{
    setParent(hypervisor.parent());
    this->host = hypervisor.host;
    this->account = hypervisor.account;
    this->port = hypervisor.port;
    this->protocol = hypervisor.protocol;
    this->path = hypervisor.path;
    this->m_connection = hypervisor.m_connection;
    this->auto_connect = hypervisor.auto_connect;
}

Hypervisor::Hypervisor(QString host, QString user, int port, QString protocol, QString path, QObject *parent) :
    QObject(parent),
    m_connection(0),
    m_libVersion(0)
{
    this->host = host;
    this->account = user;
    this->port = port;
    this->protocol = protocol;
    this->path = path;
    connection();
}

Hypervisor::~Hypervisor()
{
//    virConnectClose(m_connection);
}

void Hypervisor::write(QDataStream &out) const
{
    out << protocol << account << host << port << path << auto_connect;
}

void Hypervisor::read(QDataStream &in)
{
    in >> protocol >> account >> host >> port >> path >> auto_connect;
}

QString Hypervisor::uri()
{
    return uri(host, account, port, protocol, path);
}

QString Hypervisor::uri(QString host, QString account, int port, QString protocol, QString path)
{
    QString result;
    if(account.isEmpty()) {
        result = protocol+"://"+host+":"+QString("%1").arg(port)+"/"+path;
    } else {
        result = protocol+"://"+account+"@"+host+":"+QString("%1").arg(port)+"/"+path;
    }
    return result;
}

QString Hypervisor::name()
{
    return host;
}

virConnectPtr Hypervisor::connection()
{
//    TODO: error handling if connection fails
    if(alive() < 1)
      m_connection = virConnectOpen(this->uri().toLatin1().data());
#ifdef DEBUG
    qDebug() << "Hypervisor::connection: version = " << version();
    qDebug() << "Hypervisor::connection: libVersion = " << libVersion();
#endif
   return m_connection;
}

void Hypervisor::disconnect()
{
    virConnectClose(m_connection);
    m_connection = 0;
    auto_connect = false;
}

int Hypervisor::alive()
{
    if(m_connection)
        return virConnectIsAlive(m_connection);
    return 0;
}

QList<Domain *> Hypervisor::domains()
{
    virDomainPtr *domains;
    Domain *domain;
    int number_of_domains;
    int i;
    QList<Domain *> list = QList<Domain *>();
    ulong version = libVersion();

    if(version > 8000) {
        number_of_domains = virConnectListAllDomains(m_connection, &domains, 0);

        if (number_of_domains < 1) {
            return list;
        }

        for (i = 0; i < number_of_domains; i++) {
            domain = new Domain(domains[i], version);
            list.append(domain);
            virDomainFree(domains[i]);
        }

        free(domains);
    } else {
        number_of_domains = virConnectNumOfDomains(m_connection);
        int domain_ids[number_of_domains];
        number_of_domains = virConnectListDomains(m_connection, domain_ids, number_of_domains);

        if (number_of_domains < 1) {
            return list;
        }

        for (i = 0; i < number_of_domains; i++) {
            domain = new Domain(virDomainLookupByID(m_connection, domain_ids[i]), version);
            list.append(domain);
        }

        number_of_domains = virConnectNumOfDefinedDomains(m_connection);
        char *names[number_of_domains];
        number_of_domains = virConnectListDefinedDomains(m_connection, names, number_of_domains);

        if (number_of_domains < 1) {
            return list;
        }

        for (i = 0; i < number_of_domains; i++) {
            domain = new Domain(virDomainLookupByName(m_connection, names[i]), version);
            list.append(domain);
            free(names[i]);
        }
    }

    return list;
}

void Hypervisor::setAutoConnect(bool enable)
{
    auto_connect = enable;
}

bool Hypervisor::autoConnect()
{
    return auto_connect;
}

ulong Hypervisor::version()
{
    ulong hvVer;
    int error = virConnectGetVersion(m_connection, &hvVer);
    if (error)
        return -1;

    return hvVer;
}

ulong Hypervisor::libVersion()
{
    if(m_libVersion) return m_libVersion;

    ulong libVer;
    int error = virConnectGetLibVersion(m_connection, &libVer);
    if (error)
        return -1;

    return libVer;
}

QString Hypervisor::capabilities()
{
    if(alive()) {
        char *str = virConnectGetCapabilities(m_connection);
        QString result = QString(str);
        free(str);
        return result;
    }
    return QString("");
}

QDataStream &operator<<(QDataStream &out, const Hypervisor &hypervisor)
{
    hypervisor.write(out);
    return out;
}


QDataStream &operator>>(QDataStream &in, Hypervisor &hypervisor)
{
    hypervisor.read(in);
    return in;
}
