#ifndef %PluginName:u%GLOBAL_H
#define %PluginName:u%GLOBAL_H
#if defined(%PluginName:u%_LIBRARY)
#  define %PluginName:u%LIB Q_DECL_EXPORT
#else
#  define %PluginName:u%LIB Q_DECL_IMPORT
#endif
#endif // %PluginName:u%GLOBAL_H
