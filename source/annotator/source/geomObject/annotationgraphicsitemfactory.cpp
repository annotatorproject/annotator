#include "annotationgraphicsitemfactory.h"

#include "ellipseannotation.h"
#include "rectangleannotation.h"


AnnotationGraphicsItem *AnnotationGraphicsItemFactory::createItem(AnnotatorLib::Annotation *annotation)
{
    assert(annotation);
    if(annotation->getType() == AnnotatorLib::AnnotationType::ELLIPSE){
        return new EllipseAnnotation(annotation);
    }else{
        return new RectangleAnnotation(annotation);
    }
}
