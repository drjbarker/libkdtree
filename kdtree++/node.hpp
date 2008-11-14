/** \file
 * Defines interfaces for nodes as used by the KDTree class.
 *
 * \author Martin F. Krafft <libkdtree@pobox.madduck.net>
 */

#ifndef INCLUDE_KDTREE_NODE_HPP
#define INCLUDE_KDTREE_NODE_HPP

#ifdef KDTREE_DEFINE_OSTREAM_OPERATORS
#  include <ostream>
#endif

#include <cstddef>
#include "kdtree_size_t.hpp"

namespace KDTree
{
  struct _Node_base
  {
    typedef _Node_base* _Base_ptr;
    typedef _Node_base const* _Base_const_ptr;

    _Base_ptr _M_parent;
    _Base_ptr _M_left;
    _Base_ptr _M_right;

    _Node_base(_Base_ptr const __PARENT = NULL,
               _Base_ptr const __LEFT = NULL,
               _Base_ptr const __RIGHT = NULL)
      : _M_parent(__PARENT), _M_left(__LEFT), _M_right(__RIGHT) {}

    static _Base_ptr
    _S_minimum(_Base_ptr __x)
    {
      while (__x->_M_left) __x = __x->_M_left;
      return __x;
    }

    static _Base_ptr
    _S_maximum(_Base_ptr __x)
    {
      while (__x->_M_right) __x = __x->_M_right;
      return __x;
    }
  };

  template <typename _Val>
    struct _Node : public _Node_base
    {
      using _Node_base::_Base_ptr;
      typedef _Node* _Link_type;

      _Val _M_value;

      _Node(_Val const& __VALUE = _Val(),
            _Base_ptr const __PARENT = NULL,
            _Base_ptr const __LEFT = NULL,
            _Base_ptr const __RIGHT = NULL)
        : _Node_base(__PARENT, __LEFT, __RIGHT), _M_value(__VALUE) {}

#ifdef KDTREE_DEFINE_OSTREAM_OPERATORS

     template <typename Char, typename Traits>
       friend
       std::basic_ostream<Char, Traits>&
       operator<<(typename std::basic_ostream<Char, Traits>& out,
                  _Node_base const& node)
       {
         out << &node;
         out << " parent: " << node._M_parent;
         out << "; left: " << node._M_left;
         out << "; right: " << node._M_right;
         return out;
       }

     template <typename Char, typename Traits>
       friend
       std::basic_ostream<Char, Traits>&
       operator<<(typename std::basic_ostream<Char, Traits>& out,
                  _Node<_Val> const& node)
       {
         out << &node;
         out << ' ' << node._M_value;
         out << "; parent: " << node._M_parent;
         out << "; left: " << node._M_left;
         out << "; right: " << node._M_right;
         return out;
       }

#endif
    };

  template <typename _Val, typename _Acc, typename _Cmp>
    class _Node_compare
    {
    public:
      typedef _Node<_Val> const* _Link_const_type;

      _Node_compare(size_type const __DIM, _Acc const& acc, _Cmp const& cmp)
	: _M_DIM(__DIM), _M_acc(acc), _M_cmp(cmp) {}

      bool
      operator()(_Link_const_type const& __A, _Link_const_type const& __B) const
      {
        return _M_cmp(_M_acc(__A->_M_value, _M_DIM),
                      _M_acc(__B->_M_value, _M_DIM));
      }

      bool
      operator()(_Link_const_type const& __A, _Val const& __B) const
      {
        return _M_cmp(_M_acc(__A->_M_value, _M_DIM), _M_acc(__B, _M_DIM));
      }

      bool
      operator()(_Val const& __A, _Link_const_type const& __B) const
      {
        return _M_cmp(_M_acc(__A, _M_DIM), _M_acc(__B->_M_value, _M_DIM));
      }

      bool
      operator()(_Val const& __A, _Val const& __B) const
      {
        return _M_cmp(_M_acc(__A, _M_DIM), _M_acc(__B, _M_DIM));
      }

