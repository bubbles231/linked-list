/* ll.c
   Copyright (c) 2013 Arthur Marble
   This project is licensed under the MIT license.
   See LICENSE for complete description.
*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<getopt.h>
#include<string.h>

#define VERSION_NUMBER 0.1
#define VERSION_STRING_HELPER(X) #X
#define VERSION_STRING(X) VERSION_STRING_HELPER(X)
#define TRUE 1
#define FALSE 0

struct myArgs_t
{
        int verbose;     // The -v --verbose args to turn on verbose mode
        int help;        // The -h --help args to print a help message
        int version;     // The -V --version args to print the version
        int print;       // The -p --print args to print the linked lists
        int debug;       // WILL MAKE VERBOSE TAKE DEBUG PLACE
        char *debug_opt; // The --debug option which will print options given
} myArgs;

static const char *optString = "vhVpd;";
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

void usage(int exit_code)
{
        printf("Usage: ll [OPTION]\n\nMandatory arguments for long options are"
               " also mandatory for short options\n  -v, --verbose\t\trun program verbosely\n"
               "  -h, --help\t\tprint this help message\n  -V, --version\t\tprint the program version\n"
               "  -p, --print\t\tprint the linked lists\n  -d, --debug=ANYTHING\tprint the state of myArgs and option"
               " used for -d, --debug=ANYTHING\n\n");
        exit(exit_code);
}

void print_list()
{
        int v;
        int i;

        for (i = 0; i < 10; i++) {
                v = 100 + i;
                printf("%2d: %3d\n", i, v);
        }
}

int main(int argc, char *argv[])
{
        int opt = 0;

        // Initialize global arguments
        myArgs.verbose = FALSE;
        myArgs.help = FALSE;
        myArgs.version = FALSE;
        myArgs.print = FALSE;
        myArgs.debug = FALSE;
        myArgs.debug_opt = NULL;
        int *longIndex = NULL; // cheap hack


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
//                case 0: // long option without a short arg (NO LONG OPTION WITHOUT A SHORT ARG SO BREAK)
//                        usage(1);
//                        break;
                default:
                        /* you should not get to here unless you have a invalid argument */
                        usage(1);
                        break;
                }
                opt = getopt_long(argc, argv, optString, longOpts, longIndex);
        }
        if (myArgs.help == TRUE) {
                usage(0);
        }
        if (myArgs.verbose == TRUE) {
                printf("Running program verbosely...\n");
        }
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
                print_list();
        }

        return 0;
}
