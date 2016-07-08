#ifndef ANNOTATIONGRAPHICSITEMFACTORY_H
#define ANNOTATIONGRAPHICSITEMFACTORY_H

#include <AnnotatorLib/Annotation.h>
#include "annotationgraphicsitem.h"

class AnnotationGraphicsItemFactory
{
public:
    static AnnotationGraphicsItem *createItem(AnnotatorLib::Annotation *annotation);

};

#endif // ANNOTATIONGRAPHICSITEMFACTORY_H
