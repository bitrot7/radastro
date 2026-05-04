/* -*- c++ -*- */
/*
 * Copyright 2026 Q.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <gnuradio/io_signature.h>
#include "filewriter_impl.h"
#include <chrono>
#include <iomanip>  // std::put_time
#include <ctime>    // std::time_t, std::localtime
#include <sstream>  // std::ostringstream

namespace gr {
  namespace rad_astro {
    using input_type = float;
    filewriter::sptr
    filewriter::make(int vlen, std::string& filename)
    {
      return gnuradio::make_block_sptr<filewriter_impl>(vlen,
        filename);
    }

    std::string filewriter_impl::GetTimestamp() 
    {
      auto t = std::time(nullptr);
      auto tm = *std::localtime(&t);

      std::ostringstream oss;
      // Format: YYYY-MM-DD_HH-MM-SS (Safe for filenames)
      oss << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S");
      return oss.str();
    }

    /*
     * The private constructor
     */
    filewriter_impl::filewriter_impl(int vlen, std::string& filename)
      : gr::block("filewriter",
              gr::io_signature::make(1 /* min inputs */, 1 /* max inputs */, sizeof(input_type) * vlen),
              gr::io_signature::make(0, 0, 0)),
              vlen_(vlen),
              baseFilename_(filename)
    {
      // Integrated FFT data usually though could be any floating point vector really.
      outFileData_ = std::ofstream(baseFilename_ + GetTimestamp() + ".bin", std::ios::out | std::ios::binary);
      // Contains timestamp of the time the data was recorded (must factor in integration time / 2 to get center of energy being integrated).
      outFileMeta_= std::ofstream(baseFilename_ + "timestamps_" + GetTimestamp() + ".txt");
      
    }

    /*
     * Our virtual destructor.
     */
    filewriter_impl::~filewriter_impl()
    {
      outFileData_.close();
      outFileMeta_.close();
    }

    void filewriter_impl::forecast(int noutput_items,
                                   gr_vector_int& ninput_items_required)
    {
      // Example: For every output item. require num averages.
      unsigned ninputs = ninput_items_required.size();
      /* for each output vector, d_n input vectors are needed */
      for(unsigned int i = 0; i < ninputs; i++)
      {
        ninput_items_required[i] = noutput_items; // require numAvgs worth of vlen input vectors to produce single vlen output vector.
      }
    }

    int
    filewriter_impl::general_work(
              int noutput_items,
              gr_vector_int &ninput_items,
              gr_vector_const_void_star &input_items,
              gr_vector_void_star &output_items)
    {
      auto in = static_cast<const input_type*>(input_items[0]);


      //std::cout << "N OUTPUT ITEMS " << input_items.size() << std::endl;
      // if any input.
      if(ninput_items[0] > 0)
      {
        // loop over first sample of all vectors.
        // Write vector data to a file.
        if (outFileData_.is_open()) 
        {
            // Cast the address of your variable to (char*) or (const char*)
            outFileData_.write(reinterpret_cast<const char*>(in), sizeof(input_type)*ninput_items[0]*vlen_);
            outFileData_.flush();
        }

        // Write timestamp to meta file for this vectors recording.
        if(outFileMeta_.is_open())
        {
          // Get current time from the system clock
          auto now = std::chrono::system_clock::now();

          // Calculate duration since the epoch and cast it to milliseconds
          auto duration = now.time_since_epoch();
          auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

          outFileMeta_ << millis << std::endl;
          outFileMeta_.flush();
        }
        consume_each(noutput_items);
      }
      
      // Tell runtime system how many output items we produced.
      return 0;
    }

  } /* namespace rad_astro */
} /* namespace gr */
