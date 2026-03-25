#include "spell_corrector.hpp"
#include <algorithm>
#include <fstream>

/**
 * Main namespace for inner neuro network
 */
namespace NJamSpell {
    static Wide_vec_t GetDeletes1(const std::wstring &w) {
        Wide_vec_t results;
        for (size_t i = 0; i < w.size(); ++i) {
            auto nw = w.substr(0, i) + w.substr(i + 1);
            if (!nw.empty()) {
                results.push_back(nw);
            }
        }
        return results;
    }

    static std::vector<Wide_vec_t> GetDeletes2(const std::wstring &w) {
        std::vector<Wide_vec_t> results;
        for (size_t i = 0; i < w.size(); ++i) {
            auto nw = w.substr(0, i) + w.substr(i + 1);
            if (!nw.empty()) {
                Wide_vec_t currResults = GetDeletes1(nw);
                currResults.push_back(nw);
                results.push_back(currResults);
            }
        }
        return results;
    }

    bool TSpellCorrector::load_lang_model(Ustring_t &modelFile) {
        if (!LangModel.Load(modelFile)) {
            return false;
        }
        std::string cacheFile = modelFile + cache_postfix;
        if (!load_cache(cacheFile)) {
            prepare_cache();
            save_cache(cacheFile);
        }
        return true;
    }

    bool TSpellCorrector::train_model(Ustring_t &txt_file, Ustring_t &alphabet_file, Ustring_t &modelFile) {
        if (!LangModel.Train(txt_file, alphabet_file)) {
            return false;
        }
        prepare_cache();
        if (!LangModel.Dump(modelFile)) {
            return false;
        }
        std::string cacheFile = modelFile + cache_postfix;
        if (!save_cache(cacheFile)) {
            return false;
        }
        return true;
    }

    [[nodiscard]] TScoredWords
    TSpellCorrector::get_raw_candidates_with_scores(const TWords &sentence, const size_t position) const {
        TScoredWords scoredCandidates;

        if (position >= sentence.size()) {
            return scoredCandidates;
        }

        TWord  w          = sentence[position];
        TWords candidates = Edits2(w);

        bool firstLevel = true;
        bool knownWord  = false;
        if (candidates.empty()) {
            candidates = Edits(w);
            firstLevel = false;
        }

        if (candidates.empty()) {
            return scoredCandidates;
        }

        //Candidate get word section
        {
            TWord c = LangModel.Get_word(std::wstring(w.Ptr, w.Len));
            if (c.Ptr && c.Len) {
                w = c;
                candidates.push_back(c);
                knownWord = true;
            } else {
                candidates.push_back(w);
            }
        }

        std::unordered_set<TWord, TWordHashPtr> uniqueCandidates(candidates.begin(), candidates.end());

        FilterCandidatesByFrequency(uniqueCandidates, w);
        scoredCandidates.reserve(uniqueCandidates.size());

        for (TWord cand: uniqueCandidates) {
            TWords candSentence;
            for (size_t i = 0; i < sentence.size(); ++i) {
                if (i == position) {
                    candSentence.push_back(cand);
                } else if ((i < position && i + 2 >= position) ||
                           (i > position && i <= position + 2)) {
                    candSentence.push_back(sentence[i]);
                }
            }

            TScoredWord scored;
            scored.Word  = cand;
            scored.Score = LangModel.Score(candSentence);
            if (scored.Word != w) {
                if (knownWord) {
                    if (firstLevel) {
                        scored.Score -= KnownWordsPenalty;
                    } else {
                        scored.Score *= 50.0;
                    }
                } else {
                    scored.Score -= UnknownWordsPenalty;
                }
            }
            scoredCandidates.push_back(scored);
        }

        std::sort(scoredCandidates.begin(), scoredCandidates.end(), [](const TScoredWord &w1, const TScoredWord &w2) {
            return w1.Score > w2.Score;
        });
        return scoredCandidates;
    }

    /**
     * Is word already known to language model
     * @param word wide string object
     * @return bool result, true if model knows word
     */
    [[maybe_unused]] bool TSpellCorrector::is_word_known(Widestring_t &word) const {
        TWord w = LangModel.Get_word(word);
        if (w.Ptr && w.Len) {
            return true;
        }
        return false;
    }

    [[maybe_unused]] TWords TSpellCorrector::get_candidates_raw(const NJamSpell::TWords &sentence, size_t position) const {
        TWords       candidates;
        TScoredWords scoredCandidates = get_raw_candidates_with_scores(sentence, position);

        for (auto s: scoredCandidates) {
            candidates.push_back(s.Word);
        }
        return candidates;
    }

