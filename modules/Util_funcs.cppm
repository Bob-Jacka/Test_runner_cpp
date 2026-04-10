/*
 Header file with some useless functions in check runner utility
 */
module;

#include <iostream>

/**
 Module with different useful functions.
 */
export module UtilFuncs_mod;

/**
 Namespace with utility functions of the C+ compiler
 */
namespace Utility {
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

    /**
     * Check for directive line
     * @param directive_string
     * @return bool value
     */
    export bool check_directive_line(const std::string &directive_string) {
        if (not directive_string.contains("|") and directive_string.contains(':')) {
            return true;
        }
        return false;
    }

    /**
     *
     * @param directive_string
     * @return
     */
    export bool check_tc_line(const std::string &directive_string) {
        if (directive_string.contains("|")) {
            return true;
        }
        return false;
    }
}
