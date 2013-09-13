/*
 * ll.c
 * Copyright (c) 2013 Arthur Marble
 * This project is licensed under the MIT license.
 * See LICENSE for complete description.
 */

#include<stdio.h>
#include<stdlib.h>
#include<getopt.h>
#include<string.h>

#define VERSION_NUMBER 0.1
#define VERSION_STRING_HELPER(X) #X
#define VERSION_STRING(X) VERSION_STRING_HELPER(X)
#define TRUE 1
#define FALSE 0

struct myArgs_t
{
	int verbose;            /* The -v --verbose args to turn on verbose mode */
	int help;               /* The -h --help args to print a help message */
	int version;            /* The -V --version args to print the version */
	int print;              /* The -p --print args to print the linked lists */
	int debug;              /* WILL MAKE VERBOSE TAKE DEBUG PLACE */
	char *debug_opt;        /* The --debug arg which will print options given */
	int first;              /* The --first arg which will print the first element of linked list */
	int rest;               /* The --rest arg which will print all but the first element of linked list */
	char *nth;              /* The --nth arg which will print the nth element of linked list */
	char *list;             /* The --list argument which will take numbers and use those for the list */
} myArgs;

struct link_t
{
	int value;              /* value of this link */
	struct link_t *next;    /* pointer to the next link */
};

struct linked_list_t
{
	int length;             /* length of the list */
	struct link_t *head;    /* pointer to the beginning of the list */
	struct link_t *tail;    /* pointer to the end of the list */
};

static const struct option longOpts[] = {
	{ "verbose", no_argument, NULL, 'v'
	},
	{ "help", no_argument, NULL, 'h'
	},
	{ "version", no_argument, NULL, 'V'
	},
	{ "print", no_argument, NULL, 'p'
	},
	{ "debug", optional_argument, NULL, 'd'
	},
	{"first", no_argument, NULL, 0
	},
	{"rest", no_argument, NULL, 0
	},
	{"nth", required_argument, NULL, 0
	},
	{"list", required_argument, NULL, 0
	},
	{ NULL, no_argument, NULL, 0
	}
};

static const char *optString = "vhVpd;";

void usage(int exit_code)
{
	printf("Usage: ll [OPTION]\nA program that prints linked lists\n\nsome args cannot be run with debug"
	       " because they exit the program\n  -v, --verbose\t\trun the program verbosely\n"
	       "  -h, --help\t\tprint this help message and exit\n  -V, --version\t\tprint the program version"
	       "and exit\n  -p, --print\t\tprint all links in the linked list\n  -d, --debug [OPTIONS]\tprint"
	       "the state of myArgs and options given\n      --first\t\tprint the first link in the list\n"
	       "      --rest\t\tprint all links minus the first one\n      --nth=LINK\tprint the link specified"
               ", if the option is more than the number of links then\n\t\t\tthis prints the last link\n"
	       "      --list=LIST\tuse numbers given to add to the linked list like this: 1,2,3,44,123\n\n");
	exit(exit_code);
}

struct linked_list_t* create_empty_list()
{
	struct linked_list_t *list = malloc(sizeof(struct linked_list_t));
	list->length = 0;
	list->head = NULL;
	list->tail = NULL;

	return list;
}

void add_to_list(struct linked_list_t *list, int value)
{
	struct link_t *link = malloc(sizeof(struct link_t));
	link->value = value;
	link->next = NULL;
	if (list->tail == NULL) {
		list->tail = link;
		list->head = link;
		list->length = 1;
	} else {
		list->tail->next = link;
		list->tail = link;
		list->length++;
	}
}

/* fill list with int starting at 100 ending 109 */
void fill_list(struct linked_list_t *list)
{
	int i;
	for (i = 0; i < 10; i++) {
		add_to_list(list, 100 + i);
	}
}

void print_all_list(struct linked_list_t *list)
{
	struct link_t *link = list->head;
	int i = 0;

	printf("list has %d elements\n", list->length);
	while (link != NULL) {
		printf("%2d: %3d, location: %p\n", i, link->value, link);
		link = link->next;
		i++;
	}
}

/* returns the nth link */
struct link_t* find_link_at(struct linked_list_t *list, int nth)
{
	struct link_t *link = list->head;
	int i;
	for (i = 0; link != NULL; i++) {
		if (i == nth) {
			return link;
		} else {
			link = link->next;
		}
	}
	return NULL;
}

/* prints the links specified */
void print_some_links(struct linked_list_t *list, int links[])
{
	int i;
	for (i = 0; i < sizeof(links); i++) {
		struct link_t *link = find_link_at(list, links[i]);
		printf("%2d: %3d, location: %p\n", i, link->value, link);
	}
}

/* prints links in list starting from int start and stopping at int stop */
void print_slice_list(struct linked_list_t *list, int start, int stop)
{
	int i;
	struct link_t *link;

	printf("printing links starting at: %d\n", start);
	for (i = start; link = find_link_at(list, i), i < stop && link != NULL; i++) {
		printf("%2d: %3d, location: %p\n", i, link->value, link);
	}
	printf("stopped printing links at link: %d\n", i - 1);
}

