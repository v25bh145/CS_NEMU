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
char *rl_gets()
{
	static char *line_read = NULL;

	if (line_read)
	{
		free(line_read);
		line_read = NULL;
	}

	line_read = readline("(nemu) ");

	if (line_read && *line_read)
	{
		add_history(line_read);
	}

	return line_read;
}

static int cmd_c(char *args)
{
	cpu_exec(-1);
	return 0;
}

static int cmd_q(char *args)
{
	return -1;
}

static int cmd_si(char *args);

static int cmd_info(char *args);

static int cmd_p(char *args);

static int cmd_x(char *args);

static int cmd_w(char *args);

static int cmd_d(char *args);

static int cmd_nemu(char *args);

static int cmd_bt(char* args);

//external
WP *get_wp_head();
int setWatchpoint(swaddr_t step);
void printAllPool();
int delWatchpoint(int count);
void load_elf_tables(int argc, char *argv[]);
long long int expr_cmd(char *e, bool* success);
char* get_function_by_addr(uint32_t ebp, bool* success);

static int cmd_help(char *args);

static struct
{
	char *name;
	char *description;
	int (*handler)(char *);
} cmd_table[] = {
	{"help", "Display informations about all supported commands", cmd_help},
	{"c", "Continue the execution of the program", cmd_c},
	{"q", "Exit NEMU", cmd_q},

	/* TODO: Add more commands */
	{"si", "Continue the execution for one step of the program", cmd_si},
	{"info", "Print info from regs and wds", cmd_info},
	{"w", "Set a watchpoint", cmd_w},
	{"d", "Del a watchpoint", cmd_d},
	{"p", "Calculate an expr", cmd_p},
	{"x", "Scan the memory", cmd_x},
	{"nemu", "run program", cmd_nemu},
	{"bt", "print stack call", cmd_bt}

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args)
{
	/* extract the first argument */
	char *arg = strtok(NULL, " ");
	int i;

	if (arg == NULL)
	{
		/* no argument given */
		for (i = 0; i < NR_CMD; i++)
		{
			printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
		}
	}
	else
	{
		for (i = 0; i < NR_CMD; i++)
		{
			if (strcmp(arg, cmd_table[i].name) == 0)
			{
				printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
				return 0;
			}
		}
		printf("Unknown command '%s'\n", arg);
	}
	return 0;
}

static int cmd_si(char *args)
{
	char *arg = strtok(args, " ");
	char *argOverflow = strtok(NULL, " ");
	if (argOverflow != NULL)
	{
		Log("Wrong params!");
		Log("help: si [N>0]");
		return 1;
	}

	int step = atoi(arg);
	if (step == 0)
	{
		Log("Wrong params!");
		Log("help: si [N>0]");
		return 1;
	}
	cpu_exec(atoi(arg));
	return 0;
}

static int cmd_info(char *args)
{
	char *arg = strtok(args, " ");
	char *argOverflow = strtok(NULL, " ");
	if (argOverflow != NULL)
	{
		Log("Wrong params!");
		Log("help: info [SUBCMD {r, w}]");
		return 1;
	}
	if (!strcmp(arg, "r"))
	{
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
	}
	else if (!strcmp(arg, "w"))
	{
		printf("Now print the watchpoint info: \n");
		printAllPool();
		return 0;
	}
	else
	{
		Log("Wrong params!");
		Log("help: info [SUBCMD {r, w}]");
		return 0;
	}
}

static int cmd_w(char *args)
{
	bool flag = false;
	char *arg = strtok(args, " ");
	char *argOverflow = strtok(NULL, " ");
	if (argOverflow != NULL || arg == NULL)
	{
		Log("Wrong params!");
		Log("help: w addr");
		return 1;
	}
	if (arg[0] == '*')
	{
		char *substr = (char *)malloc(strlen(arg) * sizeof(char));
		char *substrFor0x = (char *)malloc(strlen(arg) * sizeof(char));
		int i;
		for (i = 1; i <= strlen(arg); i++)
		{
			substr[i - 1] = arg[i];
			if (i > 2)
				substrFor0x[i - 3] = arg[i];
		}
		uint32_t addr;
		if ((substr[1] == 'x' && substr[0] == '0') || (substr[1] == 'X' && substr[0] == '0'))
		{
			sscanf(substrFor0x, "%x", &addr);
		}
		else
		{
			sscanf(substr, "%d", &addr);
		}
		if (addr != 0)
		{
			flag = true;
			setWatchpoint(addr);
		}
	}
	else
	{
		char *regStr[] = {"eax", "ebx", "ecx", "edx", "esp", "ebp", "esi", "edi", "eip"};
		uint32_t regs[] = {cpu.eax, cpu.ebx, cpu.ecx, cpu.edx, cpu.esp, cpu.ebp, cpu.esi, cpu.edi, cpu.eip};
		int i;
		for (i = 0; i < strlen(arg); i++)
			arg[i] = tolower(arg[i]);
		for (i = 0; i < 9; i++)
		{
			if (!strcmp(arg, regStr[i]))
			{
				setWatchpoint(regs[i]);
				flag = true;
				break;
			}
		}
	}
	if (flag == false)
	{
		Log("Wrong params!");
		Log("help: w addr");
		return 1;
	}
	else
	{
		return 0;
	}
}

