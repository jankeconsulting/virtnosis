/**
 * Copyright (c) 2014 Ralph Janke. All rights reserved.
 *
 * Author: Ralph Janke virtnosis@jankeconsulting.ca
 */

#include "hypervisor.h"


Hypervisor::Hypervisor(QObject *parent) :
    QObject(parent)
{
    this->host = "";
    this->account = "";
    this->port = 22;
    this->protocol = "qemu+ssh";
    this->path = "system";
}

Hypervisor::Hypervisor(QString host, QString user, int port, QString protocol, QString path, QObject *parent) :
    QObject(parent)
{
    this->host = host;
    this->account = user;
    this->port = port;
    this->protocol = protocol;
    this->path = path;
}

QString Hypervisor::uri()
{
    return uri(host, account, port, protocol, path);
}

QString Hypervisor::uri(QString host, QString account, int port, QString protocol, QString path)
{
    return protocol+"://"+account+"@"+host+":"+QString("%1").arg(port)+"/"+path;
}

QString Hypervisor::name()
{
    return host;
}

void Hypervisor::start()
{

}
