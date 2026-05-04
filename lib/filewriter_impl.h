/* -*- c++ -*- */
/*
 * Copyright 2026 Q.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_RAD_ASTRO_FILEWRITER_IMPL_H
#define INCLUDED_RAD_ASTRO_FILEWRITER_IMPL_H

#include <gnuradio/rad_astro/filewriter.h>
#include <fstream>
namespace gr {
  namespace rad_astro {

    class filewriter_impl : public filewriter
    {
     private:
      // Nothing to declare in this block.
      int vlen_;
      std::string baseFilename_;

      std::ofstream outFileData_;
      std::ofstream outFileMeta_;
      std::string GetTimestamp();
     public:
      filewriter_impl(int vlen, std::string& filename);
      ~filewriter_impl();

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

#endif /* INCLUDED_RAD_ASTRO_FILEWRITER_IMPL_H */
