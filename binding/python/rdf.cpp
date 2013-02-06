/** @file "/owlcpp/binding/python/rdf.cpp" 
part of owlcpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2011
*******************************************************************************/
//#include "boost/python/class.hpp"
//#include "boost/python/def.hpp"
//#include "boost/python/iterator.hpp"
#include "boost/python/module.hpp"
//#include "boost/python/operators.hpp"
//#include "boost/python/return_internal_reference.hpp"
//#include "boost/utility.hpp"

void export_ids();
void export_maps();
void export_triple_store();
void export_objects();

BOOST_PYTHON_MODULE(_rdf) {
   export_ids();
   export_maps();
   export_triple_store();
   export_objects();
}
