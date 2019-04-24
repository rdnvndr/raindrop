#include <QtCore/QtGlobal>

#if defined(MAINWINDOW_LIBRARY)
#  define MAINWINDOWLIB Q_DECL_EXPORT
#else
#  define MAINWINDOWLIB Q_DECL_IMPORT
#endif
