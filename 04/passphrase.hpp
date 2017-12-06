#ifndef ADVENT_OF_CODE_04_PASSPHRASE_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_04_PASSPHRASE_HPP_INCLUDE_GUARD

#include <functional>
#include <string_view>

void iteratePassphrase(std::string_view passphrase, std::function<void(std::string_view)> callback_per_password);

bool hasDuplicates(std::string_view passphrase);

bool hasAnagrams(std::string_view passphrase);

#endif
