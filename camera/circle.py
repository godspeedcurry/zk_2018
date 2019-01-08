# -*- coding: utf-8 -*-  
""" 
Created on Tue Sep 26 23:15:39 2017 
 
@author: tina 
"""  
import cv2 as cv
import numpy as np  
import matplotlib.pyplot as plt  
  
def circles_image(image):
    dst = cv.pyrMeanShiftFiltering(image, 10, 100)
    cimage = cv.cvtColor(dst, cv.COLOR_BGR2GRAY)
    circles = cv.HoughCircles(cimage, cv.HOUGH_GRADIENT, 1, 20, param1=50, param2=30, minRadius=10, maxRadius=100)
    if circles is None:
    	return
    # print circles
    circles = np.uint16(np.around(circles))
    for i in circles[0, :]:
        cv.circle(image, (i[0], i[1]), i[2], (0, 0, 255), 2)
        cv.circle(image, (i[0], i[1]), 2, (255, 0, 255), 2)
    cv.imshow("圆形", image)
 
def detect(s): 
	src = cv.imread(s)
	cv.imshow("原来", src)
	circles_image(src)
	cv.waitKey(0)
	cv.destroyAllWindows()

for i in range(1,9):
	s = "1 (" + str(i) + ").jpg"
	detect(s)