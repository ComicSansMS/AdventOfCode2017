#include <passphrase.hpp>

#include <algorithm>
#include <string>
#include <unordered_set>


void iteratePassphrase(std::string_view passphrase, std::function<void(std::string_view)> callback_per_password)
{
    std::size_t current_pw_begin = 0;
    std::size_t current_pw_size = 0;
    auto const it_end = end(passphrase);
    for(auto it = begin(passphrase); it != it_end; ++it, ++current_pw_size) {
        if(*it == ' ') {
            callback_per_password(passphrase.substr(current_pw_begin, current_pw_size));
            current_pw_begin += current_pw_size + 1;
            current_pw_size = -1;
        }
    }
    callback_per_password(passphrase.substr(current_pw_begin));
}

bool hasDuplicates(std::string_view passphrase)
{
    std::unordered_set<std::string> pwd_set;
    bool has_duplicates = false;
    iteratePassphrase(passphrase, [&pwd_set, &has_duplicates](std::string_view pwd) {
        std::string str_pwd(pwd);
        if(pwd_set.find(str_pwd) == end(pwd_set)) {
            pwd_set.emplace(std::move(str_pwd));
        } else {
            has_duplicates = true;
        }
    });
    return has_duplicates;
}

bool hasAnagrams(std::string_view passphrase)
{
    std::unordered_set<std::string> pwd_set;
    bool has_anagrams = false;
    iteratePassphrase(passphrase, [&pwd_set, &has_anagrams](std::string_view pwd) {
        std::string str_pwd(pwd);
        std::sort(begin(str_pwd), end(str_pwd));
        if(pwd_set.find(str_pwd) == end(pwd_set)) {
            pwd_set.emplace(std::move(str_pwd));
        } else {
            has_anagrams = true;
        }
    });
    return has_anagrams;
}
