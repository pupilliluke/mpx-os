typedef struct alarm{
    int hr;
    int min;
    int sec;
    char msg[100];
    char name[9];
    struct alarm* nextPtr;
} alarm;

void create_alarm(int hr, int min, int sec, const char* name, const char* msg);
void alarm_insert(alarm* alarm);
void alarm_remove(const char* name);
void check_alarm(void);

