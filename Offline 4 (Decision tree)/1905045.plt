set terminal png
set output "1905045_accuracy vs training set size.png"
set title "Accuracy vs. Training Set Size"

set xlabel "Training set size"
set ylabel "Proportion correct on test set"

set xrange[0:100]
set yrange[0.75:1]

set key outside center below
set datafile missing "-nan"
plot "1905045_plot.dat" with linespoints