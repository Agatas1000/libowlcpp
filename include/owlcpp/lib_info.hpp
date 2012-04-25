/** @file "/owlcpp/include/owlcpp/lib_info.hpp" 
part of owlcpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2011

@mainpage %owlcpp library

@verbinclude readme.txt

@namespace owlcpp
@brief Main namespace of %owlcpp library

@namespace owlcpp::terms
@brief Definitions of standard OWL namespaces and terms
@details standard OWL namespace and term IRIs are represented by small tag classes.
Tags store namespace IRI, prefix, and name strings.
Each Tag also has a unique indices as compile-time constants.
Tag classes are generated by preprocessor macros from lists.
Tag type-lists are also generated by macros.

@namespace owlcpp::test
@brief Namespace for unit-tests

*******************************************************************************/
#ifndef LIB_INFO_HPP_
#define LIB_INFO_HPP_
#include <string>
#include <iosfwd>

namespace owlcpp{

/**@brief library info
*******************************************************************************/
struct Lib_info {
   static std::string const& name();
   static std::string const& version();
   static std::string const& description();
   static int version_1();
   static int version_2();
   static int version_3();
   static std::string const& version_e();
   static int build();

   template<class Ch, class Tr> static std::basic_ostream<Ch,Tr>&
   print(std::basic_ostream<Ch,Tr>& os) {
      os
      << name() << " - " << description()
      << ' ' << version()
      << " build:" << build()
      ;
      return os;
   }
};

}//namespace owlcpp
#endif /* LIB_INFO_HPP_ */
