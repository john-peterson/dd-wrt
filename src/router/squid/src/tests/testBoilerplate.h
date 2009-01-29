
#ifndef SQUID_SRC_TEST_BOILDERPLATE_H
#define SQUID_SRC_TEST_BOILDERPLATE_H

#include <cppunit/extensions/HelperMacros.h>

/*
 * demonstration test file, as new idioms are made they will
 * be shown in the testBoilerplate source.
 */

class testBoilerplate : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( testBoilerplate );
    /* note the statement here and then the actual prototype below */
    CPPUNIT_TEST( testDemonstration );
    CPPUNIT_TEST_SUITE_END();

public:

protected:
    void testDemonstration();
};

#endif /* SQUID_SRC_TEST_BOILDERPLATE_H */