    private:
      size_type _M_DIM;	// don't make this const so that an assignment operator can be auto-generated
      _Acc _M_acc;
      _Cmp _M_cmp;
  };

  /*! Compare two values on the same dimension using a comparison functor _Cmp
      and an accessor _Acc.

      The comparison functor and the accessor are references to the template
      parameters of the KDTree.
   */
  template <typename _Val, typename _Cmp,
	    typename _Acc>
  inline
  bool
  _S_node_compare (const size_type __dim,
		   const _Cmp& __cmp, const _Acc& __acc,
		   const _Val& __a, const _Val& __b)
  {
    return __cmp(__acc(__a, __dim), __acc(__b, __dim));
  }

  template <typename _Val, typename _Cmp,
	    typename _Acc>
  inline
  bool
  _S_node_compare (const size_type __dim,
		   const _Cmp& __cmp, const _Acc& __acc,
		   const _Val& __a, const _Node<_Val>* __b)
  {
    return __cmp(__acc(__a, __dim), __acc(__b->_M_value, __dim));
  }

  template <typename _Val, typename _Cmp,
	    typename _Acc>
  inline
  bool
  _S_node_compare (const size_type __dim,
		   const _Cmp& __cmp, const _Acc& __acc,
		   const _Node<_Val>* __a, const _Val& __b)
  {
    return __cmp(__acc(__a->_M_value, __dim), __acc(__b, __dim));
  }

  template <typename _Val, typename _Cmp,
	    typename _Acc>
  inline
  bool
  _S_node_compare (const size_type __dim,
		   const _Cmp& __cmp, const _Acc& __acc,
		   const _Node<_Val>* __a, const _Node<_Val>* __b)
  {
    return __cmp(__acc(__a->_M_value, __dim), __acc(__b->_M_value, __dim));
  }

  /*! Compute the distance between two values for one dimension only.

      The distance functor and the accessor are references to the template
      parameters of the KDTree.
   */
  template <typename _Val, typename _Dist,
	    typename _Acc>
  inline
  typename _Dist::distance_type
  _S_node_distance (const size_type __dim,
		    const _Dist& __dist, const _Acc& __acc,
		    const _Val& __a, const _Val& __b)
  {
    return __dist(__acc(__a, __dim), __acc(__b, __dim));
  }

  template <typename _Val, typename _Dist,
	    typename _Acc>
  inline
  typename _Dist::distance_type
  _S_node_distance (const size_type __dim,
		    const _Dist& __dist, const _Acc& __acc,
		    const _Val& __a, const _Node<_Val>* __b)
  {
    return __dist(__acc(__a, __dim), __acc(__b->_M_value, __dim));
  }

  template <typename _Val, typename _Dist,
	    typename _Acc>
  inline
  typename _Dist::distance_type
  _S_node_distance (const size_type __dim,
		    const _Dist& __dist, const _Acc& __acc,
		    const _Node<_Val>* __a, const _Val& __b)
  {
    return __dist(__acc(__a->_M_value, __dim), __acc(__b, __dim));
  }

  template <typename _Val, typename _Dist,
	    typename _Acc>
  inline
  typename _Dist::distance_type
  _S_node_distance (const size_type __dim,
		    const _Dist& __dist, const _Acc& __acc,
		    const _Node<_Val>* __a, const _Node<_Val>* __b)
  {
    return __dist(__acc(__a->_M_value, __dim), __acc(__b->_M_value, __dim));
  }

  /*! Compute the distance between two values and accumulate the result for all
      dimensions.

      The distance functor and the accessor are references to the template
      parameters of the KDTree.
   */
  template <typename _Val, typename _Dist,
	    typename _Acc>
  inline
  typename _Dist::distance_type
  _S_accumulate_node_distance (const size_type __dim,
			       const _Dist& __dist, const _Acc& __acc,
			       const _Val& __a, const _Val& __b)
  {
    typename _Dist::distance_type d = 0;
    for (size_type i=0; i<__dim; ++i)
      d += __dist(__acc(__a, i), __acc(__b, i));
    return d;
  }

