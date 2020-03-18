$fn=100;

module fond(){
difference(){
union(){
   linear_extrude(height = 3, center = false, convexity = 200)
   import (file = "MOD-1023.dxf", layer = "fond");
   linear_extrude(height = 10, center = false, convexity = 200)
   import (file = "MOD-1023.dxf", layer = "capot2");
   linear_extrude(height = 6, center = false, convexity = 200)
   import (file = "MOD-1023.dxf", layer = "fix");
}
translate([23/2,-5,10]) cube([10,16,4],center=true);
}
}

module capot(){
difference(){
 translate([40,0,0])  linear_extrude(height = 3, center = false, convexity = 200)
   import (file = "MOD-1023.dxf", layer = "fond");
    union(){
translate([40+23/2,27,0])cylinder(r=6.5,h=60, center=true);
       translate([40+23/2,9,0]) cube([4,4,33],center=true);
        
    }
}
}

fond();
capot();