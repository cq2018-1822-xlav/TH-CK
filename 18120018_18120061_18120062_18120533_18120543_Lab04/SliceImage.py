import numpy as np 
import cv2 as cv
import Utils

class SliceImage:
    def __init__(self, image, row, column):
        self.image = image 
        self.pieces = []
        self.row = row 
        self.column = column 


    def divide(self):
        height = self.image.shape[0]
        width = self.image.shape[1]

        heightUnit = round(height / self.column)
        widthUnit = round(width / self.row)

        self.pieces = []

        for i in range(0, self.row):
            pieceRow = []
            for j in range(0, self.column):
                item = self.image[i * heightUnit: i * heightUnit + heightUnit, j * widthUnit: j * widthUnit + widthUnit]
                pieceRow.append(item)

            self.pieces.append(pieceRow)


    def merge(self):
        rows = []
        for i in range(0, self.row):
            row = cv.hconcat(self.pieces[i])
            rows.append(row)

        self.image = cv.vconcat(rows)


    def write(self):
        for i in range(0, self.row):
            for j in range(0, self.column):
                cv.imwrite('slice/slice_' + str(i) + str(j) + ".jpg", self.pieces[i][j])


    def contrast(self):
        standardDeviations = []
        for i in range(0, self.row):
            for j in range(0, self.column):
                standardDeviation = self.pieces[i][j].std()

                if standardDeviation > 8:
                    standardDeviations.append(standardDeviation)

        return np.mean(standardDeviations)


    def edge(self):
        for i in range(0, self.row):
            for j in range(0, self.column):
                self.pieces[i][j] = Utils.canny(self.pieces[i][j])


    def blur(self, sigmaColor, sigmaSpace):
        for i in range(0, self.row):
            for j in range(0, self.column):
                self.pieces[i][j] = cv.bilateralFilter(self.pieces[i][j], 9, sigmaColor, sigmaSpace)

