#include <fstream>
#include <sstream>
#include <chrono>
#include <cassert>
#include <iostream>
#include <cassert>
#include <algorithm>

#ifdef USE_BOOST_CONVERT
#include <boost/locale/encoding_utf.hpp>
#else

#include <codecvt>

#endif

#include "utils.hpp"
#include "../contrib/cityhash/city.h"

namespace NJamSpell {
    std::string LoadFile(const std::string &fileName) {
        const std::ifstream in(fileName, std::ios::binary);
        std::ostringstream  out;
        out << in.rdbuf();
        return out.str();
    }

    void SaveFile(const std::string &fileName, const std::string &data) {
        std::ofstream out(fileName, std::ios::binary);
        out << data;
    }

    TTokenizer::TTokenizer()
        : Locale(std::locale::classic()) {
    }

    /**
     * Load alphabet file data into context
     * @param alphabetFile name of the file with alphabet
     * @return bool value of operation result
     */
    bool TTokenizer::LoadAlphabet(const std::string &alphabetFile) {
        const std::string data = LoadFile(alphabetFile);
        if (data.empty()) {
            return false;
        }
        std::wstring wdata = UTF8ToWide(data);
        if (wdata.empty()) {
            return false;
        }
        ToLower(wdata);
        std::unordered_set<wchar_t> alphabet;
        for (auto &chr: wdata) {
            if (chr == 10 || chr == 13) {
                continue;
            }
            alphabet.insert(chr);
        }
        if (alphabet.empty()) {
            return false;
        }
        Alphabet = alphabet;
        return true;
    }

    TSentences TTokenizer::Process(const std::wstring &originalText) const {
        if (originalText.empty()) {
            return TSentences();
        }

        TSentences sentences;

        TWords currSentence;
        TWord  currWord;

        for (size_t i = 0; i < originalText.size(); ++i) {
            wchar_t letter = std::tolower(originalText[i], Locale);
            if (Alphabet.contains(letter)) {
                if (currWord.Ptr == nullptr) {
                    currWord.Ptr = &originalText[i];
                }
                currWord.Len += 1;
            } else {
                if (currWord.Ptr != nullptr) {
                    currSentence.push_back(currWord);
                    currWord = TWord();
                }
            }
            if (letter == L'?' || letter == L'!' || letter == L'.') {
                if (!currSentence.empty()) {
                    sentences.push_back(currSentence);
                    currSentence.clear();
                }
            }
        }
        if (currWord.Ptr != nullptr) {
            currSentence.push_back(currWord);
        }
        if (!currSentence.empty()) {
            sentences.push_back(currSentence);
        }

        return sentences;
    }

    void TTokenizer::Clear() {
        Alphabet.clear();
    }

    const std::unordered_set<wchar_t> &TTokenizer::GetAlphabet() const {
        return Alphabet;
    }

    /**
     * Convert usual (std::string) into wide string
     * @param text given usual string
     * @return wide string object
     */
    std::wstring UTF8ToWide(const std::string &text) {
#ifdef USE_BOOST_CONVERT
        using boost::locale::conv::utf_to_utf;
        return utf_to_utf<wchar_t>(text.c_str(), text.c_str() + text.size());
#else
#pragma message("Using deprecated std function")
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t, 0x10ffff, std::little_endian> > converter;
        return converter.from_bytes(text);
#endif
    }

    /**
     * Convert wide string into usual std::string
     * @param text given wide text string
     * @return usual string object
     */
    std::string WideToUTF8(const std::wstring &text) {
#ifdef USE_BOOST_CONVERT
        using boost::locale::conv::utf_to_utf;
        return utf_to_utf<char>(text.c_str(), text.c_str() + text.size());
#else
#pragma message("Using deprecated std function")
        using convert_type = std::codecvt_utf8<wchar_t, 0x10ffff, std::little_endian>;
        std::wstring_convert<convert_type, wchar_t> converter;
        return converter.to_bytes(text);
#endif
    }

    /**
     * Get current time in milliseconds
     * @return
     */
    uint64_t GetCurrentTimeMs() {
        using namespace std::chrono;
        const auto ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
        return ms.count();
    }

    static const std::locale          GLocale(std::locale::classic());
    static const std::ctype<wchar_t> &GWctype = std::use_facet<std::ctype<wchar_t> >(GLocale);

    void ToLower(std::wstring &text) {
        std::ranges::transform(text, text.begin(), [](const wchar_t wch) {
            return GWctype.tolower(wch);
        });
    }

    wchar_t MakeUpperIfRequired(const wchar_t orig, const wchar_t sample) {
        if (GWctype.toupper(sample) == sample) {
            return GWctype.toupper(orig);
        }
        return orig;
    }

    uint16_t CityHash16(const std::string &str) {
        const uint32_t hash = CityHash32(&str[0], str.size());
        return hash % std::numeric_limits<uint16_t>::max();
    }

    uint16_t CityHash16(const char *str, size_t size) {
        const uint32_t hash = CityHash32(str, size);
        return hash % std::numeric_limits<uint16_t>::max();
    }
} // NJamSpell
