from scipy.spatial import distance as dist
from matplotlib.patches import Polygon
import numpy as np
import matplotlib.pyplot as plt
import itertools
import math
import cv2 as cv

import argparse
import os


class DocumentScanner(object):
    def __init__(self, MIN_QUAD_AREA_RATIO=0.25, MAX_QUAD_ANGLE_RANGE=40):
        self.MIN_QUAD_AREA_RATIO = MIN_QUAD_AREA_RATIO
        self.MAX_QUAD_ANGLE_RANGE = MAX_QUAD_ANGLE_RANGE

    def GetAngle(self, pointA, pointB, pointC):
        a = np.radians(np.array(pointA))
        b = np.radians(np.array(pointB))
        c = np.radians(np.array(pointC))

        avec = a - b
        cvec = c - b

        return np.degrees(math.acos(np.dot(avec, cvec) / (np.linalg.norm(avec) * np.linalg.norm(cvec))))

    def AngleRange(self, quadrilateral):
        topLeft, topRight, bottomRight, bottomLeft = quadrilateral
        upperRightAngle = self.GetAngle(
            topLeft[0], topRight[0], bottomRight[0])
        upperLeftAngle = self.GetAngle(bottomLeft[0], topLeft[0], topRight[0])
        lowerRightAngle = self.GetAngle(
            topRight[0], bottomRight[0],  bottomLeft[0])
        lowerLeftAngle = self.GetAngle(
            bottomRight[0],  bottomLeft[0], topLeft[0])
        return np.ptp([upperRightAngle, upperLeftAngle, lowerRightAngle, lowerLeftAngle])

    def GetContour(self, sourceImage):
        MORPH = 9
        CANNY = 84
        HOUGH = 25

        SOURCE_IMAGE_HEIGHT, SOURCE_IMAGE_WIDTH, _ = sourceImage.shape

        gray_image = cv.cvtColor(sourceImage, cv.COLOR_BGR2GRAY)
        gray = cv.convertScaleAbs(gray_image)
        gray = cv.GaussianBlur(gray, (5, 5), 0)

        kernel = cv.getStructuringElement(cv.MORPH_RECT, (MORPH, MORPH))
        dilated = cv.morphologyEx(gray, cv.MORPH_CLOSE, kernel)

        edged = cv.Canny(dilated, 0, CANNY)

        lsd = cv.ximgproc.createFastLineDetector()
        lines = lsd.detect(edged)

        corners = []
        filtered_corners = []
        if lines is not None:
            lines = lines.squeeze().astype(np.int32).tolist()
            horizontalLinesCanvas = np.zeros(edged.shape, dtype=np.uint8)
            verticalLinesCanvas = np.zeros(edged.shape, dtype=np.uint8)
            for line in lines:
                (x1, y1, x2, y2) = line
                if abs(x2 - x1) > abs(y2 - y1):
                    (x1, y1), (x2, y2) = sorted(
                        ((x1, y1), (x2, y2)), key=lambda pt: pt[0])
                    cv.line(horizontalLinesCanvas, (max(x1 - 5, 0), y1),
                            (min(x2 + 5, edged.shape[1] - 1), y2), 255, 2)
                else:
                    (x1, y1), (x2, y2) = sorted(
                        ((x1, y1), (x2, y2)), key=lambda pt: pt[1])
                    cv.line(verticalLinesCanvas, (x1, max(y1 - 5, 0)),
                            (x2, min(y2 + 5, edged.shape[0] - 1)), 255, 2)

            lines = []

            contours, hierarchy = cv.findContours(
                horizontalLinesCanvas, cv.RETR_EXTERNAL, cv.CHAIN_APPROX_NONE)
            # contours = contours[1]
            contours = sorted(contours, key=lambda c: cv.arcLength(
                c, True), reverse=True)[:2]
            horizontalLinesCanvas = np.zeros(edged.shape, dtype=np.uint8)
            for contour in contours:
                contour = contour.reshape((contour.shape[0], contour.shape[2]))
                min_x = np.amin(contour[:, 0], axis=0) + 2
                max_x = np.amax(contour[:, 0], axis=0) - 2
                left_y = int(np.average(
                    contour[contour[:, 0] == min_x][:, 1] is not np.nan))
                right_y = int(np.average(
                    contour[contour[:, 0] == max_x][:, 1] is not np.nan))
                lines.append((min_x, left_y, max_x, right_y))
                cv.line(horizontalLinesCanvas, (min_x, left_y),
                        (max_x, right_y), 1, 1)
                corners.append((min_x, left_y))
                corners.append((max_x, right_y))

            contours, hierarchy = cv.findContours(
                verticalLinesCanvas, cv.RETR_EXTERNAL, cv.CHAIN_APPROX_NONE)
            # contours = contours[1]
            contours = sorted(contours, key=lambda c: cv.arcLength(
                c, True), reverse=True)[:2]
            verticalLinesCanvas = np.zeros(edged.shape, dtype=np.uint8)
            for contour in contours:
                contour = contour.reshape((contour.shape[0], contour.shape[2]))
                minY = np.amin(contour[:, 1], axis=0) + 2
                maxY = np.amax(contour[:, 1], axis=0) - 2
                topX = int(np.average(
                    contour[contour[:, 1] == minY][:, 0] is not np.nan))
                bottomX = int(np.average(
                    contour[contour[:, 1] == maxY][:, 0] is not np.nan))
                lines.append((topX, minY, bottomX, maxY))
                cv.line(verticalLinesCanvas, (topX, minY),
                        (bottomX, maxY), 1, 1)
                corners.append((topX, minY))
                corners.append((bottomX, maxY))

            cornersY, cornersX = np.where(
                horizontalLinesCanvas + verticalLinesCanvas == 2)
            corners += zip(cornersX, cornersY)

            def predicate(representatives, corner):
                return all(dist.euclidean(representative, corner) >= 20
                           for representative in representatives)

            for c in corners:
                if predicate(filtered_corners, c):
                    filtered_corners.append(c)

        approx_contours = []

        if len(corners) >= 4:
            quads = []
            for quad in itertools.combinations(corners, 4):
                points = np.array(quad)
                rect = np.zeros((4, 2), dtype = "float32")
                s = points.sum(axis = 1)
                rect[0] = points[np.argmin(s)] 
                rect[2] = points[np.argmax(s)]

                diff = np.diff(points, axis = 1)
                rect[1] = points[np.argmin(diff)]
                rect[3] = points[np.argmax(diff)]
                points = np.array(
                    [rect[0],  rect[1], rect[2], rect[3]], dtype="float32")
                points = np.array([[p] for p in points], dtype="int32")
                quads.append(points)

            quads = sorted(quads, key=cv.contourArea, reverse=True)[:5]

            quads = sorted(quads, key=self.AngleRange)

            approx = quads[0]
            if (len(approx) == 4 and cv.contourArea(approx) > SOURCE_IMAGE_WIDTH * SOURCE_IMAGE_HEIGHT * self.MIN_QUAD_AREA_RATIO 
            and self.AngleRange(approx) < self.MAX_QUAD_ANGLE_RANGE):
                approx_contours.append(approx)

        (cnts, hierarchy) = cv.findContours(
            edged.copy(), cv.RETR_EXTERNAL, cv.CHAIN_APPROX_SIMPLE)
        cnts = sorted(cnts, key=cv.contourArea, reverse=True)[:5]
        for c in cnts:
            approx = cv.approxPolyDP(c, 80, True)
            if (len(approx) == 4 and cv.contourArea(approx) > SOURCE_IMAGE_WIDTH * SOURCE_IMAGE_HEIGHT * self.MIN_QUAD_AREA_RATIO 
            and self.AngleRange(approx) < self.MAX_QUAD_ANGLE_RANGE):
                approx_contours.append(approx)
                break

        if not approx_contours:
            TOP_RIGHT = (SOURCE_IMAGE_WIDTH, 0)
            BOTTOM_RIGHT = (SOURCE_IMAGE_WIDTH, SOURCE_IMAGE_HEIGHT)
            BOTTOM_LEFT = (0, SOURCE_IMAGE_HEIGHT)
            TOP_LEFT = (0, 0)
            screenCnt = np.array(
                [[TOP_RIGHT], [BOTTOM_RIGHT], [BOTTOM_LEFT], [TOP_LEFT]])

        else:
            screenCnt = max(approx_contours, key=cv.contourArea)

        return screenCnt.reshape(4, 2)

    def Scan(self, sourceImagePath):
        RESCALED_HEIGHT = 500.0

        OUTPUT_DIR = os.path.join(os.getcwd(), "output")

        # load the image and compute the ratio of the old height
        # to the new height, clone it, and resize it
        sourceImage = cv.imread(sourceImagePath)

        assert(sourceImage is not None)

        ratio = sourceImage.shape[0] / RESCALED_HEIGHT

        sourceImageClone = sourceImage.copy()

        (sourceHeight, sourceWidth) = sourceImageClone.shape[:2]

        rescaled_image = cv.resize(sourceImageClone, (int(
            sourceWidth * (RESCALED_HEIGHT / float(sourceHeight))), sourceHeight), cv.INTER_AREA)

        # get the contour of the document
        screenCnt = self.GetContour(rescaled_image)
        screenCnt = screenCnt * ratio
        # apply the perspective transformation
        rect = np.zeros((4, 2), dtype = "float32")
        s = screenCnt.sum(axis = 1)
        rect[0] = screenCnt[np.argmin(s)]
        rect[2] = screenCnt[np.argmax(s)]

        diff = np.diff(screenCnt, axis = 1)
        rect[1] = screenCnt[np.argmin(diff)]
        rect[3] = screenCnt[np.argmax(diff)]

        (topLeft,topRight,bottomRight, bottomLeft) = rect  

        widthA = np.sqrt(
            ((bottomRight[0] - bottomLeft[0]) ** 2) + ((bottomRight[1] - bottomLeft[1]) ** 2))
        widthB = np.sqrt(
            ((topRight[0] - topLeft[0]) ** 2) + ((topRight[1] - topLeft[1]) ** 2))
        maxWidth = max(int(widthA), int(widthB))

        heightA = np.sqrt(
            ((topRight[0] - bottomRight[0]) ** 2) + ((topRight[1] - bottomRight[1]) ** 2))
        heightB = np.sqrt(
            ((topLeft[0] - bottomLeft[0]) ** 2) + ((topLeft[1] - bottomLeft[1]) ** 2))
        maxHeight = max(int(heightA), int(heightB))

        M = cv.getPerspectiveTransform(rect, np.array([
            [0, 0],
            [maxWidth - 1, 0],
            [maxWidth - 1, maxHeight - 1],
            [0, maxHeight - 1]], dtype="float32"))
        warped = cv.warpPerspective(sourceImageClone, M, (maxWidth, maxHeight))
        # convert the warped image to grayscale
        gray = cv.cvtColor(warped, cv.COLOR_BGR2GRAY)

        # sharpen image
        sharpen = cv.GaussianBlur(gray, (0, 0), 3)
        sharpen = cv.addWeighted(gray, 1.5, sharpen, -0.5, 0)
        # apply adaptive threshold to get black and white effect
        thresh = cv.adaptiveThreshold(
            sharpen, 255, cv.ADAPTIVE_THRESH_GAUSSIAN_C, cv.THRESH_BINARY, 21, 15)

        # save the transformed image
        basename = os.path.basename(sourceImagePath)
        cv.imwrite("output.jpg", thresh)
        print("Proccessed " + basename)
        cv.imshow('Input Image', sourceImage)


        cv.imshow('Output Image', thresh)

        cv.waitKey(0)


if __name__ == "__main__":
    scanner = DocumentScanner()
    scanner.Scan('./test.jpg')
