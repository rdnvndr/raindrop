#include <QtCore/QtGlobal>

#if defined(SQLEDITOR_LIBRARY)
#  define SQLEDITORLIB Q_DECL_EXPORT
#else
#  define SQLEDITORLIB Q_DECL_IMPORT
#endif


