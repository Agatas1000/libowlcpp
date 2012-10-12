/** @file "/owlcpp/include/owlcpp/rdf/detail/map_node_base.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef MAP_NODE_BASE_HPP_
#define MAP_NODE_BASE_HPP_
#include <string>
#include <vector>
#include "boost/assert.hpp"
#include "boost/ptr_container/ptr_vector.hpp"
#include "boost/ptr_container/indirect_fun.hpp"
#include "boost/unordered_map.hpp"

#include "owlcpp/rdf/node.hpp"
#include "owlcpp/rdf/node_blank.hpp"
#include "owlcpp/rdf/node_iri.hpp"
#include "owlcpp/rdf/node_literal.hpp"
#include "owlcpp/rdf/exception.hpp"
#include "owlcpp/rdf/print_node.hpp"
#include "owlcpp/rdf/hash_node.hpp"
#include "owlcpp/doc_id.hpp"
#include "owlcpp/node_id.hpp"
#include "owlcpp/detail/member_iterator.hpp"
#include "owlcpp/detail/datatype_impl.hpp"
#include "owlcpp/terms/detail/max_standard_id.hpp"

namespace owlcpp{

/**@brief Store polymorphic RDF nodes
@details
Validity of node IDs is assumed and asserted in debug mode.
Calling node map methods with invalid node IDs results in undefined behavior.
*******************************************************************************/
class Map_node {
public:
   typedef Node_id id_type;
   typedef Node node_type;
private:
   typedef std::auto_ptr<node_type> ptr_t;
   typedef boost::ptr_vector<boost::nullable<Node> > vector_t;
   typedef boost::unordered_map<
            Node const*,
            Node_id,
            boost::indirect_fun<boost::hash<Node> >,
            boost::indirect_fun<std::equal_to<Node> >
   > map_t;

   typedef map_t::iterator map_iter_t;
   typedef map_t::const_iterator map_citer_t;
   typedef std::pair<map_iter_t,bool> insert_t;

public:
   typedef Member_iterator<
            map_citer_t,
            Node_id const,
            &std::pair<Node const*const,Node_id>::second
   > iterator;
   typedef iterator const_iterator;

   struct Err : public Rdf_err {};

   std::size_t size() const { return m_.size(); }
   const_iterator begin() const {return m_.begin();}
   const_iterator end() const {return m_.end();}
   bool empty() const {return m_.empty();}

   bool valid(const Node_id id) const {
      if( id < detail::min_node_id() ) return false;
      const std::size_t n = sz(id);
      return n < v_.size() && ! v_.is_null(n);
   }

   Node const& operator[](const Node_id id) const {return get(id);}

   Node const& at(const Node_id id) const {
      if( ! valid(id) ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("invalid node ID")
               << Err::int1_t(id())
      );
      return get(id);
   }

   Node const* find(const Node_id id) const {
      if(
               id < detail::min_node_id() ||
               id() >= v_.size() + detail::min_node_id()()
      ) return 0;
      return  &v_[sz(id)];
   }

   Node_id const* find(Node const& node) const {
      const map_citer_t i = m_.find(&node);
      return i == m_.end() ? 0 : &i->second;
   }

   Node_id const* find_iri(const Ns_id ns, std::string const& val) const {
      return find(Node_iri(ns, val));
   }

   Node_id const* find_literal(
            std::string const& val, const Node_id dt, std::string const& lang
   ) const {
      switch(internal_type_id(dt)) {
      case detail::Bool_tid:
         return find(Node_bool(val, dt));
      case detail::Int_tid:
         return find(Node_int(val, dt));
      case detail::Unsigned_tid:
         return find(Node_unsigned(val, dt));
      case detail::Double_tid:
         return find(Node_double(val, dt));
      case detail::Empty_tid:
      case detail::String_tid:
      case detail::Unknown_tid:
      default:
         return find(Node_string(val, dt, lang));
      }
   }

   Node_id const* find_blank(const unsigned n, const Doc_id doc) const {
      return find(Node_blank(n, doc));
   }

   Node_id insert_iri(const Ns_id ns, std::string const& val) {
      //todo: do not new if node already stored
      ptr_t np(new Node_iri(ns,val));
      return insert(np);
   }

   Node_id insert_literal(
            std::string const& val,
            const Node_id dt = terms::T_empty_::id(),
            std::string const& lang = ""
   ) {
      //todo: do not new if node already stored
      ptr_t np;
      switch(internal_type_id(dt)) {
      case detail::Bool_tid:
         np.reset( new Node_bool(val, dt) );
         break;
      case detail::Int_tid:
         np.reset( new Node_int(val, dt) );
         break;
      case detail::Unsigned_tid:
         np.reset( new Node_unsigned(val, dt) );
         break;
      case detail::Double_tid:
         np.reset( new Node_double(val, dt) );
         break;
      case detail::Empty_tid:
      case detail::String_tid:
      case detail::Unknown_tid:
      default:
         np.reset( new Node_string(val, dt, lang) );
         break;
      }
      return insert(np);
   }

   Node_id insert_blank(const unsigned n, const Doc_id doc) {
      //todo: do not new if node already stored
      ptr_t np(new Node_blank(n, doc));
      return insert(np);
   }

   std::auto_ptr<Node> remove(const Node_id id) {
      BOOST_ASSERT(valid(id));
      const std::size_t n = m_.erase(&get(id));
      BOOST_ASSERT(n);
      erased_.push_back(id);
      return ptr_t( v_.replace(sz(id), 0).release() );
   }

   void clear() {
      erased_.clear();
      m_.clear();
      v_.clear();
   }

private:
   vector_t v_;
   map_t m_;
   std::vector<Node_id> erased_;

   std::size_t sz(const Node_id id) const {
      BOOST_ASSERT(id >= detail::min_node_id());
      return id() - detail::min_node_id()();
   }

   Node_id nid(const std::size_t n) const {return Node_id(n + detail::min_node_id()());}

   Node const& get(const Node_id id) const {return v_[sz(id)];}

   Node_id make_id(ptr_t np) {
      if( erased_.empty() ) {
         const Node_id id = nid(v_.size());
         v_.push_back(np);
         return id;
      }
      const Node_id id = erased_.back();
      erased_.pop_back();
      const std::size_t n = sz(id);
      BOOST_ASSERT(v_.is_null(n));
      v_.replace(n, np);
      return id;
   }

   Node_id insert(ptr_t np) {
      insert_t ip = m_.emplace(np.get(), Node_id());
      if( ip.second ) {
         const Node_id id = make_id(np);
         ip.first->second = id;
         return id;
      }
      return ip.first->second;
   }

   void insert(const Node_id id, ptr_t np) {
      Node_id const* id0 = find(*np);
      if( id0 ) {
         if( *id0 == id ) return;
         Node const& node = *np;
         BOOST_THROW_EXCEPTION(
                  Err()
                  << Err::msg_t("node already exists")
                  << Err::node_id_t(*id0)
                  << Err::str1_t(to_string(node))
         );
      }
      if( valid(id) ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("node ID is reserved")
               << Err::int1_t(id())
      );

      Node* p = np.get();
      //ignore erased_
      const std::size_t n = sz(id);
      if( n < v_.size() ) {
         v_.replace(n, np);
      } else {
         //v_.resize(n,0) does not compile for some reason
         for( ; n > v_.size(); v_.push_back(0));
         v_.push_back(np);
      }
      m_.emplace(p, id);
   }

};

}//namespace owlcpp
#endif /* MAP_NODE_BASE_HPP_ */
