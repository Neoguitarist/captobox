$fn=100;
linear_extrude(height = 3, center = false, convexity = 200)
   import (file = "dust_sensor.dxf", layer = "fond");
   linear_extrude(height = 26, center = false, convexity = 200)
   import (file = "dust_sensor.dxf", layer = "capot2");