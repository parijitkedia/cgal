// ======================================================================
//
// Copyright (c) 1997 The CGAL Consortium
//
// This software and related documentation is part of an INTERNAL release
// of the Computational Geometry Algorithms Library (CGAL). It is not
// intended for general use.
//
// ----------------------------------------------------------------------
//
// release       : 
// release_date  : 
//
// file          : include/CGAL/Alpha_shape_cell_base_3.h
// package       : Alpha_shapes_3(1.0)
// revision      : $Revision$
// revision_date : $Date$
// author(s)     : Tran Kai Frank DA
//
// coordinator   : INRIA Sophia-Antipolis (<Mariette.Yvinec@sophia.inria.fr>)
//
// ======================================================================

#ifndef CGAL_ALPHA_SHAPE_CELL_BASE_3_H
#define CGAL_ALPHA_SHAPE_CELL_BASE_3_H

#include <vector>
#include <CGAL/utility.h>
#include <CGAL/Triangulation_cell_base_3.h>

CGAL_BEGIN_NAMESPACE

template < class Gt, class Cb = Triangulation_cell_base_3<Gt> >
class Alpha_shape_cell_base_3
  : public Cb
{
public:
  typedef typename Cb::Vertex_handle   Vertex_handle;
  typedef typename Cb::Cell_handle     Cell_handle;

  template < typename TDS2 >
  struct Rebind_TDS {
    typedef typename Cb::template Rebind_TDS<TDS2>::Other   Cb2;
    typedef Alpha_shape_cell_base_3<Gt, Cb2>                Other;
  };

  typedef typename Gt::FT                            Coord_type;
  typedef Triple<Coord_type, Coord_type, Coord_type> Interval3;

private:

  Interval3 vec_facet[4];
  Interval3 vec_edge[4][4];
  Coord_type A;

public:
  
  Alpha_shape_cell_base_3() 
    : Cb() {}
  
  Alpha_shape_cell_base_3(Vertex_handle v0, Vertex_handle v1,
                          Vertex_handle v2, Vertex_handle v3)
    : Cb(v0, v1, v2, v3) {}
  
  Alpha_shape_cell_base_3(Vertex_handle v0, Vertex_handle v1,
                          Vertex_handle v2, Vertex_handle v3,
                          Cell_handle n0, Cell_handle n1,
                          Cell_handle n2, Cell_handle n3)
    : Cb(v0, v1, v2, v3, n0, n1, n2, n3) {}


  const Coord_type & get_alpha() const
    {
      return A;
    }
  
  void set_alpha(const Coord_type & AA)
    {
      A = AA;
    }

  const Interval3 & get_facet_ranges(int i) const
    {
      return vec_facet[i];
    }

  void set_facet_ranges(int i, const Interval3& Inter)
    {
      vec_facet[i]=Inter;
    }
  
  const Interval3 & get_edge_ranges(int i, int j) const
    {
      return vec_edge[i][j];
    }

  void set_edge_ranges(int i, int j, const Interval3& Inter)
    {
      vec_edge[i][j]=Inter;
    }
};

CGAL_END_NAMESPACE

#endif // CGAL_ALPHA_SHAPE_CELL_BASE_3_H
