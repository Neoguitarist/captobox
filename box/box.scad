$fn=100;
module boitier(){
difference(){
union(){
linear_extrude(height = 3, center = false, convexity = 200)
   import (file = "capto.dxf", layer = "fond2");
   
   linear_extrude(height = 25, center = false, convexity = 200)
   import (file = "capto.dxf", layer = "fond");
    
     linear_extrude(height = 25, center = false, convexity = 200)
   import (file = "capto.dxf", layer = "fix");
}
union(){
translate ([58.1-3,-5,17]) cube([12,15,9]);//usb
translate ([60,32,6.5]) cube([20,11.5,5]);//switch
translate ([68,42,6.5])  rotate ([0,0,90]) cube([20,14,7]); //grove
translate ([25,42,6.5])  rotate ([0,0,90]) cube([20,14,7]); //grove
   translate ([71.5,31.5,14.5]) rotate ([-90,180,-90])  linear_extrude(height = 6, center = false, convexity = 200)
   import (file = "onoff.dxf", layer = "0");
}
}
}

module capot(){
 union(){
translate ([-10 ,0,0])  linear_extrude(height = 3, center = false, convexity = 200)
   import (file = "capto.dxf", layer = "capot");
   
  translate ([-10 ,0,0])  linear_extrude(height = 6, center = false, convexity = 200)
   import (file = "capto.dxf", layer = "capot2");
    
   
}
}
 capot();
//boitier();
