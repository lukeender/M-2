# M-2

This is a program I wrote to demonstrate a simple scientific programming task and later extended to show performance gains that can come from using pybind11 to allow easy interfacing between C++ classes and Python. For more information about the task itself, please open the M2.ipynb and read the first few markdown cells.

## Dependencies:
* Numpy
* Scipy
* Jupyter
* [pybind11](https://pybind11.readthedocs.io/en/stable/) (tested with version 2.11.1)
* [OpenCV](https://opencv.org/) (tested with 4.8.0)
* [Ceres](http://ceres-solver.org/) (tested with 2.2.0)

## Installation
* Ensure all prerequisites are installed
* Clone the repository into the folder of your choice
* In the terminal with the chosen folder as your working directory run the following commands:

        mkdir build

        cd build

        cmake ..

        sudo make

to build the C++ module.

## Running

* Open M2.ipynb and run all cells
