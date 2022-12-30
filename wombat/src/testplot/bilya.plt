set output "bilya.png"

set size ratio -1

plot "bilya_field.csv" using 1:2:3:4:5 with vectors filled head lw 2 lc palette, \
     "bilya_robot.csv" using 2:3:1 with lines lw 2