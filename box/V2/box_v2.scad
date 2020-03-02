// BOITIER ET CAPOT POUR LA CAPTOBOX

// placer captobox.dxf dans le même dossier ou indiquer le chemin d'accés au niveau des lignes : file = "chemin/captobox.dxf"

// version 2 du 8 novembre 2016
// Auteur: AFPD / Yann Le Moigne & Julien Rat

// paramétres :

$fn=400; // résolution

ep_fond=3; // épaisseur du fond de la boite
hauteur=25; // hauteur de la boite

haut_capt= 6;// largeur des ouvertures pour les capteurs, longueur définie dans le dxf
z_capt=6; // hauteur des ouvertures pour les capteurs par rapport au fond de la boite

haut_inter= 5;// largeur des ouvertures pour l'interrupteur on/off, longueur définie dans le dxf
z_inter=6; // hauteur de l'ouverture pour l'interrupteur on/off  par rapport au fond de la boite

haut_usb= 8;// largeur de l'ouverture pour le passage d'un câble USB pour la recharge, longueur définie dans le dxf
z_usb= hauteur-haut_usb; // hauteur de l'ouverture pour le cable de charge  par rapport au haut de la boite

prof_percage= 15;// profondeur des percages pour vis M3
z_percage= hauteur-prof_percage; // profondeur des perçages pour les vis de fixation par rapport au haut de la boite

haut_calle_pcb=5; // hauteur de la calle pour maintenir le PCB

ep_print= 1; // épaisseur du support d'impression

// modules des différentes parties

    module box(){ // juste la boite sans les accés pour connecteur
    
difference(){

linear_extrude(height = hauteur, convexity = 4)    import (file = "captobox.dxf", layer = "exterieur");
translate ([0 ,0,ep_fond]) linear_extrude(height = hauteur, convexity =4)    import (file = "captobox.dxf", layer = "interieur"); 
                         }
                                }

    module cut(){ // les découpes de la boite
        
        translate ([0 ,0,z_capt]) linear_extrude(height = haut_capt, convexity =4)    import (file = "captobox.dxf", layer = "ouv_capt");      // découpe des accés capteurs en connectique groove

        translate ([0 ,0,z_inter]) linear_extrude(height = haut_inter, convexity =4)    import (file = "captobox.dxf", layer = "ouv_inter");      // découpe du passage de l'interrupteur 
        
        translate ([0 ,0,z_usb]) linear_extrude(height = haut_usb, convexity =4)    import (file = "captobox.dxf", layer = "ouv_usb");      // découpe du passage du cable usb de charge
        
        translate ([0 ,0,z_percage]) linear_extrude(height = prof_percage+0.1, convexity =4)    import (file = "captobox.dxf", layer = "percages");      // découpe des percages pour les vis de fermeture

        translate ([0 ,0,-0.25]) linear_extrude(height = ep_fond+0.5, convexity =4)    import (file = "captobox.dxf", layer = "ouv_bp");      // découpe pour le bouton poussoir sous la boite
    }
    
    module boitier() { // boite avec les découpes et les rajouts (calle pcb, logo, etc)
        
      difference(){
              box();
              cut();
                                 }
        translate ([0 ,0,ep_fond]) linear_extrude(height = haut_calle_pcb, convexity =4)    import (file = "captobox.dxf", layer = "cales");      // rajout d'une calle pour maintenir le PCB
                                 
                                 
                                         }  
        
        
module capot(){
 
 difference(){   
        linear_extrude(height = ep_fond, convexity = 4)    import (file = "captobox.dxf", layer = "exterieur");
       linear_extrude(height = prof_percage+0.1, convexity =4)    import (file = "captobox.dxf", layer = "percages");      // découpe des percages pour les vis de fermeture 
 }
                                    }
                                    
module support_print () { // rajout d'éléments dans les coins pour favoriser l'accroche au plateau et éviter les décollement de pièces, valable pour le boitier et le capot
 linear_extrude(height = ep_print, convexity = 4)    import (file = "captobox.dxf", layer = "support_imp");
                                                        }
                                    
 // génération des différents éléments, commenter (rajout des //) ou non (enlever les //) suivant les besoins
                                    
boitier();
                                    
//translate ([0 ,0,hauteur + 5]) // pour visualisation d'ensemble rajouter // en début de ligne pour l'ignorer
 //capot();

    
support_print();
    
