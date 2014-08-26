/**
 * Copyright (c) 2014 Ralph Janke. All rights reserved.
 *
 * Author: Ralph Janke virtnosis@jankeconsulting.ca
 */

#include <QString>
#include <QtTest>
#include "../../src/hypervisor.h"

class HypervisorTest : public QObject
{
    Q_OBJECT

public:
    HypervisorTest();

private Q_SLOTS:
    void testUri();
};

HypervisorTest::HypervisorTest()
{
}

void HypervisorTest::testUri()
{
    Hypervisor *hypervisor;
    hypervisor = new Hypervisor("test.example.org", "user", 33, "qemu+ssh", "system");
    QCOMPARE(hypervisor->uri(), QString("qemu+ssh://user@test.example.org:33/system"));
    delete hypervisor;
    hypervisor = 0; // Just for sanity

    hypervisor = new Hypervisor("test.example.org", "user");
    QCOMPARE(hypervisor->uri(), QString("qemu+ssh://user@test.example.org:22/system"));
    delete hypervisor;
    hypervisor = 0; // Just for sanity
}


QTEST_APPLESS_MAIN(HypervisorTest)

#include "tst_hypervisortest.moc"
