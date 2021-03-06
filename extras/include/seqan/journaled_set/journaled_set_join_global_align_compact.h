// ==========================================================================
//                 SeqAn - The Library for Sequence Analysis
// ==========================================================================
// Copyright (c) 2006-2012, Knut Reinert, FU Berlin
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of Knut Reinert or the FU Berlin nor the names of
//       its contributors may be used to endorse or promote products derived
//       from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL KNUT REINERT OR THE FU BERLIN BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
// DAMAGE.
//
// ==========================================================================
// Author: Rene Rahn <rene.rahn@fu-berlin.de>
// ==========================================================================

#ifndef EXTRAS_INCLUDE_SEQAN_JOURNALED_SET_JOURNALED_SET_JOIN_GLOBAL_ALIGN_COMPACT_H_
#define EXTRAS_INCLUDE_SEQAN_JOURNALED_SET_JOURNALED_SET_JOIN_GLOBAL_ALIGN_COMPACT_H_

namespace seqan {

// ============================================================================
// Forwards
// ============================================================================

// ============================================================================
// Tags, Classes, Enums
// ============================================================================

// ============================================================================
// Metafunctions
// ============================================================================

// ============================================================================
// Functions
// ============================================================================

// ----------------------------------------------------------------------------
// Function _joinInternal()                     [GlobalAlign<JournaledCompact>]
// ----------------------------------------------------------------------------

template <typename TValue, typename THostSpec, typename TJournalSpec, typename TBuffSpec>
inline void
_joinInternal(String <TValue, THostSpec> const & reference,
              String <TValue, Journaled<THostSpec, TJournalSpec, TBuffSpec> > & journal,
              JoinConfig<GlobalAlign<JournaledCompact> > const & joinConfig)
{

    typedef String <TValue, Journaled<THostSpec, TJournalSpec, TBuffSpec> > TJournalString;
    typedef typename Size<TJournalString>::Type TSize;
    typedef typename JoinConfig<GlobalAlign<JournaledCompact> >::TScoringScheme TScoringScheme;
    typedef typename Value<TScoringScheme>::Type TScoreValue;

    //assure we compare only to the ref, not to the journal of the ref, which might has a tre

    JournalTraceDescriptor<TJournalString> traceDescriptor;
    String<unsigned char> trace;

    if (isBandSet(joinConfig))
    {
        TScoreValue overallMaxValue[2];
        TSize overallMaxIndex[4];

        _alignBandedGotoh(trace, reference, journal, scoringScheme(joinConfig), overallMaxValue,
                          overallMaxIndex, lowerDiagonal(joinConfig), upperDiagonal(joinConfig),
                          joinConfig._alignConfig);

        // Follow the trace and create the graph
        _alignBandedGotohTrace(traceDescriptor, reference, journal, 0, 0, trace, overallMaxValue, overallMaxIndex,
                               lowerDiagonal(joinConfig), upperDiagonal(joinConfig));
    }
    else
    {
        unsigned char initialDir;
        TScoreValue overallMaxValue[2];
        TSize overallMaxIndex[2];

        _alignGotoh(trace, reference, journal, scoringScheme(joinConfig), overallMaxValue, overallMaxIndex, initialDir,
                    joinConfig._alignConfig);
        _alignGotohTrace(traceDescriptor, reference, journal, 0, 0, trace, overallMaxIndex, initialDir);
    }

    reverseTrace(traceDescriptor);
    _applyTraceOperations(journal, reference, traceDescriptor);
}

}  // namespace seqan

#endif  // #ifndef EXTRAS_INCLUDE_SEQAN_JOURNALED_SET_JOURNALED_SET_JOIN_GLOBAL_ALIGN_COMPACT_H_
