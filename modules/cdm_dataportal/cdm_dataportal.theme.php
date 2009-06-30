<?php
// $Id$

/*
 * Copyright (C) 2007 EDIT
 * European Distributed Institute of Taxonomy
 * http://www.e-taxonomy.eu
 */

function _add_js_thickbox(){
  // ---- jQuery thickbox:
  /*
  * bug: compat-1.0.js && thickbox.js line 237 .trigger("unload")
  * -> event is not triggered because of problems with compat-1.0.js'
  * see INSTALL.txt
  *
  */
  drupal_add_js(drupal_get_path('module', 'cdm_dataportal').'/js/jquery.imagetool.min.js');
  drupal_add_js(drupal_get_path('module', 'cdm_dataportal').'/js/thickbox.js');
  drupal_add_css(drupal_get_path('module', 'cdm_dataportal').'/js/cdm_thickbox.css');
}

/**
 * TODO if getting fragment from request is possible remove $_REQUEST['highlite'] HACK
 * NOT WORKING since fragments are not available to the server
 function fragment(){
 global $fragment;
 if(!$fragment){
 $fragment = substr($_SERVER['REQUEST_URI'], strrpos($_SERVER['REQUEST_URI'], '#'));
 }
 return $fragment;
 }
 */

function uuid_anchor($uuid, $innerHTML){
  $highlite = $_REQUEST['highlite'] == $uuid;
  return '<a name="'.$uuid.'" ></a><span class="'.($highlite ? 'highlite' : '').'">'.$innerHTML.'</span>';
}

/**
 * Enter description here...
 *
 * @param unknown_type $name
 * @param unknown_type $numOfNameTokens
 * @return unknown
 * @deprecated looks like this is not used anymore
 */
function tagNameParts($name, $numOfNameTokens){

  $out = '<span class="name">';

  $token = strtok($name, " \n\t");
  $i = 0;
  $noSpace = true;
  while($token != false){
    if($i == $numOfNameTokens){
      $out .= '</span> <span class="authors">';
      $noSpace = true;
    }
    $out .= ($noSpace?'':' ').$token;
    $noSpace = false;
    $token = strtok(" \n\t");
    $i++;
  }
  return $out.'</span>';
}

/**
 * Converts an array of TagedText items into a sequence of corresponding html tags whereas
 * each item will provided with a class attribute which set to the key of the TaggedText item.
 *
 * @param array $taggedtxt
 * @param String $tag
 * @param String $glue the string by which the chained text tokens are concatenated together.
 *       Default is a blank character
 * @return String of HTML
 */
function theme_cdm_taggedtext2html(array &$taggedtxt, $tag = 'span', $glue = ' ', $skiptags = array()){
  $out = '';
  $i = 0;
  foreach($taggedtxt as $tt){
    if(!in_array($tt->type, $skiptags) && strlen($tt->text) > 0){
      $out .= (strlen($out) > 0 && ++$i < count($taggedtxt)? $glue : '').'<'.$tag.' class="'.$tt->type.'">'.t($tt->text).'</'.$tag.'>';
    }
  }
  return $out;
}

/**
 * Almost any cdmObject may be annotated. Therefore we provide a generic way to display
 * as well as create or update annotations.
 *
 * TODO it should be configurable which objects can be annotated as this might differ in dataportals
 *
 */
function theme_cdm_annotation($cdmBase){
  if(!$cdmBase->uuid){
    return;
  }else{

    drupal_add_js(drupal_get_path('module', 'cdm_dataportal').'/js/cdm_annotations.js');
    drupal_add_js(drupal_get_path('module', 'cdm_dataportal').'/js/jquery.form.js');

    $annotatableUuid = $cdmBase->uuid;
    //FIXME annotations only available as property of e.g. taxon, name, ...
    $annotationUrl = cdm_compose_url(CDM_WS_ANNOTATIONS, array($annotatableUuid));

    $annotationProxyUrl = url('cdm_api/proxy/'. urlencode($annotationUrl).'/cdm_annotation_content');

    $out = ' <span class="annotation">';
    $out .= '<span class="annotation_toggle" rel="'.$annotationProxyUrl.'">+</span>';
     

    $out .= '<div class="annotation_box"></div>';
    $out .= '</span>';

    return $out;

  }
}

function theme_cdm_annotation_content($AnnotationTO){
  
  drupal_add_js(drupal_get_path('module', 'cdm_dataportal').'/js/cdm_annotations.js');
  drupal_add_js(drupal_get_path('module', 'cdm_dataportal').'/js/jquery.form.js');

  $out .= theme('cdm_list_of_annotations', $AnnotationTO->annotationElements);

  $annotationUrl = cdm_compose_url(CDM_WS_ANNOTATIONS, array($AnnotationTO->uuid));
  $annotationProxyUrl = url('cdm_api/proxy/'. urlencode($annotationUrl).'/cdm_annotation_post');

  // TODO users have to be authenticated to the dataportal to be able to write annotations
  $out .= '
  			<div class="annotation_create">
  				<form action="'.$annotationProxyUrl.'" method="POST">
  					<textarea name="annotation"></textarea>
  					<input type="hidden" name="commentator" value="">
  					<input type="submit" value="'.t('Save annotation').'" />
  				</form>
 			</div>
	';

  return $out;
}

function theme_cdm_list_of_annotations($annotationElements){

  $out = '<ul class="annotation_list">';

  foreach ($annotationElements as $key => $row){
    $created[$key] = $row;
  }
  array_multisort($created, SORT_ASC, $annotationElements);

  foreach ($annotationElements as $annotation){
    $out .= '<li>' . $annotation->text . '</li>';
  }

  $out .= '</ul>';

  return $out;

}

function theme_cdm_media($descriptionElement, $mimeTypePreference){
  $out = "";

  _add_js_thickbox();

  $uuid = $descriptionElement->uuid;
  $feature = $descriptionElement->feature;
  $medias = $descriptionElement->media;

  foreach($medias as $media){
     
    $prefRepresentations = cdm_preferred_media_representations($media, $mimeTypePreference, 300, 400);
    $mediaRepresentation = array_shift($prefRepresentations);
    if($mediaRepresentation) {

      $contentTypeDirectory = substr($mediaRepresentation->mimeType, 0, stripos($mediaRepresentation->mimeType, '/'));

      $out = theme('cdm_media_mime_' . $contentTypeDirectory,  $mediaRepresentation, $feature);

      //			$attributes = array('class'=>'thickbox', 'rel'=>'descriptionElement-'.$uuid, 'title'=>$feature->term);
      //		    for($i = 0; $part = $mediaRepresentation->representationParts[$i]; $i++){
      //		    	if($i == 0){
      //		    	    $image_url = drupal_get_path('module', 'cdm_dataportal').'/images/'.$feature->term.'-media.png';
      //		    	    $media = '<img src="'.$image_url.'" height="14px" alt="'.$feature->term.'" />';
      //		    	    $out .= l($media, $part->uri, $attributes, NULL, NULL, TRUE, TRUE);
      //		    	} else {
      //		    		$out .= l('', $part->uri, $attributes, NULL, NULL, TRUE);
      //		    	}
      //		  	}
    } else {
      // no media available, so display just the type term
      $out .=  $feature->representation_L10n;
    }
  }
  return $out;

}

function theme_cdm_mediaTypeTerm($feature){
  $icon_url = drupal_get_path('module', 'cdm_dataportal').'/images/'.$feature->representation_L10n.'-media.png';
  return '<img src="'.$icon_url.'" height="14px" alt="'.$feature->representation_L10n.'" />';
}

function theme_cdm_media_mime_application($mediaRepresentation, $feature){

  foreach($representation->representationParts as $part){
    $attributes = array('title'=>$feature->representation_L10n, 'target'=>'_blank');
    $out .= l(theme('cdm_mediaTypeTerm', $feature), $part->uri, $attributes, NULL, NULL, TRUE, TRUE);
  }
  return $out;
}

function theme_cdm_media_mime_image($mediaRepresentation, $feature){
  $out = '';
  $attributes = array('class'=>'thickbox', 'rel'=>'representation-'.$representation->uuid, 'title'=>$feature->representation_L10n);
  for($i = 0; $part = $representation->representationParts[$i]; $i++){
    if($i == 0){
       
      $out .= l(theme('cdm_mediaTypeTerm', $feature), $part->uri, $attributes, NULL, NULL, TRUE, TRUE);
    } else {
    		$out .= l('', $part->uri, $attributes, NULL, NULL, TRUE);
    }
  }
  return $out;
}

function theme_cdm_media_mime_text($representation, $feature){

  foreach($representation->parts as $part){
    $attributes = array('title'=>$feature->representation_L10n . t(' link will open in a new window'), 'target'=>'_blank');
    $out .= l(theme('cdm_mediaTypeTerm', $feature), $part->uri, $attributes, NULL, NULL, TRUE, TRUE);
  }
  return $out;
}