  template <typename _Val, typename _Dist,
	    typename _Acc>
  inline
  typename _Dist::distance_type
  _S_accumulate_node_distance (const size_type __dim,
			       const _Dist& __dist, const _Acc& __acc,
			       const _Val& __a,
			       const _Node<_Val>* __b)
  {
    typename _Dist::distance_type d = 0;
    for (size_type i=0; i<__dim; ++i)
      d += __dist(__acc(__a, i), __acc(__b->_M_value, i));
    return d;
  }

  template <typename _Val, typename _Dist,
	    typename _Acc>
  inline
  typename _Dist::distance_type
  _S_accumulate_node_distance (const size_type __dim,
			       const _Dist& __dist, const _Acc& __acc,
			       const _Node<_Val>* __a,
			       const _Val& __b)
  {
    typename _Dist::distance_type d = 0;
    for (size_type i=0; i<__dim; ++i)
      d += __dist(__acc(__a->_M_value, i), __acc(__b, i));
    return d;
  }

  template <typename _Val, typename _Dist,
	    typename _Acc>
  inline
  typename _Dist::distance_type
  _S_accumulate_node_distance (const size_type __dim,
			       const _Dist& __dist, const _Acc& __acc,
			       const _Node<_Val>* __a,
			       const _Node<_Val>* __b)
  {
    typename _Dist::distance_type d = 0;
    for (size_type i=0; i<__dim; ++i)
      d += __dist(__acc(__a->_M_value, i), __acc(__b->_M_value, i));
    return d;
  }

  /*! Descend on the left or the right of the node according to the comparison
      between the node's value and the value.

      \note it's the caller responsibility to check if node is NULL.
   */
  template <typename _Val, typename _Cmp, typename _Acc>
  inline
  _Node_base*
  _S_node_descend (const size_type __dim,
		   const _Cmp& __cmp, const _Acc& __acc,
		   const _Val& __val, const _Node_base* __node)
  {
    if (_S_node_compare(__dim, __cmp, __acc, __val,  static_cast<const _Node<_Val>* >(__node)))
      return __node->_M_left;
    else
      return __node->_M_right;
  }

