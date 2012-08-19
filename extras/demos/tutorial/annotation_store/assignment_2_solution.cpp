#include <fstream>
#include <iostream>
#include <seqan/sequence.h>
#include <seqan/file.h>
#include <seqan/store.h>

using namespace seqan;

int main()
{
    FragmentStore<> store;
    std::ifstream file("../../seqan-trunk/extras/demos/tutorial/annotation_store/assignment_annotations.gtf", std::ios_base::in | std::ios_base::binary);
    read(file, store, Gtf());
    // Iterate over all leafs, count and print the result
    Iterator<FragmentStore<>, AnnotationTree<> >::Type it;
    it = begin(store, AnnotationTree<>());
    unsigned count = 0;
    std::cout << "Number of children for each mRNA: " << std::endl;
    // Go down to the first leaf (first child of the first mRNA)
    while (goDown(it)) ;
    ++count;
    while (!atEnd(it)){
        // Iterate over all siblings and count
        while (goRight(it))
            ++count;
        std::cout << count << std::endl;
        count = 0;
        // Jump to the next mRNA or gene, go down to its first leaf and count it
        if (!atEnd(it)) {
            goNext(it);
            if (!atEnd(it)) while(goDown(it)) ;
            if (!atEnd(it))
                ++count;
        }
    }
    return 0;
}