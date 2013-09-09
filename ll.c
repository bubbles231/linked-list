#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<getopt.h>
#include<string.h>

#define VERSION_NUMBER 0.1
#define VERSION_STRING_HELPER(X) #X
#define VERSION_STRING(X) VERSION_STRING_HELPER(X)

struct myArgs_t
{
        int verbose; // The -v --verbose args to turn on verbose mode
        int help; // The -h --help args to print a help message
        int version; // The -V --version args to print the version
        int print; // The -p --print args to print the linked lists !JUST PRINTS OUT "Hello, World!" for now
        int debug; // WILL MAKE VERBOSE TAKE DEBUG PLACE
        char *debug_opt; // The --debug option which will print options given
} myArgs;

static const char *optString = "vhVpd:";
static const struct option longOpts[] = {
        { "verbose", no_argument, NULL, 'v'
        },
        { "help", no_argument, NULL, 'h'
        },
        { "version", no_argument, NULL, 'V'
        },
        { "print", no_argument, NULL, 'p'
        },
        { "debug", required_argument, NULL, 'd'
        },
        { NULL, no_argument, NULL, 0
        }
};

int main(int argc, char *argv[])
{
        int opt = 0;

        // Initialize global arguments
        myArgs.verbose = 0; // 0 == false
        myArgs.help = 0;
        myArgs.version = 0;
        myArgs.print = 0;
        myArgs.debug = 0;
        myArgs.debug_opt = NULL;
        int *longIndex = NULL;


        opt = getopt_long(argc, argv, optString, longOpts, longIndex);
        while(opt != -1) {
                switch(opt) {
                case 'v':
                        myArgs.verbose = 1; // 1 == true
                        break;
                case 'h':
                        myArgs.help = 1;
                        break;
                case 'V':
                        myArgs.version = 1;
                        break;
                case 'p':
                        myArgs.print = 1;
                        break;
                case 'd':
                        myArgs.debug = 1;
                        myArgs.debug_opt = optarg;
                        break;
                case 0: // long option without a short arg (NO LONG OPTION WITHOUT A SHORT ARG SO BREAK)
                        break;
                default:
                        /* you should not get to here unless you have a invalid argument */
                        break;
                }
                opt = getopt_long(argc, argv, optString, longOpts, longIndex);
        }
        if(myArgs.verbose == 1) {
                printf("Running program verbosely...\n");
        }
        if(myArgs.help == 1) {
                printf("Usage: ll [OPTION]\n\nMandatory arguments for long options are"
                       " also mandatory for short options\n  -v, --verbose\t\trun program verbosely\n"
                       "  -h, --help\t\tprint this help message\n  -V, --version\t\tprint the program version\n"
                       "  -p, --print\t\tprint the linked lists\n  -d, --debug=ANYTHING\tprint the state of myArgs and option"
                       " used for -d, --debug=ANYTHING\n\n");
        }
        if(myArgs.version == 1) {
                char *p = VERSION_STRING(VERSION_NUMBER);
                printf("Linked Lists version: %s\n", p);
        }
        if(myArgs.print == 1) {
                printf("Hello, World!\n");
        }
        if(myArgs.debug == 1) {
                printf("the -d options: %s\n", myArgs.debug_opt);
                printf("-v: %d, -h: %d, -V: %d, -p: %d, -d: %d\n", myArgs.verbose, myArgs.help,
                       myArgs.version, myArgs.print, myArgs.debug);
        }

        return 0;
}
