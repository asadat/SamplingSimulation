

echo "set terminal pdf size 40cm, 15cm" > gp
echo "set output 'graph.pdf'" >>gp
echo "set multiplot layout 2,5" >> gp
for((i=2; i<=20; i=i+2))
do
echo "set key left">>gp 
echo "set yrange [1000:8100]">>gp
echo "set xrange [8:52]">>gp
echo "set xtics font \"Time-Roman, 7\"">>gp
echo "set ytics font \"Time-Roman, 7\"">>gp
#echo "set xlabel \"% of the environment that is interesting\" font \",8\"">>gp
#echo "set ylabel \"Length of the coverage path (m)\" font \",8\"">>gp
echo "set pointsize 1">>gp
echo "plot 'statistics' using 2:(\$1==0&&\$3==$i?\$4:1/0):5 with yerrorbars ls 3 title 'Breadth-First', \\">>gp
echo       "'statistics' using 2:(\$1==1&&\$3==$i?\$4:1/0):5 with yerrorbars ls 25 title 'Depth-First', \\">>gp
echo       "'statistics' using 2:(\$1==2&&\$3==$i?\$4:1/0):5 with yerrorbars ls 7 title 'Shortcut heuristic', \\">>gp
echo       "'statistics' using 2:(\$1==3&&\$3==$i?\$4:1/0):5 with yerrorbars ls 9 title 'Lawnmower'">>gp
done
echo "unset multiplot" >> gp
#echo "replot">>gp
gnuplot gp
evince graph.pdf

       
       
       
       
