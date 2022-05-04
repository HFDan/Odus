#pragma once
#include <string_view>
#include <unordered_map>
#include <fstream>
#include <set>

/*
 *
 * Config syntax:
 *
 * user <username> [attributes]
 * command <command> [attributes]
 * target <target> [attributes]
 *
 * if user == "*" -> Apply to all users
 * command must be global path
 *
 * user refers to the user launching the command
 * target refers to the user the command runs as
 *
 */


struct UserPermissions {
	bool NoPassword{false};
	bool TargetPassword{true};
};

struct CommandPermissions {
	bool NoPassword{false};
	bool TargetPassword{true};
};

struct GroupPermissions {
	bool NoPassword{false};
	bool TargetPassword{true};

	std::set<std::string_view> allowedGroups;
	std::set<std::string_view> allowedUsers;
};

class ConfigParser final {
	using CommandPermType = std::unordered_map<std::string_view, CommandPermissions>;
	using UserPermType = std::unordered_map<std::string_view, UserPermissions>;
	using GroupPermType = std::unordered_map<std::string_view, GroupPermissions>;
	public:
		explicit ConfigParser(std::string_view ConfigPath);
		explicit ConfigParser();
		~ConfigParser() = default;

		CommandPermType& GetCommandPermissions();

		[[deprecated]]
		UserPermType& GetUserPermissions();

		const UserPermissions GetCurrentUserPermissions();

		const GroupPermType& GetGroupPermissions();

	private:
		CommandPermType CommandPerms;
		UserPermType UserPerms;
		GroupPermType GroupPerms;

		void ParseConfig(std::ifstream& fs);
};