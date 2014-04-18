#ifndef JSONPARSETEST_H
#define JSONPARSETEST_H

#include <cppunit/extensions/HelperMacros.h>

class JSONParseTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( JSONParseTest );
  CPPUNIT_TEST( testConstructor );
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp();
  void tearDown();

  void testConstructor();
};

#endif  // JSONPARSETEST_H