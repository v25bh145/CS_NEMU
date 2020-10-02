#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint32_t);

/* We use the `readline' library to provide more flexibility to read from stdin. */
char* rl_gets() {
	static char *line_read = NULL;

	if (line_read) {
		free(line_read);
		line_read = NULL;
	}

	line_read = readline("(nemu) ");

	if (line_read && *line_read) {
		add_history(line_read);
	}

	return line_read;
}

static int cmd_c(char *args) {
	cpu_exec(-1);
	return 0;
}

static int cmd_q(char *args) {
	return -1;
}

static int cmd_si(char *args) ;

static int cmd_info(char* args);

// static int cmd_p(char *args) ;

// static int cmd_x(char* args);

static int cmd_w(char* args);

// static int cmd_d(char* args);

// static int cmd_bt(char* args);

WP* get_wp_head();
int setBreakpoint(swaddr_t step);

static int cmd_help(char *args);

static struct {
	char *name;
	char *description;
	int (*handler) (char *);
} cmd_table [] = {
	{ "help", "Display informations about all supported commands", cmd_help },
	{ "c", "Continue the execution of the program", cmd_c },
	{ "q", "Exit NEMU", cmd_q },

	/* TODO: Add more commands */
	{ "si", "Continue the execution for one step of the program", cmd_si},
	{"info", "Print info from regs and wds", cmd_info},
	{"w", "Set a watchpoint", cmd_w}

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
	/* extract the first argument */
	char *arg = strtok(NULL, " ");
	int i;

	if(arg == NULL) {
		/* no argument given */
		for(i = 0; i < NR_CMD; i ++) {
			printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
		}
	}
	else {
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(arg, cmd_table[i].name) == 0) {
				printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
				return 0;
			}
		}
		printf("Unknown command '%s'\n", arg);
	}
	return 0;
}

static int cmd_si(char *args) {
	char* arg = strtok(args,  " ");
	char* argOverflow = strtok(NULL, " ");
	if(argOverflow != NULL) {
		Log("Wrong params!");
		Log("help: si [N>0]");
		return 1;
	}

	int step = atoi(arg);
	if(step == 0 ){
		Log("Wrong params!");
		Log("help: si [N>0]");
		return 1;
	}
	cpu_exec(atoi(arg));
	return 0;
}

static int cmd_info(char* args) {
	char* arg = strtok(args,  " ");
	char* argOverflow = strtok(NULL, " ");
	if(argOverflow != NULL) {
		Log("Wrong params!");
		Log("help: info [SUBCMD {r, w}]");
		return 1;
	}
	if(!strcmp(arg, "r")) {
		printf("Now print the regs info: \n");
		printf("eax: %ld\n", (long int)cpu.eax);
		printf("ecx: %ld\n", (long int)cpu.ecx);
		printf("edx: %ld\n", (long int)cpu.edx);
		printf("ebx: %ld\n", (long int)cpu.ebx);
		printf("esp: %ld\n", (long int)cpu.esp);
		printf("ebp: %ld\n", (long int)cpu.ebp);
		printf("esi: %ld\n", (long int)cpu.esi);
		printf("edi: %ld\n", (long int)cpu.edi);
		printf("eip: %8x\n", cpu.eip);
		return 0;
	} else if(!strcmp(arg, "w")) {
		printf("Now print the watchpoint info: \n");
		int count = 0;
		WP* head = get_wp_head();
		while(head != NULL) {
			printf("wp %d: at addr %8x", count++, head->addr);
			head = head->next;
		}
		return 0;
	} else {
		Log("Wrong params!");
		Log("help: info [SUBCMD {r, w}]");
		return 1;
	}
}

static int cmd_w (char* args) {
	char* arg = strtok(args,  " ");
	char* argOverflow = strtok(NULL, " ");
	if(argOverflow != NULL || arg == NULL) {
		Log("Wrong params!");
		Log("help: w addr");
		return 1;
	}
	if(arg[0] == '*') {
		char* substr = (char *)malloc(strlen(arg) * sizeof(char));
		char* substrFor0x = (char *)malloc(strlen(arg) * sizeof(char));
		int i;
		for(i = 1; i <= strlen(arg); i++) {
			substr[i - 1] = arg[i];
			if(i > 2) substrFor0x[i - 3] = arg[i];
		}
		// Log("3 %s", substrFor0x);
		// Log("3 %c", substr[2]);
		uint32_t addr;
		char* ptr;
		int len;
		if((substr[1] == 'x' && substr[0] == '0' )|| (substr[1] == 'X' && substr[0] == '0')) {
			addr = sscanf(substrFor0x, "%x", &len);
			Log("1");
		} else {
			addr = strtol(substr, &ptr, 10);
			Log("2");
		}
		// if(ptr != NULL) {
			// Log("help: w addr %I64u\n", addr);
			// Log("Wrong params!");
			// Log("help: w addr");
			// return 1;
		// }
		Log("help: w addr %I64u\n", addr);
		Log("%s", substr);
		setBreakpoint(addr);
	} else {
		char* regStr[] = {"eax", "ebx", "ecx", "edx", "esp", "ebp", "esi", "edi", "eip"};
		uint32_t regs[] = {cpu.eax, cpu.ebx, cpu.ecx, cpu.edx, cpu.esp, cpu.ebp, cpu.esi, cpu.edi, cpu.eip};
		int i;
		for(i = 0; i < 9; i++) {
			if(!strcmp(arg, regStr[i])) {
				setBreakpoint(regs[i]);
				break;
			}
		}
	}
	// Log("%s", regs[0]);
	return 0;
} 

void ui_mainloop() {
	while(1) {
		char *str = rl_gets();
		char *str_end = str + strlen(str);

		/* extract the first token as the command */
		char *cmd = strtok(str, " ");
		if(cmd == NULL) { continue; }

		/* treat the remaining string as the arguments,
		 * which may need further parsing
		 */
		char *args = cmd + strlen(cmd) + 1;
		if(args >= str_end) {
			args = NULL;
		}

#ifdef HAS_DEVICE
		extern void sdl_clear_event_queue(void);
		sdl_clear_event_queue();
#endif
		int i;
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(cmd, cmd_table[i].name) == 0) {
				if(cmd_table[i].handler(args) < 0) { return; }
				break;
			}
		}

		if(i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
	}
}
