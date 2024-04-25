set term png size 3840, 3840
set output 'walk_of_power.png'

set key autotitle columnhead

set size square

unset border
unset xtics
unset ytics

plot for [i=0:17889] '../data/building_coordinates/coordinates_'.i.'.txt' w l lc 'black' t '', \
	"../data/city_area.txt" u 1:2:3 with circles lw 2 lc 'black' t '', \
	"../results/metropolis.txt" u 1:2 w l lc 1 t '', \
	"../results/powerup.txt" u 1:2 w l lc 2 t '', \
	"../data/provisioning.txt" u 1:2 with points ps 4 pt 7 lc 'green' t ''
