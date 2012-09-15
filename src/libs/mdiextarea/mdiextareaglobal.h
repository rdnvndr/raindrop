#include <QtCore/QtGlobal>

#if defined(MDIEXTAREA_LIBRARY)
#  define MDIEXTAREALIB Q_DECL_EXPORT
#else
#  define MDIEXTAREALIB Q_DECL_IMPORT
#endif
