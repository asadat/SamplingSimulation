echo "set terminal pdf size 40cm, 15cm" > gp
echo "set output 'graph.pdf'" >>gp
echo "set multiplot layout 2,4" >> gp
for((i=10; i<50; i=i+5))
do
echo "set key left">>gp 

if [ $i -gt 10 ]; then
echo "unset key">>gp
fi

#echo "set lmargin 5">>gp 
echo "set yrange [1:8.1]">>gp
echo "set xrange [0:10.5]">>gp
echo "set format y \"%g km\"">>gp
#echo "unset label">>gp
#echo "set label \"%$i Interesting\" at graph 0.7,0.7 font \",8\"">>gp
echo "set title \"%$i Interesting\" font \"Time Bold,8\"">>gp
echo "set xtics 0,1,10 font \"Time-Roman, 7\"">>gp
echo "set ytics 1, 2, 8 font \"Time-Roman, 7\"">>gp

if [ $i -eq 10 -o $i -eq 30 ]; then
echo "set ylabel \"Coverage path length\" font \",8\"">>gp
echo "set lmargin 10">>gp 
else
echo "unset ylabel">>gp
echo "set lmargin 4">>gp 
fi 

echo "set xlabel \"#Patches\" font \",8\"">>gp

echo "set pointsize 1">>gp
echo "plot 'statistics' using 3:(\$1==0&&\$2==$i?(\$4/1000):1/0):(\$5/1000) with yerrorbars ls 3 title 'Breadth-First', \\">>gp
echo       "'statistics' using 3:(\$1==1&&\$2==$i?(\$4/1000):1/0):(\$5/1000) with yerrorbars ls 25 title 'Depth-First', \\">>gp
echo       "'statistics' using 3:(\$1==2&&\$2==$i?(\$4/1000):1/0):(\$5/1000) with yerrorbars ls 7 title 'Shortcut heuristic', \\">>gp
echo       "'statistics' using 3:(\$1==3&&\$2==$i?(\$4/1000):1/0):(\$5/1000) with yerrorbars ls 9 title 'Lawnmower'">>gp
done
echo "unset multiplot" >> gp
#echo "replot">>gp
gnuplot gp
evince graph.pdf

       
       
       
       
