
** Revision

2002/09/05

(previous version 002/05/06;
 since then, corrected program to work on ATI graphics cards;
 also, added programs FilterPM and Filtermesh to allow extraction of meshes.)


** Source models

Thanks to Stanford University and Cyberware for the models.

The buddha and dragon models were scanned at Stanford University, as part of
 Brian Curless's PhD thesis work.

Both of these models suffered from topological noise --- tiny nearly
 invisible handles that altered the surface topology.
The original buddha had genus 106.  (Its correct genus is 6.)
The original dragon had genus 60.   (Its correct genus is 1.)

Both of these models were processed with an Isosurface Topology
 Simplification algorithm to obtain the correct genus, as described in:

  Z. Wood, H. Hoppe, M. Desbrun, P. Schroder. 
  Isosurface topology simplification.
  Microsoft Research MSR-TR-2002-28.
  (submitted for publication).


** Extraction of meshes with texture-coordinates

For either model, you can extract parametrized triangle meshes of any
 desired complexity from the progressive mesh.
A sample command is:

  FilterPM.exe buddhaf.tmpm.trunc.pm -nfaces 5000 -outmesh >buddhaf.nf5000.m

The ascii format of the resulting mesh file is rather simple.
The mesh consists of a set of vertices and faces (numbered starting with 1).
A "Corner" is a (vertex,face) tuple and is used to indicate attributes
 near chart boundaries.

If you don't want to deal with the added complexity of corners, run the
command:

  Filtermesh.exe buddhaf.nf5000.m -splitcorners >buddhaf.nf5000.split.m

This splits all vertices with multiple corner attributes into separate
 vertices.  The mesh charts are therefore no longer connected together,
 since each chart has a surface boundary.

To see the overall process, just run the script:

  extract_and_render_buddha_5000faces.bat


** Interactive Demo

This demo shows results of parametrizing triangle meshes to support texture
mapping at various levels of detail.  A texture atlas is constructed that
can be re-used on coarse and fine geometries.  Examples include the Stanford
dragon and buddha models.

The pre-process is described in the paper:

Texture mapping progressive meshes.
P. Sander, J. Snyder, S. Gortler, H. Hoppe. 
Computer Graphics (SIGGRAPH 2001 Proceedings), pages 409-416.

(see http://research.microsoft.com/~hoppe/ )

Hardware requirements:
  All demos except *_preshaded.bat attempt to perfrom normal-mapping.
  For normal-mapping, the program first looks for the NVIDIA
   register-combiner OpenGL extension.
  If that extension is not present, it then looks for the texture_dot3
   OpenGL extension.
  If neither extension is present, the program just displays the normal-map
   as a regular (unlit) RGB texture.


_demo_buddhaf.bat
  Show the texture-mapped progressive mesh of the buddha.
   
_demo_tyra.bat
  Show the texture-mapped progressive mesh of the tyra.

_demo_buddhaf_compare.bat
  Compare a texture-mapped progressive mesh (left) with a traditional
   Gouraud-shaded progressive mesh (right).

_demo_dragonf_compare.bat
  Compare a texture-mapped progressive mesh (left) with a traditional
   Gouraud-shaded progressive mesh (right).

_demo_bunny_preshaded.bat
  Show the texture-mapped progressive mesh of the bunny,
   where the texture image contains a pre-shaded version of the model.
  This demo works well on PC's that cannot do efficient normal-mapping.
   

Look at the files *.bmp to see the texture atlases.

Sorry, the code to construct the progressive meshes and their atlases is
not available.