    [[maybe_unused]] void
    TSpellCorrector::FilterCandidatesByFrequency(std::unordered_set<TWord, TWordHashPtr> &uniqueCandidates, TWord origWord) const {
        if (uniqueCandidates.size() <= MaxCandidatesToCheck) {
            return;
        }

        using TCountCand = std::pair<TCount, TWord>;
        std::vector<TCountCand> candidateCounts;
        for (auto &&c: uniqueCandidates) {
            TCount cnt = LangModel.GetWordCount(LangModel.GetWordIdNoCreate(c));
            candidateCounts.emplace_back(cnt, c);
        }
        uniqueCandidates.clear();
        std::stable_sort(candidateCounts.begin(), candidateCounts.end(), [](const TCountCand &a, const TCountCand &b) {
            return a.first > b.first;
        });

        for (size_t i = 0; i < MaxCandidatesToCheck; ++i) {
            uniqueCandidates.insert(candidateCounts[i].second);
        }
        uniqueCandidates.insert(origWord);
    }

    [[maybe_unused]] std::vector<std::pair<std::wstring, double> > TSpellCorrector::get_candidates_with_scores(
        const Wide_vec_t &sentence, size_t position
    ) const {
        const TWords words(sentence.begin(), sentence.end());
        TScoredWords scoredCandidates = get_raw_candidates_with_scores(words, position);

        std::vector<std::pair<std::wstring, double> > results;
        for (auto [Word, Score]: scoredCandidates) {
            auto word = std::wstring(Word.Ptr, Word.Len);
            results.emplace_back(word, Score);
        }
        return results;
    }

    [[maybe_unused]] Wide_vec_t
    TSpellCorrector::get_candidates(const Wide_vec_t &sentence, size_t position) const {
        TWords words;
        for (auto &&w: sentence) {
            words.emplace_back(w);
        }
        TWords     candidates = get_candidates_raw(words, position);
        Wide_vec_t results;
        for (auto &&c: candidates) {
            results.emplace_back(c.Ptr, c.Len);
        }
        return results;
    }

    [[maybe_unused]] Widestring_t TSpellCorrector::fix_fragment(Widestring_t &text) const {
        TSentences   origSentences = LangModel.Tokenize(text);
        std::wstring lowered       = text;
        ToLower(lowered);
        TSentences   sentences = LangModel.Tokenize(lowered);
        std::wstring result;
        size_t       origPos = 0;
        for (size_t i = 0; i < sentences.size(); ++i) {
            TWords        words     = sentences[i];
            const TWords &origWords = origSentences[i];
            for (size_t j = 0; j < words.size(); ++j) {
                const TWord orig          = origWords[j];
                const TWord lowered_tword = words[j];
                TWords      candidates    = get_candidates_raw(words, j);
                if (!candidates.empty()) {
                    words[j] = candidates[0];
                }
                const size_t currOrigPos = orig.Ptr - &text[0];
                while (origPos < currOrigPos) {
                    result.push_back(text[origPos]);
                    origPos += 1;
                }
                auto newWord     = std::wstring(words[j].Ptr, words[j].Len);
                auto origWord    = std::wstring(orig.Ptr, orig.Len);
                auto origLowered = std::wstring(lowered_tword.Ptr, lowered_tword.Len);
                if (newWord != origLowered) {
                    for (size_t k = 0; k < newWord.size(); ++k) {
                        size_t        n        = k < origWord.size() ? k : origWord.size() - 1;
                        const wchar_t newChar  = newWord[k];
                        const wchar_t origChar = origWord[n];
                        result.push_back(MakeUpperIfRequired(newChar, origChar));
                    }
                } else {
                    result += origWord;
                }
                origPos += orig.Len;
            }
        }
        while (origPos < text.size()) {
            result.push_back(text[origPos]);
            origPos += 1;
        }
        return result;
    }

    [[maybe_unused]] Widestring_t TSpellCorrector::FixFragmentNormalized(Widestring_t &text) const {
        std::wstring lowered = text;
        ToLower(lowered);
        TSentences   sentences = LangModel.Tokenize(lowered);
        std::wstring result;
        for (auto words: sentences) {
            for (size_t y = 0; y < words.size(); ++y) {
                TWords candidates = get_candidates_raw(words, y);
                if (!candidates.empty()) {
                    words[y] = candidates[0];
                }
                result += std::wstring(words[y].Ptr, words[y].Len) + L" ";
            }
            if (!words.empty()) {
                result.resize(result.size() - 1);
                result += L". ";
            }
        }
        if (!result.empty()) {
            result.resize(result.size() - 1);
        }
        return result;
    }

