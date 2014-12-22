#ifndef CLASSMODEL_H
#define CLASSMODEL_H

#include <treexmlmodel/treexmlhashmodel.h>

class ClassModel : public TreeXmlHashModel
{
public:
    ClassModel(QDomNode document, QObject *parent = 0);
    virtual ~ClassModel();

    void initTagFilters();
    void initDisplayedAttrs();
    void initInsertTags();
    void initHashAttrs();
    void initRelations();
};

#endif // CLASSMODEL_H
