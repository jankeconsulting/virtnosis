/**
 * Copyright (c) 2014 Ralph Janke. All rights reserved.
 *
 * Author: Ralph Janke virtnosis@jankeconsulting.ca
 */

#include "hypervisor.h"
#include <QDebug>

Hypervisor::Hypervisor(QObject *parent) :
    QObject(parent),
    m_connection(0)
{
    this->host = "";
    this->account = "";
    this->port = 22;
    this->protocol = "qemu+ssh";
    this->path = "system";
}

Hypervisor::Hypervisor(const Hypervisor &hypervisor) :
    QObject(),
    m_connection(0)
{
    setParent(hypervisor.parent());
    this->host = hypervisor.host;
    this->account = hypervisor.account;
    this->port = hypervisor.port;
    this->protocol = hypervisor.protocol;
    this->path = hypervisor.path;
    this->m_connection = hypervisor.m_connection;
}

Hypervisor::Hypervisor(QString host, QString user, int port, QString protocol, QString path, QObject *parent) :
    QObject(parent),
    m_connection(0)
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
    return m_connection;
}

int Hypervisor::alive()
{
    return virConnectIsAlive(m_connection);
}