function theme_cdm_media_gallerie($mediaList, $maxExtend, $cols = 4, $maxRows = 1 ){
  
  if(!isset($mediaList[0])){
    return;
  }
  $out = '<table class="media_gallerie">';
  for($r = 0; $r < $maxRows && count($mediaList) > 0; $r++){
    $out .= '<tr>';  
    for($r = 0; $r < $cols; $r++){
      $media = array_shift($mediaList);
      if(isset($media->representations[0]->parts[0])){
        $contentTypeDirectory = substr($media->representations[0]->mimeType, 0, stripos($media->representations[0]->mimeType, '/'));
        $mediaPartHtml = theme('cdm_media_gallerie_'.$contentTypeDirectory, $media->representations[0], $maxExtend);
//        if($mediaPartHtml){
//          '<img src="" width="'.$maxExtend.'" height="'.$maxExtend.'" />';
//        }
      } else {
        $mediaPartHtml = '';
      }
      $out .= '<td>'.$mediaPartHtml.'</td>';    
    }
    $out .= '</tr>';  
  }
  if(count($mediaList) > 0){
     $out .= '<tr><td colspan="'.$cols.'">'.count($mediaList).' '.t('more ...').'</td></tr>';
  }
  $out .= '</table>';
  return $out;
}

function theme_cdm_media_gallerie_image($mediaRepresentation, $maxExtend){
  if(isset($mediaRepresentation->parts[0])){
    return  '<img src="'.$mediaRepresentation->parts[0]->uri.'" width="'.$maxExtend.'" height="'.$maxExtend.'" />';
  }

}

/**
 * TODO
 * Quick-and-dirty solution to show distribution service to exemplar groups
 *
 * @param unknown_type $featureTo
 * @return unknown
 */
