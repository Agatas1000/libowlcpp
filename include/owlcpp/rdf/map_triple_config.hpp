/** @file "/owlcpp/include/owlcpp/rdf/map_triple_config.hpp"
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2013
*******************************************************************************/
#ifndef MAP_TRIPLE_CONFIG_HPP_
#define MAP_TRIPLE_CONFIG_HPP_
#include "boost/mpl/vector.hpp"
#include "boost/mpl/at.hpp"

#include "owlcpp/rdf/detail/map_triple_config_macro.hpp"
#include "owlcpp/rdf/detail/triple_index_fwd.hpp"
#include "owlcpp/rdf/triple_tags.hpp"

#ifndef OWLCPP_TRIPLE_INDICES
#define OWLCPP_TRIPLE_INDICES \
         ((Subj, Obj, Pred, Doc)) \
         ((Pred, Subj, Obj, Doc))
/*
*/
#endif

namespace owlcpp{ namespace map_triple_detail{

/**@brief
*******************************************************************************/
template<class Id, class Set> class Fragment_map_vector;
template<class Id, class Set> class Fragment_map_ordered;

typedef OWLCPP_TRIPLE_INDEX_CONFIG(OWLCPP_TRIPLE_INDICES)
index_config_default;

/**@brief
*******************************************************************************/
template<class Tag0, class Tag1, class Tag2, class Tag3>
struct Triple_index_selector2 {
   typedef Triple_index<
            Fragment_map_vector,
            Tag0,Tag1,Tag2,Tag3
            > type;
};

template<class Tag1, class Tag2, class Tag3>
struct Triple_index_selector2<Pred_tag,Tag1,Tag2,Tag3> {
   typedef Triple_index<
            Fragment_map_ordered,
            Pred_tag,Tag1,Tag2,Tag3
            > type;
};

/**@brief
*******************************************************************************/
template<class Config> struct Triple_index_selector
         : public Triple_index_selector2<
              typename boost::mpl::at_c<Config,0>::type,
              typename boost::mpl::at_c<Config,1>::type,
              typename boost::mpl::at_c<Config,2>::type,
              typename boost::mpl::at_c<Config,3>::type
           >
{};

}//namespace map_triple_detail
}//namespace owlcpp
#endif /* MAP_TRIPLE_CONFIG_HPP_ */
