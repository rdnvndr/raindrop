#include <QtCore/QtGlobal>

#if defined(METADATAMODEL_LIBRARY)
#  define METADATAMODELLIB Q_DECL_EXPORT
#else
#  define METADATAMODELLIB Q_DECL_IMPORT
#endif
