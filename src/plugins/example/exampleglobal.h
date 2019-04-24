#include <QtCore/QtGlobal>

#if defined(EXAMPLE_LIBRARY)
#  define EXAMPLELIB Q_DECL_EXPORT
#else
#  define EXAMPLELIB Q_DECL_IMPORT
#endif
