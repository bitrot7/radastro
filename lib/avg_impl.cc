/* -*- c++ -*- */
/*
 * Copyright 2026 Q.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <gnuradio/io_signature.h>
#include "avg_impl.h"

namespace gr {
  namespace rad_astro {

    using input_type = float;
    using output_type = float;

    avg::sptr
    avg::make(int vlen, int numAvgs, int avgType, float alpha, float beta, float delta, float gamma)
    {
      return gnuradio::make_block_sptr<avg_impl>(
        vlen, numAvgs, avgType, alpha, beta, delta, gamma);
    }

    /*
     * The private constructor
     * Take in vector length vlen: (probably 1024 for 1024 point FFT magnitudes coming in).  
     * Input Vector will accumulate vlen*numAvgs samples and we will decimate by numAvgs.  
     * E.g. if numAvgs is 500e3, we will take in 500e3 frames of 1024 samples and average them producing a single 1024 
     * frame average.
     * 
     * avgType = Type of Average, 0 = Straight Average input is vector of floats, 1 = RMS average : sqrt(Average of Mag^2) - Input is assumed mag squared, 2 = EMF, 3 = WMF.
     * 
     * You may need to instantiate multiple blocks.
     */
    avg_impl::avg_impl(int vlen, int numAvgs, int avgType, float alpha, float beta, float delta, float gamma)
      : gr::block("avg",

              gr::io_signature::make(1 /* min inputs */,  1 /* max inputs */, sizeof(input_type) * vlen),  // vectors of vlen come in
              gr::io_signature::make(1 /* min outputs */, 1 /*max outputs */, sizeof(output_type) * vlen )), // vector (singular) of vlen comes out.
              vlen_(vlen),
              numAvgs_(numAvgs),
              avgType_(avgType),
              alpha_(alpha),
              beta_(beta),
              delta_(delta),
              gamma_(gamma)
    {
      // Jesus christ they could have put that in the tutorials.  This gives you more input history.
      set_history(numAvgs_); // numAvgs_ vectors of vlen come in, single vector of vlen comes out.
      // there is also set output multiple for interpolator
      curFftSum_.resize(vlen, 0.0f);
      curFftIter_ = 0;
    }

    /*
     * Our virtual destructor.
     */
    avg_impl::~avg_impl()
    {
    }


    void avg_impl::forecast(int noutput_items, gr_vector_int &ninput_items_required) {
        // Example: For every output item. require num averages.
      unsigned ninputs = ninput_items_required.size();
      /* for each output vector, d_n input vectors are needed */

        ninput_items_required[0] = numAvgs_*noutput_items; // require numAvgs worth of vlen input vectors to produce single vlen output vector.

      // std::cout << "MJK: " << ninput_items_required[0] << std::endl;
      // std::cout << "MJK2: " << noutput_items << std::endl;
      
    }


    int
    avg_impl::general_work(int noutput_items,
        gr_vector_int &ninput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      // std::cout << "MJK: " << ninput_items.size() << std::endl; 1
      // std::cout << "MJK2: " << ninput_items[0] << std::endl; 60410
      // std::cout << "MJK3: " << noutput_items << std::endl; 1

      const auto in = (const input_type *) input_items[0];
      auto out = (output_type *) output_items[0];

      // number of iterations of FFT size for this average.
      // scheduler can give us a bunch to iterate on.
      // int niters = ninput_items[0] / vlen_;

      // for (int j = 0; j < niters; j++) 
      // {
      //        float sum = 0;
      //        const float a0 = (1.0f - alpha_);
      //        float a = a0;

      //        // loop over vectors.
      //        for (int i = 0; i < vlen_; i++) 
      //        { 
      //          if(i == 0)
      //          {
      //            sum = in[i * vlen_ + j];
      //          }
      //          else
      //          {
      //            sum += in[i * vlen_ + j];
      //            //a = a*a0; // (1-a), (1-a)^2, (1-a)^3....
      //            //sum += a*in[i * vlen_ + j];
      //            //a = a*a0; // (1-a), (1-a)^2, (1-a)^3....
      //          }

      //        }
      //        out[j] = sum / numAvgs_;
      // }

      // consume_each(ninput_items[0]); 

      // return vlen_;

      // noutput_items is the sample or number of samples coming in from the last block.
      // In the work or general_work function, the noutput_items parameter refers to the number of vectors available, not individual samples

      // Non moving averages
      // if(avgType_ < 2)
      // {
        //Currently only non moving 
        //loop over first sample of all vectors.
        //samples available on first input port.
        //only one input stream.
        //if I had more input ports would be more input streams and would need to index.

        // UMM hey dum dum ninput_items is ninput_items of vlen input vectors.
        // int niters = ninput_items[0] / vlen_;


        for (int i = 0; i < numAvgs_; i++)
        {
          for (int j = 0; j < vlen_; j++) 
          {
            // vlen_ is effectively fft size.
            // fft*[0 .. N-1] + sample index.
            out[j] += in[i*vlen_ + j];
          }
          curFftIter_++; // increment number of averages that occurred.
        } 
        

        static int fuckyou = 10000;
        if(fuckyou >= 10000)
        {
          std::cout << "Which iteration of the average are we on " << curFftIter_ << std::endl;
          std::cout << "ninput_items " << ninput_items[0] << std::endl;
          std::cout << " each item is vlen size" << vlen_ << std::endl;
          std::cout << " output items of vlen " << noutput_items << std::endl;
          fuckyou = 0;
        }
        fuckyou++;

        // Compute average.
        if(curFftIter_ >= numAvgs_)
        {
          // Straight Average assumes data is magnitude.
          // "Arithmetic mean"
          // Best for observing small signals near the noise floor.
          if(avgType_ == 0)
          {
            // sum all of samples in index 0 per vector.  
            // then 2, then 3 and get the average and store in decimated output.
            for (int j = 0; j < vlen_; j++) 
            {
              out[j] = out[j] / numAvgs_;
            }
          }
          // RMS average assumes data is magnitude squared.
          // divide by squared magnitude and take sqrt.
          // "Quadratic Mean"
          // RMS is better for power measurement.  
          else if(avgType_ == 1)
          {
            for (int j = 0; j < vlen_; j++) 
            {
              out[j] = sqrt(out[j] / (numAvgs_*numAvgs_));
            }
          }

          // reset to zero.
          // for (int j = 0; j < vlen_; j++) 
          // {
          //   curFftSum_[j] = 0.0f;
          // }

          curFftIter_ = 0; // Reset to zero for next set of averages.

          //https://wiki.gnuradio.org/index.php/BlocksCodingGuide
          //
          //  Item = a unit of data, e.g. vector, sample, matrix.
          consume_each(numAvgs_); 
          
          // Return the averaged vector
          return noutput_items;
        }
        //consume_each(ninput_items[0]); 
        return 0; // Return nothing, still averaging.
      // }
    }
      // }
      // else // moving averages EMA and WMA.
      // {
      //   // EMA.
      //   // if(avgType_ == 2)
      //   // {
      //   //   // loop over first sample of all vectors.

          
      //   // }
      //   // else //if(avgType_ == 3) // TODO Q:
      //   // {
      //     // not implemented correct same as EMA.
      //     for (int j = 0; j < vlen_; j++) 
      //     {
      //       float sum = 0;
      //       const float a0 = (1.0f - alpha_);
      //       float a = a0;

      //       // loop over vectors.
      //       for (int i = 0; i < numAvgs_; i++) 
      //       { 
      //         if(i == 0)
      //         {
      //           sum = in[i * vlen_ + j];
      //         }
      //         else
      //         {
      //           sum += a*in[i * vlen_ + j];
      //           a = a*a0; // (1-a), (1-a)^2, (1-a)^3....
      //         }

      //       }
      //       out[j] = sum / numAvgs_;
      //     }

      //   //}
      // }



  } /* namespace rad_astro */
} /* namespace gr */
