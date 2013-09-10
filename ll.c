/*
 * ll.c
 * Copyright (c) 2013 Arthur Marble
 * This project is licensed under the MIT license.
 * See LICENSE for complete description.
 */

#include<stdio.h>
#include<stdlib.h>
#include<getopt.h>

#define VERSION_NUMBER 0.1
#define VERSION_STRING_HELPER(X) #X
#define VERSION_STRING(X) VERSION_STRING_HELPER(X)
#define TRUE 1
#define FALSE 0

struct myArgs_t
{
	int verbose;            // The -v --verbose args to turn on verbose mode
	int help;               // The -h --help args to print a help message
	int version;            // The -V --version args to print the version
	int print;              // The -p --print args to print the linked lists
	int debug;              // WILL MAKE VERBOSE TAKE DEBUG PLACE
	char *debug_opt;        // The --debug option which will print options given
} myArgs;

struct link_t
{
	int value;              // value of this link
	struct link_t *next;    // pointer to the next link
};

struct linked_list_t
{
	int length;             // length of the list
	struct link_t *head;    // pointer to the beginning of the list
	struct link_t *tail;    // pointer to the end of the list
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
	{ NULL, no_argument, NULL, 0
	}
};

static const char *optString = "vhVpd;";

void usage(int exit_code)
{
	printf("Usage: ll [OPTION]\n\nMandatory arguments for long options are"
	      " also mandatory for short options\n  -v, --verbose\t\trun program verbosely\n"
	      "  -h, --help\t\tprint this help message\n  -V, --version\t\tprint the program version\n"
	      "  -p, --print\t\tprint the linked lists\n  -d, --debug=ANYTHING\tprint the state of myArgs and option"
	      " used for -d, --debug=ANYTHING\n\n");
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

void fill_list(struct linked_list_t *list)
{
	int i;
	for (i = 0; i < 10; i++) {
		add_to_list(list, 100 + i);
	}
}

void print_list(struct linked_list_t *list)
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

int main(int argc, char *argv[])
{
	int opt = 0;

	// Initialize myArgs elements
	myArgs.verbose = FALSE;
	myArgs.help = FALSE;
	myArgs.version = FALSE;
	myArgs.print = FALSE;
	myArgs.debug = FALSE;
	myArgs.debug_opt = NULL;
	int *longIndex = NULL; // cheap hack for the last arg of getopt_long FIXME


	opt = getopt_long(argc, argv, optString, longOpts, longIndex);
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
		case 0: // long option without a short arg (NO LONG OPTION WITHOUT A SHORT ARGUMENT exit 1)
			usage(1);
			break;
		default:
			// you should not get to here unless you have a invalid argument
			usage(1);
			break;
		}
		opt = getopt_long(argc, argv, optString, longOpts, longIndex);
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
		printf("-v: %d, -h: %d, -V: %d, -p: %d, -d: %d\n", myArgs.verbose, myArgs.help,
		       myArgs.version, myArgs.print, myArgs.debug);
	}
	if (myArgs.print == TRUE) {
		struct linked_list_t *list = create_empty_list();
		fill_list(list);
		print_list(list);
		delete_list(list);
	}
	return 0;
}
