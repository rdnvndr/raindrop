#include <QtCore/QtGlobal>

#if defined(FILTERTREEWIDGET_LIBRARY)
#  define FILTERTREEWIDGETLIB Q_DECL_EXPORT
#else
#  define FILTERTREEWIDGETLIB Q_DECL_IMPORT
#endif
