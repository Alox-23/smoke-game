// Needed for localtime_r / clock_gettime under -std=c17 -Wpedantic
#define _POSIX_C_SOURCE 200809L

#include "log.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>

#define LOG_DIR_MAX  240
#define LOG_PATH_MAX (LOG_DIR_MAX + 64)
#define LOG_LINE_MAX 1024

static const char *LOG_LEVEL_NAMES[LOG_LEVEL_COUNT] = {
    "DEBUG", "INFO", "WARN", "ERROR"
};

typedef struct {
    FILE    *file;
    unsigned long long tick_count; // total ticks since engine launch
    LogLevel min_level;
    bool     console_output;
    bool     initialized;
} Logger;

static Logger g_logger = {0};

bool log_init(const char *log_dir) {
    if (g_logger.initialized) {
        return false;
    }

    char dir[LOG_DIR_MAX];
    strncpy(dir, log_dir, LOG_DIR_MAX - 1);
    dir[LOG_DIR_MAX - 1] = '\0';

    if (mkdir(dir, 0755) != 0 && errno != EEXIST) {
        fprintf(stderr, "log: failed to create log directory '%s': %s\n",
                dir, strerror(errno));
        return true;
    }

    time_t now = time(NULL);
    struct tm tm_now;
    localtime_r(&now, &tm_now);

    char path[LOG_PATH_MAX];
    snprintf(path, sizeof(path), "%s/log_%04d%02d%02d_%02d%02d%02d.txt",
             dir,
             tm_now.tm_year + 1900, tm_now.tm_mon + 1, tm_now.tm_mday,
             tm_now.tm_hour, tm_now.tm_min, tm_now.tm_sec);

    // If a file with this exact second already exists (e.g. two runs
    // started within the same second), fall back to an incrementing suffix
    // rather than silently truncating an existing log.
    if (access(path, F_OK) == 0) {
        for (int suffix = 1; suffix < 100; suffix++) {
            char candidate[LOG_PATH_MAX];
            snprintf(candidate, sizeof(candidate),
                     "%s/log_%04d%02d%02d_%02d%02d%02d_%02d.txt",
                     dir,
                     tm_now.tm_year + 1900, tm_now.tm_mon + 1, tm_now.tm_mday,
                     tm_now.tm_hour, tm_now.tm_min, tm_now.tm_sec, suffix);
            if (access(candidate, F_OK) != 0) {
                strncpy(path, candidate, sizeof(path) - 1);
                path[sizeof(path) - 1] = '\0';
                break;
            }
        }
    }

    g_logger.file = fopen(path, "w");
    if (!g_logger.file) {
        fprintf(stderr, "log: failed to open log file '%s': %s\n",
                path, strerror(errno));
        return true;
    }

    g_logger.tick_count = 0;
    g_logger.min_level = LOG_LEVEL_DEBUG;
    g_logger.console_output = true;
    g_logger.initialized = true;
    return false;
}

void log_shutdown(void) {
    if (!g_logger.initialized) {
        return;
    }
    if (g_logger.file) {
        fflush(g_logger.file);
        fclose(g_logger.file);
        g_logger.file = NULL;
    }
    g_logger.initialized = false;
}

void log_tick(void) {
    if (!g_logger.initialized) {
        return;
    }
    g_logger.tick_count++;
}

void log_set_min_level(LogLevel level) {
    g_logger.min_level = level;
}

void log_set_console_output(bool enabled) {
    g_logger.console_output = enabled;
}

void log_write(LogLevel level, const char *file, int line, const char *fmt, ...) {
    if (!g_logger.initialized || level < g_logger.min_level) {
        return;
    }

    // Wall-clock timestamp, millisecond precision
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    struct tm tm_now;
    localtime_r(&ts.tv_sec, &tm_now);

    char timestamp[32];
    snprintf(timestamp, sizeof(timestamp), "%02d:%02d:%02d.%03ld",
             tm_now.tm_hour, tm_now.tm_min, tm_now.tm_sec,
             ts.tv_nsec / 1000000);

    // Format the caller's message
    char message[LOG_LINE_MAX];
    va_list args;
    va_start(args, fmt);
    vsnprintf(message, sizeof(message), fmt, args);
    va_end(args);

    // Trim to just the filename for readability
    const char *filename = strrchr(file, '/');
    filename = filename ? filename + 1 : file;

    char line_buf[LOG_LINE_MAX + 128];
    int len = snprintf(line_buf, sizeof(line_buf),
                        "[tick %08llu][%s][%-5s] %s:%d: %s\n",
                        g_logger.tick_count,
                        timestamp,
                        LOG_LEVEL_NAMES[level],
                        filename, line,
                        message);
    if (len < 0) {
        return;
    }

    if (g_logger.file) {
        fwrite(line_buf, 1, (size_t)len, g_logger.file);
        // Flush immediately on warnings and above so nothing is lost on crash
        if (level >= LOG_LEVEL_WARN) {
            fflush(g_logger.file);
        }
    }

    if (g_logger.console_output) {
        FILE *out = (level >= LOG_LEVEL_WARN) ? stderr : stdout;
        fwrite(line_buf, 1, (size_t)len, out);
    }
}