  /*! Find the nearest node to __val from __node

    If many nodes are equidistant to __val, the node with the lowest memory
    address is returned.

    \return the nearest node of __end node if no nearest node was found for the
    given arguments.
   */
  template <typename _Val, typename _Cmp,
	    typename _Acc, typename _Dist,
	    typename _Predicate>
  inline
  std::pair<const _Node<_Val>*,
	    std::pair<size_type, typename _Dist::distance_type> >
  _S_node_nearest (const size_type __k, size_type __dim, const _Val& __val,
		   const _Node<_Val>* __node, const _Node<_Val>* __end,
		   const _Node<_Val>* __best, typename _Dist::distance_type __max,
		   const _Cmp& __cmp, const _Acc& __acc, const _Dist& __dist,
		   _Predicate __p)
  {
    const _Node_base* pcur = __node;
    const _Node_base* cur = _S_node_descend(__dim % __k, __cmp, __acc, __val, __node);
    size_type cur_dim = __dim+1;
    // find the smallest __max distance in direct descent
    while (cur)
      {
	if (__p(static_cast<const _Node<_Val>* >(cur)->_M_value))
	  {
	    typename _Dist::distance_type d = 0;
	    for (size_type i=0; i < __k && d <= __max; ++i)
	      d += _S_node_distance(i, __dist, __acc, __val, static_cast<const _Node<_Val>* >(cur));
	    if (d < __max || ( d == __max && cur < __best ))
	      {
		__best = static_cast<const _Node<_Val>* >(cur);
		__max = d;
		__dim = cur_dim;
	      }
	  }
	pcur = cur;
	cur = _S_node_descend(cur_dim % __k, __cmp, __acc, __val, cur);
	++cur_dim;
      }
    // Swap cur to prev, only prev is a valid node.
    cur = pcur;
    --cur_dim;
    pcur = NULL;
    // Probe all node's children not visited yet (siblings of the visited nodes).
    const _Node_base* probe = cur;
    const _Node_base* pprobe = probe;
    const _Node_base* near;
    const _Node_base* far;
    size_type probe_dim = cur_dim;
    if (_S_node_compare(probe_dim % __k, __cmp, __acc, __val, static_cast<const _Node<_Val>* >(probe)))
      near = probe->_M_right;
    else
      near = probe->_M_left;
    if (near
	// only visit node's children if node's plane intersect hypersphere
	&& (_S_node_distance(probe_dim % __k, __dist, __acc, __val, static_cast<const _Node<_Val>* >(probe)) <= __max))
      {
	probe = near;
	++probe_dim;
      }
    while (cur != __end)
      {
	while (probe != cur)
	  {
	    if (_S_node_compare(probe_dim % __k, __cmp, __acc, __val, static_cast<const _Node<_Val>* >(probe)))
	      {
		near = probe->_M_left;
		far = probe->_M_right;
	      }
	    else
	      {
		near = probe->_M_right;
		far = probe->_M_left;
	      }
	    if (pprobe == probe->_M_parent) // going downward ...
	      {
		if (__p(static_cast<const _Node<_Val>* >(probe)->_M_value))
		  {
		    typename _Dist::distance_type d = 0;
		    for (size_type i=0; i < __k && d <= __max; ++i)
		      d += _S_node_distance(i, __dist, __acc, __val, static_cast<const _Node<_Val>* >(probe));
		    if (d < __max || ( d == __max && probe < __best ) )
		      {
			__best = static_cast<const _Node<_Val>* >(probe);
			__max = d;
			__dim = probe_dim;
		      }
		  }
		pprobe = probe;
		if (near)
		  {
		    probe = near;
		    ++probe_dim;
		  }
		else if (far &&
			 // only visit node's children if node's plane intersect hypersphere
			 _S_node_distance(probe_dim % __k, __dist, __acc, __val, static_cast<const _Node<_Val>* >(probe)) <= __max)
		  {
		    probe = far;
		    ++probe_dim;
		  }
		else
		  {
		    probe = probe->_M_parent;
		    --probe_dim;
		  }
	      }
	    else // ... and going upward.
	      {
		if (pprobe == near && far
		    // only visit node's children if node's plane intersect hypersphere
		    && _S_node_distance(probe_dim % __k, __dist, __acc, __val, static_cast<const _Node<_Val>* >(probe)) <= __max)
		  {
		    pprobe = probe;
		    probe = far;
		    ++probe_dim;
		  }
		else
		  {
		    pprobe = probe;
		    probe = probe->_M_parent;
		    --probe_dim;
		  }
	      }
	  }
	pcur = cur;
	cur = cur->_M_parent;
	--cur_dim;
	pprobe = cur;
	probe = cur;
	probe_dim = cur_dim;
	if (cur != __end)
	  {
	    if (pcur == cur->_M_left)
	      near = cur->_M_right;
	    else
	      near = cur->_M_left;
	    if (near
		// only visit node's children if node's plane intersect hypersphere
		&& (_S_node_distance(cur_dim % __k, __dist, __acc, __val, static_cast<const _Node<_Val>* >(cur)) <= __max))
	      {
		probe = near;
		++probe_dim;
	      }
	  }
      }
    return std::pair<const _Node<_Val>*,
      std::pair<size_type, typename _Dist::distance_type> >
      (__best, std::pair<size_type, typename _Dist::distance_type>
       (__dim, __max));
  }


} // namespace KDTree

#endif // include guard

/* COPYRIGHT --
 *
 * This file is part of libkdtree++, a C++ template KD-Tree sorting container.
 * libkdtree++ is (c) 2004-2007 Martin F. Krafft <libkdtree@pobox.madduck.net>
 * and Sylvain Bougerel <sylvain.bougerel.devel@gmail.com> distributed under the
 * terms of the Artistic License 2.0. See the ./COPYING file in the source tree
 * root for more information.
 *
 * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES
 * OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */
