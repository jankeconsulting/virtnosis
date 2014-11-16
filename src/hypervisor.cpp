/**
 * Copyright (c) 2014 Ralph Janke. All rights reserved.
 *
 * Author: Ralph Janke virtnosis@jankeconsulting.ca
 */

#include "hypervisor.h"
#include <QDebug>

/**
 * @brief contructs hypervisor
 * @param parent - parent object of the hypervisor created
 *
 * Constructs an empty hypervisor that cannot be connected
 */
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

/**
 * @brief contructs a hypervisor from another hypervisor object
 * @param hypervisor
 *
 * Constructs a hypervisor by copying it from an existing hypervisor object
 *
 * This contructor is necessary for declaring this class a Qt meta object
 */
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

/**
 * @brief constructs a hypervisor and connects to it
 * @param host - host of the hypervisor
 * @param user - user for the connection
 * @param port - port for the connection
 * @param protocol - protocol for the connection
 * @param path - path to find the hypervisor on the host
 * @param parent - parent object for the hypervisor object
 *
 * Constructs the Hypervisor object from the uri information given in its parts
 */
Hypervisor::Hypervisor(QString host, QString user, int port, QString protocol,
                       QString path, QObject *parent) :
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

/**
 * @brief destroys the hypervisor object
 */
Hypervisor::~Hypervisor()
{
// TODO(txwikinger): Do to copying, the m_connection needs to be
// a smart pointer to be only deleted when last reference is this object
//    virConnectClose(m_connection);
}

/**
 * @brief writes the hypervisor object into a serialized data stream
 * @param out - stream to write the data to
 *
 * This method is required for hypervisor be able to be used
 * in the settings.
 */
void Hypervisor::write(QDataStream &out) const
{
    out << protocol << account << host << port << path << auto_connect;
}

/**
 * @brief reads the hypervisor object data from a stream
 * @param in - stream from which the data be read
 *
 * This method is required for hypervisor be able to be used
 * in the settings.
 */
void Hypervisor::read(QDataStream &in)
{
    in >> protocol >> account >> host >> port >> path >> auto_connect;
}

/**
 * @brief creates the uri string from the object member data
 * @return uri string
 */
QString Hypervisor::uri()
{
    return uri(host, account, port, protocol, path);
}

/**
 * @brief creates the uri string from parts given in parameters
 * @param host - host of the hypervisor
 * @param account - user for the connection
 * @param port - port for the connection
 * @param protocol - protocol for the connection
 * @param path - path to find the hypervisor on the host
 * @return uri string
 */
QString Hypervisor::uri(QString host, QString account, int port,
                        QString protocol, QString path)
{
    QString result;
    if (account.isEmpty())
    {
        result = protocol+"://"+host+":"+QString("%1").arg(port)+"/"+path;
    } else
    {
        result = protocol+"://"+account+"@"+host+":"+
                   QString("%1").arg(port)+"/"+path;
    }
    return result;
}

/**
 * @brief provides the host name of the hypervisor
 * @return host name
 */
QString Hypervisor::name()
{
    return host;
}

/**
 * @brief provides connection pointer
 * @return connection pointer
 *
 * Provides the pointer to libvirt data structure representing
 * the connection to the hypervisor
 *
 * If the connection is not alive, the hypervisor is re-connected
 */
virConnectPtr Hypervisor::connection()
{
//    TODO: error handling if connection fails
    if (alive() < 1)
    {
        m_connection = virConnectOpen(this->uri().toLatin1().data());
    }
#ifdef DEBUG
    qDebug() << "Hypervisor::connection: version = " << version();
    qDebug() << "Hypervisor::connection: libVersion = " << libVersion();
#endif
   return m_connection;
}

/**
 * @brief disconnects the hypervisor
 */
void Hypervisor::disconnect()
{
    virConnectClose(m_connection);
    m_connection = 0;
    auto_connect = false;
}

/**
 * @brief checks if the hypervisor connection is alive
 * @return
 */
