/*
 Header file with some useful functions
 */

module;

#include <iostream>
#include <regex>

/**
 Module with different useful functions.
 */
export module UtilFuncs_mod;

/**
 Namespace with utility functions of the C+ compiler
 */
namespace Utility {
    /**
     Function for replacing all strings in string
     @param str source string
     @param replace replace this in source
     @param with replace with this string
     @return string with replacements
     */
    export [[maybe_unused]] inline std::string &replace_string_all(std::string &str, const std::string &replace, const std::string &with) {
        if (!replace.empty()) {
            std::size_t pos = 0;
            while ((pos = str.find(replace, pos)) != std::string::npos) {
                str.replace(pos, replace.length(), with);
                pos += with.length();
            }
        }
        return str;
    }

    /**
     * Split string into vector and return changed string.
     * @param s source string to split
     * @param delim delimiter to split on
     * @return vector if you want to assign to variable.
     */
    export std::vector<std::string> split(const std::string &s, const char delim = ' ') {
        std::stringstream ss(s);
        std::string item;
        std::vector<std::string> elems;
        while (std::getline(ss, item, delim)) {
            elems.push_back(item);
        }
        return elems;
    }

    /**
     * Split string into vector and return changed string.
     * @param s source string to split
     * @param delim delimiter to split on
     * @return vector if you want to assign to variable.
     */
    export std::vector<std::string> split_by_first_delim(const std::string &s, const char delim = ' ') {
        size_t pos = s.find(delim);
        if (pos != std::string::npos) {
            std::stringstream ss(s);
            std::string item;
            std::vector<std::string> elems;

            const std::string firstPart = s.substr(0, pos);
            const std::string secondPart = s.substr(pos + 1);
            elems.push_back(firstPart);
            elems.push_back(secondPart);
            return elems;
        }
        throw std::runtime_error("split_by_first_delim: delimiter not found");
    }

    /**
     * Some kind of custom hash function for grouping.
     * Get 5 first characters from source string into "hash"
     * @tparam hash_len_mod length modifier of hash
     * @param str source string object to "count" hash on it.
     * @param group_id name of the group
     * @return "hashed" string
     */
    export template<std::size_t hash_len_mod = 1>
    std::string hash(const std::string &group_id) {
        return std::string("#") + group_id.substr(0, (hash_len_mod % 2 == 0) ? hash_len_mod + 2 : hash_len_mod - 1) + std::string("-");
    }
}
