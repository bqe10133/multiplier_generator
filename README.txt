
[1] The generator is used to write the document
[A comparison of 32bx32b Dadda multiplier and “pre-sum
before Dadda tree” multiplier.]
file name: 20221028_presum_8bcut_12.pdf

[2] 20231202_multiplier_generator.pdf
the document of the generator.

[3] The generator has the five files.
(version 1.1 July 24,2023)
 script_multiplier
 tree.cpp
 cla4b_template
 layout_template
 multiplier_tb_template

[4] command

Put the five files
(script_multiplier,tree.cpp,cla4b_template,layout_template,multiplier_tb_template)
into a new directory(any name is OK). Under the directory,

% chmod 755 script_multiplier

4bx4b
% ./script_multiplier 4

8bx8b
% ./script_multiplier 8

16bx16b
% ./script_multiplier 16

32bx32b
% ./script_multiplier 32


