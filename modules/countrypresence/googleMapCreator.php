<?php

/* 
 *  An attempt at automatic map generation using:
 *  - ogr2ogr - which will combine shape files
 *  - Google map creator - which will create the google maps from a single shape file.
 *
 *  This script returns JavaScript, therefore it can be embedded on ANY page!
 */

// We take as arguments to this script the two letter country codes of countries
// we wish to include within out map.  This means that we can easily expand this
// to include any world countries (not just African ones), althoughh this may require
// three letters!

// Following temp filename is used in the configuration section, and therefore needs to come
// before it.
$tmp_file_name = tempnam("/tmp","googlemaps");
$tmp_file_name = substr($tmp_file_name,5);
$tmp_file = "/tmp/".$tmp_file_name;

// Configuration -----------------------------------------------------------------------
$earth_shape_file    = dirname(__FILE__)."/country-earth.shp";
$maps_extensions  = array("dbf","shp","shx","prj","xml"); // Not too sure which of these IS needed.
$ogr2ogr_path    = "/usr/share/fwtools/bin_safe/ogr2ogr";
$gmap_creator_path  = "/var/www/html/sites/all/gmap/gmapcreator.jar";
if (!isset($_GET['apikey'])||$_GET['apikey']==""){
  ?>alert('EDIT GMap colourer: You must supply an API key');<?php
  exit;
}
$google_api_key    = $_GET['apikey'];
$gmap_settings_xml  = "<gmapcreator>
<apikey>".$google_api_key."</apikey>
<apiversion>2</apiversion>
<maptitle>Test Map</maptitle>
<shpfilename>".$tmp_file_name.".shp</shpfilename>
<outlinesenabled>true</outlinesenabled>
<maxzoomlevel>5</maxzoomlevel>
<datafield>IDR_ID</datafield>
<limitarea>false</limitarea>
<mapboundswgs84>
<minlat>-55.72056961021754</minlat>
<minlon>-180.0</minlon>
<maxlat>83.62303161612134</maxlat>
<maxlon>179.99981689453125</maxlon>
</mapboundswgs84>
<mapviewenvelope>
<minx>-2.1397475258323826E7</minx>
<maxx>2.139745487511628E7</maxx>
<miny>-9944658.00979416</miny>
<maxy>2.0852293210672483E7</maxy>
</mapviewenvelope>
<colourscale>
<isdiscrete>true</isdiscrete>
<colourThresholds>
<colourThreshold>
<colour>ff0000ff</colour>
<threshold>0.0</threshold>
<description>First Colour</description>
</colourThreshold>
</colourThresholds>
</colourscale>
</gmapcreator>
";
if (!isset($_GET['countries'])){
  
  // The countries have not been sent! Error.
  ?>alert('EDIT GMap colourer: You haven\'t supplied any countries');<?php
  exit;
}

// Get the countries.
$countries = split(",",$_GET['countries']);

