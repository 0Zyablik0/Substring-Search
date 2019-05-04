#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

enum
{
    in_error = 2,
    func_error = 3
};

struct prefix_table
{
    char* substr;
    int* table;
    size_t len;
};



int     build_prefix_table(struct prefix_table* prefix_table, char* pattern); // создание таблицы для алгоритма Кнута — Морриса — Пратта
int     destroy_prefix_table(struct prefix_table* prefix_table);
int     process_stream(struct prefix_table* prefix_tables, size_t size);

int main(int argc, char* argv[]) {
    if (argc < 2)
    {
        perror("Not enough arguments");
        exit(in_error);
    }
    size_t substr_num = (size_t)argc-1;
    struct prefix_table* prefix_tables = (struct prefix_table*)calloc(substr_num, sizeof(struct prefix_table));
    for (int i = 0; i < substr_num; i++) {
        if (build_prefix_table(&prefix_tables[i], argv[i+1]) == -1)
        {
            perror("build_prefix_table problem");
            exit(func_error);
        };
        for (int j = 0; j < prefix_tables[i].len; j++) {
        }
    }
    if (process_stream(prefix_tables, substr_num)==-1)
    {
        perror("process_stream_error");
        return func_error;
    };
    for (int i = 0; i < substr_num; i++) {
        destroy_prefix_table(&prefix_tables[i]);
    }
    free(prefix_tables);
    return 0;
}


int build_prefix_table(struct prefix_table* prefix_table, char* pattern)
{
    if (!prefix_table) return -1;
    if(!pattern) return -1;
    size_t len = strlen(pattern);
    prefix_table->substr = pattern;
    prefix_table->len = len;
    prefix_table->table = (int*) calloc(len, sizeof(struct prefix_table));
    if  (prefix_table->table == NULL)
    {
        perror(strerror(errno));
        return -1;
    }

    int j = 0;
    prefix_table->table[0]= 0;
    for (int i = 1; i < len; i++) {
        if (pattern[i]==pattern[j])
        {
            j++;
            prefix_table->table[i] = j;
        }
        else
        {
            if (j == 0)
            {
                prefix_table->table[i] = j;
            }
            else
            {
                j = prefix_table->table[j - 1];
                i--;
            }
        }

    }
    return 0;
}
int destroy_prefix_table(struct prefix_table* prefix_table)
{
    if (!prefix_table) return -1;
    assert(prefix_table->table != NULL);
    free(prefix_table->table);
    prefix_table->table = NULL;
    prefix_table->len = 0;
    prefix_table->substr = NULL;
    return 0;
}

int process_stream(struct prefix_table* prefix_tables, size_t size) {
    if (prefix_tables == NULL) {
        perror("NULL prefix tables");
        return -1;
    }

    int *substr_pos = (int *) calloc(size, sizeof(int));

    if (substr_pos == NULL) {
        perror(strerror(errno));
        return -1;
    }
    int stream_char = fgetc(stdin);
    char substr_char = 0;
    while (stream_char != EOF){
        printf("%c", stream_char);
        for (int i = 0; i < size; i++) {
            substr_char = prefix_tables[i].substr[substr_pos[i]];
            while((stream_char!= substr_char) & (substr_pos[i] != 0))
            {
                substr_pos[i] = prefix_tables[i].table[substr_pos[i] - 1];
                substr_char = prefix_tables[i].substr[substr_pos[i]];
            }
            if (stream_char == substr_char) {
                substr_pos[i]++;
                if (substr_pos[i] == prefix_tables[i].len) {
                    printf("*");
                }
            }
        }
    stream_char = fgetc(stdin);
    }
    free(substr_pos);
    return 0 ;
}