    [[maybe_unused]] void TSpellCorrector::set_penalty(double knownWordsPenalty, double unknownWordsPenalty) {
        KnownWordsPenalty   = knownWordsPenalty;
        UnknownWordsPenalty = unknownWordsPenalty;
    }

    [[maybe_unused]] void TSpellCorrector::SetMaxCandidatesToCheck(size_t maxCandidatesToCheck) {
        MaxCandidatesToCheck = maxCandidatesToCheck;
    }

    [[maybe_unused]] const TLangModel &TSpellCorrector::get_lang_model() const {
        return LangModel;
    }

    /**
     * Add vector (or another stl container) to target
     * @tparam T stl container with insert method
     * @param target
     * @param source
     */
    template<typename T>
    inline void AddVec(T &target, const T &source) {
        target.insert(target.end(), source.begin(), source.end());
    }

    [[maybe_unused]] TWords TSpellCorrector::Edits(const TWord &word) const {
        std::wstring w(word.Ptr, word.Len);
        TWords       result;

        std::vector<Wide_vec_t> cands = GetDeletes2(w); //candidates
        cands.push_back(Wide_vec_t({w}));

        for (auto &&w1: cands) {
            for (auto &&w2: w1) {
                TWord c = LangModel.Get_word(w2);
                if (c.Ptr && c.Len) {
                    result.push_back(c);
                }
                std::string s = WideToUTF8(w2);
                if (Deletes1->contains(s)) {
                    Inserts(w2, result);
                }
                if (Deletes2->contains(s)) {
                    Inserts2(w2, result);
                }
            }
        }

        return result;
    }

    [[maybe_unused]] TWords TSpellCorrector::Edits2(const TWord &word, bool lastLevel) const {
        const std::wstring w(word.Ptr, word.Len);
        TWords             result;

        for (size_t i = 0; i < w.size() + 1; ++i) {
            // delete
            if (i < w.size()) {
                std::wstring s = w.substr(0, i) + w.substr(i + 1);
                TWord        c = LangModel.Get_word(s);
                if (c.Ptr && c.Len) {
                    result.push_back(c);
                }
                if (!lastLevel) {
                    AddVec(result, Edits2(TWord(s)));
                }
            }

            // transpose
            if (i + 1 < w.size()) {
                std::wstring s = w.substr(0, i);
                s              += w.substr(i + 1, 1);
                s              += w.substr(i, 1);
                if (i + 2 < w.size()) {
                    s += w.substr(i + 2);
                }
                TWord c = LangModel.Get_word(s);
                if (c.Ptr && c.Len) {
                    result.push_back(c);
                }
                if (!lastLevel) {
                    AddVec(result, Edits2(TWord(s)));
                }
            }

            // replace
            if (i < w.size()) {
                for (auto &&ch: LangModel.GetAlphabet()) {
                    std::wstring s = w.substr(0, i) + ch + w.substr(i + 1);
                    TWord        c = LangModel.Get_word(s);
                    if (c.Ptr && c.Len) {
                        result.push_back(c);
                    }
                    if (!lastLevel) {
                        AddVec(result, Edits2(TWord(s)));
                    }
                }
            }

            // inserts
            {
                for (auto &&ch: LangModel.GetAlphabet()) {
                    std::wstring s = w.substr(0, i) + ch + w.substr(i);
                    TWord        c = LangModel.Get_word(s);
                    if (c.Ptr && c.Len) {
                        result.push_back(c);
                    }
                    if (!lastLevel) {
                        AddVec(result, Edits2(TWord(s)));
                    }
                }
            }
        }

        return result;
    }

    [[maybe_unused]] void TSpellCorrector::Inserts(const std::wstring &w, TWords &result) const {
        for (size_t i = 0; i < w.size() + 1; ++i) {
            for (auto &&ch: LangModel.GetAlphabet()) {
                std::wstring s = w.substr(0, i) + ch + w.substr(i);
                TWord        c = LangModel.Get_word(s);
                if (c.Ptr && c.Len) {
                    result.push_back(c);
                }
            }
        }
    }

    [[maybe_unused]] void TSpellCorrector::Inserts2(const std::wstring &w, TWords &result) const {
        for (size_t i = 0; i < w.size() + 1; ++i) {
            for (auto &&ch: LangModel.GetAlphabet()) {
                std::wstring s = w.substr(0, i) + ch + w.substr(i);
                if (Deletes1->contains(WideToUTF8(s))) {
                    Inserts(s, result);
                }
            }
        }
    }

