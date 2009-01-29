
#ifndef SQUID_SRC_TEST_ACLMAXUSERIP_H
#define SQUID_SRC_TEST_ACLMAXUSERIP_H

#include <cppunit/extensions/HelperMacros.h>

/*
 * demonstration test file, as new idioms are made they will
 * be shown in the testBoilerplate source.
 */

class testACLMaxUserIP : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( testACLMaxUserIP );
    /* note the statement here and then the actual prototype below */
    CPPUNIT_TEST( testDefaults );
    CPPUNIT_TEST( testParseLine );
    CPPUNIT_TEST_SUITE_END();

public:

protected:
    void testDefaults();
    void testParseLine();
};

#endif /* SQUID_SRC_TEST_ACLMAXUSERIP_H */

