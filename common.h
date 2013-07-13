/*
    fasync - sync utility based on fanotify

    Copyright (C) 2013  Dmitry Yu Okunev <xai@mephi.ru> 0x8E30679C

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#define _LARGEFILE64_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <getopt.h>
#include <limits.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <iconv.h>
#include <errno.h>
#include <endian.h>
#include <ctype.h>
#include <regex.h>
#include <signal.h>
#include <wait.h>
#include <sys/fanotify.h>
#include "config.h"

#ifndef MIN
#define MIN(a,b) (a>b?b:a)
#endif

#ifndef MAX
#define MAX(a,b) (a>b?a:b)
#endif
/*
#define sizeof_bit(a) (sizeof(a)*CHAR_BIT)
#define intvalues(a) (1 << sizeof_bit(a))

#define parse_le32uint(ptr_p) le32toh(*(uint32_t *)*(ptr_p)); *(ptr_p) = (typeof(*(ptr_p)))((char *)*(ptr_p)) + sizeof(uint32_t);
*/
enum flags_enum {
	BACKGROUND	= 'b',
	HELP		= 'h',
	DEBUG		= 'd',
	QUITE		= 'q',
	VERBOSE		= 'v'
};

typedef enum flags_enum flags_t;

extern int flags[];

enum ruleaction_enum {
	RULE_END = 0,	// Terminator. To be able to end rules' chain
	RULE_ACCEPT,
	RULE_REJECT
};
typedef enum ruleaction_enum ruleaction_t;

struct rule {
	regex_t		expr;
	mode_t		objtype;
	ruleaction_t	action;
};
typedef struct rule rule_t;

