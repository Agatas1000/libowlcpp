/** @file "/owlcpp/include/owlcpp/rdf/map_triple_crtpb.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef MAP_TRIPLE_CRTPB_HPP_
#define MAP_TRIPLE_CRTPB_HPP_
#include "boost/assert.hpp"
#include "boost/concept/assert.hpp"

#include "owlcpp/rdf/detail/map_traits.hpp"

namespace owlcpp{

/**Enable operations on RDF triples.
Base for CRTP (Curiously Recurring Template Pattern).
*******************************************************************************/
template<class Super> class Map_triple_crtpb {
   typedef detail::Map_traits<Super> traits;
   typedef typename traits::map_triple_type map_triple_type;

   map_triple_type const& _map_triple() const {
      return static_cast<Super const&>(*this).map_triple_;
   }

   map_triple_type& _map_triple() {
      return static_cast<Super&>(*this).map_triple_;
   }

public:

   template<class Subj, class Pred, class Obj, class Doc> struct result
   : public map_triple_type::template result<Subj,Pred,Obj,Doc>{};

   template<bool Subj, bool Pred, bool Obj, bool Doc>struct result_b
   : public map_triple_type::template result_b<Subj,Pred,Obj,Doc>{};

   /**@brief Search triples by subject, predicate, object, or document IDs.
    @details Polymorphically search stored triples to find ones that match
    specified node IDs for subject, predicate, or object nodes or document ID.
    An instance of \b any matches all values for the corresponding triple
    element.
    If none of the nodes are specified, i.e., <tt>find(any(), any(), any(), any())</tt>,
    the search returns a range of all stored triples, [begin(), end()).
    @param subj predicate for first element of triple (subject node),
    e.g., \b Node_id, \b any
    @param pred predicate for second element of triple (predicate node),
    e.g., \b Node_id, \b any
    @param obj predicate for third element of triple (object node),
    e.g., \b Node_id, \b any
    @param doc predicate for fourth element of triple (document ID),
    e.g., \b Doc_id, \b any
    @return iterator range of triples matching the query.
    @details
    The type of the range can be obtained from
    @code template<class Subj, class Pred, class Obj, class Doc> class result; @endcode
    or from
    @code template<bool Subj, bool Pred, bool Obj, bool Doc> class result_b; @endcode
    For example,
    @code Triple_map<>::result_b<1,0,0,1>::type range = triple_map.find(subj, any(), any(), doc);
    @endcode
   */
   template<class Subj, class Pred, class Obj, class Doc>
   typename result<Subj,Pred,Obj,Doc>::type
   find_triple(const Subj subj, const Pred pred, const Obj obj, const Doc doc) const {
      return _map_triple().find(subj, pred, obj, doc);
   }

   /**@brief Insert a new triple
   */
   void insert_triple(
            const Node_id subj,
            const Node_id pred,
            const Node_id obj,
            const Doc_id doc
   ) {
      BOOST_CONCEPT_ASSERT((Node_store<Super>));
      BOOST_ASSERT( static_cast<Super const&>(*this).valid(subj) && "invalid subject ID" );
      BOOST_ASSERT( static_cast<Super const&>(*this).valid(pred) && "invalid predicate ID" );
      BOOST_ASSERT( static_cast<Super const&>(*this).valid(obj) && "invalid object ID" );
      BOOST_ASSERT( static_cast<Super const&>(*this).valid(doc) && "invalid document ID" );
      _map_triple().insert(subj, pred, obj, doc);
   }

};

}//namespace owlcpp
#endif /* MAP_TRIPLE_CRTPB_HPP_ */
