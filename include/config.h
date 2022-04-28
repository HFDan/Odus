#pragma once
#include <string_view>
#include <unordered_map>
#include <fstream>

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

class ConfigParser final {
	using CommandPermType = std::unordered_map<std::string_view, CommandPermissions>;
	using UserPermType = std::unordered_map<std::string_view, UserPermissions>;
	public:
		explicit ConfigParser(std::string_view ConfigPath);
		explicit ConfigParser();
		~ConfigParser() = default;

		CommandPermType& GetCommandPermissions();

		[[deprecated]]
		UserPermType& GetUserPermissions();

		const UserPermissions GetCurrentUserPermissions();

	private:
		CommandPermType CommandPerms;
		UserPermType UserPerms;

		void ParseConfig(std::ifstream& fs);
};