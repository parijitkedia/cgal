// Copyright (c) 1997-2001  Utrecht University (The Netherlands),
// ETH Zurich (Switzerland), Freie Universitaet Berlin (Germany),
// INRIA Sophia-Antipolis (France), Martin-Luther-University Halle-Wittenberg
// (Germany), Max-Planck-Institute Saarbruecken (Germany), RISC Linz (Austria),
// and Tel-Aviv University (Israel).  All rights reserved.
//
// This file is part of CGAL (www.cgal.org); you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; version 2.1 of the License.
// See the file LICENSE.LGPL distributed with CGAL.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL$
// $Id$
// 
//
// Author(s)     : Sven Schoenherr <sven@inf.ethz.ch>, Sylvain Pion, Andreas Fabri

#ifndef CGAL_RANDOM_H
#define CGAL_RANDOM_H

#include <string>
#include <utility>
#include <CGAL/basic.h>

#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>


namespace CGAL {

class Random {
  public:
    // types

 struct State {
    std::string rng;
    unsigned int random_value, val, seed;

    State()
    {}

    State(std::string rng, 
          unsigned int random_value, 
          unsigned int val, 
          unsigned int seed)
      : rng(rng), random_value(random_value), val(val), seed(seed)
    {}
 };
    // creation
    Random( );
    Random( unsigned int  seed);

    // seed
    unsigned int get_seed ( ) const;
    
    // operations
    bool    get_bool  ( );


  template <typename IntType>
  IntType
  get_int(IntType lower, IntType upper)
  {
    // uniform_int has a closed interval, CGAL a halfopen
    boost::uniform_int<IntType> dist(lower,upper-1);
    boost::variate_generator<boost::rand48&, boost::uniform_int<IntType> > generator(rng,dist);
    
    return generator();
  }
 

  template <typename RealType>
  RealType
  get_double( RealType lower, RealType upper)
  {
    // uniform_real as well as CGAL have a halfopen interval
    boost::uniform_real<RealType> dist(lower,upper);
    boost::variate_generator<boost::rand48&, boost::uniform_real<RealType> > generator(rng,dist);
    
    return generator();
  }


  template <typename RealType>
  RealType
  get_double( RealType lower)
  {
    return get_double<RealType>(lower, RealType(1.0));
  }

  double
  get_double(double lower, double upper)
  {
    return get_double<double>(lower,upper);
  }

  double
  get_double(double lower)
  {
    return get_double<double>(lower,1.0);
  }

  double
  get_double()
  {
    return get_double<double>(0.0,1.0);
  }

    // state 
    void save_state( State& state) const;
    void restore_state( const State& state);

    // Computes a random int value smaller than 2^b.
    // It's supposed to be fast, useful for randomized algorithms.
    // The distribution is not perfectly flat, but this is a sacrifice against
    // efficiency.
    template <int b>
    int get_bits()
    {
	CGAL_assertion(0<b && b<16);
        if (val == 0) {
            random_value = (421U * random_value + 2073U) % 32749U;
            val = random_value;
        }
        int ret = val & ((1<<b)-1);
        val >>= 1; // Shifting by b would be slightly better, but is slower.
        return ret;
    }

    int     operator () ( int upper);
  std::size_t     operator () ( std::size_t upper);
  std::ptrdiff_t     operator () ( std::ptrdiff_t upper);


    
  bool    operator==(Random rd) const
  {
    return (rng == rd.rng) 
      && (random_value == rd.random_value)
      && (val == rd.val)
      && (seed == rd.seed);
  }

  private:
    // data members
    unsigned int random_value; // Current 15 bits random value.
    unsigned int val; // random_value shifted by used bits.
    unsigned int seed; 
    boost::rand48 rng;
};

// Global variables
// ================
extern  Random  default_random;

} //namespace CGAL

// ============================================================================

// Class implementation (inline functions)
// =======================================
// includes
#  include <cstdlib>

namespace CGAL {

// operations
inline
bool
Random::
get_bool( )
{
    return( static_cast< bool>( rng() & 1));
}

inline
int
Random::
operator () ( int upper)
{
  return( get_int<int>( 0, upper));
}

inline
std::size_t
Random::
operator () ( std::size_t upper)
{
  return( get_int<std::size_t>( 0, upper));
}

inline
std::ptrdiff_t
Random::
operator () ( std::ptrdiff_t upper)
{
  return( get_int<std::ptrdiff_t>( 0, upper));
}
} //namespace CGAL

#endif // CGAL_RANDOM_H

// ===== EOF ==================================================================
