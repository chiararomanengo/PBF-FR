# PBF-FR

PBF-FR is a method for the automatic recognition of façades within complex urban scenes represented as point clouds. The algorithm employs an enhanced partitioning strategy that extends beyond strict building footprints by incorporating surrounding buffer zones, allowing for a more complete capture of façade geometry, particularly in dense urban contexts. This is combined with a primitive recognition stage based on the Hough transform, enabling the detection of façade structures. The main steps of the method are described in the paper "PBF-FR: Partitioning Beyond Footprints for Façade Recognition in Urban Point Clouds".

The first submodule ...

The second submodule https://github.com/chiararomanengo/facades_recognition.git is developed in MATLAB and it is detailed in Section 4.2.

## Clone
The repository includes the submodules necessary to make the code work. Please, clone it recursively:

- Clone recursively the repository into your local machine:
```
git clone --recursive https://github.com/DanielaCabiddu/PBF-FR.git
```

In the following, assume the **${ROOT}** folder to be the one whee this *README* lies.

## Content of the repository
- `src`: source code 
- `scripts`: scripts necessary to build C++ code and replicate experiments described in the paper

## Build the source code
To build the C++ source code, use the following pipeline:

```
cd ${ROOT}
./scripts/build_all.ps1
```

Binaries will be available in the **${ROOT}/bin** folder

## Paper Replicability

Scripts in the **${ROOT}/scripts** folder allow the replicability of the experimental results descried in the paper. 

Use the **${ROOT}/scripts/run_H3D.ps1** script to replicate the experiments using H3D dataset. First, the script will download the input data (too large to be included into the repository). Then, the partitioning approach will be executed. An OpenGL will will show Figure 4(c). By closing the window, the partitioning is finalized and the result will be saved in **${ROOT}/PBF-FR-H3D-results**. The facade recognition approach is then automatically started, and the result will be available in **${ROOT}/src/recognition/output** folder. Finally, the computation of the evaluation metrices is automatically run and results are shown in the command line shell.

## Author & Copyright

Chiara Romanengo (CNR-IMATI). Contact Email: chiara.romanengo@cnr.it

Daniela Cabiddu (CNR-IMATI). Contact Email: daniela.cabiddu@cnr.it

## Citing us
```bibtex
@article{pbf-fr,
  author       = {Chiara Romanengo, Daniela Cabiddu, Michela Mortara},
  title        = {{PBF-FR: Partitioning Beyond Footprints for Façade Recognition in Urban Point Clouds}},
  year         = {2025 (to appear)},
  journal      = {Computers \& Graphics - Special Issue 3DOR 2025}
}
```

## Acknowledgment

This work was partially supported by the project RAISE – Robotics and AI for Socio-economic Empowerment (project code ECS00000035), funded by the European Union – NextGenerationEU and by the Italian Ministry of University and Research (MUR) under the National Recovery and Resilience Plan (NRRP), Mission 4, Component 2, Investment 1.5, Creation and strengthening of innovation ecosystems for sustainability.

The authors also acknowledge support from the project PON METRO – ASSE 1 – Agenda Digitale, titled Urban Intelligence Science Hub (UISH) for City Network (CUP: B51B21000430001), funded under the Italian National Operational Program for Metropolitan Cities 2014–2020.
