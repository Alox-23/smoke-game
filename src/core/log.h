#ifndef CORE_LOG_H
#define CORE_LOG_H

#include <stdbool.h>

typedef enum {
    LOG_LEVEL_DEBUG = 0,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_COUNT
} LogLevel;

// Creates log_dir if it doesn't exist and opens a new log file named after
// the current local timestamp (e.g. logs/log_20260725_143012.txt).
// Called once, at engine startup. Returns false on failure.
bool log_init(const char *log_dir);

// Flushes and closes the log file. Safe to call even if log_init failed
// or was never called.
void log_shutdown(void);

// Call exactly once per main loop iteration, before any log_write calls
// that should be attributed to this tick. Increments the engine's tick
// counter. Any log_write() calls before the first log_tick() are recorded
// as tick 0.
void log_tick(void);

// Sets the minimum level that will be written; anything below it is
// discarded before formatting. Default: LOG_LEVEL_INFO.
void log_set_min_level(LogLevel level);

// Enables/disables mirroring log output to stdout (INFO and below) /
// stderr (WARN and above) in addition to the file. Default: enabled.
void log_set_console_output(bool enabled);

// Internal - use the LOG_* macros below instead so __FILE__/__LINE__ are
// captured automatically.
void log_write(LogLevel level, const char *file, int line, const char *fmt, ...);

#define LOG_DEBUG(...) log_write(LOG_LEVEL_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_INFO(...)  log_write(LOG_LEVEL_INFO,  __FILE__, __LINE__, __VA_ARGS__)
#define LOG_WARN(...)  log_write(LOG_LEVEL_WARN,  __FILE__, __LINE__, __VA_ARGS__)
#define LOG_ERROR(...) log_write(LOG_LEVEL_ERROR, __FILE__, __LINE__, __VA_ARGS__)

#endif // UTIL_LOG_H
