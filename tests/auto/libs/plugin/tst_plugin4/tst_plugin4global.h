#ifndef TST_PLUGIN4GLOBAL_H
#define TST_PLUGIN4GLOBAL_H
#if defined(TST_PLUGIN4_LIBRARY)
#  define TST_PLUGIN4LIB Q_DECL_EXPORT
#else
#  define TST_PLUGIN4LIB Q_DECL_IMPORT
#endif
#endif // TST_PLUGIN4GLOBAL_H

