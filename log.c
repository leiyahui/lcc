
#include "lcc.h"
static int g_logfile_fd = -1;
#define LOG_PREFIX "C:/"
#define LOG_LEVLE 1
//========================================= log =========================================//
const char *loglevel_to_string(LogLevel l)
{
	switch (l) {
	case LL_DEBUG:
		return "DEBUG";
	case LL_TRACE:
		return "TRACE";
	case LL_INFO:
		return "INFO";
	case LL_WARNING:
		return "WARN";
	case LL_ERROR:
		return "ERROR";
	default:
		return "UNKNOWN";
	}
}

int string_to_loglevel(char *str)
{
	if (oa_str_case_equal("DEBUG", str))
		return LL_DEBUG;
	else if (oa_str_case_equal("TRACE", str))
		return LL_TRACE;
	else if (oa_str_case_equal("INFO", str))
		return LL_INFO;
	else if (oa_str_case_equal("WARN", str))
		return LL_WARNING;
	else if (oa_str_case_equal("ERROR", str))
		return LL_ERROR;
	else
		return LL_OFF;
}

static int get_logfilefd(const char *filename)
{
    int fd = -1;
	if (filename && (*filename)) {
        fd = open(filename, O_CREAT | O_WRONLY | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	}
	return fd;
}

void init_logfile_fd(int log_limit)
{
#define ONEAPM_AGENT_LOG	"/oneapm-agent.log"
	char* p_prefix = LOG_PREFIX;
	char* p_log_file = NULL;
	int prefix_len, agent_log_len, log_file_len;

	struct stat file_stat;

	if (g_logfile_fd == -1) {
		prefix_len = l_strlen(p_prefix);
		agent_log_len = l_strlen(ONEAPM_AGENT_LOG);
		log_file_len = prefix_len + agent_log_len + 1;
		p_log_file = (char*)l_malloc(log_file_len);
		strncpy(p_log_file, p_prefix, prefix_len);
		strncat(p_log_file, ONEAPM_AGENT_LOG, agent_log_len);
		p_log_file[log_file_len - 1] = '\0';
		g_logfile_fd = get_logfilefd(p_log_file);
		l_free(p_log_file);
	}

	if (!log_limit) {
		return;
	}

	if (fstat(g_logfile_fd, &file_stat) != 0) {
		return;
	}

	if (file_stat.st_size > LOG_FILE_SIZE) {
		oa_change_file_size(g_logfile_fd);
	}
}

void close_logfile_fd()
{
	if (g_logfile_fd != -1)
	_close(g_logfile_fd);
}

void out_put_log(LogLevel l, char *file, int lineno, const char *func_name, char *logformat, ...)
{
	va_list args;
	int _size;
	time_t cur_time;
	struct tm local_tm;

	char *agentlog = NULL;
	int agentlog_len;
	if (l >= LOG_LEVLE) {
		char message[LOG_BUFFSIZE] = {0};
		va_start(args, logformat);
		_size = vsnprintf(message, LOG_BUFFSIZE - 1, logformat, args);
		va_end(args);
		message[LOG_BUFFSIZE - 1] = 0;
		if (_size > LOG_BUFFSIZE - 1 || _size < 0) {
			_size = LOG_BUFFSIZE - 1;
		}
		cur_time = time(NULL);
#ifdef _WIN32
		if (g_logfile_fd != -1 && cur_time > 0 && localtime_s(&local_tm, &cur_time) == NULL) {
#else
		if (g_logfile_fd != -1 && cur_time > 0 && localtime_r(&cur_time, &local_tm) != NULL) {
#endif
			agentlog = malloc(LOG_BUFFSIZE + 128);
			agentlog_len = _snprintf(agentlog, LOG_BUFFSIZE + 128, "%s: %04d-%02d-%02d %02d:%02d:%02d [%s:%d][%s] %s\n", loglevel_to_string(l),
				(1900 + local_tm.tm_year), local_tm.tm_mon + 1, local_tm.tm_mday,
				local_tm.tm_hour, local_tm.tm_min, local_tm.tm_sec, file, lineno, func_name, message);
			if (agentlog_len > 0) {
				write(g_logfile_fd, agentlog, agentlog_len);
            }
            if (agentlog) {
                free(agentlog);
                agentlog = NULL;
            }
		}
	}
}

//========================================= signal handle =========================================//
//
//static void print_backtrace(int sig)
//{
//#define AGENT_BT	"/agent_bt.txt"
//	//11: SIGSEGV   //segment violation，无效内存引用
//	//7 : SIGBUS    //与平台相关的硬件错误，一般是内存错误，通常是未对齐的数据访问所致
//	//8 : SIGFPE    //浮点数异常
//	//4 : SIGILL    //非法硬件指令
//	//6 : SIGABRT   //调用abort函数时产生此信号，进程异常终止
//
//	char* p_prefix = INI_STR(OA_CF_VAR_PATH);
//	int prefix_len, agent_bt_len, log_file_len;
//	char* p_log_file;
//	struct stat file_stat;
//	int mask = oa_umask(0);
//	int bt_fd;
//	char *msg = NULL;
//
//	time_t cur_time;
//	struct tm local_tm;
//	char *banner = NULL;
//#ifndef PHP_WIN32
//	size_t size;
//	void *array[50];
//#endif
//	prefix_len = l_strlen(p_prefix);
//	agent_bt_len = l_strlen(AGENT_BT);
//	log_file_len = prefix_len + agent_bt_len + 1;
//	p_log_file = (char*)oa_malloc(log_file_len);
//	strncpy(p_log_file, p_prefix, prefix_len);
//	strncat(p_log_file, AGENT_BT, agent_bt_len);
//	p_log_file[log_file_len - 1] = '\0';
//
//	bt_fd = oa_open_mode(p_log_file, O_RDWR | O_CREAT | O_APPEND, 0666);
//	SAFE_DELETE(p_log_file);
//	if (bt_fd == -1) {
//		oa_umask(mask);
//		return;
//	}
//
//	if (fstat(bt_fd, &file_stat) == 0) {
//		if (file_stat.st_size > LOG_FILE_SIZE) {
//			close(bt_fd);
//			umask(mask);
//			return;
//		}
//	}
//
//	switch (sig)
//	{
//	case SIGSEGV:
//		msg = "segmentation violation";
//		break;
//#ifndef PHP_WIN32
//	case SIGBUS:
//		msg = "SIGBUS";
//		break;
//#endif
//	case SIGFPE:
//		msg = "SIGFPE: likely integer zero divide";
//		break;
//	case SIGILL:
//		msg = "SIGILL";
//		break;
//	case SIGABRT:
//		msg = "SIGABRT";
//		break;
//	default:
//		msg = "?";
//		break;
//	}
//
//	cur_time = time(NULL);
//#ifdef PHP_WIN32
//	if (cur_time > 0 && localtime_s(&local_tm, &cur_time) != NULL) {
//#else
//	if (cur_time > 0 && localtime_r(&cur_time, &local_tm) != NULL) {
//#endif
//		banner = oa_sprintf("\n %04d-%02d-%02d %02d:%02d:%02d Version %s\n Process %d received signal %2d: %s , bss[%p]\n", \
//			(1900 + local_tm.tm_year), local_tm.tm_mon + 1, local_tm.tm_mday, local_tm.tm_hour, local_tm.tm_min, local_tm.tm_sec, DETAIL_VERSION, getpid(), sig, msg, &print_backtrace);
//	} else {
//		banner = oa_sprintf("Version %s\n Process %d received signal %2d: %s , bss[%p]\n", \
//			DETAIL_VERSION, getpid(), sig, msg, &print_backtrace);
//	}
//
//	oa_write(bt_fd, banner, strlen(banner));
//#ifndef PHP_WIN32
//	size = backtrace(array, 50);
//	backtrace_symbols_fd(array, size, bt_fd);
//#endif
//	oa_close(bt_fd);
//	umask(mask);
//}
//
//static void agent_fatal_signal_handler(int sig)
//{
//	print_backtrace(sig);
//	signal_reraise(sig);
//}
//
//void apm_fatal_signal_handler()
//{
//#ifndef PHP_WIN32
//	struct sigaction sa;
//	sa.sa_flags = SA_RESTART;
//	sa.sa_handler = &agent_fatal_signal_handler;
//	sigfillset(&sa.sa_mask);
//	sigaction(SIGSEGV, &sa, NULL);
//	sigaction(SIGBUS, &sa, NULL);
//	sigaction(SIGFPE, &sa, NULL);
//	sigaction(SIGILL, &sa, NULL);
//	sigaction(SIGABRT, &sa, NULL);
//#else
//	signal(SIGSEGV, &agent_fatal_signal_handler);
//	signal(SIGFPE, &agent_fatal_signal_handler);
//	signal(SIGILL, &agent_fatal_signal_handler);
//	signal(SIGABRT, &agent_fatal_signal_handler);
//#endif
//}
