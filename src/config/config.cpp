#include <config.h>
#include <auth.h>
#include <unistd.h>
#include <fstream>
#include <string>
#include <sstream>

ConfigParser::ConfigParser(): ConfigParser("/etc/odus.cfg") {}

ConfigParser::ConfigParser(std::string_view ConfigPath) {
	std::ifstream configFile;
	configFile.open(ConfigPath.data());

	// Default config
	UserPerms.insert({"*", {} } );

	// We have no command perms by default
	CommandPerms.clear();

	if (configFile.is_open()) {
		ParseConfig(configFile);
	}

}

std::unordered_map<std::string_view, CommandPermissions>& ConfigParser::GetCommandPermissions() {
	return CommandPerms;
}

std::unordered_map<std::string_view, UserPermissions>& ConfigParser::GetUserPermissions() {
	return UserPerms;
}

const UserPermissions ConfigParser::GetCurrentUserPermissions() {
	std::string user = GetUserByUid(getuid());
	if (UserPerms.contains(user)) {
		return UserPerms[user];
	} else {
		return UserPerms["*"];
	}
}

static inline constexpr unsigned long djb2a(const std::string_view str) {
	unsigned long hash = 5381;
	for (auto c : str) {
		hash = ((hash << 5) + hash) ^ c;
	}
	return hash;
}

static inline constexpr unsigned long operator""_hash(const char* str, size_t len) {
	return djb2a({str, len});
}

// FIXME: This is a horrible implementation
void ConfigParser::ParseConfig(std::ifstream& fs) {
	std::string line;
	while (std::getline(fs, line)) {
		std::stringstream ss(line);
		std::string tok;

		ss >> tok;
		switch (djb2a(tok)) {
			case "user"_hash: // Defines a user rule
			{
				std::string user;
				ss >> user;

				UserPermissions rules;

				// Avoid overwriting existing rules
				if (!UserPerms.contains(user))
					UserPerms[user] = rules;

				while (std::getline(ss, tok, ' ')) {
					switch (djb2a(tok)) {
						case "nopasswd"_hash:
						{
							UserPerms[user].NoPassword = true;
							break;
						}

						case "targetpasswd"_hash:
						{
							UserPerms[user].TargetPassword = true;
							break;
						}

						case "selfpasswd"_hash:
						{
							UserPerms[user].TargetPassword = false;
							break;
						}

						default:
							// TODO: Error Handling
							break;
					}
				}

				break;
			}

			case "cmd"_hash: // Defines a command rule
			case "command"_hash:
			{
				std::string command;
				ss >> command;

				CommandPermissions rules;

				// Avoid overwriting existing rules
				if (!CommandPerms.contains(command))
					CommandPerms[command] = rules;

				while (std::getline(ss, tok, ' ')) {
					switch (djb2a(tok)) {
						case "nopasswd"_hash:
						{
							CommandPerms[command].NoPassword = true;
							break;
						}

						case "targetpasswd"_hash:
						{
							CommandPerms[command].TargetPassword = true;
							break;
						}

						default:
							// TODO: Error Handling
							break;
					}
				}

				break;
			}

			default: // Otherwise, defines a global rule (TO BE IMPLEMENTED)
			// TODO
			break;
		}
	}
}
