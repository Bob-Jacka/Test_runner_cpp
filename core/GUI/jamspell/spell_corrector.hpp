#ifndef SPELL_CORRECTOR
#define SPELL_CORRECTOR

#include <memory>

#include "lang_model.hpp"
#include "bloom_filter.hpp"

namespace NJamSpell {

    using Wide_vec_t = std::vector<std::wstring>; ///vector with wide strings
    using Ustring_t = const std::string; ///usual const string object alias
    using Widestring_t = const std::wstring; ///wide string object alias

    constexpr std::string cache_postfix = ".spell";

    class TSpellCorrector {
    public:
        bool load_lang_model(Ustring_t &modelFile);

        [[nodiscard]] std::vector<TLangModel> get_models_list() const;

        bool train_model(Ustring_t &text_file, Ustring_t &alphabetFile, Ustring_t &modelFile);

        [[maybe_unused]] bool is_word_known(Widestring_t &word) const;

        [[maybe_unused]] NJamSpell::TScoredWords
        get_raw_candidates_with_scores(const NJamSpell::TWords &sentence, size_t position) const;

        [[maybe_unused]] NJamSpell::TWords get_candidates_raw(const NJamSpell::TWords &sentence, size_t position) const;

        [[maybe_unused]] Wide_vec_t
        get_candidates(const std::vector<std::wstring> &sentence, size_t position) const;

        [[maybe_unused]] std::vector<std::pair<std::wstring, double> >
        get_candidates_with_scores(const std::vector<std::wstring> &sentence, size_t position) const;

        [[maybe_unused]] Widestring_t fix_fragment(Widestring_t &text) const;

        [[maybe_unused]] Widestring_t FixFragmentNormalized(Widestring_t &text) const;

        [[maybe_unused]] void set_penalty(double knownWordsPenalty, double unknownWordsPenalty);

        [[maybe_unused]] void SetMaxCandidatesToCheck(size_t maxCandidatesToCheck);

        [[maybe_unused]] const TLangModel &get_lang_model() const;

    private:
        [[maybe_unused]] void
        FilterCandidatesByFrequency(std::unordered_set<NJamSpell::TWord, NJamSpell::TWordHashPtr> &uniqueCandidates,
                                    NJamSpell::TWord origWord) const;

        [[maybe_unused]] NJamSpell::TWords Edits(const NJamSpell::TWord &word) const;

        [[maybe_unused]] NJamSpell::TWords Edits2(const NJamSpell::TWord &word, bool lastLevel = true) const;

        [[maybe_unused]] void Inserts(const std::wstring &w, NJamSpell::TWords &result) const;

        [[maybe_unused]] void Inserts2(const std::wstring &w, NJamSpell::TWords &result) const;

        [[maybe_unused]] void prepare_cache();

        [[maybe_unused]] bool load_cache(const std::string &cacheFile);

        [[maybe_unused]] bool save_cache(const std::string &cacheFile);

    private:
        TLangModel LangModel; //main lang entity object
        double KnownWordsPenalty = 20.0;
        double UnknownWordsPenalty = 5.0;
        size_t MaxCandidatesToCheck = 14;
        std::unique_ptr<TBloomFilter> Deletes1;
        std::unique_ptr<TBloomFilter> Deletes2;
    };

} // NJamSpell

#endif
