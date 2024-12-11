#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <errno.h>

#include <hashtable.h>

#define NUM_SECTORS 2048
#define DISK_DELAY 80
#define HASHTABLE_CAPACITY 10
#define PRINT_DELAY 275
#define FILENAME_BUF_SIZE 16
#define FIN_BUFFER_SIZE 16

typedef struct String
{
    char* buf;
    size_t len;
} String;

String string(const char* c_str)
{
    String str;
    str.len = strlen(c_str); //not including null terminator
    str.buf = strdup(c_str); //does have a null terminator, but freed at destroy().
    return str;
}

void printn(String s)
{
    for (int i = 0; i < (int) s.len; i++)
        printf("%c", s.buf[i]);
}

void destroy(String* s)
{
    free(s->buf); // weird segv
}



// UTILITY FUNCTIONS

int msleep(long msec)
{
    struct timespec ts;
    int res;

    if (msec < 0)
    {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;
}

char* strclr(char* str)
{
    printf("clearing: %s\n", str);
    for (int i = 0; str[i] != '\0'; i++)
    {
        str[i] = ' ';
    }
    return str;
}

char* cat_name(char* dest, const char* type, int id)
{
    //strclr(dest);
    strcpy(dest, type);
    sprintf(dest + strlen(type), "%d", id);
    return dest;
}

typedef struct FileInfo
{
    int disk_number;
    int start_sector;
    int file_len;
} FileInfo;

FileInfo* fileinfo(int disk_number, int start_sector, int file_len)
{
    FileInfo* this = malloc(sizeof(FileInfo));
    this->disk_number = disk_number;
    this->start_sector = start_sector;
    this->file_len = file_len;
    return this;
}

void print_FileInfo(FileInfo* p)
{
    printf("File(%d, %d, %d)\n", p->disk_number, p->start_sector, p->file_len);
}

typedef struct DirectoryManager
{
    Hashtable* T;
} DirectoryManager;

DirectoryManager* make_dir_manager()
{
    DirectoryManager* dir_manager = malloc(sizeof(DirectoryManager));
    dir_manager->T = make_hashtable(HASHTABLE_CAPACITY);
    return dir_manager;
}

void enter_file(DirectoryManager* this, const char* file_name, FileInfo* file_info)
{
    hashtable_set(this->T, file_name, (void*) file_info);
}

FileInfo* lookup_file(DirectoryManager* this, const char* file_name)
{
    return (FileInfo*) hashtable_get(this->T, file_name);
}

void kill_dir_manager(DirectoryManager* this)
{
    destroy_hashtable(this->T);
    free(this);
}


typedef struct DiskManager
{
    int NUM_DISKS;
    bool* is_free;
    pthread_mutex_t** mutexes;
    int* next_sector;
} DiskManager;

DiskManager* make_disk_manager(int NUM_DISKS)
{
    DiskManager* disk_manager = malloc(sizeof(DiskManager));
    disk_manager->NUM_DISKS = NUM_DISKS;
    disk_manager->is_free = malloc(sizeof(bool) * NUM_DISKS);
    disk_manager->next_sector = malloc(sizeof(int) * NUM_DISKS);
    disk_manager->mutexes = malloc(sizeof(pthread_mutex_t*) * NUM_DISKS);
    for (int i = 0; i < NUM_DISKS; i++)
    {
        disk_manager->is_free[i] = true;
        disk_manager->mutexes[i] = malloc(sizeof(pthread_mutex_t));
        pthread_mutex_init(disk_manager->mutexes[i], NULL);
        disk_manager->next_sector[i] = 0;
    }
    return disk_manager;
}

void kill_disk_manager(DiskManager* this)
{
    free(this->is_free);
    for (int i = 0; i < this->NUM_DISKS; i++)
        free(this->mutexes[i]);
    free(this->mutexes);
    free(this->next_sector);
    free(this);
}

void increment_sector(DiskManager* this, int sector)
{
    (this->next_sector[sector])++;
}

int request_disk(DiskManager* disk_manager)
{
    while (true)
    {
        for (int i = 0; i < disk_manager->NUM_DISKS; i++)
        {
            if (disk_manager->is_free[i] == true)
            {
                disk_manager->is_free[i] = false;
                pthread_mutex_lock(disk_manager->mutexes[i]);
                return i;
            }
        }
        //printf("Everything's full rn\n");
    }
    // wait()
    return -1;
}

void release_disk(DiskManager* disk_manager, int disk_number)
{
    disk_manager->is_free[disk_number] = true;
    // notify()
    pthread_mutex_unlock(disk_manager->mutexes[disk_number]);
}


typedef struct PrinterManager
{
    int NUM_PRINTERS;
    bool* is_free;
    pthread_mutex_t** mutexes;
    //pthread_cond_t** conds;
} PrinterManager;

PrinterManager* make_printer_manager(int NUM_PRINTERS)
{
    PrinterManager* printer_manager = malloc(sizeof(PrinterManager));
    printer_manager->NUM_PRINTERS = NUM_PRINTERS;
    printer_manager->is_free = malloc(sizeof(bool) * NUM_PRINTERS);
    printer_manager->mutexes = malloc(sizeof(pthread_mutex_t*) * NUM_PRINTERS);
    //printer_manager->conds = malloc(sizeof(pthread_cond_t*) * NUM_PRINTERS);
    for (int i = 0; i < NUM_PRINTERS; i++)
    {
        printer_manager->is_free[i] = true;
        printer_manager->mutexes[i] = malloc(sizeof(pthread_mutex_t));
        pthread_mutex_init(printer_manager->mutexes[i], NULL); // default mutex_attr: undef behavior on deadlock
        //pthread_cond_init(printer_manager->conds[i], NULL);
    }
    return printer_manager;
}

void kill_printer_manager(PrinterManager* this)
{
    free(this->is_free);
    for (int i = 0; i < this->NUM_PRINTERS; i++)
        free(this->mutexes[i]);
    free(this->mutexes);
    free(this);
}

int request_printer(PrinterManager* printer_manager)
{
    while (true)
    {
        for (int i = 0; i < printer_manager->NUM_PRINTERS; i++)
        {
            if (printer_manager->is_free[i])
            {
                printer_manager->is_free[i] = false;
                pthread_mutex_lock(printer_manager->mutexes[i]);
                return i;
            }
        }
        //printf("Everything's full rn\n");
        //pthread_cond_wait(printer_managers->conds[i], printer_manager->mutexes[i]);
        // wait()
    }
    return -1;
}

void release_printer(PrinterManager* printer_manager, int printer_id)
{
    printer_manager->is_free[printer_id] = true;
    pthread_mutex_unlock(printer_manager->mutexes[printer_id]);
}


// NON-THREAD classes
typedef struct Disk
{
    String sectors[NUM_SECTORS];
} Disk;

Disk** load_disks(int NUM_DISKS)
{
    Disk** disks = malloc(sizeof(Disk*) * NUM_DISKS);
    for (int i = 0; i < NUM_DISKS; i++)
    {
        disks[i] = malloc(sizeof(Disk));
        for (int sect = 0; sect < NUM_SECTORS; sect++)
        {
            disks[i]->sectors[sect].buf = NULL;
            disks[i]->sectors[sect].len = 0;
        }
    }
    return disks;
}

void unload_disks(Disk** disks, int NUM_DISKS)
{   
    for (int disk = 0; disk < NUM_DISKS; disk++)
    {
        for (int sect = 0; sect < NUM_SECTORS; sect++)
        {
            //printf("%d, %d\n", disk, sect);
            char* buf = disks[disk]->sectors[sect].buf;
            //printf("%p\n", buf);
            if (buf != NULL)
                free(buf);
            //  destroy(&(disks[disk]->sectors[sect]));
        }
        free(disks[disk]);
    }
    free(disks);
}

void disk_write(Disk* D, int sector, String data)
{
    D->sectors[sector] = data;
    msleep(DISK_DELAY);
}

String disk_read(Disk* D, int sector)
{
    return D->sectors[sector];
    msleep(DISK_DELAY);
}


typedef struct Printer
{
    int id;
    FILE* fout;
} Printer;

Printer** open_printers(int NUM_PRINTERS)
{
    Printer** printers = malloc(sizeof(Printer*) * NUM_PRINTERS);
    char file_name[FILENAME_BUF_SIZE];
    for (int i = 0; i < NUM_PRINTERS; i++)
    {
        printers[i] = malloc(sizeof(Printer));
        printers[i]->id = i;
        cat_name(file_name, "PRINTER", printers[i]->id);
        printers[i]->fout = fopen(file_name, "w");
    }
    return printers;
}

void printer_print(Printer* printer, String line)
{
    for (int i = 0; i < (int) (line.len - 1); i++)
    {
        printf("%c", line.buf[i]);
        fprintf(printer->fout, "%c", line.buf[i]);
    }
    fprintf(printer->fout, "\n");
    msleep(PRINT_DELAY);
    fflush(printer->fout);
}

void close_printers(Printer** printers, int NUM_PRINTERS)
{   
    for (int i = 0; i < NUM_PRINTERS; i++)
    {
        fclose(printers[i]->fout);
        free(printers[i]);
    }
    free(printers);
}

typedef struct User
{
    int id;
    FILE* fin;
} User;

typedef struct OS141
{
    int NUM_USERS;
    int NUM_DISKS;
    int NUM_PRINTERS;
    User** users;
    Disk** disks;
    Printer** printers;
    DirectoryManager* dir_manager;
    DiskManager* disk_manager;
    PrinterManager* printer_manager;
} OS141;

OS141* OS; //forgive me for using a global instance ptr here


typedef struct PrintThread
{
    const char* file_name;
} PrintThread;

void* run_PrintThead(void* args)
{
    PrintThread* this = (PrintThread*) args;
    FileInfo* file_info = (FileInfo*) lookup_file(OS->dir_manager, this->file_name);
    int printer_id = request_printer(OS->printer_manager);
    printf("starting here\n");
    for (int sector = file_info->start_sector;
             sector < file_info->start_sector + file_info->file_len; sector++)
    {
        printer_print(OS->printers[printer_id], OS->disks[file_info->disk_number]->sectors[sector]);
    }
    release_printer(OS->printer_manager, printer_id);
    return NULL;
}

User** spawn_users(int NUM_USERS)
{
    User** users = malloc(sizeof(User*) * NUM_USERS);
    char file_name[FILENAME_BUF_SIZE];
    for (int i = 0; i < NUM_USERS; i++)
    {
        users[i] = malloc(sizeof(User));
        users[i]->id = i;
        cat_name(file_name, "USER", users[i]->id);
        users[i]->fin = fopen(file_name, "r");
    }
    return users;
}

void user_save(User* user, const char* file_name)
{
    printf("SAVE CALLED\nfile_name: ");
    printf("%s\n", file_name);
    int disk_number = request_disk(OS->disk_manager);
    int file_len = 0;
    int start_sector = OS->disk_manager->next_sector[disk_number];

    char buf[FIN_BUFFER_SIZE * 2];
    if (!fgets(buf, sizeof(buf), user->fin)) // skips a blank line;
        return;
    for (; fgets(buf, sizeof(buf), user->fin); file_len++)
    {
        if (strncmp(buf, ".end", 4) == 0)
        {
            break;
        }
        printf("save: %s\n", buf);
        disk_write(OS->disks[disk_number], start_sector + file_len, string(buf));
        increment_sector(OS->disk_manager, disk_number);
    }
    enter_file(OS->dir_manager, file_name, fileinfo(disk_number, start_sector, file_len));
    release_disk(OS->disk_manager, disk_number);
}

void user_print(User* user, const char* file_name)
{
    printf("PRINT CALLED\nfile_name: ");
    printf("%s\n", file_name);
    PrintThread pt = {.file_name = file_name};
    run_PrintThead((void*) &pt);
}

void* user_run(void* args)
{
    User* this = *((User**) args);
    char buf1[FIN_BUFFER_SIZE];
    char buf2[FIN_BUFFER_SIZE];
    while (fscanf(this->fin, "%s %s", buf1, buf2) != EOF)
    {
        printf(">> %s %s\n", buf1, buf2);
        if (strcmp(buf1, ".save") == 0)
            user_save(this, buf2);
        else if (strcmp(buf1, ".print") == 0)
            user_print(this, buf2);
        else if (strcmp(buf1, ".end") == 0)
        {// buf2 contains the next instruction
            if (fscanf(this->fin, "%s", buf1) != EOF)
            {
                long int offset = strlen(buf2) + 4; // 4 = strlen(".end")
                fseek(this->fin, -(offset), SEEK_CUR);
            }
            else
                break; // EOF reached
        }
    }
    return NULL;
}

void abort_users(User** users, int NUM_USERS)
{
    for (int i = 0; i < NUM_USERS; i++)
    {
        fclose(users[i]->fin);
        free(users[i]);
    }
    free(users);
}

OS141* init_os(int NUM_USERS, int NUM_DISKS, int NUM_PRINTERS)
{
    OS141* OS = malloc(sizeof(OS141));
    OS->NUM_USERS = NUM_USERS;
    OS->NUM_DISKS = NUM_DISKS;
    OS->NUM_PRINTERS = NUM_PRINTERS;
    OS->users = spawn_users(NUM_USERS);
    OS->disks = load_disks(NUM_DISKS);
    OS->printers = open_printers(NUM_PRINTERS);
    OS->dir_manager = make_dir_manager();
    OS->disk_manager = make_disk_manager(NUM_DISKS);
    OS->printer_manager = make_printer_manager(NUM_PRINTERS);
    return OS;
}

void destroy_os(OS141* OS)
{
    abort_users(OS->users, OS->NUM_USERS);
    unload_disks(OS->disks, OS->NUM_DISKS);
    close_printers(OS->printers, OS->NUM_PRINTERS);
    kill_dir_manager(OS->dir_manager);
    kill_disk_manager(OS->disk_manager);
    kill_printer_manager(OS->printer_manager); 
}

OS141* args_to_os(int argc, char* argv[])
{
    if (argc <= 1 || argc > 4)
    {
        printf("Invalid command line options!\n\tFormat: -num_users -num_disks -num_printers\n");
        return NULL;
    }
    for (int i = 1; i < argc; i++)
    {
        argv[i]++; // removes prefix "-" from command line options
    }
    int NUM_USERS = atoi(argv[1]);
    int NUM_DISKS = atoi(argv[2]);
    int NUM_PRINTERS = atoi(argv[3]);
    return init_os(NUM_USERS, NUM_DISKS, NUM_PRINTERS);
}

void start_user_threads(pthread_t* threads, int NUM_THREADS)
{
    User** users = OS->users;
    int result_code;
    for (int i = 0; i < NUM_THREADS; i++)
    {
        printf("Creating thread %d.\n", i);
        result_code = pthread_create(&threads[i], NULL, &user_run, &users[i]);
        assert(!result_code);
    }
}

void join_user_threads(pthread_t* threads, int NUM_THREADS)
{
    int result_code;
    for (int i = 0; i < NUM_THREADS; i++)
    {
        result_code = pthread_join(threads[i], NULL);
        assert(!result_code);
        printf("Thread %d has ended.\n", i);
    }
}

int main(int argc, char* argv[])
{
    OS = args_to_os(argc, argv);
    pthread_t threads[OS->NUM_USERS];
    start_user_threads(threads, OS->NUM_USERS);
    printf("In main: All threads are created.\n");
    join_user_threads(threads, OS->NUM_USERS);    
    printf("Main program has ended.\n");
    
    destroy_os(OS);
    return 0;
}
