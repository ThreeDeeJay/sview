/**
 * StGLWidgets, small C++ toolkit for writing GUI using OpenGL.
 * Copyright © 2009-2013 Kirill Gavrilov <kirill@sview.ru>
 *
 * Distributed under the Boost Software License, Version 1.0.
 * See accompanying file license-boost.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt
 */

#include <StGLWidgets/StGLWidget.h>
#include <StGLWidgets/StGLWidgetList.h>

StGLWidgetList::StGLWidgetList()
: myFirst(NULL) {
    //
}

StGLWidgetList::~StGLWidgetList() {
    //
}

StGLWidget* StGLWidgetList::getLast() {
    if(myFirst == NULL) {
        return NULL;
    }
    StGLWidget* lastItem = myFirst;
    while(lastItem->hasNext()) {
        lastItem = lastItem->getNext();
    }
    return lastItem;
}

StGLWidgetList* StGLWidgetList::add(StGLWidget* theAddItem) {
    if(myFirst == NULL) {
        myFirst = theAddItem;
    } else {
        getLast()->setNext(theAddItem);
    }
    return this;
}

StGLWidgetList* StGLWidgetList::remove(StGLWidget* theRemItem) {
    if(myFirst == NULL || theRemItem == NULL) {
        // item does not exists in the list
        return this;
    } else if(myFirst == theRemItem) {
        // removed first item from the list
        myFirst = myFirst->hasNext() ? myFirst->getNext() : NULL;
    }

    if(theRemItem->hasPrev()) {
        // connect previous and next items
        theRemItem->getPrev()->setNext(theRemItem->getNext());
    } else if(theRemItem->hasNext()) {
        theRemItem->getNext()->setPrev(NULL);
    }
    return this;
}

void StGLWidgetList::moveToTop(StGLWidget* theTopmost) {
    remove(theTopmost);
    add(theTopmost);
}
