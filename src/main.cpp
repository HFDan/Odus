#ifdef _WIN32
#error This project is not supported on windows
#endif

#include <auth.h>
#include <config.h>
#include <unistd.h>
#include <argp.h>

const char* argp_program_version = "0.0.1";
const char* argp_program_bug_address = "the Github repository";

struct arguments {
	char* user{nullptr};
	char** positional{nullptr};
	int positional_argc{0};
};

static argp_option options[] {
		{"user", 'u', "user", 0, "User to run as. Default is root."},
		{0}
};

static error_t parse_opt(int key, char* arg, struct argp_state* state) {
	arguments* args = (arguments*)state->input;

	switch (key) {
		case 'u':
			args->user = arg;
			break;

		case ARGP_KEY_ARG:
			return ARGP_ERR_UNKNOWN;
			break;

		case ARGP_KEY_ARGS:
			args->positional = state->argv + state->next;
			args->positional_argc = state->argc - state->next + 1;

		case ARGP_KEY_END:
			break;

		default:
			return ARGP_ERR_UNKNOWN;
	}
	return 0;

}

static char args_doc[] = "command";

static char doc[] = "A replacement for sudo. Written in 2 hours.";

static argp argp = {options, parse_opt, args_doc, doc};

int main(int argc, char** argv) {
	arguments cmd_args;
	// Default arguments
	cmd_args.user = (char*)"root";

	// Parse arguments
	argp_parse(&argp, argc, argv, 0, 0, &cmd_args);

	ConfigParser cp;

	// This works due to short-circuiting
	if ( cp.GetCurrentUserPermissions().NoPassword || CheckPasswd(cmd_args.user) ) {
		seteuid(GetUidByUser(cmd_args.user));

		char** args = new char*[cmd_args.positional_argc];
		for (int i = 0; i < cmd_args.positional_argc; i++) {
			args[i] = cmd_args.positional[i];
		}
		args[cmd_args.positional_argc-1] = nullptr; // Convention: Last argument has to be nullptr

		for (int i = 0; i < cmd_args.positional_argc; i++) {
			printf("%s ", cmd_args.positional[i]);
		}

		execvp(args[0], args);

	} else {
		errno = EACCES;
		perror("Error");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
