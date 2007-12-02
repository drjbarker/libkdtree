/** \file
 * Defines interfaces for nodes as used by the KDTree class.
 *
 * \author Martin F. Krafft <libkdtree@pobox.madduck.net>
 */

#ifndef INCLUDE_KDTREE_NODE_HPP
#define INCLUDE_KDTREE_NODE_HPP

#ifdef KDTREE_DEFINE_OSTREAM_OPERATORS
#include <iostream>
#endif


//#include <set>

#include <cstddef>

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
               _Base_ptr const __RIGHT = NULL) throw ()
      : _M_parent(__PARENT), _M_left(__LEFT), _M_right(__RIGHT) {}

    static _Base_ptr
    _S_minimum(_Base_ptr __x) throw ()
    {
      while (__x->_M_left) __x = __x->_M_left;
      return __x;
    }

    static _Base_ptr
    _S_maximum(_Base_ptr __x) throw ()
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
            _Base_ptr const __RIGHT = NULL) throw ()
        : _Node_base(__PARENT, __LEFT, __RIGHT), _M_value(__VALUE) {}
    };

  template <typename _Val, typename _Acc, typename _Cmp>
    class _Node_compare
    {
    public:
      typedef _Node<_Val> const* _Link_const_type;

      _Node_compare(size_t const __DIM, _Acc const& acc, _Cmp const& cmp)
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

      size_t const _M_DIM;
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
  _S_node_compare (const size_t __dim,
		   const _Cmp& __cmp, const _Acc& __acc,
		   const _Val& __a, const _Val& __b)
  {
    return __cmp(__acc(__a, __dim), __acc(__b, __dim));
  }

  template <typename _Val, typename _Cmp,
	    typename _Acc>
  inline
  bool
  _S_node_compare (const size_t __dim,
		   const _Cmp& __cmp, const _Acc& __acc,
		   const _Val& __a, const _Node<_Val>* __b)
  {
    return __cmp(__acc(__a, __dim), __acc(__b->_M_value, __dim));
  }

  template <typename _Val, typename _Cmp,
	    typename _Acc>
  inline
  bool
  _S_node_compare (const size_t __dim,
		   const _Cmp& __cmp, const _Acc& __acc,
		   const _Node<_Val>* __a, const _Val& __b)
  {
    return __cmp(__acc(__a->_M_value, __dim), __acc(__b, __dim));
  }

  template <typename _Val, typename _Cmp,
	    typename _Acc>
  inline
  bool
  _S_node_compare (const size_t __dim,
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
  _S_node_distance (const size_t __dim,
		    const _Dist& __dist, const _Acc& __acc,
		    const _Val& __a, const _Val& __b)
  {
    return __dist(__acc(__a, __dim), __acc(__b, __dim));
  }

  template <typename _Val, typename _Dist,
	    typename _Acc>
  inline
  typename _Dist::distance_type
  _S_node_distance (const size_t __dim,
		    const _Dist& __dist, const _Acc& __acc,
		    const _Val& __a, const _Node<_Val>* __b)
  {
    return __dist(__acc(__a, __dim), __acc(__b->_M_value, __dim));
  }

  template <typename _Val, typename _Dist,
	    typename _Acc>
  inline
  typename _Dist::distance_type
  _S_node_distance (const size_t __dim,
		    const _Dist& __dist, const _Acc& __acc,
		    const _Node<_Val>* __a, const _Val& __b)
  {
    return __dist(__acc(__a->_M_value, __dim), __acc(__b, __dim));
  }

  template <typename _Val, typename _Dist,
	    typename _Acc>
  inline
  typename _Dist::distance_type
  _S_node_distance (const size_t __dim,
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
  _S_accumulate_node_distance (const size_t __dim,
			       const _Dist& __dist, const _Acc& __acc,
			       const _Val& __a, const _Val& __b)
  {
    typename _Dist::distance_type d = 0;
    for (size_t i=0; i<__dim; ++i)
      d += __dist(__acc(__a, i), __acc(__b, i));
    return d;
  }

  template <typename _Val, typename _Dist,
	    typename _Acc>
  inline
  typename _Dist::distance_type
  _S_accumulate_node_distance (const size_t __dim,
			       const _Dist& __dist, const _Acc& __acc,
			       const _Val& __a,
			       const _Node<_Val>* __b)
  {
    typename _Dist::distance_type d = 0;
    for (size_t i=0; i<__dim; ++i)
      d += __dist(__acc(__a, i), __acc(__b->_M_value, i));
    return d;
  }

  template <typename _Val, typename _Dist,
	    typename _Acc>
  inline
  typename _Dist::distance_type
  _S_accumulate_node_distance (const size_t __dim,
			       const _Dist& __dist, const _Acc& __acc,
			       const _Node<_Val>* __a,
			       const _Val& __b)
  {
    typename _Dist::distance_type d = 0;
    for (size_t i=0; i<__dim; ++i)
      d += __dist(__acc(__a->_M_value, i), __acc(__b, i));
    return d;
  }

  template <typename _Val, typename _Dist,
	    typename _Acc>
  inline
  typename _Dist::distance_type
  _S_accumulate_node_distance (const size_t __dim,
			       const _Dist& __dist, const _Acc& __acc,
			       const _Node<_Val>* __a,
			       const _Node<_Val>* __b)
  {
    typename _Dist::distance_type d = 0;
    for (size_t i=0; i<__dim; ++i)
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
  _S_node_descend (const size_t __dim,
		   const _Cmp& __cmp, const _Acc& __acc,
		   const _Val& __val, const _Node_base* __node)
  {
    if (_S_node_compare(__dim, __cmp, __acc, __val,  static_cast<const _Node<_Val>* >(__node)))
      return __node->_M_left;
    else
      return __node->_M_right;
  }

  /*! Find the nearest node to __val from __node

    \return the nearest node of __end node if no nearest node was found for the
    given arguments.
   */
  template <typename _Val, typename _Cmp,
	    typename _Acc, typename _Dist,
	    typename _Predicate>
  inline
  std::pair<const _Node<_Val>*,
	    std::pair<size_t, typename _Dist::distance_type> >
  _S_node_nearest (const size_t __k, size_t __dim,
		   const _Val& __val,
		   const _Node<_Val>* __node, const _Node<_Val>* __end,
		   typename _Dist::distance_type __max,
		   const _Cmp& __cmp, const _Acc& __acc, const _Dist& __dist,
		   _Predicate __p)
  {
    const _Node_base* pcur = __node;
    const _Node_base* cur = _S_node_descend(__dim % __k, __cmp, __acc, __val, __node);
    size_t cur_dim = __dim+1;

//    std::set<_Node_base*> visited;
//    visited.insert(pcur);

    // find the smallest __max distance in direct descent
    while (cur)
      {
	if (__p(static_cast<const _Node<_Val>* >(cur)->_M_value))
	  {
	    typename _Dist::distance_type d = 0;
	    for (size_t i=0; i < __k && d < __max; ++i)
	      d += _S_node_distance(i, __dist, __acc, __val, static_cast<const _Node<_Val>* >(cur));
	    if (d < __max)
	      {
		__node = static_cast<const _Node<_Val>* >(cur);
		__max = d;
		__dim = cur_dim;
	      }
	  }

//	visited.insert(cur);

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
    size_t probe_dim = cur_dim;
    if (_S_node_compare(probe_dim % __k, __cmp, __acc, __val, static_cast<const _Node<_Val>* >(probe)))
      near = probe->_M_right;
    else
      near = probe->_M_left;
    if (near
	// only visit node's children if node's plane intersect hypersphere
	&& (_S_node_distance(probe_dim % __k, __dist, __acc, __val, static_cast<const _Node<_Val>* >(probe)) < __max))
      {
	probe = near;
	++probe_dim;
      }
    while (cur != __end)
      {
	while (probe != cur)
	  {

//	    std::cout << "** pprobe: " << static_cast<_Node<_Val>* >(pprobe)->_M_value << ", val: " << __val << std::flush;
//	    if (pprobe->_M_left == probe)
//	      {
//		std::cout << ", goes left for dim " << (probe_dim-1) << "(" << (probe_dim-1) % __k << ")" << std::endl;
//		if (!pprobe->_M_right) std::cout << "**    info: right is null" << std::endl;
//		else if (visited.find(pprobe->_M_right)!=visited.end())
//		  std::cout << "**    info: right was visited" << std::endl;
//		if (_S_node_distance((probe_dim-1) % __k, __dist, __acc, __val, static_cast<_Node<_Val>* >(pprobe)) < __max)
//		  std::cout << "**    info: pprobe node intersect hypersphere" << std::endl;
//		assert(_S_node_compare((probe_dim-1) % __k, __cmp, __acc, __val, static_cast<_Node<_Val>* >(pprobe))
//		       || ((!pprobe->_M_right || visited.find(pprobe->_M_right)!=visited.end())
//			   && _S_node_distance((probe_dim-1) % __k, __dist, __acc, __val, static_cast<_Node<_Val>* >(pprobe)) < __max));
//	      }
//	    else if (pprobe->_M_right == probe)
//	      {
//		std::cout << ", goes right for dim " << (probe_dim-1) << "(" << (probe_dim-1) % __k << ")" << std::endl;
//		if (!pprobe->_M_left) std::cout << "**    info: left is null" << std::endl;
//		else if (visited.find(pprobe->_M_left)!=visited.end())
//		  std::cout << "**    info: left was visited" << std::endl;
//		if (_S_node_distance((probe_dim-1) % __k, __dist, __acc, __val, static_cast<_Node<_Val>* >(pprobe)) < __max)
//		  std::cout << "**    info: pprobe node intersect hypersphere" << std::endl;
//		assert(!_S_node_compare((probe_dim-1) % __k, __cmp, __acc, __val, static_cast<_Node<_Val>* >(pprobe))
//		       || ((!pprobe->_M_left || visited.find(pprobe->_M_left)!=visited.end())
//			   && _S_node_distance((probe_dim-1) % __k, __dist, __acc, __val, static_cast<_Node<_Val>* >(pprobe)) < __max));
//	      }
//	    else if (pprobe->_M_parent == probe)
//	      {
//		std::cout << ", goes up for dim " << (probe_dim+1) << "(" << (probe_dim+1) % __k << ")" << std::endl;
//	      }
//	    else
//	      {
//		std::cout << ", pprobe in incorrect states: " << pprobe << " " << probe << std::endl;
//		assert(0);
//	      }
//	    visited.insert(probe);

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
		    for (size_t i=0; i < __k && d < __max; ++i)
		      d += _S_node_distance(i, __dist, __acc, __val, static_cast<const _Node<_Val>* >(probe));
		    if (d < __max)
		      {
			__node = static_cast<const _Node<_Val>* >(probe);
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
			 _S_node_distance(probe_dim % __k, __dist, __acc, __val, static_cast<const _Node<_Val>* >(probe)) < __max)
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
		    && _S_node_distance(probe_dim % __k, __dist, __acc, __val, static_cast<const _Node<_Val>* >(probe)) < __max)
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

//	    std::cout << "** cur: " << static_cast<_Node<_Val>* >(cur)->_M_value << ", val: " << __val << std::flush;
//		std::cout << ", goes up for dim " << (cur_dim) << "(" << (cur_dim) % __k << ")" << std::endl;

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
		&& (_S_node_distance(cur_dim % __k, __dist, __acc, __val, static_cast<const _Node<_Val>* >(cur)) < __max))
	      {
		probe = near;
		++probe_dim;
	      }
	  }
      }
    return std::pair<const _Node<_Val>*,
      std::pair<size_t, typename _Dist::distance_type> >
      (__node, std::pair<size_t, typename _Dist::distance_type>
       (__dim, __max));
  }

#ifdef KDTREE_DEFINE_OSTREAM_OPERATORS

  template <typename _Char, typename _Traits, typename _Val>
    std::basic_ostream<_Char, _Traits>&
    operator<<(std::basic_ostream<_Char, _Traits>& __out,
               typename KDTree::_Node<_Val> const& __N) throw ()
    {
      __out << &__N;
      __out << ' ' << __N._M_value;
      __out << "; parent: " << __N._M_parent;
      __out << "; left: " << __N._M_left;
      __out << "; right: " << __N._M_right;
      return __out;
    }

#endif

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
