#pragma once
#include <string_view>
#include <string>

/**
 *
 * @param user The user to check
 * @param pwd Password
 * @return True if password is correct
 */
[[nodiscard]]
bool CheckPasswd(const std::string_view user);

/**
 *
 * @param user User to prompt for
 * @return Password that has been inputted
 */
[[nodiscard]]
std::string PromptPasswd(const std::string_view user);

/**
 *
 * @param user User to get euid of
 * @return Euid of user, -1 if fail
 */
[[nodiscard]]
int GetUidByUser(const std::string_view user);

[[nodiscard]]
std::string GetUserByUid(uid_t uid);
