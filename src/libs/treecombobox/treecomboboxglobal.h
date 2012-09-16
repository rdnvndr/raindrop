#include <QtCore/QtGlobal>

#if defined(TREECOMBOBOX_LIBRARY)
#  define TREECOMBOBOXLIB Q_DECL_EXPORT
#else
#  define TREECOMBOBOXLIB Q_DECL_IMPORT
#endif
