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
 *  Pavel Rojtberg
 */

// Simple example to demonstrate multi-marker tracking with ARToolKitPlus
// This sample does not open any graphics window. It just
// loads test images and shows use to use the ARToolKitPlus API.

#include "ARToolKitPlus/TrackerMultiMarkerImpl.h"

using ARToolKitPlus::TrackerMultiMarker;
using ARToolKitPlus::TrackerMultiMarkerImpl;

int main(int argc, char** argv) {
    const int width = 320, height = 240, bpp = 1;
    size_t numPixels = width * height * bpp;
    size_t numBytesRead;
    const char *fName = "data/markerboard_480-499.raw";
    unsigned char cameraBuffer[numPixels];

    // try to load a test camera image.
    // these images files are expected to be simple 8-bit raw pixel
    // data without any header. the images are expetected to have a
    // size of 320x240.
    if (FILE* fp = fopen(fName, "rb")) {
        numBytesRead = fread(cameraBuffer, 1, numPixels, fp);
        fclose(fp);
    } else {
        printf("Failed to open %s\n", fName);
        return -1;
    }

    if (numBytesRead != numPixels) {
        printf("Failed to read %s\n", fName);
        return -1;
    }

    // create a tracker that does:
    //  - 6x6 sized marker images
    //  - samples at a maximum of 6x6
    //  - works with luminance (gray) images
    //  - can load a maximum of 1 pattern
    //  - can detect a maximum of 8 patterns in one image
    TrackerMultiMarker *tracker = new TrackerMultiMarkerImpl(width, height, 6, 6, 6, 1, 8);

    tracker->setPixelFormat(ARToolKitPlus::PIXEL_FORMAT_LUM);

    // load a camera file.
    if (!tracker->init("data/PGR_M12x0.5_2.5mm.cal", "data/markerboard_480-499.cfg", 1.0f, 1000.0f)) {
        printf("ERROR: init() failed\n");
        delete tracker;
        return -1;
    }

    tracker->getCamera()->printSettings();

    // the marker in the BCH test image has a thiner border...
    tracker->setBorderWidth(0.125f);

    // set a threshold. we could also activate automatic thresholding
    tracker->setThreshold(160);

    // let's use lookup-table undistortion for high-speed
    // note: LUT only works with images up to 1024x1024
    tracker->setUndistortionMode(ARToolKitPlus::UNDIST_LUT);

    // RPP is more robust than ARToolKit's standard pose estimator
    //tracker->setPoseEstimator(ARToolKitPlus::POSE_ESTIMATOR_RPP);

    // switch to simple ID based markers
    // use the tool in tools/IdPatGen to generate markers
    tracker->setMarkerMode(ARToolKitPlus::MARKER_ID_SIMPLE);

    // switch on the hull only tracking
    tracker->setHullMode(ARToolKitPlus::HULL_FOUR);

    // do the OpenGL camera setup
    //glMatrixMode(GL_PROJECTION)
    //glLoadMatrixf(tracker->getProjectionMatrix());

    // here we go, just one call to find the camera pose
    int numDetected = tracker->calc(cameraBuffer);

    // use the result of calc() to setup the OpenGL transformation
    //glMatrixMode(GL_MODELVIEW)
    //glLoadMatrixf(tracker->getModelViewMatrix());

    printf("\n%d good Markers found and used for pose estimation.\nPose-Matrix:\n  ", numDetected);
    for (int i = 0; i < 16; i++)
        printf("%.2f  %s", tracker->getModelViewMatrix()[i], (i % 4 == 3) ? "\n  " : "");

    bool showConfig = false;

    if (showConfig) {
        const ARToolKitPlus::ARMultiMarkerInfoT *artkpConfig = tracker->getMultiMarkerConfig();
        printf("%d markers defined in multi marker cfg\n", artkpConfig->marker_num);

        printf("marker matrices:\n");
        for (int multiMarkerCounter = 0; multiMarkerCounter < artkpConfig->marker_num; multiMarkerCounter++) {
            printf("marker %d, id %d:\n", multiMarkerCounter, artkpConfig->marker[multiMarkerCounter].patt_id);
            for (int row = 0; row < 3; row++) {
                for (int column = 0; column < 4; column++)
                    printf("%.2f  ", artkpConfig->marker[multiMarkerCounter].trans[row][column]);
                printf("\n");
            }
        }
    }

    delete tracker;
    return 0;
}
