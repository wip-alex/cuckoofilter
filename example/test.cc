/* -*- Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*- */
#include "cuckoofilter.h"

#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>

using namespace cuckoofilter;

int main(int argc, char** argv) {
    size_t total_items  = 1000000;

    // Create a simple cuckoo filter, accepting keys of size_t and
    // making 12 bits for each key:
    //    CuckooFilter<size_t, 12> filter(total_items);
    // To use semi-sorting, define the cuckoo filter using
    // PackedTable, accepting keys of size_t type and making 13 bits
    // for each key:
    //   CuckooFilter<size_t, 13, PackedTable> filter(total_items);
    CuckooFilter<size_t, 12> filter(total_items);

    // Insert items into this cuckoo filter
    size_t num_inserted = 0;
    for (size_t i = 0; i < total_items; i++, num_inserted++) {
        if (filter.Add(i) != Ok) {
            break;
        }
    }

    // Check if previously inserted items are in the filter, expected
    // true for all items
    for (size_t i = 0; i < num_inserted; i++) {
        assert(filter.Contain(i) == Ok);
    }

    // Check non-existing items, a few false positives expected
    size_t total_queries = 0;
    size_t false_queries = 0;
    for (size_t i = total_items; i < 2 * total_items; i++) {
        if (filter.Contain(i) == Ok) {
            false_queries++;
        }
        total_queries++;
    }

    // Output the measured false positive rate
    std::cout << "false positive rate is "
              << 100.0 * false_queries / total_queries
              << "%\n";

    return 0;
 }
