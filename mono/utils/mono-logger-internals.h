#ifndef __MONO_LOGGER_INTERNAL_H__
#define __MONO_LOGGER_INTERNAL_H__

#include <glib.h>
#include "mono-logger.h"

G_BEGIN_DECLS

typedef enum {
	MONO_TRACE_ASSEMBLY		= (1<<0),
	MONO_TRACE_TYPE			= (1<<1),
	MONO_TRACE_DLLIMPORT		= (1<<2),
	MONO_TRACE_GC			= (1<<3),
        MONO_TRACE_CONFIG		= (1<<4),
	MONO_TRACE_AOT			= (1<<5),
	MONO_TRACE_SECURITY		= (1<<6),
	MONO_TRACE_THREADPOOL		= (1<<7),
	MONO_TRACE_IO_THREADPOOL	= (1<<8),
	MONO_TRACE_IO_LAYER		= (1<<9),
	MONO_TRACE_W32HANDLE	= (1<<10),
	MONO_TRACE_ALL			= MONO_TRACE_ASSEMBLY |
					  MONO_TRACE_TYPE |
					  MONO_TRACE_DLLIMPORT |
					  MONO_TRACE_GC |
					  MONO_TRACE_CONFIG |
					  MONO_TRACE_AOT |
					  MONO_TRACE_SECURITY |
					  MONO_TRACE_THREADPOOL |
					  MONO_TRACE_IO_THREADPOOL |
					  MONO_TRACE_IO_LAYER |
					  MONO_TRACE_W32HANDLE
} MonoTraceMask;

extern GLogLevelFlags mono_internal_current_level;
extern MonoTraceMask mono_internal_current_mask;

void 
mono_trace_init (void);

void 
mono_trace_cleanup (void);

void 
mono_tracev_inner (GLogLevelFlags level, MonoTraceMask mask, const char *format, va_list args);

void 
mono_trace_set_level (GLogLevelFlags level);

void 
mono_trace_set_mask (MonoTraceMask mask);

void 
mono_trace_push (GLogLevelFlags level, MonoTraceMask mask);

void 
mono_trace_pop (void);

gboolean
mono_trace_is_traced (GLogLevelFlags level, MonoTraceMask mask);

G_GNUC_UNUSED static void
mono_tracev (GLogLevelFlags level, MonoTraceMask mask, const char *format, va_list args)
{
	if(G_UNLIKELY (level <= mono_internal_current_level && mask & mono_internal_current_mask))
		mono_tracev_inner (level, mask, format, args);
}

/**
 * mono_trace:
 *
 *	@level: Verbose level of the specified message
 *	@mask: Type of the specified message
 *
 * Traces a new message, depending on the current logging level
 * and trace mask.
 */
G_GNUC_UNUSED static void
mono_trace (GLogLevelFlags level, MonoTraceMask mask, const char *format, ...) 
{
	if(G_UNLIKELY (level <= mono_internal_current_level && mask & mono_internal_current_mask)) {
		va_list args;
		va_start (args, format);
		mono_tracev_inner (level, mask, format, args);
		va_end (args);
	}
}

#ifdef G_HAVE_ISO_VARARGS
#define mono_trace_error(...)	mono_trace(G_LOG_LEVEL_ERROR, \
											__VA_ARGS__)
#define mono_trace_warning(...) mono_trace(G_LOG_LEVEL_WARNING, \
											__VA_ARGS__)
#define mono_trace_message(...) mono_trace(G_LOG_LEVEL_MESSAGE, \
											__VA_ARGS__)
#elif defined(G_HAVE_GNUC_VARARGS)
#define mono_trace_error(format...)	mono_trace(G_LOG_LEVEL_ERROR, \
											format)
#define mono_trace_warning(format...) mono_trace(G_LOG_LEVEL_WARNING, \
											format)
#define mono_trace_message(format...) mono_trace(G_LOG_LEVEL_MESSAGE, \
											format)
#else /* no varargs macros */
G_GNUC_UNUSED static void
mono_trace_error(MonoTraceMask mask, const char *format, ...)
{
	va_list args;
	va_start (args, format);
	mono_tracev(G_LOG_LEVEL_ERROR, mask, format, args);
	va_end (args);
}

G_GNUC_UNUSED static void
mono_trace_warning(MonoTraceMask mask, const char *format, ...)
{
	va_list args;
	va_start (args, format);
	mono_tracev(G_LOG_LEVEL_WARNING, mask, format, args);
	va_end (args);
}

G_GNUC_UNUSED static void
mono_trace_message(MonoTraceMask mask, const char *format, ...)
{
	va_list args;
	va_start (args, format);
	mono_tracev(G_LOG_LEVEL_MESSAGE, mask, format, args);
	va_end (args);
}

#endif /* !__GNUC__ */

#if defined (PLATFORM_ANDROID) || (defined (TARGET_IOS) && defined (TARGET_IOS))

#define mono_gc_printf(gc_log_file, format, ...) g_log ("mono-gc", G_LOG_LEVEL_MESSAGE, format, ##__VA_ARGS__)
#define mono_runtime_printf(format, ...) g_log ("mono-rt", G_LOG_LEVEL_MESSAGE, format "\n", ##__VA_ARGS__)
#define mono_runtime_printf_err(format, ...) g_log ("mono-rt", G_LOG_LEVEL_CRITICAL, format "\n", ##__VA_ARGS__)
#define mono_runtime_stdout_fflush() do { } while (0)

#else

#define mono_gc_printf(gc_log_file, format, ...) do {	\
	fprintf (gc_log_file, format, ##__VA_ARGS__);	\
	fflush (gc_log_file);	\
} while (0)

#define mono_runtime_printf(format, ...) fprintf (stdout, format "\n", ##__VA_ARGS__)
#define mono_runtime_printf_err(format, ...) fprintf (stderr, format "\n", ##__VA_ARGS__)
#define mono_runtime_stdout_fflush() do { fflush (stdout); } while (0)

#endif

/* Internal logging API */
typedef void (*MonoLoggerOpen) (const char *, void *);
typedef void (*MonoLoggerWrite) (const char *, GLogLevelFlags, mono_bool, const char *);
typedef void (*MonoLoggerClose) (void);

typedef struct _MonoLogCallParm_ {
	MonoLoggerOpen 	opener;		/* Routine to open logging */
	MonoLoggerWrite	writer;		/* Routine to write log data */
	MonoLoggerClose closer; 	/* Routine to close logging */
	char		*dest;		/* Log destination */
	void		*user_data;	/* User data from legacy handler */
	mono_bool       header;		/* Whether we want pid/time/date in log message */
} MonoLogCallParm;

void mono_trace_set_log_handler_internal (MonoLogCallParm *callback, void *user_data);
void mono_trace_set_logdest_string (const char *value);
void mono_trace_set_logheader_string (const char *value);

void mono_log_open_syslog (const char *, void *);
void mono_log_write_syslog (const char *, GLogLevelFlags, mono_bool, const char *);
void mono_log_close_syslog (void);

void mono_log_open_logfile (const char *, void *);
void mono_log_write_logfile (const char *, GLogLevelFlags, mono_bool, const char *);
void mono_log_close_logfile (void);

G_END_DECLS

#endif /* __MONO_LOGGER_INTERNAL_H__ */
