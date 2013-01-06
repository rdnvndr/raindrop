#ifndef %LibName:u%GLOBAL_H
#define %LibName:u%GLOBAL_H
#if defined(%LibName:u%_LIBRARY)
#  define %LibName:u%LIB Q_DECL_EXPORT
#else
#  define %LibName:u%LIB Q_DECL_IMPORT
#endif
#endif // %LibName:u%GLOBAL_H
