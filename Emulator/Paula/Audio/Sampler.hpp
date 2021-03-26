// -----------------------------------------------------------------------------
// This file is part of vAmiga
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#pragma once

#include "Constants.hpp"
#include "RingBuffer.hpp"
#include "Reflection.hpp"

namespace va {

#include "Sampler.h"

//
// Reflection APIs
//

struct SamplingMethodEnum : util::Reflection<SamplingMethodEnum, SamplingMethod> {
    
    static bool isValid(long value)
    {
        return (unsigned long)value < SMP_COUNT;
    }

    static const char *prefix() { return "SMP"; }
    static const char *key(SamplingMethod value)
    {
        switch (value) {
                
            case SMP_NONE:     return "NONE";
            case SMP_NEAREST:  return "NEAREST";
            case SMP_LINEAR:   return "LINEAR";
            case SMP_COUNT:    return "???";
        }
        return "???";
    }
};

/* This buffer type is used to temporarily store the generated sound samples as
 * they are produced by the state machine. Note that the state machine doesn't
 * output samples at a constant sampling rate. Instead, a new sound sample is
 * generated whenever the period counter underflows. To preserve this timing
 * information, each sample is tagged by the cycle it was produced.
 */

struct TaggedSample
{
    Cycle tag;
    i16   sample;
};

struct Sampler : util::RingBuffer <TaggedSample, VPOS_CNT * HPOS_CNT> {
    
    /* Initializes the ring buffer by removing all existing elements and adding
     * a single dummy element. The dummy element is added because some methods
     * assume that the buffer is never empty.
     */
    void reset();

    // Clones another Sampler
    void clone(Sampler &other);
     
    /* Interpolates a sound sample for the specified target cycle. Two major
     * steps are involved. In the first step, the function computes index
     * position r1 with the following property:
     *
     *     Cycle of sample at r1 <= Target cycle < Cycle of sample at r1
     *
     * In the second step, the function interpolated between the two samples at
     * r1 and r1 + 1 based on the requested method.
     */
    template <SamplingMethod method> i16 interpolate(Cycle clock);
};

}
