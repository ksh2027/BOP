========================================
BOP: Boolean Operations on Polygons
========================================


Basic Information
=================

BOP aims to develop an efficient and user-friendly library for performing Boolean Operations on Polygons 
in 2D Euclidean space. The project will provide a set of algorithms for performing operations such as 
AND, OR, NOT, XOR, and more on polygons, which will be useful in extracting geometrical properties of graphics.


Problem to Solve
================

Boolean Operations on Polygons (BOP) is a critical problem in computer graphics and geometry processing. 
The goal is to compute the intersection, union, difference, or exclusive-or (XOR) of two or more polygons. 

The physics and mathematics behind BOP involve analyzing the topological relationships between the polygons, 
such as whether they intersect or contain each other. Efficient algorithms for BOP require handling 
complex geometric data structures, such as the arrangement of line segments and vertices.

I choose to implement an efficient and simple algorithm for computing BOP, which is suitable for real-time 
applications and can handle large datasets.

Overall, efficient implementation of BOP is an important problem in computer graphics and geometry processing.
Developing efficient algorithms and numerical methods for BOP is an important research direction in this field.


Prospective Users
=================

The users of this software are primarily researchers, engineers, and practitioners in various fields, 
such as computer-aided design (CAD), graphical information system (GIS), architectural modeling, 
and digital fabrication.

In addition, the software can also be useful for educational purposes, as it provides a practical 
demonstration of geometric algorithms and data structures.


System Architecture
===================

C++: kernel system
  1. a class of polygons data structure
  2. several boolean operations on polygons
  3. a class of BOP algorithm (kernel)
  4. wrap the kernel system by using pybind11

Python: user interface
  1. an user-friendly UI
  2. illustrate the result of BOP
  

API Description
===============

Users can directly import the python module of BOP.
The python script might look like this::

  # Import python module
  from _BOP import BoolOperaPolygon, Polygon as myPolygon

  # create the input polygons
  p1 = myPolygon([[(2.0, 3.0), (8.0, 3.0), (8.0, 7.0), (2.0, 7.0)],[(3.0, 4.0), (5.0, 4.0), (5.0, 6.0), (3.0, 6.0)]])
  p2 = myPolygon([[(4.0, 1.0), (7.0, 1.0), (7.0, 5.0), (4.0, 5.0)]])

  # call BOP functions
  BOP = BoolOperaPolygon([p1, p2], “Union”)
  BOP.create_SweepEvent()
  BOP.subdividing_edge()
  BOP.joining_edge()

  # get result, 
  # resultP:list of contours in result polygon
  # result_INOUT:bool type, if resultP[i] is a inside-outside contour, result_INOUT[i]=TRUE
  return(BOP.resultP, BOP.result_INOUT)  


Directly execute python script (BOP.py)
=============================================

**Read input file mode**
------------------------

.. code-block:: bash

    $  ./BOP.py input/case1.txt

Input file format::

  Polygon 1
  (2.0,3.0),(8.0,3.0),(8.0,7.0),(2.0,7.0)
  (3.0,4.0),(5.0,4.0),(5.0,6.0),(3.0,6.0)

  Polygon 2
  (4.0,1.0),(7.0,1.0),(7.0,5.0),(4.0,5.0)

  Operation XOR

**Interactive mode**
---------------------

.. code-block:: bash

  $  ./BOP.py

Interface:

.. image:: https://i.imgur.com/h1AAuYK.png
  :width: 400px

Sample Result 
==========================

.. image:: https://i.imgur.com/9dqyTpQ.png


Engineering Infrastructure
==========================

``Build:`` make

``Version control:`` git

``Documentation:`` github repository


References
==========

A simple algorithm for Boolean operations on polygons: https://www.sciencedirect.com/science/article/pii/S0965997813000379

A new algorithm for computing Boolean operations on polygons: https://www.sciencedirect.com/science/article/pii/S0098300408002793
