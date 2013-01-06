#ifndef %LibName:u%_%CppHeaderSuffix:u%
#define %LibName:u%_%CppHeaderSuffix:u%

#include <QObject>
#include "%LibName:l%global.%CppHeaderSuffix:l%"
class  %LibName:u%LIB %LibName%:
        public QObject
{
    Q_OBJECT

public:

    //! Конструктор библиотеки
    explicit %LibName%(QObject *parent = 0);
};

#endif