static int cmd_d(char *args)
{
	char *arg = strtok(args, " ");
	int count;
	char *argOverflow = strtok(NULL, " ");
	if (argOverflow != NULL || arg == NULL)
	{
		Log("Wrong params!");
		Log("help: w addr");
		return 1;
	}
	sscanf(arg, "%d", &count);
	delWatchpoint(count);
	return 0;
}

void ui_mainloop()
{
	while (1)
	{
		char *str = rl_gets();
		char *str_end = str + strlen(str);

		/* extract the first token as the command */
		char *cmd = strtok(str, " ");
		if (cmd == NULL)
		{
			continue;
		}

		/* treat the remaining string as the arguments,
		 * which may need further parsing
		 */
		char *args = cmd + strlen(cmd) + 1;
		if (args >= str_end)
		{
			args = NULL;
		}

#ifdef HAS_DEVICE
		extern void sdl_clear_event_queue(void);
		sdl_clear_event_queue();
#endif
		int i;
		for (i = 0; i < NR_CMD; i++)
		{
			if (strcmp(cmd, cmd_table[i].name) == 0)
			{
				if (cmd_table[i].handler(args) < 0)
				{
					return;
				}
				break;
			}
		}

		if (i == NR_CMD)
		{
			printf("Unknown command '%s'\n", cmd);
		}
	}
}

static int cmd_p(char *args)
{
	bool status;
	long long int res;
	res = expr_cmd(args, &status);
	if (!status)
	{
		Log("error: please input right expr!");
		return 2;
	}
	else
	{
		Log("%lld", res);
		return 0;
	}
}

static int cmd_x(char *args)
{
	if (strlen(args) < 3)
	{
		Log("Wrong params!");
		Log("help: x N expr");
		return 1;
	}
	int i;
	char *strN = (char *)malloc(strlen(args + 1) * sizeof(char));
	char *expr = (char *)malloc(strlen(args + 1) * sizeof(char));
	uint32_t n;
	bool status;
	for (i = 0; args[i] != ' '; i++)
		strN[i] = args[i];
	strN[i++] = '\0';
	for (; i <= strlen(args); i++)
		expr[i - strlen(strN) - 1] = args[i];
	n = atoi(strN);
	long long int val = expr_cmd(expr, &status);
	if(!status || val < 0) {
		Log("error: please input right expr!");
		return 2;
	}
	for(i = 0; i < n; i++) {
		printf("%d: %d\n", i, swaddr_read(val + i * 4, 1));
	}
	return 0;
}

#define maxArgc 20
static int cmd_nemu(char *args) {
	if(args == NULL) {
		Log("error: please input right expr!");
		Log("example: nemu ./obj/testcase/add");
		return 2;
	}

	char *argv[maxArgc];
	char* token;
   token = strtok(args, " ");
   int argc = 1;
   argv[argc - 1] = "nemu";
   while( token != NULL ) {
	  argc++;
	  if(argc >= maxArgc + 1) {
			Log("error: too many args!");
			return 2;
	  }
	  argv[argc - 1] = token;
      token = strtok(NULL, " ");
   }
	load_elf_tables(argc, argv);
	Log("successfully load elf in file %s", argv[1]);
	return 0;
}
#undef maxArgc

static int cmd_bt(char* args) {
	// Log("if fail, please check if you DO run this program.");
	int i = 0;
	char* func_name;
	bool success = true;

	//first - now
	func_name = get_function_by_addr(cpu.eip, &success);
	if(success) {
		printf("#stack(%d):\t %x <%s>\n", i++, cpu.eip, func_name);
	} else {
		Log("please check if you DO run this program or DO reach the <main>");
		return 2;
	}

	//stack
	uint32_t ebp = cpu.ebp;
	uint32_t addr = 0;
	while(ebp != 0) {
		addr = swaddr_read(ebp + 4, 4);
		func_name = get_function_by_addr(addr, &success);
		if(!success) {
			addr = 0;
			break;
		}
		printf("#stack(%d):\t %x <%s>\n", i++, addr, func_name);
		ebp = swaddr_read(ebp, 4);
	}
	printf("#stack(%d)\t %x\n", i++, addr);
	return 0;
}