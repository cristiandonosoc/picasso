#include "log.h"

#include <cstdio>
#include <cstdarg>
#include <map>
#include <string>

namespace picasso {
namespace utils {
namespace log {

static std::map<LogLevel, const char*> level_map = {
  { LogLevel::LOG_DEBUG,  "[DEBUG]" },
  { LogLevel::LOG_INFO,   "[INFO]" },
  { LogLevel::LOG_WARN,   "[WARN]" },
  { LogLevel::LOG_ERROR,  "[ERROR]" },
};

void Log(FILE *output, LogLevel level, const char *file, int line,
         int indent, const char *fmt, ...) {
  const char *prefix = level_map[level];
  char buf[1024];
  // Log output will be like
  // <INDENT>[INFO] (main.cc:234): Created shader
  snprintf(buf, sizeof(buf), 
           "%*s%s (%s:%d): %s\n", indent, " ", prefix, file, line, fmt);
  va_list arglist;
  va_start(arglist, fmt);
  vfprintf(output, buf, arglist);
  va_end(arglist);
  fflush(output);
}

void Separator(FILE *output, int length, const char *c) {                                 
  if (length > 128) { length = 128; }                                       
  char buf[128 + 1];                                                        
  char *p = (char*)&buf;                                                    
  for (int i = 0; i < length; i++) { *p++ = *c; }                           
  *p = 0;                                                                   
  fprintf(output, "%s\n", buf);                                               
  fflush(output);                                                             
}


}   // namespaec log
}   // namespace utils
}   // namespace picasso
