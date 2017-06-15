// Copyright (c) 2006,2007,2009,2010,2011 Tel-Aviv University (Israel).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org).
// You can redistribute it and/or modify it under the terms of the GNU
// General Public License as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// Author(s)     : Baruch Zukerman <baruchzu@post.tau.ac.il>
//                 Ron Wein <wein@post.tau.ac.il>
//                 Efi Fogel <efif@post.tau.ac.il>

#ifndef CGAL_SURFACE_SWEEP_2_VISITORS_H
#define CGAL_SURFACE_SWEEP_2_VISITORS_H

#include <CGAL/license/Surface_sweep_2.h>

/*! \file
 *
 * Definition of the basic sweep-line visitors, for the usage of the global
 * sweep-line functions.
 */

#include <vector>
#include <iterator>

#include <CGAL/Surface_sweep_2.h>
#include <CGAL/Surface_sweep_2/Surface_sweep_2_utils.h>
#include <CGAL/Surface_sweep_2/Visitor.h>

namespace CGAL {
namespace Surface_sweep_2 {

/*! \class Surface_sweep_points_visitor
 *
 * A simple sweep-line visitor that reports all intersection points among a
 * set of input curves.
 */
template <typename GeometryTraits_2, typename OutputIerator>
class Surface_sweep_points_visitor :
  public Default_visitor<Surface_sweep_points_visitor<GeometryTraits_2,
                                                      OutputIerator>,
                         GeometryTraits_2>
{
public:
  typedef GeometryTraits_2                              Geometry_traits_2;
  typedef OutputIerator                                 Output_ierator;

private:
  typedef Geometry_traits_2                             Gt2;
  typedef Surface_sweep_points_visitor<Gt2, Output_ierator>
                                                        Self;
  typedef Default_visitor<Self, Gt2>                    Base;

public:
  typedef typename Base::Event                          Event;
  typedef typename Base::Subcurve                       Subcurve;

  typedef typename Subcurve::Status_line_iterator       Status_line_iterator;

  typedef typename Gt2::X_monotone_curve_2              X_monotone_curve_2;
  typedef typename Gt2::Point_2                         Point_2;

  typedef typename Base::Surface_sweep_2                Surface_sweep_2;

protected:
  Output_ierator m_out;                 // The output points.
  bool m_includeEndPoints;              // Should we include endpoints.

public:
  Surface_sweep_points_visitor(Output_ierator out, bool endpoints) :
    m_out(out),
    m_includeEndPoints(endpoints)
  {}

  template <typename CurveIterator>
  void sweep(CurveIterator begin, CurveIterator end)
  {
    std::vector<X_monotone_curve_2> curves_vec;
    std::vector<Point_2> points_vec;

    curves_vec.reserve(std::distance(begin,end));
    make_x_monotone(begin, end,
                    std::back_inserter(curves_vec),
                    std::back_inserter(points_vec),
                    this->traits());

    //Perform the sweep
    Surface_sweep_2* sl = this->surface_sweep();
    sl->sweep(curves_vec.begin(), curves_vec.end(),
              points_vec.begin(), points_vec.end());
  }

  bool after_handle_event(Event* event,
                          Status_line_iterator /* iter */,
                          bool /* flag */)
  {
    if ((m_includeEndPoints ||
         event->is_intersection() ||
         event->is_weak_intersection()) && event->is_closed())
    {
      *m_out = event->point();
      ++m_out;
    }
    return true;
  }

  Output_ierator output_iterator() { return m_out; }
};

/*! \class Surface_sweep_subcurves_visitor
 *
 * A simple sweep-line visitor that reports all non-intersecting
 * x-monotone curves induced by a set of input curves.
 */
template <typename GeometryTraits_2, typename OutputIerator>
class Surface_sweep_subcurves_visitor :
  public Default_visitor<Surface_sweep_subcurves_visitor<
                           GeometryTraits_2, OutputIerator>,
                         GeometryTraits_2>
{
public:
  typedef GeometryTraits_2                              Geometry_traits_2;
  typedef OutputIerator                                 Output_ierator;

private:
  typedef Geometry_traits_2                             Gt2;
  typedef Surface_sweep_subcurves_visitor<Gt2, Output_ierator>
                                                        Self;
  typedef Default_visitor<Self, Gt2>                    Base;

public:
  typedef typename Gt2::X_monotone_curve_2              X_monotone_curve_2;
  typedef typename Gt2::Point_2                         Point_2;

  typedef typename Base::Event                          Event;
  typedef typename Base::Subcurve                       Subcurve;

  typedef typename Subcurve::Status_line_iterator       Status_line_iterator;

  typedef typename Base::Surface_sweep_2                Surface_sweep_2;

protected:
  // Data members:
  Output_ierator m_out;           // The output curves.
  bool m_overlapping;             // Should we report overlapping curves twice.

public:
  Surface_sweep_subcurves_visitor(Output_ierator out, bool overlapping) :
    m_out(out),
    m_overlapping(overlapping)
  {}

  template <typename CurveIterator>
  void sweep(CurveIterator begin, CurveIterator end)
  {
    std::vector<X_monotone_curve_2> curves_vec;
    std::vector<Point_2> points_vec;

    curves_vec.reserve(std::distance(begin,end));
    make_x_monotone(begin, end,
                    std::back_inserter(curves_vec),
                    std::back_inserter(points_vec),
                    this->traits());

    // Perform the sweep.
    Surface_sweep_2* sl = this->surface_sweep();
    sl->sweep(curves_vec.begin(), curves_vec.end(),
              points_vec.begin(), points_vec.end());
  }

  void add_subcurve(const X_monotone_curve_2& cv, Subcurve *sc)
  {
    if (!m_overlapping) {
      // Report the curve once, whether it represents an overlap or not.
      *m_out = cv;
      ++m_out;
    }
    else {
      unsigned int overlap_depth = sc->overlap_depth();
      for (unsigned int i = 0; i < overlap_depth; ++i) {
        *m_out = cv;
        ++m_out;
      }
    }
  }

  Output_ierator output_iterator() { return m_out; }
};

/*! \class Surface_sweep_do_curves_x_visitor
 *
 * A simple sweep-line visitor that determines if there are intersections
 * in the interiors of the given curve set.
 */
template <typename GeometryTraits_2>
class Surface_sweep_do_curves_x_visitor :
  public Default_visitor<Surface_sweep_do_curves_x_visitor<GeometryTraits_2>,
                         GeometryTraits_2>
{
public:
  typedef GeometryTraits_2                              Geometry_traits_2;

private:
  typedef Geometry_traits_2                             Gt2;
  typedef Surface_sweep_do_curves_x_visitor<Gt2>        Self;
  typedef Default_visitor<Self, Gt2>                    Base;

public:
  typedef typename Base::Event                          Event;
  typedef typename Base::Subcurve                       Subcurve;

  typedef typename Subcurve::Status_line_iterator       Status_line_iterator;

  typedef typename Gt2::X_monotone_curve_2              X_monotone_curve_2;
  typedef typename Gt2::Point_2                         Point_2;

  typedef typename Base::Surface_sweep_2                Surface_sweep_2;

protected:
  // Data members:
  bool m_found_x;               // Have we found an intersection so far.

public:
  Surface_sweep_do_curves_x_visitor() : m_found_x(false) {}

  template <typename CurveIterator>
  void sweep(CurveIterator begin, CurveIterator end)
  {
    std::vector<X_monotone_curve_2> curves_vec;
    std::vector<Point_2> points_vec;

    curves_vec.reserve(std::distance(begin,end));
    make_x_monotone(begin, end,
                    std::back_inserter(curves_vec),
                    std::back_inserter(points_vec),
                    this-> traits());

    // Perform the sweep.
    Surface_sweep_2* sl = this->surface_sweep();
    sl->sweep(curves_vec.begin(), curves_vec.end(),
              points_vec.begin(), points_vec.end());
  }

  void update_event(Event* /* e */,
                    Subcurve* /* sc1 */,
                    Subcurve* /* sc2 */,
                    bool /* is_new */)
  { m_found_x = true; }

  void update_event(Event* /* e */,
                    Subcurve* /* sc1 */)
  { m_found_x = true; }

  void update_event(Event* /* e */,
                    const Point_2& /* end_point */,
                    const X_monotone_curve_2& /* cv */,
                    Arr_curve_end /* cv_end */,
                    bool /* is_new */)
  {}

  void update_event(Event* /* e */,
                    const X_monotone_curve_2& /* cv */,
                    Arr_curve_end /* cv_end */,
                    bool /* is_new */)
  {}

  void update_event(Event* /* e */,
                    const Point_2& /* pt */,
                    bool /* is_new */)
  {}

  template <typename XCurveIterator>
  void sweep_xcurves(XCurveIterator begin, XCurveIterator end)
  {
    // Perform the sweep.
    Surface_sweep_2* sl = this->surface_sweep();
    sl->sweep(begin, end);
  }

  void found_overlap(Subcurve* /* sc1 */,
                     Subcurve* /* sc2 */,
                     Subcurve* /* ov_sc */)
  { m_found_x = true; }

  bool after_handle_event(Event* /* event */,
                          Status_line_iterator /* iter */,
                          bool /* flag */)
  {
    if (m_found_x) {
      Surface_sweep_2* sl = this->surface_sweep();
      sl->stop_sweep();
    }
    return true;
  }

  bool found_intersection() { return m_found_x; }
};

} // namespace Surface_sweep_2
} // namespace CGAL

#endif
