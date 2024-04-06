# C++ & Python Mandelbulb generation and rendering

## Author: [Najib El khadir](https://github.com/NajibXY)
## French Version

## 1. Motivations

<figure text-align="right">
  <img align="right" src="https://github.com/NajibXY/Mandelbulb-with-Ray-marching/blob/master/assets/mandelbulb.gif" width="400">
</figure>

Who doesn't love fractals ? </br>
As an exercise to learn more about the maths behind generating and rendering fractals. I worked on this example of Mandelbulb generation. Mandelbulb is a 3D fractal emerging from the experimentations to find a 3D canonical Mandelbrot set.
<p align="center">
  <img src="https://upload.wikimedia.org/wikipedia/commons/2/21/Mandel_zoom_00_mandelbrot_set.jpg" width="200">
  </br>
  <em>Mandelbrot set</em>
</p>

</br> </br>
</br>

## 2. Technologies Used

- C++14, OpenMP
- Python, Numpy, Matplotlib, FFMPEG, Conda (for my personal environment)

## 3. References

- [Distance Estimated 3D Fractals](http://blog.hvidtfeldts.net/index.php/2011/06/distance-estimated-3d-fractals-part-i/)
- [Understanding the View Matrix](https://www.3dgep.com/understanding-the-view-matrix/)
- [The Coding train's Mandelbulb Challenge](https://www.youtube.com/watch?v=NJCiUVGiNyA&ab_channel=TheCodingTrain)

## 4. Data generation

- The data points of the mandelbulb are pretty easy to generate.
- But in order to be able to render the data, a ray marching method has been implemented, that involves lights and distances calculation according to the movement of a virtual camera. This requires to do some parallelization with OpenMP and a lot of three-dimensional vector algebra to achieve an acceptable execution time.
- It takes about 30 minutes to generate the data with a base exponent equal to 8.

### Compiling the generate_data.cpp file

- Requires C++ and gcc/g++ installed.
- Just run :
  > g++ -Wall -Wextra -fopenmp generate_data.cpp -o generate_data.exe
- Running the .exe will generate the data in the /data/ folder.

## 5. Displaying the rendering data

### Set up your environment

- You'll need to set up a Python environment (in my case conda), to display the generated data with Matplotlib.
- For installing the requirements with conda you can just run :
  > conda create --name `<your_env_name>` --file requirements.txt
- Or if you are using pip :
  > pip install -r requirements.txt
- Then you will be able to run the script.

### Run the python script

> python .\generate_video.py
- This will generate all the rendered PNGs from the previously generated data. These images are saved in the /images/ folder.
- Additionally, the script compiles these images into a video using FFMPEG and saves it in the same folder as the script.
</br>

### Examples of generated images and video 

+ Example of a generated video :
   </br>
   </br>
  <img src="https://github.com/NajibXY/Mandelbulb-with-Ray-marching/blob/master/assets/mandelbulb.gif" width="400">
  </br>
+ Examples of generated & randomly colored images :
  </br>
  </br>
  <img src="https://github.com/NajibXY/Mandelbulb-with-Ray-marching/blob/master/assets/example1.png" width="350">
  </br>
  <img src="https://github.com/NajibXY/Mandelbulb-with-Ray-marching/blob/master/assets/example2.png" width="350"> 

</br></br>

## 5. Possible Improvements

- Faster data generation.
- Using other formulas for the data generation.
- Using other methods for rendering.
- Utilizing an efficient C++ library for visualization alongside the data.
- Providing the ability to tune generation parameters.
- [...]
