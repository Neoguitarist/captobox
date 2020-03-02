$fn=100;
difference(){
    union(){
        sphere(d=50,center=true);
        translate ([0,0,-10/2]) cube([30,50,8],center=true);
    }
    
    union(){
       translate ([0,0,-80/2-4]) cube([100,100,80],center=true);
       translate ([0,0,150/2-2]) cube([100,28.5,150],center=true);
    }
}