#include <QtCore/QtGlobal>

#if defined(DATABASEMODEL_LIBRARY)
#  define DATABASEMODELLIB Q_DECL_EXPORT
#else
#  define DATABASEMODELLIB Q_DECL_IMPORT
#endif
