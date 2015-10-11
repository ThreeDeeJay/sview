/**
 * StGLWidgets, small C++ toolkit for writing GUI using OpenGL.
 * Copyright © 2011-2014 Kirill Gavrilov <kirill@sview.ru>
 *
 * Distributed under the Boost Software License, Version 1.0.
 * See accompanying file license-boost.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt
 */

#include <StGLWidgets/StGLRadioButtonFloat32.h>

StGLRadioButtonFloat32
    ::StGLRadioButtonFloat32(StGLWidget* theParent,
                             const StHandle<StFloat32Param>& theTrackedValue,
                             const float theOnValue,
                             const int theLeft, const int theTop,
                             const StGLCorner theCorner)
: StGLRadioButton(theParent,
                  StHandle<StInt32Param>(),
                  int32_t(theOnValue),
                  theLeft, theTop,
                  theCorner),
  myTrackValue(theTrackedValue),
  myValueOn(theOnValue) {
    //
}

StGLRadioButtonFloat32::~StGLRadioButtonFloat32() {}

bool StGLRadioButtonFloat32::isActiveState() const {
    return myTrackValue->areEqual(myTrackValue->getValue(), myValueOn);
}

void StGLRadioButtonFloat32::setValue() {
    myTrackValue->setValue(myValueOn);
}
