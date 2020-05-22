#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "test_common.h"

#define LINE_LENGTH 255

const char* OUT_PATH = "build/tmp.out";

const char* fail_msg_ppl =
    "wrong pure literals:\n\texpected: %s\n\tactual: %s\n";
const char* fail_msg_IO = "IO Problem\n";
// const char* fail_msg_inv_lit = "Error: invalid literal in assignment\n";
// const char* fail_msg_format = "Error: invalid SAT assignment file\n";
// const char* fail_msg_miss_var = "Error: missing variable\n";
const char* fail_msg_no_file = "file not found\n";

result_t ppl_test(const char* exp_res) {
    char line[LINE_LENGTH];
    FILE* output = fopen(OUT_PATH, "r");
    if (output == NULL) {
        return FAILURE;
    }
    char* f = fgets(line, LINE_LENGTH, output);
    fclose(output);
    if (f == NULL) {
        fputs(fail_msg_IO, stderr);
        return FAILURE;
    }
    line[strcspn(line, "\n")] = 0;
    if (strcmp(exp_res, line) == 0) {
        return SUCCESS;
    }
    fprintf(stderr, fail_msg_ppl, exp_res, line);
    return FAILURE;
}

result_t tiny_ppl_test(const char* test) {
    (void)test;
    return ppl_test("4 -5 0");
}

unsigned parse_var_num(FILE* input) {
    char line[LINE_LENGTH];
    unsigned vars = 0;
    do {
        fgets(line, LINE_LENGTH, input);
    } while ('c' == line[0]);
    sscanf(line, "%*s %*s %u %*u", &vars);
    return vars;
}

int* parse_assignm(FILE* output, int var_num) {
    int* res = malloc(var_num * sizeof(int));
    for (int i = 0; i < var_num; i++) {
        int exp = i + 1;
        if (fscanf(output, "%d", &res[i])) {
            if (res[i] != exp && res[i] != -exp) {
                // fputs(fail_msg_inv_lit, stderr);
                return NULL;
            }
        } else {
            // fputs(fail_msg_miss_var , stderr);
            break;
        }
    }
    return res;
}

int is_almost_eof(FILE* stream) {
    char line[LINE_LENGTH];
    char* f = fgets(line, LINE_LENGTH, stream);
    line[strcspn(line, "\n")] = 0;
    if (f && strcmp("", line) != 0) {
        return 0;
    }
    f = fgets(line, LINE_LENGTH, stream);
    line[strcspn(line, "\n")] = 0;
    if (f && strcmp("", line) != 0) {
        return 0;
    }
    return feof(stream);
}

result_t unsat_test(const char* test) {
    (void)test;
    char line[LINE_LENGTH];
    FILE* output = fopen(OUT_PATH, "r");
    if (output == NULL) {
        fputs(fail_msg_no_file, stderr);
        return FAILURE;
    }
    fgets(line, LINE_LENGTH, output);
    line[strcspn(line, "\n")] = 0;
    if (strcmp("UNSAT", line) == 0) {
        if (is_almost_eof(output)) {
            fclose(output);
            return SUCCESS;
        }
    }
    fclose(output);
    return FAILURE;
}

result_t sat_test(const char* test) {
    // 'test' is expected to be the relative path to the input file
    char* buf = strdup(test);
    const char* delim = ":";
    char* ptr = NULL;

    ptr = strtok(buf, delim);
    assert(ptr != NULL);
    // ptr contains 'sat'
    ptr = strtok(NULL, delim);
    assert(ptr != NULL);
    const char* input_path = (const char*)(ptr);

    // read the number of variables from the input file
    FILE* input = fopen(input_path, "r");
    if (input == NULL) {
        fputs(fail_msg_no_file, stderr);
        return FAILURE;
    }
    unsigned var_num = parse_var_num(input);
    fclose(input);

    // check whether the output file is an assignment for every variable
    char line[LINE_LENGTH];
    FILE* output = fopen(OUT_PATH, "r");
    if (output == NULL) {
        fputs(fail_msg_no_file, stderr);
        return FAILURE;
    }
    fgets(line, LINE_LENGTH, output);
    line[strcspn(line, "\n")] = 0;
    if (strcmp("SAT", line) == 0) {
        int* assignm = parse_assignm(output, var_num);
        if (assignm != NULL) {
            unsigned zero;
            if (fscanf(output, "%u", &zero) && zero == 0) {
                if (is_almost_eof(output)) {
                    for (unsigned i = 0; i < var_num; i++) {
                        int exp = i + 1;
                        if (assignm[i] != exp && assignm[i] != -exp) {
                            // fputs(fail_msg_inv_lit, stderr);
                            free(assignm);
                            fclose(output);
                            return FAILURE;
                        }
                    }
                    fclose(output);
                    free(assignm);
                    return SUCCESS;
                }
            }
        }
        free(assignm);
    }
    fclose(output);
    //  fputs(fail_msg_format, stderr);
    return FAILURE;
}

test_fun_t get_test(const char* test) {
    TEST("unsat", unsat_test);
    TEST("sat", sat_test);
    TEST("public.print_pl.test.tiny_ppl", tiny_ppl_test);
    return NULL;
}
