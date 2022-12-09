set output "shooter_spinup_pid.png"
set multiplot layout 2,1 title "Shooter Spinup"

set title "Speed (rpm)"
set yrange[0:4500]
plot "shooter_spinup_pid.csv" using 1:2 w l lc "red", "" using 1:3 w l lc "green"

set title "Current (A)"
set yrange[0:100]
plot "shooter_spinup_pid.csv" using 1:5 w l;