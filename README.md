# Synthetic Tracking Algorithm (CPU single threaded version)

## <span style='color:#0abab5'>Requirements</span>

-   A linux machine
-   A c++ compiler which supports c++17 standard
-   Cmake (minimum version: 3.10)

---

The algorithm is implemented based on the paper
[DETECTION OF A FAINT FAST-MOVING NEAR-EARTH ASTEROID USING
THE SYNTHETIC TRACKING TECHNIQUE](https://iopscience.iop.org/article/10.1088/0004-637X/792/1/60/pdf).

This code is a pretty preliminary version so far, still has a lot to do **_IF NECESSARY_**.

The main functionality contains only 3 parts, the functions are exposed in a header file called
"<span style='color:orange'>**_api.h_**</span>"

-   Calculating background given a list of images of the same shape

    The function is called "<span style='color:green'>**_calc_bg_**</span>"
    (default sigma set to 5, you can change this value)

-   Shift & Add a set of images based on the 1st image. The final added image is smaller than the original images.

    The function is called "<span style='color:green'>**_add_all_images_**</span>"

-   Search the best parameter (x0, y0, vx, vy), so that we can add the input images based on these 4 parameters.

    The function is called "<span style='color:green'>**_fit_**</span>"

---

Besides public function api, there are 2 data structure used for "<span style='color:green'>**_fit_**</span>".

-   FittingSpace
-   FittingResult

<span style='color:gold'>**_FitingSpace_**</span> takes 3 parameters: lower bound, upper bound and how many
points to seach.

<span style='color:gold'>**_FittingResult_**</span> is a simple wrapper for the 4 vector (x0, y0, vx, vy)

---

The programe can run in multiple CPUs for now. But be careful, it's easy to run out of CPU resources.
Switching between multi-core and single-core is simple.

---

## <span style='color:#0abab5'>Installation Instructions</span>
1. Run the following command to get the code: <br/>
   ```git clone https://github.com/grewwc/st.git```
2. Go to the "***st***" directory, make a "***build***" subdirectory.
3. Inside the "***build***" directory, run the following command: <br/>
   ``` cmake .. ; make ; make install ```
4. Navigate to the root directory "***st***", the subdirectory "***bin***" is created, the executable "***main***" is in this subdirectory.

---
## <span style='color:#0abab5'>Sample</span>
The fits file is: "***st/data/example.fits***"