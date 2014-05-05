#define LOG_SIZE 4096

#ifndef SUCCESS
#define SUCCESS 0
#endif

#ifndef MAX_IP_LEN
#define MAX_IP_LEN 25
#endif

#ifndef LOG_DEBUG
#define LOG_DEBUG    0
#endif
#ifndef LOG_INFO
#define LOG_INFO     1
#endif
#ifndef LOG_CRITICAL
#define LOG_CRITICAL 2
#endif
#ifndef LOG_ERROR
#define LOG_ERROR    3
#endif
#ifndef LOGTYPECOUNT
#define LOGTYPECOUNT 4
#endif


#ifndef MAX_PARAM_LEN
#define MAX_PARAM_LEN 128
#endif

#ifndef MAXLOGTYPELEN
#define MAXLOGTYPELEN 25
#endif

#ifndef MAXPATHLEN
#define MAXPATHLEN 4096
#endif



#ifndef MAX_MSG_LEN
#define MAX_MSG_LEN 4096
#endif

void SQ_LOG(int loglevel,char * fmt, ...);

