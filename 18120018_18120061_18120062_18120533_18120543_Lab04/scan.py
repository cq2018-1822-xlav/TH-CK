import numpy as np 
import cv2 as cv
import os
import argparse

class DocumentScanner:
    def __init__(self) -> None:
        super().__init__()

    def scan(self, imagePath):
        # read image
        sourceImage = cv.imread(imagePath, 1)
        # resize image
        sourceImageClone = sourceImage.copy()
        sourceImageClone = cv.resize(sourceImageClone,(600,800))
        sourceImage = cv.resize(sourceImage,(600,800))
        # convert image to grayscale
        grayscale = cv.cvtColor(sourceImageClone, cv.COLOR_BGR2GRAY)
        #blurr image to smooth
        bluredImage = cv.GaussianBlur(grayscale, (5,5),0) 
        #finding edges 
        edgeDectect = cv.Canny(bluredImage, 0, 50) 
        #find contours in thresholded image and sort them according to decreasing area
        contours, hierarchy = cv.findContours(edgeDectect, cv.RETR_LIST, cv.CHAIN_APPROX_SIMPLE)
        contours = sorted(contours, key=cv.contourArea, reverse= True) 
        #contour approximation 
        for i in contours:
	        elip =  cv.arcLength(i, True)
	        approx = cv.approxPolyDP(i,0.08*elip, True)

	        if len(approx) == 4 : 
		        doc = approx 
		        break
        #draw contours 
        cv.drawContours(sourceImageClone, [doc], -1, (0, 255, 0), 2)
        #reshape to avoid errors ahead
        doc=doc.reshape((4,2))
        #create a new array and initialize 
        rect = np.zeros((4,2), dtype="float32")
        Sum = doc.sum(axis = 1)
        rect[0] = doc[np.argmin(Sum)]
        rect[2] = doc[np.argmax(Sum)]

        Diff = np.diff(doc, axis=1)
        rect[1] = doc[np.argmin(Diff)]
        rect[3] = doc[np.argmax(Diff)]

        (topLeft,topRight,bottomRight,bottomLeft) = rect

        #find distance between points and get max 
        dist1 = np.linalg.norm(bottomRight-bottomLeft)
        dist2 = np.linalg.norm(topRight-topLeft)

        maxWidth = max(int(dist1),int(dist2))

        dist3 = np.linalg.norm(topRight-bottomRight)
        dist4 = np.linalg.norm(topLeft-bottomLeft)

        maxHeight = max(int(dist3), int(dist4))

        dst = np.array([[0,0],[maxWidth-1, 0],[maxWidth-1, maxHeight-1], [0, maxHeight-1]], dtype="float32")

        M = cv.getPerspectiveTransform(rect, dst)
        warp = cv.warpPerspective(sourceImage, M, (maxWidth, maxHeight))

        destinationImage = cv.cvtColor(warp, cv.COLOR_BGR2GRAY)
        destinationImage = cv.resize(destinationImage,(600,800))

        # sharpen image
        sharpen = cv.GaussianBlur(destinationImage, (0, 0), 3)
        sharpen = cv.addWeighted(destinationImage, 1.5, sharpen, -0.5, 0)
        # apply adaptive threshold to get black and white effect
        thresh = cv.adaptiveThreshold(
            sharpen, 255, cv.ADAPTIVE_THRESH_GAUSSIAN_C, cv.THRESH_BINARY, 21, 15)
         
        cv.imshow("Original Image", sourceImage)
        cv.imshow("Grayscale Image",grayscale)
        cv.imshow("Gaussian Blur Image", bluredImage)
        cv.imshow("Canny Edge Detect", edgeDectect)
        cv.imshow("Contours", sourceImageClone)
        cv.imshow("Scanned.jpg", destinationImage)
        cv.imshow("white effect.jpg", thresh)
        cv.waitKey(0)
        
         # save the transformed image
        cv.imwrite('gray.jpg', grayscale)
        cv.imwrite('gaussianBlured.jpg', bluredImage)
        cv.imwrite('canny.jpg', edgeDectect)
        cv.imwrite('contours.jpg', sourceImageClone)
        cv.imwrite('scanned.jpg',  destinationImage)
        cv.imwrite('white effect.jpg', thresh)
        return thresh


if __name__ == "__main__":
    ap = argparse.ArgumentParser()
    group = ap.add_mutually_exclusive_group(required=True)
    group.add_argument("--images", help="Directory of images to be scanned")
    group.add_argument("--image", help="Path to single image to be scanned")
    ap.add_argument("-help", action='store_true',
        help = "Authors: CQ2018-XLAV-1822")

    args = vars(ap.parse_args())
    im_dir = args["images"]
    im_file_path = args["image"]

    scanner = DocumentScanner()

    valid_formats = [".jpg", ".jpeg", ".jp2", ".png", ".bmp", ".tiff", ".tif"]

    get_ext = lambda f: os.path.splitext(f)[1].lower()

    # Scan single image specified by command line argument --image <IMAGE_PATH>
    if im_file_path:
        scanner.scan(im_file_path)

    # Scan all valid images in directory specified by command line argument --images <IMAGE_DIR>
    else:
        im_files = [f for f in os.listdir(im_dir) if get_ext(f) in valid_formats]
        for im in im_files:
            scanner.scan(im_dir + '/' + im)
