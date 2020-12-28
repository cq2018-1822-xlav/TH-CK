import numpy as np 
import cv2 as cv


def canny(image, sigma=0.33):
	# Compute the median of the single channel pixel intensities
	median = np.median(image)

	lower = 0
	upper = 0

	if median > 191:
		lower = int(max(0, (1.0 - 2 * sigma) * (255 - median)))
		upper = int(max(85, (1.0 + 2 * sigma) * (255 - median)))
	elif median > 127:
		lower = int(max(0, (1.0 - sigma) * (255 - median)))
		upper = int(min(255, (1.0 + sigma) * (255 - median)))
	elif median < 63:
		lower = int(max(0, (1.0 - 2 * sigma) * median))
		upper = int(max(85, (1.0 + 2 * sigma) * median))
	else:
		lower = int(max(0, (1.0 - sigma) * median))
		upper = int(min(255, (1.0 + sigma) * median))

	edge = cv.Canny(image, lower, upper)
	return edge


def write(image, directory):
	cv.imwrite(directory, image)