**Author:** Austin Brennan  
**Date:** 10/24/2016  
**Course:** [CPSC 8170 - Physically Based Animation (Dr. Donald House)](https://people.cs.clemson.edu/~dhouse/courses/817/)    
**Institution:** Clemson University  
**Repository:** https://github.com/awbrenn/squshy-mesh

# Squishy Mesh
A mesh that deforms in a springy like fashion rendered in OpenGL.

## Assignment
Now that you can simulate particles flying freely through space, it 
is time to link them together so that you can start building geometry. 
For this project, you are to follow the work of Haumann and Parent in 
building springy structures from simple behavioral "actors". Particles, 
which have mass, are connected to each other by massless springy-links, 
which in turn can be connected to form triangles. The triangles can be 
connected along common edges to form polyhedral structures. The common 
edges that the triangles share can have torsional springs associated 
with them. The springy-links between particles provide spring forces 
that tend to keep the particles at a specific rest-distance from each 
other. The torsional springs between triangles provide spring forces 
that keep the triangles at a specific rest-angle with respect to each 
other. Both the link and torsional springs should have some damping 
associated with them so that vibrations tend to die out over time.

Your assignment is to put together some experiments using these ideas 
to build springy objects that can "live" in a simulated virtual world. 
Ideas are jello cubes that can be tossed around, flags that can blow in 
the wind, slinky toys, butterflys, etc.

Finally, once you have done a nice springy object simulation, you may 
notice that the motion sometimes doesn't look completely realistic, and 
it may be hard to keep the simulation stable. Both of these 
characteristics may be due to the fact that you have been using a simple 
numerical integration algorithm. I would like you to improve your 
simulation  program by moving to the fourth-order Runga-Kutta 
integration algorithm, but without changing the geometry or dynamics. 
Details of this algorithm can be found in the notes, and in the book 
Numerical Recipes, by Press et al. You should note some real improvement
in performance, both in terms of stability and quality of motion. 

##Features
* Obj loader for arbitrary springy mesh
* Ability to interact with the object

## Work In Progress
* Torsional forces still aren't working correctly
* Stability overall could be improved

## Compilation
```
$> cmake CMakeLists.txt
$> make
```


## Usage
```
$> SquishyMesh parameters
```

## Interaction
```
w/a/s/d - move the ball around
g       - Toggle grid
esc     - Quit the program
q       - Quit the program
```

## Acknowledgements
Vector Class - Donald House

Matrix Class - Donald House

Camera Class - Christopher Root, Donald House, Yujie Shu
