#include <QtCore/QtGlobal>

#if defined(GUIEXTENSION_LIBRARY)
#  define GUIEXTENSIONLIB Q_DECL_EXPORT
#else
#  define GUIEXTENSIONLIB Q_DECL_IMPORT
#endif
