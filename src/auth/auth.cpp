#include <auth.h>

#include <string_view>
#include <pwd.h>
#include <shadow.h>
#include <crypt.h>
#include <cstring>
#include <termios.h>
#include <unistd.h>
#include <iostream>

bool CheckPasswd(const std::string_view user) {

	const std::string pwd = PromptPasswd(user);

	passwd* passwdEntry = getpwnam(user.data());

	if (!passwdEntry) {
		return false;
	}

	if (strcmp(passwdEntry->pw_passwd, "x") != 0) {
		// Password is in /etc/passwd
		return strcmp(passwdEntry->pw_passwd, crypt(pwd.c_str(), passwdEntry->pw_passwd)) == 0;
	} else {
		// Password is in /etc/shadow
		spwd* shadowPasswd = getspnam(user.data());
		if (!shadowPasswd) {
			return false;
		}

		// If account is locked
		if (strcmp(shadowPasswd->sp_pwdp, "!!") == 0 || strcmp(shadowPasswd->sp_pwdp, "!") == 0 || strcmp(shadowPasswd->sp_pwdp, "!*") == 0) {
			return false;
		}

		char* hash = crypt(pwd.c_str(), shadowPasswd->sp_pwdp);
		return strcmp(shadowPasswd->sp_pwdp, hash) == 0;
	}
}

std::string PromptPasswd(const std::string_view user) {
	termios term, orig;
	tcgetattr(STDIN_FILENO, &term);
	orig = term;

	term.c_lflag &= ~ECHO;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
	printf("Password for %s: ", user.data());
	std::string ret;
	std::cin >> ret;

	tcsetattr(STDIN_FILENO, TCSANOW, &orig);
	write(STDIN_FILENO, "\n", 1);

	return ret;
}

int GetUidByUser(const std::string_view user) {
	passwd* passwdEntry = getpwnam(user.data());
	if (!passwdEntry) {
		return -1;
	}

	return passwdEntry->pw_uid;
}

std::string GetUserByUid(uid_t uid) {
	passwd* pwd = getpwuid(uid);

	return std::string(pwd->pw_name);
}

bool CanUserRunCommand(const std::string_view cmd, const ConfigParser& cp, const std::string_view user) {

}