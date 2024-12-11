#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned long hash(const char* str)
{
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = (((hash << 5) + hash) + c);
    return hash;
}

typedef struct TableEntry
{
    const char* key;
    void* value;
    struct TableEntry* next;
} TableEntry;

TableEntry* table_entry(const char* key, void* value, TableEntry* next)
{// ownership of value relinquished.
    TableEntry* this = malloc(sizeof(TableEntry));
    this->key = key;
    this->value = value;
    this->next = next;
    return this;
}

void free_entry(TableEntry* this)
{
    free(this->value);
    free(this);
}

typedef struct Hashtable
{
    TableEntry** buf;
    size_t capacity;
} Hashtable;

Hashtable* make_hashtable(int capacity)
{
    Hashtable* this = malloc(sizeof(Hashtable));
    this->buf = malloc(sizeof(TableEntry*) * capacity);
    this->capacity = capacity;
    for (int i = 0; i < capacity; i++)
        this->buf[i] = NULL;
    return this;
}

void destroy_hashtable(Hashtable* this)
{
    TableEntry* next = NULL;
    for (int i = 0; i < this->capacity; i++)
    {
        for (TableEntry* p = this->buf[i]; p != NULL; p = next)
        {
            next = p->next;
            free_entry(p);
        }
    }
    free(this->buf);
    free(this);
}

void hashtable_set(Hashtable* this, const char* key, void* value)
{// gives ownership of value to hashtable.
    int index = hash(key) % this->capacity;
    TableEntry* p = this->buf[index];
    if (p == NULL)
    {
        this->buf[index] = table_entry(key, value, NULL);
    }
    else
    {
        for (; p->next != NULL; p = p->next);
        p->next = table_entry(key, value, NULL);
    }
    printf("set %s\n", key);
}

void* hashtable_get(Hashtable* this, const char* key)
{// return reference should not be modified.
    printf("get %s\n", key);
    int index = hash(key) % this->capacity;
    TableEntry* p = this->buf[index];
    for (; p != NULL; p = p->next)
    {
        if (strcmp(p->key, key) == 0)
        {
            return p->value;
        }
    }
    printf("Entry not found!\n");
    return NULL;
}

/*
int main()
{
    Hashtable* T = make_hashtable(10);
    int A[5] = {0, 1, 2, 3, 4};
    hashtable_set(T, "three", &A[0]);
    printf("T['three'] = %d\n", *((int*) hashtable_get(T, "three")));
    hashtable_set(T, "A0", &A[1]);
    hashtable_set(T, "A1", &A[2]);
    hashtable_set(T, "B0", &A[3]);
    hashtable_set(T, "B1", &A[4]);
    printf("T['A0'] = %d\n", *((int*) hashtable_get(T, "A0")));
    printf("T['A1'] = %d\n", *((int*) hashtable_get(T, "A1")));
    printf("T['B0'] = %d\n", *((int*) hashtable_get(T, "B0")));
    printf("T['B1'] = %d\n", *((int*) hashtable_get(T, "B1")));
    printf("T['three'] = %d\n", *((int*) hashtable_get(T, "three")));
    hashtable_set(T, "A0", &A[2]); // first inserted value gets the key  
    printf("T['A0'] = %d\n", *((int*) hashtable_get(T, "A0")));
    destroy_hashtable(T);
}
*/


/*

void test_disks()
{
    Disk** disks = load_disks(3);
    String hi = string("\n\nnima\n\n");
    disk_write(disks[1], 3, hi);
    printn(disk_read(disks[1], 3));
    String wuwuwu = string("wokule\n");
    disk_write(disks[0], 999, wuwuwu);
    printn(disk_read(disks[0], 999));
    unload_disks(disks, 3);
}

void test_fin()
{
    FILE* fin = fopen("USER0", "r");
    char buf1[FIN_BUFFER_SIZE];
    char buf2[FIN_BUFFER_SIZE];
    while (fscanf(fin, "%s %s", buf1, buf2) != EOF)
    {
        if (strcmp(buf1, ".save") == 0)
            printf(".save called\n");
        else if (strcmp(buf1, ".print") == 0)
            printf(".print called\n");
        else if (strcmp(buf1, ".end") == 0)
        {// buf2 contains the next instruction
            if (fscanf(fin, "%s", buf1) != EOF)
            {
                long int offset = strlen(buf2) + 4;
                fseek(fin, -(offset), SEEK_CUR);
            }
            else
                break;
        }
        printf(">> %s %s\n", buf1, buf2);
    }
}
*/
/*
    char file_name[12];
    enter_file(OS->dir_manager, cat_name(file_name, "USER", 0), fileinfo(1, 6, 9));
    print_FileInfo(lookup_file(OS->dir_manager, cat_name(file_name, "USER", 0)));
    printf("%s\n", file_name);
*/