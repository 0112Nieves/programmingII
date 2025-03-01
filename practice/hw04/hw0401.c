#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <time.h>
static struct option long_options[] = {
    {"interval", required_argument, 0, 'i'},
    {"count", required_argument, 0, 'c'},
    {"timestamp", no_argument, 0, 't'},
    {"help", no_argument, 0, 'h'},
    {0, 0, 0, 0}
};
typedef struct Data{
    int pid;
    char comm[128];
    char state;
    int ppid;
    int pqrp;
    int session;
    int tty_nr;
    int tpqid;
    uint32_t flags;
    uint64_t minflt;
    uint64_t cminflt;
    uint64_t majflt;
    uint64_t cmajflt;
    uint64_t utime;
    uint64_t stime;
    int64_t cutime;
    int64_t cstime;
    int64_t priority;
    int64_t nice;
    int64_t num_threads;
    int64_t itrealvalue;
    long long unsigned int starttime;
    uint64_t vsize;
    int64_t rss;
    uint64_t rsslim;
    uint64_t startcode;
    uint64_t endcode;
    uint64_t startstack;
    uint64_t kstkesp;
    uint64_t kstkeip;
    uint64_t signal;
    uint64_t blocked;
    uint64_t siqignore;
    uint64_t siqcatch;
    uint64_t wchan;
    uint64_t nswap;
    uint64_t cnswap;
    int exit_signal;
    int processor;
    uint32_t rt_priority;
    uint32_t policy;
    long long unsigned int delayacct_blkio_ticks;
    uint64_t quest_time;
    int64_t cquest_time;
    uint64_t start_data;
    uint64_t end_data;
    uint64_t start_brk;
    uint64_t arq_start;
    uint64_t arq_end;
    uint64_t env_start;
    uint64_t env_end;
    int exit_code;
}data;
void printHelp()
{
    printf("Usage: hw0401 [options]... <pid>\n"
            "Options:\n"
            "\t-i, --interval <int> Display the process information every <int>seconds.\n"
            "\t\tDefault value: 1, Range: 1-100\n"
            "\t-c, --count <count > Display the process information <count > times.\n"
            "\t\tDefault behavior: infinite loop, Range: 1-1000\n"
            "\t-t, --timestamp Display the timestamp.\n"
            "\t-h, --help Display this description.\n"
    );
}
int getmem(){
    char str[1024] = "\0";
    FILE *mem = fopen("/proc/meminfo", "r");
    if(mem == NULL) return -1;
    int ans = 0;
    while(fgets(str, 1024, mem) != NULL){
        if(strncmp(str, "MemTotal:", strlen("MemTotal:")) == 0){
            for(int i = strlen("MemTotal:"); str[i] != '\0'; i++){
                if(str[i] >= '0' && str[i] <= '9'){
                    ans *= 10;
                    ans += str[i] - '0';
                }
            }
            break;
        }
    }
    fclose(mem);
    return ans;
}
int getActiveCPUs(){
    char str[1024] = "\0";
    FILE *cpu = fopen("/proc/cpinfo", "r");
    if(cpu == NULL) return -1;
    int ans = 0;
    while(fgets(str, 1024, cpu) != NULL){
        if(strncmp(str, "processor", strlen("processor")) == 0) ans++;
    }
    fclose(cpu);
    return ans;
}
int getPage()
{
    char str[1024] = "\0";
    FILE *page = fopen("/proc/self/smaps", "r");
    if(page == NULL) return -1;
    int ans = 0;
    while(fgets(str, 1024, page) != NULL){
        if(strncmp(str, "KernelPageSize:", strlen("KernelPageSize:") == 0)){
            for(int i = strlen("KernelPageSize:"); str[i] != '\0'; i++){
                if(str[i] >= '0' && str[i] <= '9'){
                    ans *= 10;
                    ans += str[i] - '0';
                }
            }
            break;
        }
    }
    fclose(page);
    return ans;
}
void printTime(){
    time_t current_time;
    time(&current_time);
    struct tm *info = localtime(&current_time);
    printf("%04d-%02d-%02d %02d:%02d:%02d ", info->tm_year +1900,
            info->tm_mon + 1, info->tm_mday, info->tm_hour, info->tm_min,
            info->tm_sec);
    return;
}
int main(int argc, char *argv[]){
    char c;
    int s = 1, times = -1;
    int totalmem = getmem(), activeCPUs = getActiveCPUs(), page = getPage();
    char PID[32];
    bool defaults = true, pt = false;
    while((c = getopt_long(argc, argv, "i:c:th", long_options, NULL)) != -1){
        switch(c){
            case 'i':
                s = strtol(optarg, NULL, 10);
                break;
            case 'c':
                times = strtol(optarg, NULL, 10);
                if(optind < argc){
                    sprintf(PID, "%s", argv[optind]);
                    defaults = false;
                }
                break;
            case 't':
                pt = true;
                break;
            case 'h':
                printHelp();
                return 0;
        }
    }
    if(defaults) sprintf(PID, "%s", "1");
    char path[1024];
    int64_t lastustime = 0, lastTotalTime = 0;
    sprintf(path, "/proc/%s/stat > tmpfile.txt", PID);
    while(times){
        FILE *tmpfile = fopen("tmpfile.txt", "w+");
        if(!tmpfile){
            perror("File opening failed");
            return 0;
        }
        int fd = fileno(tmpfile);
        char code[2048];
        sprintf(code, "cat %s > tmpfile.txt", path);
        system(code);
        fseek(tmpfile, 0, SEEK_END);
        int size = ftell(tmpfile);
        rewind(tmpfile);
        char *addr = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
        char *strAddr;
        if(addr == MAP_FAILED){
            printf("mmap error\n");
            sleep(s);
            fclose(tmpfile);
            continue;
        }
        strAddr = calloc(size + 1, sizeof(char));
        strncpy(strAddr, addr, size);
        munmap(addr, size);
        fclose(tmpfile);
        data d;
        sscanf(strAddr, "%d %s %c %d %d %d %d %d %u %lu %lu %lu %lu "
        "%lu %lu %ld %ld %ld %ld %ld %ld %llu %lu %ld %lu %lu "
        "%lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %d %d %u "
        "%u %llu %lu %ld %lu %lu %lu %lu %lu %lu %lu %d",
        &d.pid, d.comm, &d.state, &d.ppid, &d.pqrp, &d.session, &d.tty_nr,
        &d.tpqid, &d.flags, &d.minflt, &d.cminflt, &d.majflt, &d.cmajflt,
        &d.utime, &d.stime, &d.cutime, &d.cstime, &d.priority, &d.nice,
        &d.num_threads, &d.itrealvalue, &d.starttime, &d.vsize, &d.rss, &d.rsslim,
        &d.startcode, &d.endcode, &d.startstack, &d.kstkesp, &d.kstkeip,
        &d.signal, &d.blocked, &d.siqignore, &d.siqcatch, &d.wchan, &d.nswap,
        &d.cnswap, &d.exit_signal, &d.processor, &d.rt_priority, &d.policy,
        &d.delayacct_blkio_ticks, &d.quest_time, &d.cquest_time, &d.start_data, &d.end_data, &d.start_brk,
        &d.arq_start, &d.arq_end, &d.env_start, &d.env_end, &d.exit_code);
        free(strAddr);
        if(pt) printTime();
        if(d.comm[strlen(d.comm) - 1] == ')') d.comm[strlen(d.comm) - 1] = '\0';
        FILE *file = fopen("/proc/stat", "r");
        char buffer[256];
        fgets(buffer, 256, file);
        if(strncmp(buffer, "cpu", 3)) return -1;
        fclose(file);
        uint64_t usertime, nicetime, systemtime, idletime;
        uint64_t ioWait = 0, irq = 0, softIrq = 0, steal = 0, guest = 0, guestnice = 0;
        sscanf(buffer, "cpu %16lu %16lu %16lu %16lu %16lu %16lu %16lu %16lu %16lu %16lu",
                &usertime, &nicetime, &systemtime, &idletime, &ioWait, &irq,
                &softIrq, &steal, &guest, &guestnice);
        uint64_t idle_all_time = idletime + ioWait;
        uint64_t system_all_time = systemtime + irq + softIrq;
        uint64_t virt_all_time = guest + guestnice;
        uint64_t totaltime = usertime + nicetime + system_all_time + idletime + steal + virt_all_time;
        double period = (double)(totaltime - lastTotalTime) > 0 ? totaltime - lastTotalTime : 0 / activeCPUs;
        printf("%s %lf(%%CPU) %lf(%%MEM) %s(COMMAND)\n", PID, (period < 1E-6) 
                ? 0.0F : ((d.utime + d.stime - lastustime) * 100.0 / period),
                (double)d.rss * page * 100.0 / totalmem, d.comm + (d.comm[0] == '('));
        lastustime = ((double)d.utime + (double)d.stime);
        lastTotalTime = totaltime;
        system(code);
        sleep(s);
        if(times > 0) times--;
        sprintf(code, "rm tmpfile.txt");
        printf("%s\n", code);
        system(code);
    }
    return 0;
} 