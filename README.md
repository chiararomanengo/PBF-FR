# PBF-FR

PBF-FR is a method for the automatic recognition of façades within complex urban scenes represented as point clouds. The algorithm employs an enhanced partitioning strategy that extends beyond strict building footprints by incorporating surrounding buffer zones, allowing for a more complete capture of façade geometry, particularly in dense urban contexts. This is combined with a primitive recognition stage based on the Hough transform, enabling the detection of façade structures. The main steps of the method are described in the paper "PBF-FR: Partitioning Beyond Footprints for Fac ̧ade Recognition in Urban Point Clouds".

The first submodule ...

The second submodule is developed in MATLAB and it is detailed in Section 4.2.

## Clone

The repository includes some submodules necessary to make PBF-FR work. Please, clone it recursively:

git clone --recursive https://github.com/chiararomanengo/PBF-FR/
