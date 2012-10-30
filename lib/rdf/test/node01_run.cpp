/** @file "/owlcpp/lib/rdf/test/node01_run.cpp" 
part of owlcpp project.
Distributed under GNU General Public License; see doc/license.txt.
@date 2012 @author Mikhail K Levin
*******************************************************************************/
#define BOOST_TEST_MODULE node01_run
#include <iostream>
#include <vector>
#include <memory>
#include "boost/assign/std/vector.hpp"
#include "boost/foreach.hpp"
#include "boost/test/unit_test.hpp"
#include "boost/range/algorithm/sort.hpp"
#include "boost/range/algorithm/unique.hpp"
#include "test/exception_fixture.hpp"
#include "owlcpp/rdf/node_iri.hpp"
#include "owlcpp/rdf/node_literal.hpp"
#include "owlcpp/rdf/node_blank.hpp"
#include "owlcpp/rdf/hash_node.hpp"
#include "owlcpp/rdf/print_node.hpp"
#include "owlcpp/terms/node_tags_owl.hpp"

namespace owlcpp{ namespace test{

BOOST_GLOBAL_FIXTURE( Exception_fixture );

/** IRI nodes
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_iri_node ) {
   const Node_iri n1(Ns_id(42), "blah");
   const Node_iri n2(Ns_id(41), "blah");
   BOOST_CHECK_NE(n1, n2);

   BOOST_CHECK_THROW(Node_iri(owlcpp::terms::N_blank::id(), "blah"), Rdf_err);

   const Node_iri n3(Ns_id(42), "blahblah");
   BOOST_CHECK_NE(n1,  n3);
   BOOST_CHECK_NE(n2,  n3);

   const Node_iri n4(Ns_id(42), "blah");
   BOOST_CHECK_EQUAL(n1,  n4);

   const Node_iri n5(owlcpp::terms::N_empty::id(), "");
   BOOST_CHECK(n5.empty());
   const Node_iri n6;
   BOOST_CHECK(n6.empty());
   BOOST_CHECK_EQUAL(n5,  n6);

   const Node_iri n7(owlcpp::terms::N_empty::id(), "blah");
   BOOST_CHECK_NE(n5,  n7);
}

/** clone IRI nodes
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_clone_iri_node ) {
   const Node_iri n1(Ns_id(42), "blah");
   std::auto_ptr<Node> pn1(n1.clone());
   BOOST_CHECK_EQUAL(n1, *pn1);

   const Node_iri n5(owlcpp::terms::N_empty::id(), "");
   std::auto_ptr<Node> pn5(n5.clone());
   BOOST_CHECK_EQUAL(n5, *pn5);
}

/** Literal string nodes
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_literal_string_node ) {
   const Node_string n1("blah", Node_id(42), "en");
   BOOST_CHECK_EQUAL(n1.ns_id(), owlcpp::terms::N_empty::id());
   const Node_string n2("blah", Node_id(42), "en");
   BOOST_CHECK_EQUAL(n1,  n2);
   BOOST_CHECK_EQUAL(n1.hash(), n2.hash());

   const Node_string n3("blahh", Node_id(42), "en");
   BOOST_CHECK_NE(n1,  n3);
   BOOST_CHECK_NE(n1.hash(), n3.hash());

   const Node_string n4("blah", Node_id(41), "en");
   BOOST_CHECK_NE(n1,  n4);
   BOOST_CHECK_NE(n1.hash(), n4.hash());

   const Node_string n5("blah", Node_id(42), "fr");
   BOOST_CHECK_NE(n1,  n5);
   BOOST_CHECK_NE(n1.hash(), n5.hash());
}

/** Literal bool nodes
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_literal_bool_node ) {
   BOOST_CHECK_THROW(Node_bool("falsee"), Rdf_err);
   BOOST_CHECK(   Node_bool("true").value() );
   BOOST_CHECK( ! Node_bool("false").value() );
   BOOST_CHECK(   Node_bool("1").value() );
   BOOST_CHECK( ! Node_bool("0").value() );
   BOOST_CHECK(   Node_bool(true).value() );
   BOOST_CHECK( ! Node_bool(false).value() );
   BOOST_CHECK_THROW(Node_bool(42), Rdf_err);
   BOOST_CHECK_THROW(Node_bool(-1), Rdf_err);
   BOOST_CHECK_THROW(Node_bool("42"), Rdf_err);
   BOOST_CHECK(   Node_bool(1).value() );
   BOOST_CHECK( ! Node_bool(0).value() );
}

/** Literal int nodes
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_literal_int_node ) {
   BOOST_CHECK_THROW(Node_int("blah"), Rdf_err);
   BOOST_CHECK_EQUAL(Node_int("-1").value(), -1);

   BOOST_CHECK_EQUAL(Node_int(0.9).value(), 0);
   BOOST_CHECK_THROW(Node_int("0.9"), Rdf_err);
   BOOST_CHECK_EQUAL(Node_int(1e10).value(), 1e10);
   BOOST_CHECK_THROW(Node_int("1e10"), Rdf_err);
}

/** Literal unsigned nodes
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_literal_unsigned_node ) {
   BOOST_CHECK_THROW(Node_unsigned(-1), Rdf_err);
   BOOST_CHECK_THROW(Node_unsigned("-1"), Rdf_err);

   BOOST_CHECK_EQUAL(Node_unsigned(1e10).value(), 1e10);
}

/** Literal real nodes
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_literal_real_node ) {
   BOOST_CHECK_THROW(Node_double("blah"), Rdf_err);
   BOOST_CHECK_EQUAL(Node_double("-1").value(), -1.0);

   BOOST_CHECK_EQUAL(Node_double(0.9).value(), 0.9);
   BOOST_CHECK_EQUAL(Node_double("-0.9").value(), -0.9);
   BOOST_CHECK_EQUAL(Node_double(1e10).value(), 1e10);
   BOOST_CHECK_EQUAL(Node_double("1e10").value(), 1e10);
}

/** Blank nodes
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_blank_node ) {
   const Node_blank n1(0, Doc_id(42));
   BOOST_CHECK_EQUAL(n1.ns_id(), owlcpp::terms::N_blank::id());
   const Node_blank n2(0, Doc_id(42));
   BOOST_CHECK_EQUAL(n1,  n2);

   const Node_blank n3(1, Doc_id(42));
   BOOST_CHECK_NE(n1,  n3);

   const Node_blank n4(0, Doc_id(41));
   BOOST_CHECK_NE(n1,  n4);
}

/** Compare nodes
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_compare ) {
   const Node_iri n1(Ns_id(42), "blah");
   const Node_blank n2(1, Doc_id(42));
   BOOST_CHECK_NE(n1,  n2);

   const Node_string n3("blah", Node_id(42));
   BOOST_CHECK_NE(n1,  n3);

   const Node_string n4("");
   BOOST_CHECK_NE(n1,  n4);

   const Node_iri n5;
   BOOST_CHECK_NE(n5,  n4);
}

/** node hash
*******************************************************************************/
BOOST_AUTO_TEST_CASE( test_hash ) {
   const Node_string n1("blah", Node_id(42), "en");
   const Node_string n2("blah", Node_id(42), "fr");
   const Node_string n3("blah", Node_id(42), "en");
   const Node_iri n4(Ns_id(42), "blah");
   const Node_blank n5(0, Doc_id(42));
   const Node_blank n6(42, Doc_id(0));
   const Node_blank n7(1, Doc_id(42));
   const Node_string n8("", Node_id(0));
   const Node_blank n9(0, Doc_id(0));

   std::vector<std::size_t> h;
   std::vector<Node const*> v;
   using namespace boost::assign;
   v += &n1, &n2, &n3, &n4, &n5, &n6, &n7, &n8, &n9;
   BOOST_CHECK_EQUAL(hash_value(*v[0]), hash_value(n1));
   BOOST_CHECK_EQUAL(hash_value(*v[0]), hash_value(*v[2]));

   BOOST_FOREACH(Node const* n, v) {
      h.push_back(hash_value(*n));
   }
   BOOST_CHECK_EQUAL(boost::unique(boost::sort(h)).size(), 8);
}

}//namespace test
}//namespace owlcpp
