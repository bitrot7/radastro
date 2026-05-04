/* -*- c++ -*- */
/*
 * Copyright 2026 Q.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_RAD_ASTRO_AVG_H
#define INCLUDED_RAD_ASTRO_AVG_H

#include <gnuradio/rad_astro/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace rad_astro {

    /*!
     * \brief <+description of block+>
     * \ingroup rad_astro
     *
     */
    class RAD_ASTRO_API avg : virtual public gr::block
    {
     public:
      typedef std::shared_ptr<avg> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of rad_astro::avg.
       *
       * To avoid accidental use of raw pointers, rad_astro::avg's
       * constructor is in a private implementation
       * class. rad_astro::avg::make is the public interface for
       * creating new instances.
       */
      static sptr make(int vlen, int numAvgs, int avgType, float alpha, float beta, float delta, float gamma);
    };

  } // namespace rad_astro
} // namespace gr

#endif /* INCLUDED_RAD_ASTRO_AVG_H */
