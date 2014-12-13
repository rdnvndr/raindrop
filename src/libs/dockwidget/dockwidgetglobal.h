#include <QtCore/QtGlobal>

#if defined(DOCKWIDGET_LIBRARY)
#  define DOCKWIDGETLIB Q_DECL_EXPORT
#else
#  define DOCKWIDGETLIB Q_DECL_IMPORT
#endif
