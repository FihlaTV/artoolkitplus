/**
 * Copyright (C) 2010  ARToolkitPlus Authors
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authors:
 *  Daniel Wagner
 */

#ifndef __TRACKERMULTIMARKER_HEADERFILE__
#define __TRACKERMULTIMARKER_HEADERFILE__

#include <ARToolKitPlus/TrackerImpl.h>

namespace ARToolKitPlus {

/**
 * Defines a simple interface for multi-marker tracking with ARToolKitPlus
 *  ARToolKit::TrackerMultiMarker provides all methods to access ARToolKit for
 *  multi marker tracking without needing to mess around with it directly.
 */
class TrackerMultiMarker: public Tracker {
public:
    virtual ~TrackerMultiMarker() {
    }

    /**
     * initializes ARToolKit
     *  nCamParamFile is the name of the camera parameter file
     *  nNearClip & nFarClip are near and far clipping values for the OpenGL projection matrix
     *  nLogger is an instance which implements the ARToolKit::Logger interface
     */
    virtual bool init(const char* nCamParamFile, const char* nMultiFile, ARFloat nNearClip, ARFloat nFarClip) = 0;

    /**
     * calculates the transformation matrix
     *	pass the image as RGBX (32-bits)
     */
    virtual int calc(const uint8_t* nImage) = 0;

    /// Returns the number of detected markers used for multi-marker tracking
    virtual int getNumDetectedMarkers() const = 0;

    /**
     * Enables usage of arDetectMarkerLite. Otherwise arDetectMarker is used
     * In general arDetectMarker is more powerful since it keeps history about markers.
     * In some cases such as very low camera refresh rates it is advantegous to change this.
     * Using the non-lite version treats each image independent.
     */
    virtual void setUseDetectLite(bool nEnable) = 0;

    /**
     * Returns array of detected marker IDs
     * Only access the first getNumDetectedMarkers() markers
     */
    virtual void getDetectedMarkers(int*& nMarkerIDs) = 0;

    /// Returns the ARMarkerInfo object for a found marker
    virtual const ARMarkerInfo& getDetectedMarker(int nWhich) const = 0;

    /**
     * Returns the loaded ARMultiMarkerInfoT object
     *  If loading the multi-marker config file failed then this method
     *  returns NULL.
     */
    virtual const ARMultiMarkerInfoT* getMultiMarkerConfig() const = 0;

    /**
     * Provides access to ARToolKit' internal version of the transformation matrix
     *  This method is primarily for compatibility issues with code previously using
     *  ARToolKit rather than ARToolKitPlus. This is the original transformation
     *  matrix ARToolKit calculates rather than the OpenGL style version of this matrix
     *  that can be retrieved via getModelViewMatrix().
     */
    virtual void getARMatrix(ARFloat nMatrix[3][4]) const = 0;
};

}
; // namespace ARToolKitPlus


#endif //__TRACKERMULTIMARKER_HEADERFILE__