void delete_list(struct linked_list_t *list)
{
	struct link_t *link = list->head;
	struct link_t *tmp_link = NULL;

	while (link != NULL) {
		tmp_link = link->next;
		free(link);
		link = tmp_link;
	}
	free(list);
}

struct linked_list_t* first(struct linked_list_t *list)
{
	struct linked_list_t *new_list = create_empty_list();
	if (list->head != NULL) {
		add_to_list(new_list, list->head->value);
	}
	delete_list(list);
	return new_list;
}

struct linked_list_t* rest(struct linked_list_t *list)
{
	struct linked_list_t *new_list = create_empty_list();
	if (list->head != NULL) {
		new_list->head = list->head->next;
		new_list->length = list->length - 1;
		list->head->next = NULL;
	}
	delete_list(list);
	return new_list;
}

int nth(struct linked_list_t *list, int n)
{
	struct link_t *link = list->head;
	int i = 0;

	while (link != NULL) {
		if (i == n) {
			return link->value;
		}
		link = link->next;
		i++;
	}
	return -1;
}

/* process --list arg here */
int parse_list_arg(char *list_arg)
{
	char tokenstring[] = "12 13 14";
	char seps[] = " ,";
	char* token;
	int var;
	int input[5];
	int i = 0;
	token = strtok(tokenstring, seps);
	while (token != NULL) {
		sscanf(token, "%d", &var);
		input[i++] = var;

		token = strtok(NULL, seps);
	}
	for (i = 0; i < sizeof(input); i++) {
		printf("i: %d, input: %d\n", i, input[i]);
	}
	printf("this is a function you have to build\n");
	return 100;
}

int main(int argc, char *argv[])
{
	int opt = 0;

	/* Initialize myArgs elements */
	myArgs.verbose = FALSE;
	myArgs.help = FALSE;
	myArgs.version = FALSE;
	myArgs.print = FALSE;
	myArgs.debug = FALSE;
	myArgs.debug_opt = NULL;
	myArgs.first = FALSE;
	myArgs.rest = FALSE;
	myArgs.nth = NULL;
	myArgs.list = NULL;
	int longIndex = 0;

	opt = getopt_long(argc, argv, optString, longOpts, &longIndex);
	while (opt != -1) {
		switch (opt) {
		case 'v':
			myArgs.verbose = TRUE;
			break;
		case 'h':
			myArgs.help = TRUE;
			break;
		case 'V':
			myArgs.version = TRUE;
			break;
		case 'p':
			myArgs.print = TRUE;
			break;
		case 'd':
			myArgs.debug = TRUE;
			myArgs.debug_opt = optarg;
			break;
		case 0: /* long option without a short arg */
			if (strcmp("first", longOpts[longIndex].name) == 0) {
				myArgs.first = TRUE;
			} else if (strcmp("rest", longOpts[longIndex].name) == 0) {
				myArgs.rest = TRUE;
			} else if (strcmp("nth", longOpts[longIndex].name) == 0) {
				myArgs.nth = optarg;
			} else if (strcmp("list", longOpts[longIndex].name) == 0) {
				myArgs.list = optarg;
			} else {
				usage(1);
			}
			break;
		default:
			/* you should not get to here unless you have a invalid argument */
			printf("default");
			usage(1);
			break;
		}
		opt = getopt_long(argc, argv, optString, longOpts, &longIndex);
	}
	if (myArgs.help == TRUE)
		usage(0);

	if (myArgs.verbose == TRUE)
		printf("Running program verbosely...\n");

	if (myArgs.version == TRUE) {
		char *p = VERSION_STRING(VERSION_NUMBER);
		printf("Linked Lists version: %s\n", p);
		exit(0);
	}
	if (myArgs.debug == TRUE) {
		printf("the -d options: %s\n", myArgs.debug_opt);
		printf("-v: %d, -h: %d, -V: %d, -p: %d, -d: %d --first: %d, --rest: %d, --nth: %s\n",
		       myArgs.verbose, myArgs.help, myArgs.version, myArgs.print, myArgs.debug, myArgs.first,
		       myArgs.rest, myArgs.nth);
		printf("--list: %s\n", myArgs.list);

	}
	if (myArgs.print == TRUE) {
		struct linked_list_t *list = create_empty_list();
		fill_list(list);
		if (myArgs.first == TRUE) {
			/* print_slice_list(list, 0, 1); // 0 for start link, 1 for stop link */
			list = first(list);
			print_all_list(list);
		} else if (myArgs.rest == TRUE) {
			/* print_slice_list(list, 1, 100); // 0 for start link, 100 so it prints all */
			list = rest(list);
			print_all_list(list);
		} else if (myArgs.nth != NULL) {
			printf("nth: %d\n", nth(list, atoi(myArgs.nth)));
		} else {
			print_all_list(list);
		}
		delete_list(list);
	}
	if (myArgs.list != NULL) {
		int i;
		printf("--list arg given: %s\n", myArgs.list);
		i = parse_list_arg(myArgs.list);
		printf("test return int: %d\n", i);
	}
	return 0;
}
