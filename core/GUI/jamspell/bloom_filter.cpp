#include <cassert>
#include <memory>

#include "bloom_filter.hpp"

#include "../contrib/bloom/bloom_filter.hpp"
#include "../contrib/handypack/handypack.hpp"

namespace NJamSpell {
    struct TBloomFilter::Impl : bloom_filter {
        Impl() : bloom_filter() {
        }

        Impl(const bloom_parameters &params) : bloom_filter(params) {
        }

        Impl(const TBloomFilter::Impl &bloomFilter) : bloom_filter(bloomFilter) {
        }

        ~Impl() override {
        }

        void Dump(std::ostream &out) const {
            NHandyPack::Dump(out, salt_, bit_table_, salt_count_, table_size_,
                             projected_element_count_, inserted_element_count_,
                             random_seed_, desired_false_positive_probability_);
        }

        void Load(std::istream &in) {
            NHandyPack::Load(in, salt_, bit_table_, salt_count_, table_size_,
                             projected_element_count_, inserted_element_count_,
                             random_seed_, desired_false_positive_probability_);
        }
    };

    TBloomFilter::TBloomFilter() {
        BloomFilter = std::make_unique<Impl>();
    }

    TBloomFilter::TBloomFilter(const uint64_t elements, const double falsePositiveRate) {
        bloom_parameters parameters;
        parameters.projected_element_count    = elements;
        parameters.false_positive_probability = falsePositiveRate;
        parameters.random_seed                = 42;
        parameters.compute_optimal_parameters();
        assert(parameters);
        BloomFilter = std::make_unique<Impl>(parameters);
    }

    TBloomFilter::~TBloomFilter() = default;

    void TBloomFilter::Insert(const std::string &element) const {
        BloomFilter->insert(element);
    }

    bool TBloomFilter::contains(const std::string &element) const {
        return BloomFilter->contains(element);
    }

    void TBloomFilter::Dump(std::ostream &out) const {
        BloomFilter->Dump(out);
    }

    void TBloomFilter::Load(std::istream &in) {
        BloomFilter->Load(in);
    }
} // NJamSpell