    [[maybe_unused]] void TSpellCorrector::prepare_cache() {
        auto &&wordToId = LangModel.GetWordToId();
        size_t n        = 0;
        size_t s        = 0;
        for (auto &&it: wordToId) {
            n += 1;
            s += it.first.size();
            if (n > 3000) {
                break;
            }
        }
        const size_t avgWordLen         = std::max(int(double(s) / n) + 1, 1);
        const size_t avgWordLenMinusOne = std::max(size_t(1), avgWordLen - 1);

        uint64_t       deletes1size = wordToId.size() * avgWordLen;
        const uint64_t deletes2size = wordToId.size() * avgWordLen * avgWordLenMinusOne;
        deletes1size                = std::max(uint64_t(1000), deletes1size);
        deletes1size                = std::max(uint64_t(1000), deletes1size);

        constexpr double falsePositiveProb = 0.001;
        Deletes1.reset(new TBloomFilter(deletes1size, falsePositiveProb));
        Deletes2.reset(new TBloomFilter(deletes2size, falsePositiveProb));

        uint64_t deletes1real = 0;
        uint64_t deletes2real = 0;

        for (auto &&it: wordToId) {
            auto deletes = GetDeletes2(it.first);
            for (auto &&w1: deletes) {
                Deletes1->Insert(WideToUTF8(w1.back()));
                deletes1real       += 1;
                const auto w1_size = w1.size();
                for (size_t i = 0; i < w1_size - 1; ++i) {
                    Deletes2->Insert(WideToUTF8(w1[i]));
                    deletes2real += 1;
                }
            }
        }
    }

    constexpr uint64_t SPELL_CHECKER_CACHE_MAGIC_BYTE = 3811558393781437494L;
    constexpr uint16_t SPELL_CHECKER_CACHE_VERSION    = 1;

    /**
     * Load cache into context
     * @param cacheFile file for cache
     * @return result bool value
     */
    [[maybe_unused]] bool TSpellCorrector::load_cache(const std::string &cacheFile) {
        std::ifstream in(cacheFile, std::ios::binary);
        if (!in.is_open()) {
            return false;
        }
        uint16_t version   = 0;
        uint64_t magicByte = 0;
        NHandyPack::Load(in, magicByte);
        if (magicByte != SPELL_CHECKER_CACHE_MAGIC_BYTE) {
            return false;
        }
        NHandyPack::Load(in, version);
        if (version != SPELL_CHECKER_CACHE_VERSION) {
            return false;
        }
        uint64_t checkSum = 0;
        NHandyPack::Load(in, checkSum);
        if (checkSum != LangModel.GetCheckSum()) {
            return false;
        }
        std::unique_ptr<TBloomFilter> deletes1 = std::make_unique<TBloomFilter>();
        std::unique_ptr<TBloomFilter> deletes2 = std::make_unique<TBloomFilter>();
        deletes1->Load(in);
        deletes2->Load(in);
        magicByte = 0;
        NHandyPack::Load(in, magicByte);
        if (magicByte != SPELL_CHECKER_CACHE_MAGIC_BYTE) {
            return false;
        }
        Deletes1 = std::move(deletes1);
        Deletes2 = std::move(deletes2);
        return true;
    }

    /**
     * Save cache in context
     * @param cacheFile file for cache
     * @return result bool value
     */
    [[maybe_unused]] bool TSpellCorrector::save_cache(const std::string &cacheFile) {
        std::ofstream out(cacheFile, std::ios::binary);
        if (!out.is_open()) {
            return false;
        }
        if (!Deletes1 || !Deletes2) {
            return false;
        }
        NHandyPack::Dump(out, SPELL_CHECKER_CACHE_MAGIC_BYTE);
        NHandyPack::Dump(out, SPELL_CHECKER_CACHE_VERSION);
        NHandyPack::Dump(out, LangModel.GetCheckSum());
        Deletes1->Dump(out);
        Deletes2->Dump(out);
        NHandyPack::Dump(out, SPELL_CHECKER_CACHE_MAGIC_BYTE);
        return true;
    }

    /**
     * Get models list
     * @return vector with models
     */
    std::vector<TLangModel> TSpellCorrector::get_models_list() const {
        //        auto models_list = std::filesystem::current;
        //        if (models_list.size() != 0) {
        //            //
        //        }
    }
} // NJamSpell
