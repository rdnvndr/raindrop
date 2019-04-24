#include <QtCore/QtGlobal>

#if defined(UNDOSTACK_LIBRARY)
#  define UNDOSTACKLIB Q_DECL_EXPORT
#else
#  define UNDOSTACKLIB Q_DECL_IMPORT
#endif

