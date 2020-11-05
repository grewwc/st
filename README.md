# Synthetic Tracking Algorithm (CPU single threaded version)


### Requirements
* A linux machine
* A c++ compiler which supports c++17 standard
* Cmake (minimum version: 3.14)
* cfitsio 

--- 
The algorithm is implemented based on the paper
[DETECTION OF A FAINT FAST-MOVING NEAR-EARTH ASTEROID USING
THE SYNTHETIC TRACKING TECHNIQUE](https://iopscience.iop.org/article/10.1088/0004-637X/792/1/60/pdf).

This code is a pretty preliminary version so far, still has a lot to do ***IF NECESSARY***.

The main functionality contains only 3 parts, the functions are exposed in a header file called 
    "<span style='color:orange'>***api.h***</span>"
* Calculating background given a list of images of the same shape
  
    The function is called "<span style='color:green'>***calc_bg***</span>" 
    (default sigma set to 5, you can change this value)

* Shift & Add a set of images based on the 1st image. The final added image is smaller than the original images.

    The function is called "<span style='color:green'>***add_all_images***</span>"
    

* Search the best parameter (x0, y0, vx, vy), so that we can add the input images based on these 4 parameters.

    The function is called "<span style='color:green'>***Fit***</span>"

--- 

Besides public function api, there are 2 data structure used for "<span style='color:green'>***Fit***</span>".
* FittingSpace 
* FittingResult

<span style='color:gold'>***FitingSpace***</span> takes 3 parameters: lower bound, upper bound and how many 
    points to seach.

<span style='color:gold'>***FittingResult***</span> is a simple wrapper for the 4 vector (x0, y0, vx, vy)

--- 

The programe can run in multiple CPUs for now. But be careful, it's easy to run out of CPU resources. 
Switching between multi-core and single-core is simple.