int Hypervisor::alive()
{
    if (m_connection)
    {
        return virConnectIsAlive(m_connection);
    }
    return 0;
}

/**
 * @brief provides the list of domains of the hypervisor
 * @return list of domains
 *
 * Depending on the version of the libvirt library, a different
 * method is used.
 */
QList<Domain *> Hypervisor::domains()
{
    virDomainPtr *domains;
    Domain *domain;
    int number_of_domains;
    int i;
    QList<Domain *> list = QList<Domain *>();
    ulong version = libVersion();

    if (version > 8000)
    {
        number_of_domains = virConnectListAllDomains(m_connection, &domains, 0);

        if (number_of_domains < 1)
        {
            return list;
        }

        for (i = 0; i < number_of_domains; i++)
        {
            domain = new Domain(domains[i], version);
            list.append(domain);
            virDomainFree(domains[i]);
        }

        free(domains);
    } else
    {
        number_of_domains = virConnectNumOfDomains(m_connection);
        int domain_ids[number_of_domains];
        number_of_domains = virConnectListDomains(m_connection,
                                                  domain_ids,
                                                  number_of_domains);

        if (number_of_domains < 1)
        {
            return list;
        }

        for (i = 0; i < number_of_domains; i++)
        {
            domain = new Domain(virDomainLookupByID(m_connection, domain_ids[i]), version);
            list.append(domain);
        }

        number_of_domains = virConnectNumOfDefinedDomains(m_connection);
        char *names[number_of_domains];
        number_of_domains = virConnectListDefinedDomains(m_connection, names, number_of_domains);

        if (number_of_domains < 1)
        {
            return list;
        }

        for (i = 0; i < number_of_domains; i++)
        {
            domain = new Domain(virDomainLookupByName(m_connection, names[i]), version);
            list.append(domain);
            free(names[i]);
        }
    }

    return list;
}

/**
 * @brief sets the auto connect flag
 * @param enable - flag indicating autoconnect
 *
 * If the autoconnect flag is set, the hypervisor is automatically
 * connected after start-up of the application.
 */
void Hypervisor::setAutoConnect(bool enable)
{
    auto_connect = enable;
}

/**
 * @brief provides the autoconnect flag of the hypervisor
 * @return autoconnect flag
 */
bool Hypervisor::autoConnect()
{
    return auto_connect;
}

/**
 * @brief provides the version of hypervisor
 * @return version of hypervisor
 */
ulong Hypervisor::version()
{
    ulong hvVer;
    int error = virConnectGetVersion(m_connection, &hvVer);
    if (error)
    {
        return -1;
    }

    return hvVer;
}

/**
 * @brief provides the libvirt library version of hypervisor
 * @return libvirt version of hypervisor
 */
ulong Hypervisor::libVersion()
{
    if (m_libVersion)
    {
        return m_libVersion;
    }

    ulong libVer;
    int error = virConnectGetLibVersion(m_connection, &libVer);
    if (error)
    {
        return -1;
    }

    return libVer;
}

/**
 * @brief provides capabilities of hypervisor
 * @return returns capabilities or empty string if not connected
 */
QString Hypervisor::capabilities()
{
    if (alive())
    {
        char *str = virConnectGetCapabilities(m_connection);
        QString result = QString(str);
        free(str);
        return result;
    }
    return QString("");
}

/**
 * @brief overloading << operator to write hypervisor into stream
 * @param out - data stream to be written into
 * @param hypervisor - hypervisor to be written in stream
 * @return data stream
 *
 * This overloading is needed for writing this object into settings
 */
QDataStream &operator<<(QDataStream &out, const Hypervisor &hypervisor)
{
    hypervisor.write(out);
    return out;
}

/**
 * @brief overloading >> operator to write hypervisor into stream
 * @param in - data stream to be read from
 * @param hypervisor - hypervisor to be read from stream
 * @return data stream
 *
 * This overloading is needed for reading this object from settings
 */
QDataStream &operator>>(QDataStream &in, Hypervisor &hypervisor)
{
    hypervisor.read(in);
    return in;
}