// Extract each country from the "Earth shape" and add it to the previous.
// If this is the first country, there is no adding!)
for($i=0;$i<count($countries);$i++){
  $exec = $ogr2ogr_path . ' ' . $tmp_file . $i . '.shp -fid ' . ($countries[$i]-1) . ' ' . $earth_shape_file;
  exec($exec);
  if ($i==0){
    $exec = $ogr2ogr_path . ' ' . $tmp_file . '.shp ' . $tmp_file . $i . '.shp';
    exec($exec);
  }
  else{
    $exec = $ogr2ogr_path . ' -update -append ' . $tmp_file.'.shp ' . $tmp_file . $i . '.shp -nln ' . $tmp_file_name;
    exec($exec);
  }
}
// Once this is done, we can have a go at creating the GoogleMap from the command line.
// Firstly, create the settings file
file_put_contents($tmp_file.".xml",$gmap_settings_xml);
// Next copy the projection file
copy($maps_directory."projection.prj",$tmp_file.".prj");
// Mv the temp files to the semi temp folder, don't forget to delete them afterwards!
foreach($maps_extensions as $map_extension){
  @copy($tmp_file.".".$map_extension,"/var/www/html/sites/all/gmap".$tmp_file.".".$map_extension);
}
$exec = "cd /var/www/html/sites/all/gmap/tmp && /usr/bin/java -jar ".$gmap_creator_path." -dfile=".$tmp_file_name.".xml";
exec($exec);
?>
    var centreLat=3.477775573709623;
    var centreLon=10.218469619750977;
    var n_buttonText="Map"; //Text that shows up on the button for the custom layer (n=normal, s=sat)
    var s_buttonText="Satellite";
    var mapBounds=new GLatLngBounds(new GLatLng(-90,-180),new GLatLng(90,180));
    var map; //the GMap2 itself
    var opacity=0.75;


    function customGetTileURL(a,b) {
      //converts tile x,y into keyhole string
      if (b>5) { return "/sites/all/gmap/tmp/<?php echo $tmp_file_name; ?>-tiles/blank-tile.png"; };

      var c=Math.pow(2,b);
      var x=360/c*a.x-180;
      var y=180-360/c*a.y;
      var x2=x+360/c;
      var y2=y-360/c;
      var lon=x; //Math.toRadians(x); //would be lon=x+lon0, but lon0=0 degrees
      var lat=(2.0*Math.atan(Math.exp(y/180*Math.PI))-Math.PI/2.0)*180/Math.PI; //in degrees
      var lon2=x2;
      var lat2=(2.0*Math.atan(Math.exp(y2/180*Math.PI))-Math.PI/2.0)*180/Math.PI; //in degrees
      var tileBounds=new GLatLngBounds(new GLatLng(lat2,lon),new GLatLng(lat,lon2));

      if (!tileBounds.intersects(mapBounds)) { return "/sites/all/gmap/tmp/<?php echo $tmp_file_name; ?>-tiles/blank-tile.png"; };
        var d=a.x;
        var e=a.y;
        var f="t";
        for(var g=0;g<b;g++){
            c=c/2;
            if(e<c){
                if(d<c){f+="q"}
                else{f+="r";d-=c}
            }
            else{
                if(d<c){f+="t";e-=c}
                else{f+="s";d-=c;e-=c}
            }
        }
        return "/sites/all/gmap/tmp/<?php echo $tmp_file_name; ?>-tiles/"+f+".png"
    }

    function changeOpacity(op) {
  //this works as long as there are at least two map types
        var current=map.getCurrentMapType();
        if (current==map.getMapTypes()[0])
          map.setMapType(map.getMapTypes()[1]);
  else
    map.setMapType(map.getMapTypes()[0]);
        opacity=op;
        map.setMapType(current); //was map.getMapTypes()[1]
    }


    function getWindowHeight() {
        if (window.self&&self.innerHeight) {
            return self.innerHeight;
        }
        if (document.documentElement&&document.documentElement.clientHeight) {
            return document.documentElement.clientHeight;
        }
        return 0;
    }

    function resizeMapDiv() {
        //Resize the height of the div containing the map.
        //Do not call any map methods here as the resize is called before the map is created.
      var d=document.getElementById("map");
        var offsetTop=0;
        for (var elem=d; elem!=null; elem=elem.offsetParent) {
            offsetTop+=elem.offsetTop;
        }
        var height=getWindowHeight()-offsetTop-16;
        if (height>=0) {
            d.style.height=height+"px";
        }
    }


    function load() {
      if (GBrowserIsCompatible()) {
        resizeMapDiv();
        var copyright = new GCopyright(1,
                              new GLatLngBounds(new GLatLng(-90, -180),
                                                new GLatLng(90, 180)),
                              0,
                              "<a href=\"http://www.casa.ucl.ac.uk\">CASA</a>");
        var copyrightCollection = new GCopyrightCollection("GMapCreator");
        copyrightCollection.addCopyright(copyright);

        //create a custom G_NORMAL_MAP layer
        var n_tileLayers = [ G_NORMAL_MAP.getTileLayers()[0], new GTileLayer(copyrightCollection , 0, 17)];
        n_tileLayers[1].getTileUrl = customGetTileURL;
        n_tileLayers[1].isPng = function() { return false; };
        n_tileLayers[1].getOpacity = function() { return opacity; };
        var n_customMap = new GMapType(n_tileLayers, new GMercatorProjection(6), n_buttonText,
            {maxResolution:5, minResolution:0, errorMessage:"Data not available"});

        //create a custom G_SATELLITE_MAP layer
        var s_tileLayers = [ G_SATELLITE_MAP.getTileLayers()[0], new GTileLayer(copyrightCollection , 0, 17)];
        s_tileLayers[1].getTileUrl = customGetTileURL;
        s_tileLayers[1].isPng = function() { return false; };
        s_tileLayers[1].getOpacity = function() { return opacity; };
        var s_customMap = new GMapType(s_tileLayers, new GMercatorProjection(6), s_buttonText,
            {maxResolution:5, minResolution:0, errorMessage:"Data not available"});

        //Now create the custom map. Would normally be G_NORMAL_MAP,G_SATELLITE_MAP,G_HYBRID_MAP
        map = new GMap2(document.getElementById("map"),{mapTypes:[n_customMap,s_customMap]});
        map.addControl(new GLargeMapControl());
        map.addControl(new GMapTypeControl());
        map.enableContinuousZoom();
        map.setCenter(new GLatLng(centreLat, centreLon), 2, n_customMap);
        map.enableDoubleClickZoom();
        /*
            var point = new GLatLng(0,0);            
            var marker = new GMarker(point);
            GEvent.addListener(marker, "click", function() {
              marker.openInfoWindowHtml("More stuff");
            });
            map.addOverlay(marker);
        */
      }
    }