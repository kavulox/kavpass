#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <stdint.h>

// Kavpassctl stuff



size_t BUF_SIZE = 2096; 
typedef struct {
    char *symb;
    char *l_let;
    char *u_let;
    char *ints;
    char *Pass;
    bool failed;
    char tmp_c;
    char *test_symb;
    bool test_symb_b;
} Password;
int pull_rand();
void init(Password *pass, size_t BUFFER);
void safe_return_ran(int line);
void unsafe_return_ran(int line);
void usage();
void safe_set_pass(size_t len);
void unsafe_set_pass(size_t len);
void write_file(char *sr, char *path);
Password pass, *p = &pass;
typedef struct {
    char *prompt;
    int len;
    char *input;
} kavpass;


void k_parse(char *msg, kavpass *kav) {
    char *token = strtok(msg, " "), *file;
    bool unsafe = false, verbose = false, made_pass = false, tmp = false, commence = false;
    if (strncmp(token,"set", 3) == 0) {
        token = strtok(NULL, " ");
        if (token == NULL) {
            fprintf(stderr, "No prompt -set- value provided.\n");
            return;
        }
        if (token != NULL) {
            if (strncmp(token, "prompt", 6) == 0) {
                token = strtok(NULL, " ");
                if (token == NULL) {
                    fprintf(stderr, "No prompt provided.\n");
                }
                int len = strlen(token);
                if (token[len - 1] == '\n') {
                    token[len - 1] = '\0';
                }
                strcat(token, " ");
                kav->prompt = token;
            }
            if (strncmp(token, "length", 6) == 0) {
                token = strtok(NULL, " ");
                kav->len = atoi(token);
            }
        }
    }
    else if (strncmp(token,"PUT", 3) == 0) {
        printf("Prompt :: %s| Input :: %s\n",kav->prompt,kav->input);
    }
    else if (strncmp(token, "generate", 8) == 0) {
        init(p,BUF_SIZE);
        safe_set_pass(kav->len);
        printf("%s\n",p->Pass);
        free(p->Pass);
    }
    else if (strncmp(token, "exit", 4) == 0) {
        return;
    }
    else {
        printf("Command not recognized!\n");
    }
    return;
}
void k_init(kavpass *kav) {
    kav->prompt = "> ";
    kav->input = ";";
}


void k_ctl(kavpass *kav) { /* This is broken currently, do not use */
    
    char buff[256];
    k_init(kav);
    while((strncmp(kav->input, "exit",4))) {
        printf("%s",kav->prompt);
        fgets(buff, 256, stdin);
        kav->input = buff;
        k_parse(kav->input, kav);
    }
}
#define ARR_S(arr) (sizeof(arr) / sizeof((arr)[0])) 
int pull_rand() {
    FILE * file = fopen("/dev/random", "r");            
    unsigned char buff[10];
    if (!file) {
        perror("fopen");
        return EXIT_FAILURE;
    }
    size_t retv = fread(buff, sizeof(*buff), ARR_S(buff), file); /* I have to add some error checking */
    if (retv != ARR_S(buff)) {
        perror("fread(): failed.");
        return EXIT_FAILURE;
    }
    int temp = buff[0];
    fclose(file);
    return temp;
}
    

void init(Password *pass, size_t BUFFER) {
    pass->symb = "!@#$%^&*()-=+_][{}";
    pass->l_let = "qwertyuiopasdfghjklzxcvbnm";
    pass->u_let = "QWERTYUIOPASDFGHJKLZXCVBNM";
    pass->ints = "1234567890";
    pass->test_symb = "™€‰—®©☺⚛";
    pass->Pass = malloc(BUFFER * sizeof(char));
}

void safe_return_ran(int line) {
    if (line >=6) {
        p->failed = true;
    }else {
        switch(line) {
            case 1:
                p->tmp_c = p->symb[pull_rand() % strlen(p->symb)];
                break;
            case 2:
                p->tmp_c = p->l_let[pull_rand() % strlen(p->l_let)];
                break;
            case 3:
                p->tmp_c = p->u_let[pull_rand() % strlen(p->u_let)];
                break;
            case 4:
                p->tmp_c = p->ints[pull_rand() % strlen(p->ints)];
                break;
            case 5:
                p->tmp_c = p->test_symb[pull_rand() % strlen(p->test_symb)];
            default:
                p->failed = true;
        }
    }
}

void unsafe_return_ran(int line) {
    if (line >=6) {
        p->failed = true;
    }else {
        srand(time(NULL));
        switch(line) {
            case 1:
                p->tmp_c = p->symb[rand() % strlen(p->symb)];
                break;
            case 2:
                p->tmp_c = p->l_let[rand() % strlen(p->l_let)];
                break;
            case 3:
                p->tmp_c = p->u_let[rand() % strlen(p->u_let)];
                break;
            case 4:
                p->tmp_c = p->ints[rand() % strlen(p->ints)];
                break;
            case 5:
                p->tmp_c = p->test_symb[rand() % strlen(p->test_symb)];
            default:
                p->failed = true;
        }
    }
}



