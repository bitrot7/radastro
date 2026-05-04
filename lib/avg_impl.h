/* -*- c++ -*- */
/*
 * Copyright 2026 Q.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_RAD_ASTRO_AVG_IMPL_H
#define INCLUDED_RAD_ASTRO_AVG_IMPL_H

#include <gnuradio/rad_astro/avg.h>

namespace gr {
  namespace rad_astro {

    class avg_impl : public avg
    {
     private:
      // Nothing to declare in this block.
      int vlen_ = 2;
      int numAvgs_ = 2;
      int avgType_ = 0;
      float alpha_ = 1.0;
      float beta_ = 1.0;
      float delta_ = 1.0;
      float gamma_ = 1.0;

      gr_vector_float curFftSum_; // Accumulation of the current average.
      int curFftIter_; // the current iteration of the average, runs from 0 to numAvgs_-1

     public:
      avg_impl(int vlen, int numAvgs, int avgType, float alpha, float beta, float delta, float gamma);
      ~avg_impl();

      void forecast(int noutput_items, gr_vector_int& ninput_items_required);

      // Where all the action really happens
      int general_work(
              int noutput_items,
              gr_vector_int &ninput_items,
              gr_vector_const_void_star &input_items,
              gr_vector_void_star &output_items
      );
    };

  } // namespace rad_astro
} // namespace gr

#endif /* INCLUDED_RAD_ASTRO_AVG_IMPL_H */
