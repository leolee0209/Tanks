

#ifndef LOGGER_MACRO
#define LOGGER_MACRO
#define NOTE(TEXT) note(__FILE__, __LINE__, TEXT, 0)
#define WARN(TEXT) warn(__FILE__, __LINE__, TEXT, 0)
#define ERROR(TEXT) error(__FILE__, __LINE__, TEXT, 0)
#define NOTE_S(TEXT) note(__FILE__, __LINE__, TEXT, 1)
#define WARN_S(TEXT) warn(__FILE__, __LINE__, TEXT, 1)
#define ERROR_S(TEXT) error(__FILE__, __LINE__, TEXT, 1)

#ifdef __cplusplus
extern "C" 
{
#endif
void note(const char *file, int line, char* s, bool stacktrace);
void error(const char *file, int line, char* s, bool stacktrace);
void warn(const char *file, int line, char* s, bool stacktrace);

void LoggerInit(char* name, bool cut);

#ifdef __cplusplus
}
#endif

#endif // LOGGER_MACRO