void usage() {
    printf("Program: Kavpass\n");
    printf("Usage: kavpass -[hloveF]\n");
    printf("\t-h\t--help\t|\tDisplays the help message.\n");
    printf("\t-l\t--length NUM\t|\tSpecifies the length.\n");
    printf("\t-o\t--output\t|\tFile to output to.\n");
    printf("\t-v\t--verbose\t|\tVerbose output.\n"); 
    printf("\t-e\t--extra-unicode\t|\tAdds extra unicode char support.\n");
    printf("\t-F\t--force-unsafe-rng NUM\t|\tReplaces --length and forces the use of an unsafe RNG.\n");
    printf("\t-i\t--interactive\t|\tEnters an interactive mode. (IN DEVELOPMENT)\n");
}
void safe_set_pass(size_t len) {
    LOOP:
    for(size_t i = 0; i < len; i++) {
        if (p->test_symb_b) {
            safe_return_ran(rand() % 5 + 1);
        } else {
            safe_return_ran(rand() % 4 + 1);
        }
        p->Pass[i] = p->tmp_c;
        if (p->failed) {
            return;
        }
    }
    if (strlen(p->Pass) != len) {
        goto LOOP;
    }
}
void unsafe_set_pass(size_t len) {
    LOOP:
    for(size_t i = 0; i < len; i++) {
        if (p->test_symb_b) {
            unsafe_return_ran(rand() % 5 + 1);
        } else {
            unsafe_return_ran(rand() % 4 + 1);
        }
        p->Pass[i] = p->tmp_c;
        if(p->failed) {
            return;
        }
    }
    if (strlen(p->Pass) != len) {
        goto LOOP;
    }
}



struct option long_options[] = {
    { "length",             required_argument,  0,      'l' },
    { "help",               no_argument,        0,      'h' },
    { "output",             required_argument,  0,      'o' },
    { "force-unsafe-rng",   required_argument,  0,      'F' },
    { "verbose",            no_argument,        0,      'v' },
    { "extra-unicode",      no_argument,        0,      'e' },
    { "interactive",        no_argument,        0,      'i' },
    { 0, 0, 0, 0 }
};



void write_file(char *str, char *path) {
    FILE *ptr = fopen(path, "w");
    fputs(str, ptr);
    fclose(ptr);
}

int main(int argc, char **argv) {
    int len, c, option_index = 0;
    bool unsafe = false, verbose = false, made_pass = false, tmp = false, commence = false;
    char *file;
    while((c = getopt_long(argc, argv, "F:hievo:l:", long_options, &option_index)) != -1) {
        switch(c) {
            case 'F':
                unsafe = true;
                commence = true;
                made_pass = true;
                len = atoi(optarg);
                break;
            case 'h':
                usage(); 
                break;
            case 'i':
                kavpass *kav = malloc(256 * sizeof(kavpass));
                k_ctl(kav);
            case 'e':
                p->test_symb_b = true;
                break;
            case 'v':
                verbose = true;
                break;
            case 'o':
                file = optarg;
                tmp = true;
                break;
            case 'l':
                len = atoi(optarg);
                commence = true;
                made_pass = true;
                break;
            case ':':
                usage();
                break;
            case '?':
                usage();
                break;
            default:
                usage();
                break;
        }
    }
    if (!unsafe) {
        if (commence) {
            init(p,BUF_SIZE);
            safe_set_pass(len);
            if (verbose) {
                printf("Password: %s\nWith Length: %zu\n",p->Pass, strlen(p->Pass));
            }
            else {
                printf("%s\n",p->Pass);
            }
        }
        if (tmp) {
            init(p,BUF_SIZE);
            if (made_pass) {
                write_file(p->Pass, file);
                if (verbose) {
                    printf("Wrote password to file: %s\n",file);
                }
            }
            else {
                fprintf(stderr, "Password not generated. Run $ kavpass -h\n");
            }
        }
    } else {
        if (commence) {
            init(p,BUF_SIZE);
            unsafe_set_pass(len);
            if (verbose) {
                printf("Unsafe password: %s\nWith Length: %zu\n",p->Pass, strlen(p->Pass));
            }
            else {
                printf("%s\n",p->Pass);
            }
        }
        if (tmp) {
            init(p,BUF_SIZE);
            if (made_pass) {
                write_file(p->Pass, file);
                if (verbose) {
                    printf("Wrote unsafe password to file: %s\n",file);
                }
            }
            else {
                fprintf(stderr, "Password not generated. Run $ kavpass -h\n");
            }
        }
    }
    if (p->Pass) {
        free(p->Pass);
    }
}
