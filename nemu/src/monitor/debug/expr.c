#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>
#include <stdlib.h>

enum
{
	NOTYPE = 256,
	EQ,
	/* TODO: Add more token types */
	UEQ,
	NUM,
	AND,
	OR,
	NEG, //negative
	DRF	 //dereference
};
int theLastRule = DRF;

static struct rule
{
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{" +", NOTYPE}, // spaces
	{"\\(", '('},
	{"\\)", ')'},
	{"!=", UEQ},
	{"!", '!'},
	{"\\|\\|", OR},
	{"\\|", '|'},
	{"&&", AND},
	{"&", '&'},
	{"\\+", '+'}, // plus
	{"==", EQ},	  // equal
	{"-", '-'},
	{"\\*", '*'},
	{"/", '/'},
	{"[0-9]+", NUM},
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]))

static regex_t re[NR_REGEX];

uint32_t swaddr_read(swaddr_t, size_t);

int *weight;

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex()
{
	int i;
	char error_msg[128];
	int ret;

	weight = (int *)malloc(theLastRule * sizeof(int) + 1);
	for (i = 0; i <= theLastRule; i++)
		weight[i] = 0x7f;
	//! NEG DRF NUM
	weight['+'] = 5;
	weight['-'] = 5;
	weight['*'] = 4;
	weight['/'] = 4;
	weight['&'] = 9;
	weight['|'] = 11;
	weight[EQ] = 8;
	weight[UEQ] = 8;
	weight[AND] = 12;
	weight[OR] = 13;
	for (i = 0; i < NR_REGEX; i++)
	{
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if (ret != 0)
		{
			regerror(ret, &re[i], error_msg, 128);
			Assert(ret == 0, "regex compilation failed: %s\n%s", error_msg, rules[i].regex);
		}
	}
}

typedef struct token
{
	int type;
	long long int num;
} Token;

Token tokens[32];
int nr_token;

static bool make_token(char *e)
{
	int position = 0;
	int i;
	regmatch_t pmatch;

	nr_token = 0;

	for (i = 0; i < 32; i++)
		tokens[i].type = 0;

	while (e[position] != '\0')
	{
		/* Try all rules one by one. */
		for (i = 0; i < NR_REGEX; i++)
		{
			if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0)
			{
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

				// Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);
				position += substr_len;

				/* TODO: Now a new token is recognized with rules[i]. Add codes
				 * to record the token in the array `tokens'. For certain types
				 * of tokens, some extra actions should be performed.
				 */
				//(1 + 4   ) / 1 && 4 & 3 || 4 | 8 != 4 * !1 == 7
				if (rules[i].token_type == '(' ||
					rules[i].token_type == ')' ||
					rules[i].token_type == '+' ||
					rules[i].token_type == '/' ||
					rules[i].token_type == UEQ ||
					rules[i].token_type == EQ ||
					rules[i].token_type == OR ||
					rules[i].token_type == AND ||
					rules[i].token_type == '&' ||
					rules[i].token_type == '|' ||
					rules[i].token_type == '!')
				{
					tokens[nr_token].type = rules[i].token_type;
					nr_token++;
				}
				else if (rules[i].token_type == '-' ||
						 rules[i].token_type == '*')
				{
					if (nr_token == 0 ||
						tokens[nr_token - 1].type == '(' ||
						tokens[nr_token - 1].type == '+' ||
						tokens[nr_token - 1].type == '-' ||
						tokens[nr_token - 1].type == '*' ||
						tokens[nr_token - 1].type == '/' ||
						tokens[nr_token - 1].type == UEQ ||
						tokens[nr_token - 1].type == EQ ||
						tokens[nr_token - 1].type == OR ||
						tokens[nr_token - 1].type == AND ||
						tokens[nr_token - 1].type == '&' ||
						tokens[nr_token - 1].type == '|' ||
						tokens[nr_token - 1].type == '!')
					//for dereference and negative
					{
						tokens[nr_token].type = rules[i].token_type == '-' ? NEG : DRF;
						nr_token++;
					}
					else
					{
						//for multiply and minus
						tokens[nr_token].type = rules[i].token_type;
						nr_token++;
					}
				}
				else if (rules[i].token_type == NUM)
				{
					//NUMBER
					if (substr_len > 31)
					{
						panic("excessive num range!");
						return false;
					}
					tokens[nr_token].type = rules[i].token_type;
					tokens[nr_token].num = atoi(substr_start);
					nr_token++;
				}
				else if (rules[i].token_type != NOTYPE)
				{
					panic("please implement me");
				}
				break;
			}
		}
		if (i == NR_REGEX)
		{
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}

	return true;
}
/**
 * find the minimum sign out of the pair of embraces
 * */
int find_domintant(int p, int q, bool *success)
{
	int i, cnt = 0;
	struct
	{
		int max, pos;
	} node;
	node.max = 0;
	node.pos = 0;
	for (i = p; i <= q; i++)
	{
		if (tokens[i].type == '(')
			cnt++;
		else if (tokens[i].type == ')')
			cnt--;
		if (cnt == 0 &&
			tokens[i].type != NUM &&
			tokens[i].type != DRF &&
			tokens[i].type != '!' &&
			tokens[i].type != NEG &&
			node.max < weight[tokens[i].type])
		{
			Log("node: %d %d", i, tokens[i].type);
			node.max = weight[tokens[i].type];
			node.pos = i;
		}
	}
	return node.pos;
}
long long int eval(int p, int q, bool *success)
{
	Log("%d %d %d", (int)*success, p, q);
	if (*success == false)
		return 0;
	if (p > q)
	{
		Log("??? %d %d", p, q);
		*success = false;
		return 0;
	}
	else if (p == q)
	{
		if (tokens[p].type != NUM)
		{
			Log("??? %d %d", p, q);
			*success = false;
			return 0;
		}
		else
		{
			return tokens[p].num;
		}
	}
	else if (tokens[p].type == '(' && tokens[q].type == ')')
	{
		Log("(+)");
		return eval(p + 1, q - 1, success);
	}
	else if (tokens[p].type == DRF || tokens[p].type == '!' || tokens[p].type == NEG)
	{
		Log("D!N");
		if (tokens[p].type == DRF)
			return -eval(p + 1, q, success);
		else if (tokens[p].type == '!')
			return !eval(p + 1, q, success);
		else
		{
			return swaddr_read(eval(p + 1, q, success), 1);
		}
	}
	else
	{
		int op = find_domintant(p, q, success);
		Log("%d", op);
		long long int val1 = eval(p, op - 1, success);
		long long int val2 = eval(op + 1, q, success);
		if (!success)
			return 0;
		switch (tokens[op].type)
		{
		case '+':
			return val1 + val2;
		case '-':
			return val1 - val2;
		case '*':
			return val1 * val2;
		case '/':
			return val1 / val2;
		case '&':
			return val1 & val2;
		case '|':
			return val1 | val2;
		case AND:
			return val1 && val2;
		case OR:
			return val1 || val2;
		case EQ:
			return val1 == val2;
		case UEQ:
			return val1 != val2;
		}
	}
	return 0;
}
uint32_t expr(char *e, bool *success)
{
	if (!make_token(e))
	{
		*success = false;
		return 0;
	}
	int i;
	for (i = 0; i < nr_token; i++)
	{

		if (tokens[i].type == NUM)
		{
			printf("num %lld\n", tokens[i].num);
		}
		else
		{
			printf("%d\n", tokens[i].type);
		}
	}
	/* TODO: Insert codes to evaluate the expression. */
	*success = true;
	return eval(0, nr_token - 1, success);
}
