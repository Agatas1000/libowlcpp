/** @file "/owl_cpp/include/owl_cpp/io/config.hpp"
part of owl_cpp project.
Distributed under GNU General Public License; see doc/license.txt.
@date 2011 @author Mikhail K Levin
*******************************************************************************/
#ifndef OWLCPP_IO_CONFIG_HPP_
#define OWLCPP_IO_CONFIG_HPP_

#include "owl_cpp/config.hpp"

#ifndef BOOST_SYMBOL_EXPORT
  #define BOOST_SYMBOL_EXPORT
  #define BOOST_SYMBOL_IMPORT
#endif
#if defined(OWLCPP_DYN_LINK) || defined(OWLCPP_IO_DYN_LINK)
#  if defined(OWLCPP_IO_SOURCE)
#     define OWLCPP_IO_DECL BOOST_SYMBOL_EXPORT
#  else
#     define OWLCPP_IO_DECL BOOST_SYMBOL_IMPORT
#  endif
#else
#  define OWLCPP_IO_DECL
#endif

#endif /* OWLCPP_IO_CONFIG_HPP_ */