function theme_cdm_descriptionElements_distribution($taxon){
  

  $server = variable_get('cdm_dataportal_geoservice_access_point', false);

  if(!server){
    return "<p>No geoservice specified</p>";
  }else{
    $map_data_parameters = cdm_ws_get(CDM_WS_GEOSERVICE_DISTRIBUTIONMAP, $taxon->uuid);

    $display_width = variable_get('cdm_dataportal_geoservice_display_width', false);
    $bounding_box = variable_get('cdm_dataportal_geoservice_bounding_box', false);
    $labels_on = variable_get('cdm_dataportal_geoservice_labels_on', 0);

    $query_string = ($display_width ? '&ms=' . $display_width: '')
      . ($bounding_box ? '&bbox=' .  $bounding_box : '')
      . ($labels_on ? '&labels=' .  $labels_on : '');
      
    if(variable_get('cdm_dataportal_map_openlayers', 1)){
      // embed into openlayers viewer
      $server = 'http://edit.csic.es/v1/areas3_ol.php';
      $map_tdwg_Uri = url($server. '?' .$map_data_parameters->String, $query_string);
      //$map_tdwg_Uri ='http://edit.csic.es/v1/areas3_ol.php?l=earth&ad=tdwg4:c:UGAOO,SAROO,NZSOO,SUDOO,SPAAN,BGMBE,SICSI,TANOO,GEROO,SPASP,KENOO,SICMA,CLCBI,YUGMA,GRCOO,ROMOO,NZNOO,CLCMA,YUGSL,CLCLA,ALGOO,SWIOO,CLCSA,MDROO,HUNOO,ETHOO,BGMLU,COROO,BALOO,POROO,BALOO|e:CZESK,GRBOO|g:AUTAU|b:LBSLB,TUEOO|d:IREIR,AUTLI,POLOO,IRENI|f:NETOO,YUGCR|a:TUEOO,BGMBE,LBSLB||tdwg3:c:BGM,MOR,SPA,SIC,ITA,MOR,SPA,FRA|a:YUG,AUT&as=a:8dd3c7,,1|b:fdb462,,1|c:4daf4a,,1|d:ffff33,,1|e:bebada,,1|f:ff7f00,,1|g:377eb8,,1&&ms=610&bbox=-180,-90,180,90';
      //$tdwg_sldFile = cdm_http_request($map_tdwg_Uri);
      $tdwg_sldFiles = cdm_ws_get($map_tdwg_Uri, null, null, "GET", TRUE);
      
      if(isset($tdwg_sldFiles[0]->layers)){
        $layerSlds = $tdwg_sldFiles[0]->layers;
        foreach($layerSlds as $layer){
          $tdwg_sldUris[$layer->tdwg] = "http://edit.csic.es/fitxers/sld/".$layer->sld;
        }
      }
      $tdwg_sldUri = "http://edit.csic.es/fitxers/sld/".substr($tdwg_sldFile, 7, 7);
      
      
      $add_tdwg1 = (isset($tdwg_sldUris['tdwg1']) ? "
          tdwg_1.params.SLD = '".$tdwg_sldUris['tdwg1']."';
          map.addLayers([tdwg_1]);" : '');
      $add_tdwg2 = (isset($tdwg_sldUris['tdwg2']) ? "
          tdwg_2.params.SLD = '".$tdwg_sldUris['tdwg2']."';
          map.addLayers([tdwg_2]);" : '');
      $add_tdwg3 = (isset($tdwg_sldUris['tdwg3']) ? "
          tdwg_3.params.SLD = '".$tdwg_sldUris['tdwg3']."';
          map.addLayers([tdwg_3]);" : '');
      $add_tdwg4 = (isset($tdwg_sldUris['tdwg4']) ? "
          tdwg_4.params.SLD = '".$tdwg_sldUris['tdwg4']."';
          map.addLayers([tdwg_4]);" : '');
      
//      $googleMapsApiKey_localhost = 'ABQIAAAAFho6eHAcUOTHLmH9IYHAeBRi_j0U6kJrkFvY4-OX2XYmEAa76BTsyMmEq-tn6nFNtD2UdEGvfhvoCQ';
//      drupal_set_html_head(' <script src="http://maps.google.com/maps?file=api&amp;v=2&amp;key='.$googleMapsApiKey_localhost.'"></script>');

      drupal_add_js(drupal_get_path('module', 'cdm_dataportal').'/js/OpenLayers/OpenLayers.js');
      drupal_add_js('
 var map;
 
 var tdwg_1 = new OpenLayers.Layer.WMS.Untiled( 
    "tdwg level 1", 
    "http://edit.csic.es/geoserver/wms",
    {layers:"topp:tdwg_level_1",transparent:"true"},
    {styles:\'\'},
    {group:\'no base\'},
    {\'displayInLayerSwitcher\':false},
    {format: \'image/png\'}
  );
  
 var tdwg_2 = new OpenLayers.Layer.WMS.Untiled( 
    "tdwg level 2", 
    "http://edit.csic.es/geoserver/wms",
    {layers:"topp:tdwg_level_2",transparent:"true"},
    {styles:\'\'},
    {group:\'no base\'},
    {\'displayInLayerSwitcher\':false},
    {format: \'image/png\'}
  );
  
 var tdwg_3 = new OpenLayers.Layer.WMS.Untiled( 
    "tdwg level 3", 
    "http://edit.csic.es/geoserver/wms",
    {layers:"topp:tdwg_level_3",transparent:"true"},
    {styles:\'\'},
    {group:\'no base\'},
    {\'displayInLayerSwitcher\':false},
    {format: \'image/png\'}
  );
  
  var tdwg_4 = new OpenLayers.Layer.WMS.Untiled( 
    "tdwg level 4", 
    "http://edit.csic.es/geoserver/wms",
    {layers:"topp:tdwg_level_4",transparent:"true"},
    {styles:\'\'},
    {group:\'no base\'},
    {\'displayInLayerSwitcher\':false},
    {format: \'image/png\'}
  );
 
 var ol_wms = new OpenLayers.Layer.WMS( 
    "OpenLayers WMS",
    "http://labs.metacarta.com/wms/vmap0",
    {layers: \'basic\'}, 
    {group:\'base\'},
    {\'displayInLayerSwitcher\':false}
  );
  
  
  // --- google layers ----
//     var gphy = new OpenLayers.Layer.Google(
//        "Google Physical",
//        {type: G_PHYSICAL_MAP}
//    );
//    var gmap = new OpenLayers.Layer.Google(
//        "Google Streets", // the default
//        {numZoomLevels: 20}
//    );
//    var ghyb = new OpenLayers.Layer.Google(
//        "Google Hybrid",
//        {type: G_HYBRID_MAP, numZoomLevels: 20}
//    );
//    var gsat = new OpenLayers.Layer.Google(
//        "Google Satellite",
//        {type: G_SATELLITE_MAP, numZoomLevels: 20}
//    );
 


  // ------------------------------
  
  
 function init() {
   var options={ 
     controls: 
       [
         new OpenLayers.Control.LayerSwitcher({\'ascending\':false}),
         new OpenLayers.Control.PanZoomBar(),
         //new OpenLayers.Control.PanZoom(),
         new OpenLayers.Control.MouseToolbar(),
         //new OpenLayers.Control.MousePosition(),
         //new OpenLayers.Control.KeyboardDefaults()
       ],
       numZoomLevels: 6,
       projection: new OpenLayers.Projection("EPSG:4326")
    };
   
   map = new OpenLayers.Map(\'openlayers_map\',options);
   map.addLayers([ol_wms]);
//   map.addLayers([gphy]);
   '.$add_tdwg1.'
   '.$add_tdwg2.'
   '.$add_tdwg3.'
   '.$add_tdwg4.'
   map.setCenter(new OpenLayers.LonLat(0, 0), 1);
 }
 
$(document).ready(function(){
  init(); 
});'
      , 'inline');
      $out = '<div id="openlayers_map" class="smallmap" style="width: '.$display_width.'; height:'.($display_width / 2).'"></div>';
      
    } else {
      // simple image
      $mapUri = url($server. '?' .$map_data_parameters->String, $query_string);
      $out .= '<img class="distribution_map" src="'.$mapUri.'" alt="Distribution Map" />'; 
    }
    
    // add a simple legend
    if(variable_get('cdm_dataportal_geoservice_legend_on', TRUE)){
      $legenddata = array(
        'native' => "4daf4a",
        'native_doubtfully_native' => "377eb8",
        'cultivated' => "984ea3",
        'introduced' => "ff7f00",
        'introduced adventitious' => "ffff33",
        'introduced cultivated' => "a65628",
        'introduced naturalized' => "f781bf"
      );
      
      $out .= '<div class="distribution_map_legend">';
      foreach($legenddata as $term => $color){
        $out .= '<img style="width: 3em; height: 1em; background-color: #'.$color.'" src="'.
        drupal_get_path('module', 'cdm_dataportal').'/images/clear.gif" />'.t($term).' ';
      }
      $out .= '</div>';
    }
    
    return $out;
  }
}

function theme_cdm_taxonName($taxonName, $nameLink = NULL, $refenceLink = NULL, $renderPath = null){
  
  $renderTemplate = get_nameRenderTemplate($renderPath, $nameLink, $refenceLink);
  
  $partDefinition = get_partDefinition($taxonName->class);
  
  // apply defintions to template
  foreach($renderTemplate as $part=>$uri){
    if(isset($partDefinition[$part])){
      $renderTemplate[$part] = $partDefinition[$part];
    }
    if(is_array($uri)){
      $renderTemplate[$part]['#uri'] = $uri['#uri'];
    }
  }
  
  if(isset($taxonName->taggedName)){
    
    $taggedName = $taxonName->taggedName;
    // due to a bug in the cdmlib the taggedName alway has a lst empty element, we will remove it:
    array_pop($taggedName);
    
    if(!isset($renderTemplate['namePart']['authorTeam'])){
      // find author and split off from name 
      // TODO expecting to find the author as the last element
      if($taggedName[count($taggedName)- 1]->type == 'authors'){
        $authorTeam = $taggedName[count($taggedName)- 1]->text;
        unset($taggedName[count($taggedName)- 1]);
      }
    }
    $name = '<span class="'.$taxonName->class.'">'.theme('cdm_taggedtext2html', $taggedName).'</span>';
  } else {  
    $name = '<span class="'.$taxonName->class.'_titleCache">'.$taxonName->titleCache.'</span>';
  }
  
  // fill name into $renderTemplate
  array_setr('name', $name, $renderTemplate);
 
  // fill with authorTeam
  if($authorTeam){
    $authorTeamHtml = ' <span class="authorTeam">'.$authorTeam.'</span>';
    array_setr('authorTeam', $authorTeamHtml, $renderTemplate);
  }
  
  // fill with reference
  if($taxonName->nomenclaturalReference){
    $citation = $taxonName->nomenclaturalReference->titleCache;
    $authorTeam = cdm_taggedtext_value($taggedName, 'authors');
    $citation = trim(str_replace($authorTeam, '', $citation));
    if(str_beginsWith($citation, ", in")){
      $citation = substr($citation, 2);
      $separator = ' ';
    } else if(!str_beginsWith($citation, "in")){
      $separator = ', ';
    } else {
      $separator = ' ';
    }
    $referenceArray['#separator'] = $separator;
    $referenceArray['#html'] = '<span class="reference">'.$citation.'</span>';
    array_setr('reference', $referenceArray, $renderTemplate);
  }

  // fill with microreference
  if($taxonName->nomenclaturalMicroReference){
    $microreferenceHtml = '<span class="microreference">:&nbsp;' . $taxonName->nomenclaturalMicroReference . '</span>';
    array_setr('microreference', $microreferenceHtml, $renderTemplate);
  }
  
  // fill with status
  if(array_setr('status', true, $renderTemplate)){
    if(isset($taxon->name->status[0])){
      foreach($taxon->name->status as $status){
        $statusHtml .= ', '.$status->type->representation_L10n;
      }
    }
    array_setr('status', ' <span class="nomenclatural_status">'.$statusHtml.'</span>', $renderTemplate);
  }

  // fill with protologues etc...
  if(array_setr('description', true, $renderTemplate)){
    $descriptions = cdm_ws_get(CDM_WS_NAME_DESCRIPTIONS, $taxonName->uuid);
    foreach($descriptions as $description){
        if(!empty($description)){
          foreach($description->elements as $description_element){
            $descriptionHtml .= theme("cdm_media", $description_element, array('application/pdf', 'image/png', 'image/jpeg', 'image/gif', 'text/html'));
          }
        }
    }
    array_setr('description', $descriptionHtml, $renderTemplate);
  }  
  
  // render
  $out = '';
  foreach($renderTemplate as $partName=>$part){
    $separator = '';
    $partHtml = '';
    $uri = false;
    if(!is_array($part)){
      continue;
    }
    if(isset($part['#uri']) && is_string($part['#uri'])){
      $uri = $part['#uri'];
      unset($part['#uri']);
    }
    foreach($part as $key=>$content){
      $html = '';
      if(is_array($content)){
        $html = $content['#html'];
        $separator = $content['#separator'];
      } else if(is_string($content)){
        $html = $content;
      }
      $partHtml .= '<span class="'.$key.'">'.$html.'</span>';
    }
    if($uri){
      $out .= $separator.'<a href="'.$uri.'" class="'.$partName.'">'.$partHtml.'</a>';
    } else {
      $out .= $separator.$partHtml;
    }
  }
  
  return $out;
}

/**
 * Recursively searches the array for the $key and sets the given value
 * @param $key
 * @param $value  
 * @param $array
 * @return true if the key has been found
 */
function &array_setr($key, $value, array &$array){
  foreach($array as $k=>&$v){
    if($key == $k){
      $v = $value;
      return $array;
    } else if(is_array($v)){
      $innerArray = array_setr($key, $value, $v);
      if($innerArray){
        return $array;
      }
    }
  }
  return null;
}


function theme_cdm_related_taxon($taxon, $reltype_uuid = '', $displayNomRef = true){

  $relsign = '';
  $name_prefix = '';
  $name_postfix = '';
  switch ($reltype_uuid){
    case UUID_HETEROTYPIC_SYNONYM_OF:
    case UUID_SYNONYM_OF:
      $relsign = '=';
      break;
    case UUID_HOMOTYPIC_SYNONYM_OF:
      $relsign = '≡';
      break;
    case UUID_MISAPPLIED_NAME_FOR:
    case UUID_INVALID_DESIGNATION_FOR:
      $relsign = '&ndash;'; // &ndash; &mdash; &minus;
      $name_prefix = '"';
      $name_postfix = '"';
      break;
    default :
      $relsign = '&ndash;';
  }

  //$taxonUri = url(path_to_taxon($taxon->uuid));
  if($taxon->name->nomenclaturalReference){
    $referenceUri = url(path_to_reference($taxon->name->nomenclaturalReference->uuid));
  }
  $nameHtml = theme('cdm_taxonname', $taxon->name, $taxonUri, $referenceUri);
  
  $out = '<span class="relation_sign">'.$relsign.'</span>'.$name_prefix . $nameHtml . $name_postfix;
  return $out;

}

/**
 * will theme form elements of type 'select_secuuid'
 * see $form['cdm_dataportal']['secuuid_widget']
 * @param FormElement $element
 */
function theme_select_secuuid($element) {

  $default_uuid = variable_get($element['#varname'], false);

  $tree = cdm_taxontree_build_tree(null, false); // get root nodes
  $secUuids = array();
  foreach($tree as $node){
    $secUuids[] = $node->secUuid;
  }
  cdm_api_secref_cache_prefetch($secUuids);

  theme('cdm_taxontree_add_scripts');
  drupal_add_js('$(document).ready(function() {$(\'ul.cdm_taxontree\').cdm_taxontree(
  {
    widget:                 true,
    element_name:           \''.$element['#varname'].'\',  //
    multiselect:            '.($element['#multiple']?'true':'false').',         //
  }
  );});', 'inline');

  $out  = '<div class="cdm_taxontree_widget">';
  $out .= '<div class="taxontree">'.theme('cdm_taxontree', $tree, NULL, FALSE, 'cdm_taxontree_node_reference').'</div>';
  $out .= $element['#children'].'<div style="clear: both;" /></div>';

  return theme(
    'form_element',
  array(
      '#title' => $element['#title'],
      '#description' => $element['#description'],
      '#id' => $element['#id'],
      '#required' => $element['#required'],
      '#error' => $element['#error'],
  ),
  $out
  );
}

function theme_cdm_dynabox($label, $content_url, $theme, $enclosingtag = 'li'){
  $cdm_proxy_url = url('cdm_api/proxy/'.urlencode($content_url)."/$theme");
  $out .= '<li class="dynabox"><span class="label" alt="'.t('Click for accepted taxon').'">'.$label.'</span>';
  $out .= '<ul class="dynabox_content" title="'.$cdm_proxy_url.'"><li><img class="loading" src="'.drupal_get_path('module', 'cdm_dataportal').'/images/loading_circle_grey_16.gif" style="display:none;"></li></ul>';
  return $out;
}

function theme_cdm_list_of_taxa($records, $showMedia = false){
  
  $renderPath = 'list_of_taxa';
  
  $showMedia_taxa = variable_get('cdm_dataportal_findtaxa_taxon_media_on', 1);
  $showMedia_synonyms = variable_get('cdm_dataportal_findtaxa_synonyms_media_on', 0);
  $prefMimeTypeRegex = 'image:.*';
  $prefMediaQuality = '*';
  $cols = variable_get('cdm_dataportal_findtaxa_media_cols', 3);
  $maxRows = variable_get('cdm_dataportal_findtaxa_media_maxRows', 1);
  $maxExtend = variable_get('cdm_dataportal_findtaxa_media_maxextend', 120);
  
  $out = '<ul class="cdm_names" style="background-image: none;">';

  $synonym_uuids = array();
  foreach($records as $taxon){
    if(!_cdm_dataportal_acceptedByCurrentView($taxon)){
      if(!array_key_exists($taxon->uuid, $synonym_uuids)){
        $synonym_uuids[$taxon->uuid] = $taxon->uuid;
      }
    }
  }
  // batch service not jet implemented: 
  // $table_of_accepted = cdm_ws_property(CDM_WS_TAXON_ACCEPTED, join(',', $synonym_uuids));
  // thus ...
  $table_of_accepted = array();
  foreach($synonym_uuids as $synUuid){
    $table_of_accepted[$synUuid] = cdm_ws_get(CDM_WS_TAXON_ACCEPTED, $synUuid);
  }
  // .. well, for sure not as performant as before, but better than nothing.

  foreach($records as $taxon){
    if(_cdm_dataportal_acceptedByCurrentView($taxon)){
      $taxonUri = url(path_to_taxon($taxon->uuid));
      if(isset($taxon->name->nomenclaturalReference)){
        $referenceUri = url(path_to_reference($taxon->name->nomenclaturalReference->uuid));
      }
      $out .= '<li class="Taxon">'.theme('cdm_taxonName', $taxon->name, $taxonUri, $referenceUri, $renderPath);
      if($showMedia_taxa){
          $mediaList = cdm_ws_get(CDM_WS_TAXON_MEDIA, array($taxon->uuid, $prefMimeTypeRegex, $prefMediaQuality));
          $out .= theme('cdm_media_gallerie', $mediaList, $maxExtend, $cols, $maxRows);
      }
      $out .= '</li>';
    } else {
      $uuid = $taxon->uuid;
      $acceptedTaxa = $table_of_accepted[$uuid];
      if(count($acceptedTaxa) == 1){
        $acceptedTaxon = $acceptedTaxa[0];
        $taxonUri = uri_to_synonym($taxon->uuid, $acceptedTaxon->uuid, 'synonymy');
        if(isset($acceptedTaxon->name->nomenclaturalReference)){
          $referenceUri = url(path_to_reference($acceptedTaxon->name->nomenclaturalReference->uuid));
        }
        $out .= '<li class="Synonym">'.theme('cdm_taxonName', $taxon->name, $taxonUri, $referenceUri, $renderPath);
        if($showMedia_synonyms){
          $mediaList = cdm_ws_get(CDM_WS_TAXON_MEDIA, array($acceptedTaxon->uuid, $prefMimeTypeRegex, $prefMediaQuality));
          $out .= theme('cdm_media_gallerie', $mediaList, $maxExtend,$cols, $maxRows);
        }
      $out .= '</li>';
      } else {
        //TODO avoid using AHAH in the cdm_dynabox
        //TODO add media
        $out .= theme('cdm_dynabox', theme('cdm_taxonName', $taxon->name, null, null, $renderPath), cdm_compose_url(CDM_WS_TAXON_ACCEPTED, array($taxon->uuid)), 'cdm_list_of_taxa');
      }
    }
  }
  $out .= '</ul>';
  return $out;
}


function theme_cdm_credits(){
  $secRef_array = _cdm_dataportal_currentSecRef_array();
  return '<span class="sec_reference_citation">'.$secRef_array['citation'].'</span>'
  .( $secRef_array['period'] ? ' <span class="year">'.partialToYear($secRef_array['period']).'</span>' : '')
  .( $secRef_array['authorTeam'] ? '<div class="author">'.$secRef_array['authorTeam']['titleCache'].'</div>' : '');
}

/**
 * @deprecated ??
 */
function theme_cdm_reference($reference, $linkToReference = FALSE, $style = NULL ){
  
  if($style == "ZoologicalName"){
     $year = partialToYear($reference->datePublished->start);
     $citation = $reference->authorTeam->titleCache.($year ? ', '.$year : '');
  } else {
    $citation = $reference->titleCache;    
  }
  if($linkToReference){
    return l('<span class="reference">'.$citation.'</span>', "/cdm_dataportal/reference/".$reference->uuid, array("class"=>"reference"), NULL, NULL, FALSE ,TRUE);
  } else {
    return '<span class="reference">'.$citation.'</span>';
  } 
}

/**
 * Enter description here...
 *
 * @param unknown_type $referenceSTO a referenceSTO or referenceTO instance
 * @param unknown_type $cssClass
 * @param unknown_type $separator
 * @param unknown_type $enclosingTag
 * @return unknown
 */
function theme_cdm_nomenclaturalReference($name, $doLink = FALSE, $displayMicroreference = TRUE, $displayPages = FALSE, $enclosingTag = 'span'){
  
  if(!$name->nomenclaturalReference){
    return;
  }
  
  $reference = $name->nomenclaturalReference;
  
  // different display for ZoologicalNames and others
  if($name->class == ZoologicalName){
    $year = partialToYear($reference->datePublished->start);
    $citation = $reference->authorTeam->titleCache.($year ? ', '.$year : '');
  } else {
    // BotanicalName and others  
    $citation = $reference->titleCache;
  }
  
  if($doLink){
    $citationHtml = l($citation, "/cdm_dataportal/reference/".$reference->uuid, array(), NULL, NULL, FALSE, TRUE);
  } else {
    $citationHtml = $citation;
  }
  
  if(!empty($citation)){
    $citationHtml = (str_beginsWith($citation, 'in') ? '&nbsp;':',&nbsp;') . $citationHtml;
    if($displayMicroreference && $name->nomenclaturalMicroReference){
      $microreference = '<span class="pages">:&nbsp;' . $name->nomenclaturalMicroReference . '</span>'; 
    }  
    if($displayPages){ //TODO substitute for microreference in dipterea? -> check theme.php#L85
      $citation .= ': '.$reference->pages;
    }
    $citationHtml .= $microreference;
  }
  
  
  return '<'.$enclosingTag.' class="reference">'.$citationHtml.'</'.$enclosingTag.'>'; ;
}

/**
 * Allows theaming of the taxon page tabs
 * 
 * @param $tabname
 * @return unknown_type
 */
function theme_cdm_taxonpage_tab($tabname){
   //TODO replace by using translations or theme the menue tabs itself instead?
  switch($tabname){
    default: return t($tabname); 
  }
}

/**
 * default title for a taxon page
 *
 * @param NameTO $nameTO
 * @return the formatted taxon name
 */
function theme_cdm_taxon_page_title($taxon){
  
  $renderPath = 'taxon_page_title';
  if(isset($taxon->name->nomenclaturalReference)){
        $referenceUri = url(path_to_reference($taxon->name->nomenclaturalReference->uuid));
  }
  return theme('cdm_taxonName', $taxon->name, null, $referenceUri, $renderPath);
}


function theme_cdm_acceptedFor($renderPath = false){
  $out = '';
  if(!$renderPath){
    $renderPath = 'acceptedFor';
  }
  if(isset($_REQUEST['acceptedFor'])){
     
     $synonym = cdm_ws_get(CDM_WS_TAXON, $_REQUEST['acceptedFor']);
     
     if($synonym){
       $out .= '<span class="acceptedFor">';
       $out .= t('is accepted for ');
       if(isset($synonym->name->nomenclaturalReference)){
             $referenceUri = url(path_to_reference($synonym->name->nomenclaturalReference->uuid));
       }
       $out .= theme('cdm_taxonName', $synonym->name, null, $referenceUri, $renderPath);
       $out .= '</span>';
     }
  }
  
  return $out;
}

function theme_cdm_back_to_search_result_button(){
  $out = '';
  if($_SESSION['cdm']['search']){
    /*['cdm']['last_search']*/
    //$out .= '<div id="backButton">'.l(t('Back to search result'), $_SESSION ).'</div>';
    $out .= '<div id="backButton">'.l(t('Back to search result'), "http://" . $_SERVER['SERVER_NAME'] . $_SESSION['cdm']['last_search'] ).'</div>';
  
  }
  return $out;
}

/**
 * A wrapper function that groups available information to show by default, when
 * a taxon page is requested by the browser.
 * Individual themeing has to decide what this page should include (see methods beneath)
 * and what information should go into tabs or should not be shown at all.
 *
 * It is headed by the name of the accepted taxon without author and reference.
 * @param $taxonTO the taxon object
 * @param $page_part name of the part to display,
 *         valid values are: 'description', 'images', 'synonymy', 'all'
 */
function theme_cdm_taxon_page_general($taxon, $page_part = 'description') {
  
  $page_part = variable_get('cdm_dataportal_taxonpage_tabs', 1) ? $page_part : 'all';
  $hideTabs = array();
  
  
  // get images
  $prefMimeTypeRegex = 'image:.*';
  $prefMediaQuality = '*';
  $media = cdm_ws_get(CDM_WS_TAXON_MEDIA, array($taxon->uuid, $prefMimeTypeRegex, $prefMediaQuality));
  if(!isset($media[0])) {
    $hideTabs[] = theme('cdm_taxonpage_tab', 'Images');
    
  }
  // $hideTabs[] = theme('cdm_taxonpage_tab', 'General');
  // $hideTabs[] = theme('cdm_taxonpage_tab', 'Synonymy')
  
  // hide tabs
  $tabhide_js = '';
  foreach($hideTabs as $tabText) {
    $tabhide_js .= "$('.tabs.primary').children('li').children('a:contains(\"$tabText\")').hide();\n";
  }
  drupal_add_js("
  $(document).ready(function(){
      $tabhide_js
    });", 'inline');
      
  $out = '';
  $out .= theme('cdm_back_to_search_result_button');
  $out .= theme('cdm_acceptedFor', 'page_general');
  
  if($page_part == 'description' || $page_part == 'all'){
    
    $featureTree = cdm_ws_get(CDM_WS_FEATURETREE, variable_get('cdm_dataportal_featuretree_uuid', false));
    $taxonDescriptions = cdm_ws_get(CDM_WS_TAXON_DESCRIPTIONS, $taxon->uuid);
    $mergedTrees = cdm_ws_descriptions_by_featuretree($featureTree, $taxonDescriptions, variable_get('cdm_dataportal_descriptions_separated', FALSE));
 
    $out .= '<div id="general">';
    $out .= theme('cdm_taxon_page_description', $taxon, $mergedTrees);
    $out .= '</div>';
  }
  
  if($page_part == 'images' || $page_part == 'all'){
    $out .= '<div id="images">';
    if($page_part == 'all'){
      $out .= '<h2>'.t('Images').'</h2>';
    }
    $out .= theme('cdm_taxon_page_images', $taxon, $media);
    $out .= '</div>';
  }

  if($page_part == 'synonymy' || $page_part == 'all'){
    $out .= '<div id="synonymy">';
    if($page_part == 'all'){
      $out .= '<h2>'.t('Synonymy').'</h2>';
    }
    $addAcceptedTaxon = !variable_get('cdm_dataportal_nomref_in_title', 1);
    $out .= theme('cdm_taxon_page_synonymy', $taxon, $addAcceptedTaxon);

    if(variable_get('cdm_dataportal_display_name_relations', 1)){
      
      $nameRelationships = cdm_ws_get(CDM_WS_TAXON_NAMERELATIONS, $taxon->uuid);
      // TODO is it correct to skip relationsFromThisName since all relationships are to be understood as 'is .... of'
      if(variable_get('cdm_dataportal_name_relations_skiptype_basionym', 1)){
        $skip = array(UUID_BASIONYM);
      }
      $out .= theme('cdm_nameRelations', $nameRelationships, $skip);
     }
    $out .= '</div>';
  }

  return $out;
}

/**
 * Outputs all descriptive data and shows the preferred picture of the
 * accepted taxon.
 *
 */
function theme_cdm_taxon_page_description($taxon, $mergedTrees){

  // description TOC
  $out .= theme('cdm_featureTreeTOCs', $mergedTrees);
  // description
  $out .= theme('cdm_featureTrees', $mergedTrees, $taxon);
  return $out;
}

/**
 * Show whole synonymy for the accepted taxon. Synonymy list is headed by the complete scientific name
 * of the accepted taxon with nomenclatural reference.
 *
 */
function theme_cdm_taxon_page_synonymy($taxon, $addAcceptedTaxon){
  
  $renderPath = 'taxon_page_synonymy';
  $synomymie = cdm_ws_get(CDM_WS_TAXON_SYNONYMY, $taxon->uuid);
  $taxonRelationships = cdm_ws_get(CDM_WS_TAXON_RELATIONS, $taxon->uuid);
  $skip = array(UUID_BASIONYM);
 
  if($addAcceptedTaxon){
    if(isset($taxon->name->nomenclaturalReference)){
           $referenceUri = url(path_to_reference($taxon->name->nomenclaturalReference->uuid));
     }
    $out .= theme('cdm_taxonName', $taxon->name, null, $referenceUri, $renderPath);
  }
  
  if($addAcceptedTaxon && !isset($synomymie->homotypicSynonymsByHomotypicGroup[0])){
    // display the type information for the added taxon
    $typeDesignations = cdm_ws_get(CDM_WS_TAXON_NAMETYPEDESIGNATIONS, $taxon->uuid);
    if($typeDesignations){
      $out .= theme('cdm_typedesignations', $typeDesignations);
    }
  } else {
    // reder the homotypicSynonymyGroup including the type information
    $out .= theme('cdm_homotypicSynonymyGroup', $synomymie->homotypicSynonymsByHomotypicGroup);
  }
  if($synomymie->heterotypicSynonymyGroups) {
    foreach($synomymie->heterotypicSynonymyGroups as $homotypicalGroup){
      $out .= theme('cdm_heterotypicSynonymyGroup', $homotypicalGroup);
    }
  }

  $out .= theme('cdm_taxonRelations', $taxonRelationships, $skip);

  return $out;
}

/**
 * Show the collection of images stored with the accepted taxon
 * 
 */
function theme_cdm_taxon_page_images($taxon, $media){

  $flashLink = isset($media[0]);
  
  if($flashLink){
    
    $taggedName = $taxon->name->taggedName;
    
    $nameArray = array();
    foreach($taggedName as $taggedText){
      if($taggedText->type == 'name'){
        $nameArray[] = $taggedText->text;
      }
    }
    
    $query = join("%5F", $nameArray) . '%20AND%20jpg';
    
  $out = '
  
  <script type="text/javascript" src="http://media.bgbm.org/erez/js/fsiwriter.js"></script>

<script type="text/javascript">
<!--
	writeFlashCode( "http://media.bgbm.org/erez/fsi/fsi.swf?&cfg=showcase_presets/showcase_info.fsi&effects=%26quality%3D95&showcase_query='.$query.'&skin=silver&showcase_labeltextheight=50&textbox_textfrom=IPTC_WP6&textbox_height=50&param_backgroundcolor=454343&publishwmode=opaque&showcase_hscroll=true&showcase_basecolor=454343&plugins=PrintSave,textbox",
		"http://media.bgbm.org/erez/erez?src=erez-private/flashrequired.svg&tmp=Large&quality=97&width=470&height=400",
		"width=470;height=400;bgcolor=454343;wmode=opaque");
// -->
</script>
<noscript>
	<object classid="clsid:D27CDB6E-AE6D-11cf-96B8-444553540000" codebase="http://download.macromedia.com/pub/shockwave/cabs/flash/swflash.cab#version=6,0,65,0" width="470" height="400">
		<param name="movie" value="http://media.bgbm.org/erez/fsi/fsi.swf?&cfg=showcase_presets/showcase_info.fsi&effects=%26quality%3D95&showcase_query='.$query.'&skin=silver&showcase_labeltextheight=50&textbox_textfrom=IPTC_WP6&textbox_height=50&param_backgroundcolor=454343&publishwmode=opaque&showcase_hscroll=true&showcase_basecolor=454343plugins=PrintSave,textbox"/>
		<param name="bgcolor" value="454343" />
		<param name="wmode" value="opaque" />
		<param name="allowscriptaccess" value="always" />
		<param name="allowfullscreen" value="true" />
		<param name="quality" value="high" />
		<embed src="http://media.bgbm.org/erez/fsi/fsi.swf?&cfg=showcase_presets/showcase_info.fsi&effects=%26quality%3D95&showcase_query='.$query.'&skin=silver&showcase_labeltextheight=50&textbox_textfrom=IPTC_WP6&textbox_height=50&param_backgroundcolor=454343&publishwmode=opaque&showcase_hscroll=true&showcase_basecolor=454343plugins=PrintSave,textbox"
			width="470"
			height="400"
			bgcolor="454343"
			wmode="opaque"
			allowscriptaccess="always"
			allowfullscreen="true"
			quality="high"
			type="application/x-shockwave-flash"
			pluginspage="http://www.adobe.com/go/getflashplayer">
		</embed>
	</object>

</noscript>';
  
  }else{
    $out = 'No images available.';
  
  }
  return $out;

}

function theme_cdm_reference_pager($referencePager, $path, $parameters = array()){
  drupal_set_title(t('Bibliographic Index'));
  $out = '';
  if(count($referencePager->records) > 0){
    $out .= '<ul>';
    foreach($referencePager->records as $reference){
      $reference->fullCitation = $reference->titleCache; //FIXME remove hack for matching cdm entity to STO
      $out .= '<li>'.theme('cdm_reference', $reference, TRUE).'</li>';
    }
    $out .= '</ul>';
    $out .= theme('cdm_pager_new', $referencePager,  $path, $parameters);
  } else {
    $out = '<h4 class="error">Sorry, this page contains not entries.</h4>';
  }
  return $out;
}

/**
 * Show a reference in it's atomized form
 */
function theme_cdm_reference_page($referenceTO){
  if($referenceTO->titleCache) {
    drupal_set_title($referenceTO->titleCache);
  } else {
    drupal_set_title($referenceTO->fullCitation);
  }
  $field_order = array(
    "title",
    //"titleCache",
    //"citation",
    "authorTeam",
    "editor",
    "publisher",
    "placePublished",
    "datePublished",
    "year",
    "edition",      // class Book
    "volume",       // class Article
    "seriesPart",
    "inSeries",
    "inJournal",     // class Article
    "inBook",        // class BookSection
    "nomRefBase",    // class BookSection, Book, Article
    "inProceedings", // class InProceedings
    "pages",         // class Article
    "series",        // class Article, PrintSeries
    "school",        // class Thesis
    "institution",   // class Report
    "organization",  // class Proceedings
    "nextVersion",
    "previousVersion",
    "isbn",         // class Book
    "issn",         // class Journal
    "uri",
  );
  
  $table_rows = array();
  foreach($field_order as $fieldname){
      if(isset($referenceTO->$fieldname)){
        if($fieldname == "datePublished") {
          $partial = $referenceTO->$fieldname;
          $datePublished = '';
          if($partial->start){
            $datePublished = substr($partial->start, 0, 4).'-'.substr($partial->start, 4, 2).'-'.substr($partial->start, 6, 2);
          }
          if($partial->end){
            $datePublished = (strlen($datePublished) > 0 ? ' '.t('to').' ' : '').substr($partial->end, 0, 4).'-'.substr($partial->end, 4, 2).'-'.substr($partial->end, 6, 2);
          }
          $table_rows[] = array(t(ucfirst(strtolower($fieldname))), $datePublished);
        } else if(is_object($referenceTO->$fieldname)){
          $table_rows[] = array(t(ucfirst(strtolower($fieldname))), $referenceTO->$fieldname->titleCache);
        } else {
          $table_rows[] = array(t(ucfirst(strtolower($fieldname))), $referenceTO->$fieldname);
        }
      }
  }
  return theme("table", array("","") , $table_rows);
}

/**
 * Show a synonym page
 *
 * TODO what should show on this page exactly?
 *
 */
function theme_cdm_synonym_page(){

}

function theme_cdm_preferredImage($mergedTrees, $defaultImage, $parameters = ''){

  if(isset($mergedTrees[0])){
    foreach($mergedTrees[0]->root->children as $node){
      if($node->feature->uuid == UUID_IMAGE){
        $preferredImage = $node->descriptionElements[0]->media[0]->representations[0]->parts[0]->uri;
      }
    }
  }

  $image = $preferredImage ? $preferredImage . $parameters : $defaultImage;
  $out = '<img class="left" src="'.$image.'" alt="no image available" />';
  return $out;
}




function theme_cdm_homotypicSynonymyGroup($synonymList, $prependedSynonyms = array()){

  if(! is_array($synonymList) || count($synonymList) == 0){
    return;
  }
  
  $out = '<ul class="homotypicSynonyms">';

  if(!empty($prependedSynonyms)){
    foreach($prependedSynonyms as $taxon){
      $out .= '<li class="synonym">'.theme('cdm_related_taxon', $taxon, UUID_HOMOTYPIC_SYNONYM_OF).'</li>';
    }
  }

  foreach($synonymList as $synonym){
    $out .= '<li class="synonym">'.theme('cdm_related_taxon', $synonym, UUID_HOMOTYPIC_SYNONYM_OF).'</li>';
  }
  
  $typeDesignations = cdm_ws_get(CDM_WS_TAXON_NAMETYPEDESIGNATIONS, $synonymList[0]->uuid);
  if($typeDesignations){
    $out .= theme('cdm_typedesignations', $typeDesignations);
  }

  $out .= '</ul>';
  return $out;
}

function theme_cdm_homotypicSynonymLine($taxon){
  $out = '';
  $out .= '<li class="synonym">'.theme('cdm_related_taxon', $taxon, UUID_HOMOTYPIC_SYNONYM_OF).'</li>';
  return $out;
}

function theme_cdm_heterotypicSynonymyGroup($homotypicalGroup){
  $out = '';
  $out = '<ul class="heterotypicSynonymyGroup">';

  $is_first_entry = true;
  $typeDesignations = null;
  foreach($homotypicalGroup as $synonym){
    if($is_first_entry){
      $is_first_entry = false;
      //$typeDesignations = cdm_ws_get(CDM_WS_NAME_TYPEDESIGNATIONS, $synonym->name->uuid);
      $typeDesignations = cdm_ws_get(CDM_WS_TAXON_NAMETYPEDESIGNATIONS, $synonym->uuid);
      // is first list entry
      $out .= '<li class="firstentry synonym">'.theme('cdm_related_taxon',$synonym, UUID_HETEROTYPIC_SYNONYM_OF).'</li>';
    } else {
      $out .= '<li class="synonym">'.theme('cdm_related_taxon',$synonym, UUID_HOMOTYPIC_SYNONYM_OF).'</li>';
    }
  }

  if($typeDesignations){
    $out .= theme('cdm_typedesignations', $typeDesignations);
  }

  $out .= '</ul>';

  return $out;
}

/**
 * renders misapplied names and invalid designations.
 * Both relation types are currently treated the same!
 *
 * @param unknown_type $taxonRelationships
 * @return unknown
 */
function theme_cdm_taxonRelations($taxonRelationships){

  if(!$taxonRelationships){
    return;
  }
  drupal_add_js(drupal_get_path('module', 'cdm_dataportal').'/js/cluetip/jquery.cluetip.js');
  drupal_add_js(drupal_get_path('module', 'cdm_dataportal').'/js/jquery.dimensions.js');
  drupal_add_js(drupal_get_path('module', 'cdm_dataportal').'/js/cluetip/jquery.hoverIntent.js');
  drupal_add_css(drupal_get_path('module', 'cdm_dataportal').'/js/cluetip/jquery.cluetip.css');

  drupal_add_js ("$(document).ready(function(){
      $('.cluetip').css({color: '#0062C2'}).cluetip({
        splitTitle: '|',
        showTitle: true,
        activation: 'hover',
        arrows: true,
        dropShadow: false,
        cluetipClass: 'rounded'
      });
    });", 'inline');

  // aggregate misapplied names having the same fullname:
  $misapplied = array();
  foreach($taxonRelationships as $taxonRelation){
    if(true || $taxonRelation->type->uuid == UUID_MISAPPLIED_NAME_FOR || $taxonRelation->type->uuid == UUID_INVALID_DESIGNATION_FOR ){
      
      $name = $taxonRelation->fromTaxon->name->titleCache;
      $authorteam = $taxonRelation->fromTaxon->sec->authorTeam->titleCache;
      
      if(!isset($misapplied[$name])){
        $misapplied[$name]['out'] = '<span class="misapplied">'.theme('cdm_related_taxon',$taxonRelation->fromTaxon, UUID_MISAPPLIED_NAME_FOR, false).'</span>';
      }

      // collect all authors for this fullname
      if(isset($authorteam)){
        $misapplied[$name]['authorteam'][$authorteam] = '&nbsp;<span class="sensu cluetip no-print" title="|sensu '.htmlspecialchars(theme('cdm_reference',$taxonRelation->fromTaxon->sec )).'|">sensu '
                    .$authorteam.'</span>'
                    .'<span class="reference only-print">sensu '.theme('cdm_reference', $taxonRelation->fromTaxon->sec ).'</span>';
      }

    }
  }
   
  // generate output
  $out = '<ul class="misapplied">';
  foreach($misapplied as $misapplied_name){
    $out .= '<li class="synonym">'.$misapplied_name['out'] . " ";
    // sorting authors
    ksort($misapplied_name['authorteam']);
    $out .= join('; ', $misapplied_name['authorteam']);
    $out .= '</li>';
  }
  $out .= '</ul>';
  return $out;
}

function theme_cdm_nameRelations($nameRelationships, $skipTypes = false){

  // group by relationship type
  $relationshipGroups = array();  
  foreach($nameRelationships as $nameRelationship){
    if(!array_key_exists($nameRelationship->type->uuid, $relationshipGroups)){
      $relationshipGroups[$nameRelationship->type->uuid] = array();
    }
    $relationshipGroups[$nameRelationship->type->uuid][] = $nameRelationship;
  }

  // generate output
  $out = '';
  foreach($relationshipGroups as $group){
    $type = $group[0]->type;
    
    if(is_array($skipTypes) && in_array($type->uuid, $skipTypes)){
      continue;
    }
    
    $block->module = 'cdm_dataportal';
    $block->subject = t(ucfirst($type->inverseRepresentation_L10n));
    $block->delta = generalizeString(strtolower($type->inverseRepresentation_L10n));
  
    foreach($group as $relationship){    
      $relatedNames[] = cdm_taggedtext2html($relationship->fromName->taggedName);
    }
    
    $block->content .= implode('; ', $relatedNames);
    $out .= theme('block', $block);
  }
  return $out;
}

/**
 * FIXME this definitively has to be in another spot. just didn't know where to put it right now.
 * Compares the status of two SpecimenTypeDesignations
 * @param String $a 	a SpecimenTypeDesignations
 * @param String $b		another SpecimenTypeDesignations
 */
function compare_specimenTypeDesignationStatus($a, $b){
  /* this is the desired sort oder as of now:
   * 	Holotype
   * 	Isotype
   * 	Lectotype
   * 	Isolectotype
   * 	Syntype
   *
   * TODO
   * Basically, what we are trying to do is, we define an ordered array of TypeDesignation-states
   * and use the index of this array for comparison. This array has to be filled with the cdm-
   * TypeDesignation states and the order should be parameterisable inside the dataportal.
   */
  // make that static for now
  $typeOrder = array('Holotype', 'Isotype', 'Lectotype', 'Isolectotype', 'Syntype');

  $aQuantifier = array_search($a->typeStatus->label, $typeOrder);
  $bQuantifier = array_search($b->typeStatus->label, $typeOrder);

  if ($aQuantifier == $bQuantifier) {
    // sort alphabetically
    return ($a->typeStatus->label < $b->typeStatus->label) ? -1 : 1;
  }
  return ($aQuantifier < $bQuantifier) ? -1 : 1;

}

function theme_cdm_typedesignations($typeDesignations = array()){
  $renderPath = 'typedesignations';
  $out = '<ul class="typeDesignations">';

  $specimenTypeDesignations = array();
  foreach($typeDesignations as $typeDesignation){
    if($typeDesignation->class == 'SpecimenTypeDesignation'){
      // SpecimenTypeDesignations should be ordered. collect theme here only
      $specimenTypeDesignations[] = $typeDesignation;
    }else {
      
      // it's a NameTypeDesignation
      if($typeDesignation->notDesignated){
        $out .= '<li class="nameTypeDesignation"><span class="status">Type</span>: '.t('not designated'). '</li>';
      }else if($typeDesignation->typeName){
        
        $out .= '<li class="nameTypeDesignation"><span class="status">Type</span>: ';
        
        if($typeDesignation->typeName->nomenclaturalReference){
          $referenceUri = url(path_to_reference($typeDesignation->typeName->nomenclaturalReference->uuid));
        }
        $out .= theme('cdm_taxonName', $typeDesignation->typeName, null, $referenceUri, $renderPath);
        
//        if($typeDesignation->typeName->class == 'ZoologicalName') {
//          // appending authorTeam which has been skipped in cdm_name
//          $authorTeam = cdm_taggedtext_value($typeDesignation->typeName->taggedName, 'authors');
//          $authorTeamPart = l('<span class="authors">'.$authorTeam.'</span>', "/cdm_dataportal/reference/".$typeDesignation->typeName->nomenclaturalReference->uuid, array(), NULL, NULL, FALSE, TRUE);
//          $out .= (str_endsWith($authorTeam, ')') ? '' : ', ').$authorTeamPart;
//        } else {
//          $out .= ' '.theme('cdm_reference', $typeDesignation->citation, true, $referenceStyle);
//          $out .= '</li>';
//        }
      }
    }
  }

  if(!empty($specimenTypeDesignations)){
    // sorting might be different for dataportals so this has to be parameterized
    usort($specimenTypeDesignations, "compare_specimenTypeDesignationStatus");
    foreach($specimenTypeDesignations as $std){

      $typeReference = '';
      if(!empty($std->citation)){
        $typeReference .= '&nbsp;(' . t('designated by');
        $typeReference .= '&nbsp;<span class="typeReference cluetip no-print" title="|'. htmlspecialchars(theme('cdm_reference',$std->citation )) .'|">';
        $typeReference .= $std->citation->authorTeam->titleCache . ' ' . partialToYear($std->citation->datePublished->start);
        $typeReference .= '</span>';
        $typeReference .= ')';
        $typeReference .= '<span class="reference only-print">(designated by '.theme('cdm_reference',$std->citation ).')</span>';
      }

      $out .= '<li class="specimenTypeDesignation">';
      $out .= '<span class="status">'.(($std->typeStatus->representation_L10n) ? $std->typeStatus->representation_L10n : t('Type')) .$typeReference.'</span>: '.$std->typeSpecimen->titleCache;
      $out .= theme('cdm_specimen', $std->typeSpecimen);
      $out .= '</li>';
    }
  }

  $out .= '</ul>';

  return $out;
}

function theme_cdm_specimen($specimen){

  _add_js_thickbox();

  $out = '';
  if(isset($specimen->media[0])){

    $image_url = drupal_get_path('module', 'cdm_dataportal').'/images/external_link.gif';
    // thickbox has problems reading the first url parameter, so a litte hack is needed here:
    // adding a meaningless patameter &tb_hack=1& ....
    $out .= '&nbsp;<a href="#TB_inline?tb_hack=1&width=300&amp;height=330&amp;inlineId=specimen_media_'.$specimen->uuid.'" class="thickbox">'
    .'<img src="'.$image_url.'" title="'.t('Show media').'" /></a>';

    $out .= '<div id="specimen_media_'.$specimen->uuid.'" class="tickbox_content"><table>';

    $media_row = '<tr class="media_data">';
    $meta_row = '<tr class="meta_data">';

    foreach($specimen->media as $media){
      foreach($media->representations as $representation){

        //TODO this this is PART 2/2 of a HACK - select preferred representation by mimetype and size
        //
        if(true || $representation->mimeType == 'image/jpeg'){
          foreach($representation->parts as $part){
            // get media uri conversion rules if the module is installed and activated
            if(module_exists('cdm_mediauri')){
              $muris = cdm_mediauri_conversion($part->uri);
            }
            // --- handle media preview rules
            if(isset($muris['preview'])){

              $a_child = '<img src="'.$muris['preview']['uri'].'" class="preview" '
              .($muris['preview']['size_x'] ? 'width="'.$muris['preview']['size_x'].'"' : '')
              .($muris['preview']['size_y'] ? 'width="'.$muris['preview']['size_y'].'"' : '')
              .'/>';
            } else {
              $a_child = '<img src="'.$part->uri.'" />';
            }

            // --- handle web application rules
            $webapp = '';
            if(isset($muris['webapp'])){
              if($muris['webapp']['embed_html']){
                // embed in same page
                $webapp = $muris['webapp']['embed_html'];
              } else {
                $webapp = l(t('web application'), $muris['webapp']['uri']);
              }
            }
            $media_row .= '<td><a href="'.$part->uri.'" target="'.$part->uuid.'">'.$a_child.'</a></td>';
            $meta_row .= '<td><span class="label">'.check_plain($specimen->titleCache).'</span><div class="webapp">'.$webapp.'</div></td>';
          } // END parts
          //TODO this is PART 2/2 of a hack
          break;
        } // END representations
      } // END media
    }
    $out .= $media_row.'</tr>';
    $out .= $meta_row.'</tr>';

    $out .= '</div></table>';
  }
  return $out;
}

function theme_cdm_featureTrees($mergedTrees, $taxon){
  
  if(!$mergedTrees){
    return;
  }
  
  foreach($mergedTrees as &$mTree){
    //TODO diplay title and reference in case of multiple $mergedTrees -> theme
    $out .= theme('cdm_feature_nodes', $mTree->root->children, $taxon);
  }
  return $out;
}

function theme_cdm_featureTreeTOCs($mergedTrees){
  
  if(!$mergedTrees){
    return;
  }
  //FIXME
  $out = '<div class="featureTOC">';
  $out .= '<h2>' . t('Content') .'</h2>';

  //TODO diplay title and reference in case of multiple $mergedTrees -> theme
  
  foreach($mergedTrees as &$mTree){
    $out .= theme('cdm_feature_nodesTOC', $mTree->root->children);
  }
  
  $out .= '</div>';
  return $out;
}

function theme_cdm_feature_nodes($featureNodes, $taxon){
  
  foreach($featureNodes as $node){
      // process $descriptionElements with content only
      if(is_array($node->descriptionElements) && count($node->descriptionElements) > 0){

        $featureRepresentation = isset($node->feature->representation_L10n) ? $node->feature->representation_L10n : 'Feature';

        $block->module = 'cdm_dataportal';
        
        if($node->feature->uuid != UUID_IMAGE){
          $block->delta = generalizeString($featureRepresentation);
          $block->subject = theme('cdm_feature_name', $featureRepresentation);
          $block->module = "cdm_dataportal-feature";

          $block->content = theme('cdm_descriptionElements', $node->descriptionElements, $block->delta);
          
          // set anchor; FIXME put anchor in $block->subject
          $out .= '<a name="'.$block->delta.'"></a>';
          $out .= theme('block', $block);

          // TODO HACK
          if($node->feature->uuid == UUID_DISTRIBUTION){
            $out .= theme('cdm_descriptionElements_distribution', $taxon);
          }
        }
      }
      // theme 
      if(count($node->children) > 0){
        $out .= '<div class="nested_description_elements">';
        $out .= theme('cdm_feature_nodes', $node->children, $taxon);
        $out .= '</div>';
      }
  }
  return $out;
}


function theme_cdm_feature_nodesTOC($featureNodes){

  $out .= '<ul>';
  
  foreach($featureNodes as $node){
    // process $descriptionElements with content only
    if(is_array($node->descriptionElements) && count($node->descriptionElements) > 0){

      $featureRepresentation = isset($node->feature->representation_L10n) ? $node->feature->representation_L10n : 'Feature';
      // HACK to implement images for taxa, should be removed
      if($node->feature->uuid != UUID_IMAGE){
        $out .= '<li>'.l(t(ucfirst($featureRepresentation)), $_GET['q'], array("class"=>"toc"), NULL, generalizeString($featureRepresentation)).'</li>';
      }
    }
  }

  $out .= '</ul>';
  return $out;
}

function theme_cdm_feature_name($feature_name){
  //TODO replace by using translations ?
  switch($feature_name){
    default: return t(ucfirst($feature_name));
  }
}

function theme_cdm_descriptionElements($descriptionElements){
  
  $outArray = array();
  $glue = '';
  $sortOutArray = false;
  $enclosingHtml = 'ul';
  
  foreach($descriptionElements as $descriptionElement){
    
    if($descriptionElement->feature->uuid == UUID_DISTRIBUTION){
       if($descriptionElement->class == 'Distribution'){
          $repr = $descriptionElement->area->representation_L10n;
       } else if($descriptionElement->class == 'TextData'){
         $repr = $descriptionElement->multilanguageText_L10n->text;
       }
      if( !array_search($repr, $outArray)){
        $outArray[] = $repr;
        $glue = ', ';
        $sortOutArray = true;
        $enclosingHtml = 'p';
      }
    } else if($descriptionElement->class == 'TextData'){
      $outArray[] = theme('cdm_descriptionElementTextData', $descriptionElement);
    } else {
      $outArray[] = '<li>No method for rendering unknown description class: '.$descriptionElement->classType.'</li>';
    }
    
  }
  // take the feature of the last $descriptionElement
  $feature = $descriptionElement->feature;
  return theme('cdm_descriptionElementArray', $outArray, $feature, $glue, $sortOutArray, $enclosingHtml);
}

function theme_cdm_descriptionElementArray($elementArray, $feature, $glue = '', $sortArray = false, $enclosingHtml = 'ul'){
  $out = '<'.$enclosingHtml.' class="description" id="'.$feature->representation_L10n.'">';
  
  if($sortArray) sort($elementArray);
  
  $out .= join($elementArray, $glue);
  
  $out .= '</'.$enclosingHtml.'>';
  return $out;
}

function theme_cdm_descriptionElementTextData($element){

  $description = str_replace("\n", "<br/>", $element->multilanguageText_L10n->text);
  $referenceCitation = '';
  if($element->citation){
      $fullCitation = $element->citation->authorTeam->titleCache;
      if(isset($element->citation->datePublished->start)) {
        $fullCitation .= ', '.partialToYear($element->citation->datePublished->start);
      }
      
      $referenceCitation = l('<span class="reference">'.$fullCitation.'</span>', path_to_reference($reference->uuid), array("class"=>"reference"), NULL, NULL, FALSE ,TRUE);
      $referenceCitation = $referenceCitation;
      if($element->citationMicroReference){
        $referenceCitation .= ': '. $element->citationMicroReference;
      }
      if($description && strlen($description) > 0){
        $referenceCitation = '; '.$referenceCitation;
      }
  }
  return '<li class="descriptionText">' . $description . $referenceCitation.'</li>';
}

function theme_cdm_search_results($pager, $path, $parameters){

  drupal_set_title(t('Search Results'));

  $out = ''; //l('Advanced Search', '/cdm_dataportal/search');
  if(count($pager->records) > 0){
    $out .= theme('cdm_list_of_taxa', $pager->records);
    $out .= theme('cdm_pager_new', $pager, $path, $parameters);
  } else {
    $out = '<h4 class="error">Sorry, no matching entries found.</h4>';
  }
  return $out;
}


function theme_cdm_pager_new(&$pager, $path, $parameters, $neighbors = 2){
  //FIXME take advantage of new pager class => nearly no calcualations needed !!!
  $out = '';

  if ($pager->pagesAvailable > 1) {

    $viewportsize = $neighbors * 2 + 1;
    if($pager->pagesAvailable <= $viewportsize){
      $viewportsize = $pager->pagesAvailable;
    }

    $out .= '<div class="pager">';
    if($pager->currentIndex > 1){
      $out .= theme('cdm_pager_link_new', t('« first'), 1,  $pager, $path, $parameters, array('class' => 'pager-first'));
      $out .= theme('cdm_pager_link_new', t('‹ previous'), $pager->currentIndex - 1, $pager, $path, $parameters, array('class' => 'pager-previous'));
    }

    if($pager->pagesAvailable <= $viewportsize || $pager->currentIndex <= $neighbors){
      $first_number = 1;
    } else if($pager->currentIndex >= $pager->pagesAvailable - $neighbors){
      $first_number = $pager->pagesAvailable - $viewportsize;
    } else {
      $first_number = $pager->currentIndex - $neighbors;
    }

    if($first_number > 1){
      $out .= '<div class="pager-list-dots-left">...</div>';
    }


    for($i = $first_number; ($i == $pager->pagesAvailable) || ($i < $first_number + $viewportsize); $i++){
      $out .= theme('cdm_pager_link_new', $i, $i,  $pager, $path, $parameters, array('class' => 'pager-first'));
    }
    if($i < $pager->pagesAvailable){
      $out .= '<div class="pager-list-dots-right">...</div>';
    }

    if($pager->currentIndex < $pager->pagesAvailable){
      $out .= theme('cdm_pager_link_new', t('next ›'), $pager->currentIndex + 1, $pager, $path, $parameters, array('class' => 'pager-next'));
      $out .= theme('cdm_pager_link_new', t('last »'), $pager->pagesAvailable, $pager, $path, $parameters, array('class' => 'pager-last'));
    }
    $out .= '</div>';

    return $out;
  }
}

function theme_cdm_pager_link_new($text, $linkPageNumber, &$pager, $path, $parameters = array(), $attributes) {

  $out = '';

  if ($linkPageNumber == $pager->currentIndex) {
    $out = '<strong>'.$text.'</strong>';
  } else {
    $out = l($text, $path.$linkPageNumber, $attributes /*, compose_url_prameterstr($parameters)*/);
  }
  return $out;
}


function theme_cdm_pager_link($text, $linkPageNumber, &$resultPageSTO, $path, $parameters = array(), $attributes) {

  $out = '';

  if ($linkPageNumber == $resultPageSTO->pageNumber) {
    $out = '<strong>'.$text.'</strong>';
  } else {
    // <a class="pager-next active" title="Go to page 3" href="/node?page=2">3</a>
    $parameters['page'] = $linkPageNumber;
    $out = l($text, $path, $attributes, compose_url_prameterstr($parameters));
  }


  return $out;
}
