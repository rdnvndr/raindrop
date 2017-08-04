#include <QtCore/QtGlobal>

#if defined(PLUGIN_LIBRARY)
#  define PLUGINLIB Q_DECL_EXPORT
#else
#  define PLUGINLIB Q_DECL_IMPORT
#endif
