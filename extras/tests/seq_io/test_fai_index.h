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
// Author: Manuel Holtgrewe <manuel.holtgrewe@fu-berlin.de>
// ==========================================================================

#ifndef EXTRAS_TESTS_SEQ_IO_TEST_FAI_INDEX_H_
#define EXTRAS_TESTS_SEQ_IO_TEST_FAI_INDEX_H_

#include <seqan/seq_io.h>

SEQAN_DEFINE_TEST(test_seq_io_genomic_fai_index_build)
{
    seqan::CharString filePath = SEQAN_PATH_TO_ROOT();
    append(filePath, "/extras/tests/seq_io/adeno_genome.fa");

    seqan::FaiIndex faiIndex;
    SEQAN_ASSERT_EQ(build(faiIndex, toCString(filePath)), 0);

    SEQAN_ASSERT_EQ(numSeqs(faiIndex), 1u);
    SEQAN_ASSERT_EQ(sequenceLength(faiIndex, 0), 4718u);
    SEQAN_ASSERT_EQ(sequenceName(faiIndex, 0), "gi|9632547|ref|NC_002077.1|");
}

SEQAN_DEFINE_TEST(test_seq_io_genomic_fai_index_write)
{
    seqan::CharString filePath = SEQAN_PATH_TO_ROOT();
    append(filePath, "/extras/tests/seq_io/adeno_genome.fa");

    seqan::FaiIndex faiIndex;
    SEQAN_ASSERT_EQ(build(faiIndex, toCString(filePath)), 0);

    // Write out.
    seqan::CharString tmpOut = SEQAN_TEMP_FILENAME();
    SEQAN_ASSERT_EQ(write(faiIndex, toCString(tmpOut)), 0);

    seqan::CharString pathToExpected = SEQAN_PATH_TO_ROOT();
    append(pathToExpected, "/extras/tests/seq_io/adeno_genome.fa.fai");
    SEQAN_ASSERT_MSG(seqan::_compareTextFiles(toCString(pathToExpected), toCString(tmpOut)), "Output should match example.");
}

SEQAN_DEFINE_TEST(test_seq_io_genomic_fai_index_read)
{
    seqan::CharString filePath = SEQAN_PATH_TO_ROOT();
    append(filePath, "/extras/tests/seq_io/adeno_genome.fa");

    seqan::FaiIndex faiIndex;
    SEQAN_ASSERT_EQ(read(faiIndex, toCString(filePath)), 0);

    SEQAN_ASSERT_EQ(numSeqs(faiIndex), 1u);
    SEQAN_ASSERT_EQ(sequenceLength(faiIndex, 0), 4718u);
    SEQAN_ASSERT_EQ(sequenceName(faiIndex, 0), "gi|9632547|ref|NC_002077.1|");
}

SEQAN_DEFINE_TEST(test_seq_io_genomic_fai_index_read_sequence)
{
    seqan::CharString filePath = SEQAN_PATH_TO_ROOT();
    append(filePath, "/extras/tests/seq_io/adeno_genome.fa");

    seqan::FaiIndex faiIndex;
    SEQAN_ASSERT_EQ(read(faiIndex, toCString(filePath)), 0);

    seqan::Dna5String str;
    SEQAN_ASSERT_EQ(readSequence(str, faiIndex, 0), 0);
    SEQAN_ASSERT_EQ(prefix(str, 20), "TTGCCCACTCCCTCTCTGCG");
    SEQAN_ASSERT_EQ(suffix(str, length(str) - 20), "CGCAGAGAGGGAGTGGGCAA");
}

SEQAN_DEFINE_TEST(test_seq_io_genomic_fai_index_read_region)
{
    // From integers.
    {
        seqan::CharString filePath = SEQAN_PATH_TO_ROOT();
        append(filePath, "/extras/tests/seq_io/adeno_genome.fa");
        
        seqan::FaiIndex faiIndex;
        SEQAN_ASSERT_EQ(read(faiIndex, toCString(filePath)), 0);
        
        seqan::Dna5String str;
        SEQAN_ASSERT_EQ(readRegion(str, faiIndex, 0, 100, 110), 0);
        SEQAN_ASSERT_EQ(str, "GAGCGCGCAG");
    }
    // From GenomicRegion.
    {
        seqan::CharString filePath = SEQAN_PATH_TO_ROOT();
        append(filePath, "/extras/tests/seq_io/adeno_genome.fa");
        
        seqan::FaiIndex faiIndex;
        SEQAN_ASSERT_EQ(read(faiIndex, toCString(filePath)), 0);
        
        seqan::GenomicRegion region("gi|9632547|ref|NC_002077.1|:101-110");
        seqan::Dna5String str;
        SEQAN_ASSERT_EQ(readRegion(str, faiIndex, region), 0);
        SEQAN_ASSERT_EQ(str, "GAGCGCGCAG");
    }
}

#endif  // #ifndef EXTRAS_TESTS_SEQ_IO_TEST_FAI_INDEX_H_
