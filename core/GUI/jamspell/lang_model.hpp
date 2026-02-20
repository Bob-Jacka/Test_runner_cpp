#ifndef LANG_MODEL_H
#define LANG_MODEL_H

#include <limits>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "perfect_hash.hpp"
#include "utils.hpp"
#include "../contrib/handypack/handypack.hpp"
#include "../contrib/tsl/robin_map.h"

namespace NJamSpell {
    constexpr uint64_t LANG_MODEL_MAGIC_BYTE = 8559322735408079685L;
    constexpr uint16_t LANG_MODEL_VERSION    = 9;
    constexpr double   LANG_MODEL_DEFAULT_K  = 0.05;

    using TWordId = uint32_t;
    using TCount  = uint32_t;

    using TGram1Key    = TWordId;
    using TGram2Key    = std::pair<TWordId, TWordId>;
    using TGram3Key    = std::tuple<TWordId, TWordId, TWordId>;
    using TWordIds     = std::vector<TWordId>;
    using TIdSentences = std::vector<TWordIds>;

    struct TGram2KeyHash {
        std::size_t operator()(const TGram2Key &x) const {
            return (size_t) x.first ^ ((size_t) x.second << 16);
        }
    };

    struct TGram3KeyHash {
        std::size_t operator()(const TGram3Key &x) const {
            return (size_t) std::get<0>(x) ^
                   ((size_t) std::get<1>(x) << 16) ^
                   ((size_t) std::get<2>(x) << 32);
        }
    };

    class TRobinSerializer : public NHandyPack::TUnorderedMapSerializer<tsl::robin_map<std::wstring, TWordId>,
                std::wstring,
                TWordId

            > {
    };

    class TRobinHash final : public tsl::robin_map<std::wstring, TWordId> {
        public:
            inline virtual void Dump(std::ostream &out) const {
                TRobinSerializer::Dump(out, *this);
            }

            inline virtual void Load(std::istream &in) {
                TRobinSerializer::Load(in, *this);
            }
    };

    /**
     * Main class for language model implementation
     */
    class TLangModel final {
        public:
            TLangModel() = default;

            ~TLangModel() = default;

            bool train(const std::string &fileName, const std::string &alphabetFile);

            double score(const TWords &words) const;

            double score(const std::wstring &str) const;

            TWord get_word(const std::wstring &word) const;

            const std::unordered_set<wchar_t> &GetAlphabet() const;

            TSentences Tokenize(const std::wstring &text) const;

            bool Dump(const std::string &modelFileName) const;

            bool Load(const std::string &modelFileName);

            void clear_data();

            const TRobinHash &GetWordToId();

            std::string get_model_name();

            TWordId GetWordId(const TWord &word);

            TWordId GetWordIdNoCreate(const TWord &word) const;

            TWord GetWordById(TWordId wid) const;

            TCount GetWordCount(TWordId wid) const;

            uint64_t GetCheckSum() const;

            HANDYPACK(WordToId, LastWordID, TotalWords, VocabSize,
                      PerfectHash, buckets, Tokenizer, CheckSum
            )

        private: //private methods
            TIdSentences convert_2_id(const TSentences &sentences);

            double GetGram1Prob(TWordId word) const;

            double GetGram2Prob(TWordId word1, TWordId word2) const;

            double GetGram3Prob(TWordId word1, TWordId word2, TWordId word3) const;

            TCount GetGram1HashCount(TWordId word) const;

            TCount GetGram2HashCount(TWordId word1, TWordId word2) const;

            TCount GetGram3HashCount(TWordId word1, TWordId word2, TWordId word3) const;

        private: //private variables
            std::string                                 model_name;
            const TWordId                               UnknownWordId = std::numeric_limits<TWordId>::max();
            double                                      K             = LANG_MODEL_DEFAULT_K;
            TRobinHash                                  WordToId;
            TWordId                                     LastWordID = 0;
            TWordId                                     TotalWords = 0;
            TWordId                                     VocabSize  = 0;
            TTokenizer                                  Tokenizer;
            uint64_t                                    CheckSum = 0;
            TPerfectHash                                PerfectHash;
            std::vector<const std::wstring *>           IdToWord;
            std::vector<std::pair<uint16_t, uint16_t> > buckets;
    };
} // NJamSpell

#endif
