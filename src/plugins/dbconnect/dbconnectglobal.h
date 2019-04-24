#include <QtCore/QtGlobal>

#if defined(DBCONNECT_LIBRARY)
#  define DBCONNECTLIB Q_DECL_EXPORT
#else
#  define DBCONNECTLIB Q_DECL_IMPORT
#endif


