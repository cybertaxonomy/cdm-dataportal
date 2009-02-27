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
function theme_cdm_annotation($baseTO){
  if(!$baseTO->uuid){
    return;
  }else{


    drupal_add_js(drupal_get_path('module', 'cdm_dataportal').'/js/cdm_annotations.js');
    drupal_add_js(drupal_get_path('module', 'cdm_dataportal').'/js/jquery.form.js');

    $annotatableUuid = $baseTO->uuid;
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

/**
 * Renders the full name string (complete scientific name including the author team)
 *
 * @param NameTO $nameTO the taxon name
 * @param $displayNomRef values are 'LINK', 'PLAIN', 'HIDE' = FALSE
 */
function theme_cdm_name($nameTO, $displayAuthor = true, $displayNomRef = true, $displayStatus = true, $displayDescription = true, $nomRefLink = true){

  if(!$nameTO){
    return '<span class="error">Invalid $nameTO in theme_cdm_name()</span>';
  }

  /* TODO: - take the different sub types of eu.etaxonomy.cdm.model.name.TaxonNameBase into account?
   * 
   * Use class names from objects?, additional field in DTO?
   * Preliminary using default value, which can be set in module settings:
   * values correspond with eu.etaxonomy.cdm.model.name.*: 'ZoologicalName', 'ViralName', 'BotanicalName'
   */
  $taxonname_type = (variable_get('cdm_taxonname_type', 'BotanicalName'));
  $class = 'taxonname taxonname_'.strtolower($taxonname_type);
  
  $hasNomRef = $nameTO->nomenclaturalReference->fullCitation;
   
  if(!$nameTO->taggedName || !count($nameTO->taggedName)){
    $out .= '<span class="'.$class.'">'.$nameTO->fullname.'</span>';
  } else {
    $skip = $hasNomRef ? array('reference') : array();
    if(!$displayAuthor || $taxonname_type == 'ZoologicalName') {
      $skip[] = 'authors';
    } 
    $out .= '<span class="'.$class.'">'.theme('cdm_taggedtext2html', $nameTO->taggedName, 'span', ' ', $skip).'</span>';
  }
  
  if($displayNomRef && $hasNomRef){
    $out .= '<span class="reference">';
    $out .= theme('cdm_nomenclaturalReferenceSTO', $nameTO->nomenclaturalReference, $nomRefLink);
    $out .= '</span>';
  }

  if($displayStatus){
    if(isset($nameTO->status[0])){
      foreach($nameTO->status as $key => $status){
        $out .= ', '.$status->term;
      }
    }
  }

  // render protologues etc...
  if($displayDescription && !empty($nameTO->descriptions)){
    foreach($nameTO->descriptions as $DescriptionTO){
    		if(!empty($DescriptionTO)){
    		  foreach($DescriptionTO->elements as $DescriptionElementSTO){
    		    $out .= theme("cdm_media", $DescriptionElementSTO, array('application/pdf', 'image/png', 'image/jpeg', 'image/gif', 'text/html'));
    		  }
    		}
    }
  }

  // testing annotations for taxon names
  //$out .= theme('cdm_annotation', $nameTO);

  return $out;
}

function theme_cdm_media($DescriptionElementSTO, $mimeTypePreference){
  $out = "";

  _add_js_thickbox();

  $uuid = $DescriptionElementSTO->uuid;
  $type = $DescriptionElementSTO->mediaType;
  $medias = $DescriptionElementSTO->media;

  foreach($medias as $media){
     
    $prefRepresentations = cdm_preferred_media_representations($media, $mimeTypePreference, 300, 400);
    $representation_inline = array_shift($prefRepresentations);
    if($representation_inline) {

      $contentTypeDirectory = substr($representation_inline->mimeType, 0, stripos($representation_inline->mimeType, '/'));

      $out = theme('cdm_media_mime_' . $contentTypeDirectory,  $representation_inline, $type);

      //			$attributes = array('class'=>'thickbox', 'rel'=>'descriptionElement-'.$uuid, 'title'=>$type->term);
      //		    for($i = 0; $part = $representation_inline->representationParts[$i]; $i++){
      //		    	if($i == 0){
      //		    	    $image_url = drupal_get_path('module', 'cdm_dataportal').'/images/'.$type->term.'-media.png';
      //		    	    $media = '<img src="'.$image_url.'" height="14px" alt="'.$type->term.'" />';
      //		    	    $out .= l($media, $part->uri, $attributes, NULL, NULL, TRUE, TRUE);
      //		    	} else {
      //		    		$out .= l('', $part->uri, $attributes, NULL, NULL, TRUE);
      //		    	}
      //		  	}
    } else {
      // no media available, so display just the type term
      $out .=  $type->term;
    }
  }
  return $out;

}

function theme_cdm_mediaTypeTerm($type){
  $icon_url = drupal_get_path('module', 'cdm_dataportal').'/images/'.$type->term.'-media.png';
  return '<img src="'.$icon_url.'" height="14px" alt="'.$type->term.'" />';
}

function theme_cdm_media_mime_application($representation, $type){

  foreach($representation->representationParts as $part){
    $attributes = array('title'=>$type->term, 'target'=>'_blank');
    $out .= l(theme('cdm_mediaTypeTerm', $type), $part->uri, $attributes, NULL, NULL, TRUE, TRUE);
  }
  return $out;
}

function theme_cdm_media_mime_image($representation, $type){
  $out = '';
  $attributes = array('class'=>'thickbox', 'rel'=>'representation-'.$representation->uuid, 'title'=>$type->term);
  for($i = 0; $part = $representation->representationParts[$i]; $i++){
    if($i == 0){
       
      $out .= l(theme('cdm_mediaTypeTerm', $type), $part->uri, $attributes, NULL, NULL, TRUE, TRUE);
    } else {
    		$out .= l('', $part->uri, $attributes, NULL, NULL, TRUE);
    }
  }
  return $out;
}

function theme_cdm_media_mime_text($representation, $type){

  foreach($representation->representationParts as $part){
    $attributes = array('title'=>$type->term . t(' link will open in a new window'), 'target'=>'_blank');
    $out .= l(theme('cdm_mediaTypeTerm', $type), $part->uri, $attributes, NULL, NULL, TRUE, TRUE);
  }
  return $out;
}

/**
 * TODO
 * Quick-and-dirty solution to show distribution service to exemplar groups
 *
 * @param unknown_type $featureTo
 * @return unknown
 */
function theme_cdm_descriptionElement_distribution($featureTo){

  $server = variable_get('cdm_dataportal_geoservice_access_point', false);

  if(!server){
    return "<p>No geoservice specified</p>";
  }else{
    $parameters = '?' . $featureTo->externalResources->geoServiceParameters;

    $display_width = variable_get('cdm_dataportal_geoservice_display_width', false);
    $bounding_box = variable_get('cdm_dataportal_geoservice_bounding_box', false);

    $serviceUrl = $server . $parameters . ($display_width ? '&ms=' . $display_width: '') . ($bounding_box ? '&bbox=' .  $bounding_box : '');

    $out .= '<img style="border: 1px solid #ddd" src="'.$serviceUrl.'" alt="Distribution Map" />';

    return $out;
  }
}

/**
 * Renders the given TaxonTO. The $enclosingTag (if not set false)
 * will get the following class attributes:
 * - name
 * - accepted (only if $ptaxon is an accepted name)
 *
 * @param TaxonTO $taxon
 * @param boolean $displayNomRef whether to display the nomenclatural reference
 * @param boolean $noSecundum defaults to false. If set to true the secundum part is omitted.
 * @param string $enclosingTag defaults to span.
 * @return string of XHTML
 *
 * usage: taxon_detail, theme_ptname_link
 */
function theme_cdm_taxon($taxonTO, $displayNomRef = true, $displayStatus = true, $displayDescription = true, $noSecundum = true, $enclosingTag = 'span', $uuidAnchor = TRUE, $nomRefLink = true){

  $refSecundum = false;
  if(!$noSecundum){
    $ref_sec = cdm_ws_get(CDM_WS_SIMPLE_REFERENCE ,$taxonTO->secUuid);
    if($ref_sec){
      $refSecundum = str_trunk($ref_sec->fullCitation, 40, '...');
    }
  }
  $out  = theme('cdm_name', $taxonTO->name, true, $displayNomRef, $displayStatus, $displayDescription, $nomRefLink);
  // append secundum information
  $out .=($refSecundum ? '&nbsp;<span class="secundum">sec. '.$refSecundum.'</span>' : '');
  // add uuid anchor
  if($uuidAnchor === TRUE){
    $out = uuid_anchor($taxonTO->uuid, $out);
  }
  //TODO:   .$ptaxon->namePhrase;
  if($enclosingTag){
    $out = '<'.$enclosingTag.' class="taxon'.($taxonTO->accepted === true ? ' accepted':'').'">'.$out.'</'.$enclosingTag.'>';
  }

  return $out;
}

/**
 * Renders a link to the taxon detail page for the given $taxon
 *
 * @param TaxonTO $taxon
 */
function theme_cdm_taxon_link($taxonTO, $fragment = NULL, $showNomRef = true, $nomRefLink = true){

  if($fragment){
    $fragment = '#'.$fragment;
  }

  if(!$taxon->accepted) {
    $out = 'ERROR: theme_cdm_taxon_link() - taxon is not accepted';
  }

   if($showNomRef && $nomRefLink){
     // name part as link and reference part as link
     $name_html = theme('cdm_taxon', $taxonTO, FALSE /*$showNomRef*/, true, false, true, '', FALSE);
     $out = l($name_html, cdm_dataportal_taxon_path($taxonTO->uuid), array('class'=>'accepted'), null, $fragment, FALSE, TRUE);
     $out .= theme('cdm_nomenclaturalReferenceSTO', $taxonTO->name->nomenclaturalReference);
   } else {
     // fullname as link
     $name_html = theme('cdm_taxon', $taxonTO, $showNomRef, true, false, true, '', FALSE, $nomRefLink);
     $out = l($name_html, cdm_dataportal_taxon_path($taxonTO->uuid), array('class'=>'accepted'), null, $fragment, FALSE, TRUE);
   }
  
  return $out;
}

/**
 * Renders a link to the misapplied taxon detail page for the given $taxon
 *
 * @param TaxonTO $taxon
 */
function theme_cdm_taxon_misapplied_link($taxonTO, $fragment = NULL, $showNomRef = true){

  if($fragment){
    $fragment = '#'.$fragment;
  }

  if(!$taxon->accepted) {
    $out = 'ERROR: theme_cdm_taxon_link() - taxon is not accepted';
  }

  $name_html = theme('cdm_taxon', $taxonTO, $showNomRef, true, false, true, '', FALSE);
  $out = '"';
  $out .= l($name_html, cdm_dataportal_taxon_path($taxonTO->uuid), array('class'=>'accepted'), null, $fragment, FALSE, TRUE);
  $out .= '"';

  /*
   if($showNomRef){
   $out .= theme('cdm_nomenclaturalReferenceSTO', $taxonTO->name->nomenclaturalReference);
   }
   */
  return $out;
}


/**
 * Renders a link to the taxon detail page for the given $taxon
 *
 * @param TaxonTO $taxon
 */
function theme_cdm_synonym_link($taxonTO, $accepted_uuid, $showNomRef = true, $showStatus = true){

  $name_html = theme('cdm_taxon', $taxonTO, $showNomRef, $showStatus, false, true, '', FALSE);
  $out = l($name_html, cdm_dataportal_taxon_path($accepted_uuid), array('class'=>'synonym'), 'highlite='.$taxonTO->uuid.'&acceptedFor='.$taxonTO->uuid, NULL, FALSE, TRUE);

  /*
   if($showNomRef){
   $out .= theme('cdm_nomenclaturalReferenceSTO', $taxonTO->name->nomenclaturalReference);
   }

   if($showStatus){
   $out .= theme('cdm_nomenclaturalStatusSTO', $taxonTO->name->status, "nomStatus");
   }*/

  return $out;
}


function theme_cdm_related_taxon($taxonSTO, $reltype_uuid = '', $displayNomRef = true){

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

  $out = '<span class="relation_sign">'.$relsign.'</span>'.$name_prefix.theme('cdm_taxon',$taxonSTO, $displayNomRef).$name_postfix;
  return $out;

}


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

function theme_cdm_list_of_taxa($taxonSTOs){

  $out = '<ul class="cdm_names" style="background-image: none;">';

  $synonym_uuids = array();
  foreach($taxonSTOs as $taxon){
    if(!_cdm_dataportal_acceptedByCurrentView($taxon)){
      if(!array_key_exists($taxon->uuid, $synonym_uuids)){
        $synonym_uuids[$taxon->uuid] = $taxon->uuid;
      }
    }
  }
  $table_of_accepted = cdm_ws_get(CDM_WS_ACCEPTED_TAXON, join(',', $synonym_uuids));

  foreach($taxonSTOs as $taxon){
    if(_cdm_dataportal_acceptedByCurrentView($taxon)){
      $out .= '<li>'.theme('cdm_taxon_link', $taxon, /*$fragment*/ NULL, /*$showNomRef*/ true, /*$nomRefLink*/ false ).'</li>';
    } else {
      $uuid = $taxon->uuid;
      $acceptedTaxa = $table_of_accepted->$uuid;
      if(count($acceptedTaxa) == 1){
        $out .= '<li>'.theme('cdm_synonym_link', $taxon, $acceptedTaxa[0]->uuid ).'</li>';
      } else {
        //TODO avoid using AHAH ion the cdm_dynabox
        $out .= theme('cdm_dynabox', theme('cdm_name', $taxon->name), cdm_compose_url(CDM_WS_ACCEPTED_TAXON, array($taxon->uuid)), 'cdm_list_of_taxa');
      }
    }
  }
  $out .= '</ul>';
  return $out;



  /*
   //  drupal_add_js(drupal_get_path('module', 'cdm_dataportal').'/js/cdm_dynabox.js');
   //  drupal_add_css(drupal_get_path('module', 'cdm_dataportal').'/cdm_dataportal.css');
   //
   $out = '<ul class="cdm_names" style="background-image: none;">';

   //get all accepted for synonyms in one blow to reduce overhead
   /* disabled! Not implemented in cdmlib-remote. Did someone forget
   * to commit???
   $synonym_uuids = array();
   foreach($taxonSTOs as $taxon){
   if(!_cdm_dataportal_acceptedByCurrentView($taxon)){
   if(!array_key_exists($taxon->uuid, $synonym_uuids)){
   $synonym_uuids[$taxon->uuid] = $taxon->uuid;
   }
   }
   }
   $table_of_accepted = cdm_ws_get(CDM_WS_ACCEPTED_TAXON, join(',', $synonym_uuids));
   */
  /*
   // get the secUuid of the actual taxonomy.
   // TODO: this is not generic and can be applied if there is one taxonomy only
   $root = cdm_ws_get(CDM_WS_TREENODE_ROOT);
   $rootSecUuid = $root[0]->secUuid;

   // RUDE HACK: group misappliedNames by filtering identical names but differing secUuids.
   $misapplied = array();
   foreach($taxonSTOs as $taxon){
   if($taxon->secUuid != $rootSecUuid){
   // is misapplied name
   if(! in_array($taxon->name->uuid, $misapplied)){
   $misapplied[$taxon->name->uuid]['printed'] = 0;
   }
   }
   }

   foreach($taxonSTOs as $taxon){
   if(_cdm_dataportal_acceptedByCurrentView($taxon)){

   if($taxon->secUuid == $rootSecUuid){
   $out .= '<li>'.theme('cdm_taxon_link', $taxon).'</li>';
   }
   else if(! $misapplied[$taxon->name->uuid]['printed']){
   // RUDE HACK for misapplied names, paging not correct with this
   $out .= '<li>'.theme('cdm_taxon_misapplied_link', $taxon).'</li>';
   $misapplied[$taxon->name->uuid]['printed'] = 1;

   }
   }
   else {
   // get accepted taxa for this taxon
   $table_of_accepted = cdm_ws_get(CDM_WS_ACCEPTED_TAXON, $taxon->uuid);
   if(count($table_of_accepted) == 1){
   $out .= '<li>'.theme('cdm_synonym_link', $taxon, $table_of_accepted[0]->uuid ).'<li>';
   }
   else {
   //TODO avoid using AHAH ion the cdm_dynabox
   $out .= theme('cdm_dynabox', theme('cdm_name', $taxon->name), cdm_compose_url(CDM_WS_ACCEPTED_TAXON, array($taxon->uuid)), 'cdm_list_of_taxa');
   }
   }
   }
   $out .= '</ul>';
   return $out;*/
}

function theme_cdm_alternative_taxa($taxonSTOs){
  $out = '<ul class="cdm_names" style="background-image: none;">';
  foreach($taxonSTOs as $taxon){
    $out .= '<li>'.theme('cdm_taxon_link', $taxon).'</li>';
  }
  $out .= '</ul>';
  return $out;
}


function theme_cdm_credits(){
  $secRef_array = _cdm_dataportal_currentSecRef_array();
  return '<span class="sec_reference_citation">'.$secRef_array['citation'].'</span>'
  .( $secRef_array['year'] ? ' <span class="year">'.$secRef_array['year'].'</span>' : '')
  .( $secRef_array['authorship'] ? '<div class="author">'.$secRef_array['authorship'].'</div>' : '');
}


function theme_cdm_fullreference($referenceTO, $link = FALSE){

  if($referenceTO->fullCitation){
    $out = $referenceTO->fullCitation;
  }else{
    $out = $referenceTO->authorship;
  }
  if($referenceTO->microReference){
    $out .= ' : '.$referenceTO->microReference;
  }
  /*
   if($referenceTO->year){
   $out .= '. '.$referenceTO->year;
   }
   */
  
  if($link){
   $out = l($out, "/cdm_dataportal/reference/".$referenceTO->uuid, array("class"=>"reference"));
  }
  return $out;
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
function theme_cdm_nomenclaturalReferenceSTO($referenceSTO, $doLink = FALSE, $cssClass = '', $separator = '<br />' , $enclosingTag = 'li'){

  if(isset($referenceSTO->microReference)){
    // it is a ReferenceTO
    $nomref_citation = theme('cdm_fullreference', $referenceSTO);
  } else {
    // it is ReferenceSTO
    $nomref_citation = $referenceSTO->fullCitation;
  }
  
  $is_IN_reference = str_beginsWith($nomref_citation, 'in');

  if($doLink){
    $nomref_citation = l($nomref_citation, "/cdm_dataportal/reference/".$referenceSTO->uuid, array(), NULL, NULL, FALSE, TRUE);      
  }
  
  if(!empty($nomref_citation)){
    $nomref_citation = ($is_IN_reference ? '&nbsp;':',&nbsp;') . $nomref_citation;
  }
  
  return $nomref_citation;
}

/**
 * Enter description here...
 *
 * @param unknown_type $statusSTO
 * @param unknown_type $cssClass
 * @param unknown_type $enclosingTag
 * @return unknown
 */
function theme_cdm_nomenclaturalStatusSTO($statusSTO, $cssClass = '', $enclosingTag = 'span'){

  $out = "<$enclosingTag" . ($cssClass == '' ? '' : ' class="' . $cssClass . '"') . ">";
  foreach ($statusSTO as $status){
    $out .= ", " . $status->term;
  }
  $out .= "</$enclosingTag>";

  return $out;
}

/**
 * default title for a taxon page
 *
 * @param NameTO $nameTO
 * @return the formatted taxon name
 */
function theme_cdm_taxon_page_title($nameTO){
  
  if(variable_get('cdm_dataportal_nomref_in_title', 1)){
    // taxon name only with author and year
    return theme('cdm_name', $nameTO, TRUE, false, false, false); 
  } else {
    return theme('cdm_name', $nameTO);
  }
}

function theme_cdm_acceptedFor(){
  $out = '';
  
  if(isset($_REQUEST['acceptedFor'])){
     
     $synonym = cdm_ws_get(CDM_WS_TAXON, $_REQUEST['acceptedFor']);
     
     if($synonym){
       $out .= '<span class="acceptedFor">';
       $out .= t('is accepted for ');
       $out .= theme('cdm_name', $synonym->name, TRUE, FALSE, FALSE, FALSE);
       $out .= '</span>';
     }
  }
  
  return $out;
}

function theme_cdm_back_to_search_result_button(){
  $out = '';
  if($_SESSION['cdm']['search']){
    $out .= '<div id="backButton">'.l(t('Back to search result'), $_SESSION['cdm']['last_search'] ).'</div>';
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
function theme_cdm_taxon_page_general($taxonTO, $page_part) {
  
  if(!$page_part){
    $page_part = 'description';
  }
  $page_part = variable_get('cdm_dataportal_taxonpage_tabs', 1) ? $page_part : 'all';  

  $out = '';
  $out .= theme('cdm_acceptedFor');
  $out .= theme('cdm_back_to_search_result_button');
  
  if($page_part == 'description' || $page_part == 'all'){
    $out .= '<div id="general">';
    $out .= theme('cdm_taxon_page_description', $taxonTO);
    $out .= '</div>';
  }
  
  if($page_part == 'images' || $page_part == 'all'){
    $out .= '<div id="images">';
    if($page_part == 'all'){
      $out .= '<h2>'.t('Images').'</h2>';
    }
    $out .= theme('cdm_taxon_page_images', $taxonTO);
    $out .= '</div>';
  }

  if($page_part == 'synonymy' || $page_part == 'all'){
    $out .= '<div id="synonymy">';
    if($page_part == 'all'){
      $out .= '<h2>'.t('Synonymy').'</h2>';
    }
    if(!variable_get('cdm_dataportal_nomref_in_title', 1)){
      $out .= theme('cdm_name', $taxonTO->name);
    }
    $out .= theme('cdm_taxon_page_synonymy', $taxonTO);

    if(variable_get('cdm_dataportal_display_name_relations', 1)){
      $out .= theme('cdm_nameRelations', $taxonTO->name->nameRelations);
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
function theme_cdm_taxon_page_description($taxonTO){
  
  // description TOC
  $out .= theme('cdm_featureTreeToc', $taxonTO->featureTree);
  // description 
  $out .= theme('cdm_featureTree', $taxonTO->featureTree);
  return $out;
}

/**
 * Show whole synonymy for the accepted taxon. Synonymy list is headed by the complete scientific name
 * of the accepted taxon with nomenclatural reference.
 *
 */
function theme_cdm_taxon_page_synonymy($taxonTO){
  
  $out .= theme('cdm_homotypicSynonyms', $taxonTO->homotypicSynonyms, $taxonTO->typeDesignations);

  foreach($taxonTO->heterotypicSynonymyGroups as $HomotypicTaxonGroupSTO){
    $out .= theme('cdm_heterotypicSynonymyGroup', $HomotypicTaxonGroupSTO);
  }
  $out .= theme('cdm_taxonRelations', $taxonTO->taxonRelations);

  return $out;
}

/**
 * Show the collection of images stored with the accepted taxon
 * TODO
 */
function theme_cdm_taxon_page_images($taxonTO){

  $descriptions = $taxonTO->featureTree->descriptions;
  foreach($descriptions as $descriptionTo){
    $features = $descriptionTo->features;
    foreach($features as $featureTo){
      if($featureTo->feature->term == 'Image'){
        $flashLink = count($featureTo->descriptionElements) > 0;
        break;        
      }
    }
  }
  
  if($flashLink){
    
    $taggedName = $taxonTO->name->taggedName;
    
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
      $out .= '<li>'.theme('cdm_fullreference', $reference, TRUE).'</li>';
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

function theme_cdm_preferredImage($taxonTo, $defaultImage, $parameters = ''){

  $descriptions = $taxonTo->featureTree->descriptions;

  foreach($descriptions as $descriptionTo){
    $features = $descriptionTo->features;
    foreach($features as $featureTo){
      if($featureTo->feature->term == 'Image'){

        $preferredImage = $featureTo->descriptionElements[0]->media[0]->representations[0]->representationParts[0]->uri;
      }
    }
  }

  $image = $preferredImage ? $preferredImage . $parameters : $defaultImage;
  $out = '<img class="left" src="'.$image.'" alt="no image available" />';
  return $out;
}




function theme_cdm_homotypicSynonyms($synonymRelationshipTOs, $typeDesignations = false, $prependedSynonyms = array()){

  $out = '';
  $out = '<ul class="homotypicSynonyms">';

  if(!empty($prependedSynonyms)){
    foreach($prependedSynonyms as $taxon){
      $out .= '<li class="synonym">'.theme('cdm_related_taxon', $taxon, UUID_HOMOTYPIC_SYNONYM_OF).'</li>';
    }
  }

  foreach($synonymRelationshipTOs as $synonym){
    $out .= '<li class="synonym">'.theme('cdm_related_taxon', $synonym->synonym, UUID_HOMOTYPIC_SYNONYM_OF).'</li>';
  }
  if($typeDesignations){
    $out .= theme('cdm_typedesignations', $typeDesignations);
  }

  $out .= '</ul>';
  return $out;
}

function theme_cdm_homotypicSynonymLine($taxonSTO){
  $out = '';
  $out .= '<li class="synonym">'.theme('cdm_related_taxon', $taxonSTO, UUID_HOMOTYPIC_SYNONYM_OF).'</li>';
  return $out;
}

function theme_cdm_heterotypicSynonymyGroup($HomotypicTaxonGroupSTO){
  $out = '';
  $out = '<ul class="heterotypicSynonymyGroup">';

  $is_first_entry = true;
  foreach($HomotypicTaxonGroupSTO->synonyms as $SynonymRelationshipTO){
    if($is_first_entry){
      $is_first_entry = false;
      // is first list entry
      $out .= '<li class="firstentry synonym">'.theme('cdm_related_taxon',$SynonymRelationshipTO->synonym, $SynonymRelationshipTO->typeUuid).'</li>';
    } else {
      $out .= '<li class="synonym">'.theme('cdm_related_taxon',$SynonymRelationshipTO->synonym, UUID_HOMOTYPIC_SYNONYM_OF).'</li>';
    }
  }

  if(isset($HomotypicTaxonGroupSTO->typeDesignations)){
    $out .= theme('cdm_typedesignations', $HomotypicTaxonGroupSTO->typeDesignations);
  }

  $out .= '</ul>';

  return $out;
}

/**
 * renders misapplied names and invalid designations.
 * Both relation types are currently treated the same!
 *
 * @param unknown_type $TaxonRelationshipTOs
 * @return unknown
 */
function theme_cdm_taxonRelations($TaxonRelationshipTOs){

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
  foreach($TaxonRelationshipTOs as $taxonRelation){
    if(true || $taxonRelation->type->uuid == UUID_MISAPPLIED_NAME_FOR || $taxonRelation->type->uuid == UUID_INVALID_DESIGNATION_FOR ){

      $sensu_reference_list = cdm_ws_get(CDM_WS_SIMPLE_REFERENCE ,$taxonRelation->taxon->secUuid);
      $sensu_reference = $sensu_reference_list[0];
      $name = $taxonRelation->taxon->name->fullname;
      if(!isset($misapplied[$name])){
        $misapplied[$name]['out'] = '<span class="misapplied">'.theme('cdm_related_taxon',$taxonRelation->taxon, UUID_MISAPPLIED_NAME_FOR, false).'</span>';
      }

      // collect all authors for this fullname
      $misapplied[$name]['authorship'][$sensu_reference->authorship] = '&nbsp;<span class="sensu cluetip no-print" title="|sensu '.htmlspecialchars(theme('cdm_fullreference',$sensu_reference )).'|">sensu '
      .$sensu_reference->authorship.'</span>'
      .'<span class="reference only-print">sensu '.theme('cdm_fullreference',$sensu_reference ).'</span>'
      ;

      /*
       $misapplied[$name]['out'] .= '&nbsp;<span class="sensu cluetip no-print" title="|sensu '.htmlspecialchars(theme('cdm_fullreference',$sensu_reference )).'|">sensu '
       .$sensu_reference->authorship.'</span>'
       .'<span class="reference only-print">sensu '.theme('cdm_fullreference',$sensu_reference ).'</span>'
       ;*/

    }
  }
   
  // generate output
  $out = '<ul class="misapplied">';
  foreach($misapplied as $misapplied_name){
    $out .= '<li class="synonym">'.$misapplied_name['out'] . " ";
    // sorting authors
    ksort($misapplied_name['authorship']);
    $out .= join('; ', $misapplied_name['authorship']);
    $out .= '</li>';
  }
  $out .= '</ul>';
  return $out;
}

function theme_cdm_nameRelations($NameRelationshipTOs){

  $out = '';

  foreach($NameRelationshipTOs as $NameRelationshipTO){

    $block->module = 'cdm_dataportal';

    $type = isset($NameRelationshipTO->type->term) ? $NameRelationshipTO->type->term : 'Name relation';

    $block->delta  = $type;
    $block->subject = t(ucfirst($block->delta));
    $block->delta = str_replace(' ', '_', strtolower($block->delta));

    $block->content = '<ul class="nameRelationships '.$block->delta.'">';
    $relatedNames = array();
    foreach($NameRelationshipTO->relatedNames as $name){
      $relatedNames[] = theme('cdm_name', $name);
    }

    $block->content .= implode(', ', $relatedNames);

    $out .= theme('block', $block);

  }
  return $out;
}

/**
 * FIXME this definitively has to be in another spot. just didn't know where to put it right now.
 *
 * @param String $a 	a typeDesignation status
 * @param String $b		another typeDesignation status
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

  $aQuantifier = array_search($a->status->text, $typeOrder);
  $bQuantifier = array_search($b->status->text, $typeOrder);

  if ($aQuantifier == $bQuantifier) {
    // sort alphabetically
    return ($a->status->text < $b->status->text) ? -1 : 1;
  }
  return ($aQuantifier < $bQuantifier) ? -1 : 1;

}

function theme_cdm_typedesignations($typeDesignations = array()){

  $out = '<ul class="typeDesignations">';

  $specimenTypeDesignations = array();
  foreach($typeDesignations as $variant => $typeDesignation){
    if($typeDesignation->typeSpecimen){
      // specimenTypeDesignations should be ordered. collect theme here only
      $specimenTypeDesignations[] = $typeDesignation;
    }else {
      if($typeDesignation->notDesignated){
        $out .= '<li class="nameTypeDesignation"><span class="status">Type</span>: '.t('not designated'). '</li>';
      }else if($typeDesignation->typeSpeciesName){
        $out .= '<li class="nameTypeDesignation"><span class="status">Type</span>: '.theme('cdm_name', $typeDesignation->typeSpeciesName, false);
        $out .= '</li>';
      }
    }
  }

  if(!empty($specimenTypeDesignations)){
    // sorting might be different for dataportals so this has to be parameterized
    usort($specimenTypeDesignations, "compare_specimenTypeDesignationStatus");
    foreach($specimenTypeDesignations as $std){

      $typeReference = '';
      if($std->reference){
        $typeReference .= '&nbsp;(' . t('designated by');
        $typeReference .= '&nbsp;<span class="typeReference cluetip no-print" title="|'. htmlspecialchars(theme('cdm_fullreference',$std->reference )) .'|">';
        $typeReference .= $std->reference->authorship . ' ' . $std->reference->year;
        $typeReference .= '</span>';
        $typeReference .= ')';
        $typeReference .= '<span class="reference only-print">(designated by '.theme('cdm_fullreference',$std->reference ).')</span>';
      }

      $out .= '<li class="specimenTypeDesignation">';
      $out .= '<span class="status">'.(($std->status->text) ? $std->status->text : t('Type')) .$typeReference.'</span>: '.$std->typeSpecimen->specimenLabel;
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
          foreach($representation->representationParts as $part){
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
            $meta_row .= '<td><span class="label">'.check_plain($specimen->specimenLabel).'</span><div class="webapp">'.$webapp.'</div></td>';
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




function theme_cdm_featureTree($featureTree){
  /*
   *	->featureTree{
   *		->descriptions{
   *			->DescriptionTo{
   *				->label
   *			    ->sources{}
   *			    ->elements{}
   *				->features{
   *					->FeatureTo{
   *						->descriptionElements{
   *							->DescriptionElementSTO{}
   *						->uuid
   * 						->type
   */

  $descriptions = $featureTree->descriptions;
  foreach($descriptions as $descriptionTO){
    $features = $descriptionTO->features;
    foreach($features as $featureTo){
      $descriptionElements = $featureTo->descriptionElements;
      // process $descriptionElements with content only
      if(is_array($descriptionElements) && count($descriptionElements) > 0){
        $block->module = 'cdm_dataportal';

        $feature = isset($featureTo->feature->term) ? $featureTo->feature->term : 'Feature';

        if($feature != "Image"){
          $block->delta = $feature;
          $block->subject = t(ucfirst($block->delta));
          $block->delta = generalizeString($block->delta);

          //
          $block->content = theme('cdm_descriptionElements', $descriptionElements, $block->delta);
          //
          // set anchor
          $out .= '<a name="'.$block->delta.'"></a>';
          $out .= theme('block', $block);



          // TODO HACK
          if($feature == 'Distribution'){
            //                  ob_start();
            //  				  echo "<pre>";
            //  				  print_r($featureTo->url);
            //  				  echo "</pre>";
            //  				  ob_flush();
            $out .= theme('cdm_descriptionElement_distribution', $featureTo);
          }
        }
      }
    }
  }
  return $out;
}

function theme_cdm_featureTreeToc($featureTree){
  
  $out = '<div class="featureTOC">';
  $out .= '<h2>' . t('Content') .'</h2>';
  $out .= '<ul>';

  $descriptions = $featureTree->descriptions;
  foreach($descriptions as $descriptionTO){
    $features = $descriptionTO->features;
    foreach($features as $featureTo){
      $descriptionElements = $featureTo->descriptionElements;
      // process $descriptionElements with content only
      if(is_array($descriptionElements) && count($descriptionElements) > 0){

        $feature = isset($featureTo->feature->term) ? $featureTo->feature->term : 'Feature';
        // HACK to implement images for taxa, should be removed
        if($feature != 'Image'){
          $out .= '<li>'.l(t(ucfirst($feature)), $_GET['q'], array("class"=>"toc"), NULL, generalizeString($feature)).'</li>';
        }
      }
    }
  }
  $out .= '</ul></div>';

  return $out;
}

/**
 * Replaces all occurrences of space characters with an underscore and tronsforms the given
 * string to lowercase.
 *
 * @param String $string
 * @return the transformed string
 */
function generalizeString($string){
  return str_replace(' ', '_', strtolower($string));
}

function theme_cdm_descriptionElements($descriptionElements, $feature){

  $outArray = array();
  $glue = '';
  $sortOutArray = false;
  $enclosingHtml = 'ul';
  
  foreach($descriptionElements as $descriptionElementSTO){

    if($descriptionElementSTO->classType == 'TextData'){
      $outArray[] = theme('cdm_descriptionElementTextData', $descriptionElementSTO);
    }else if($descriptionElementSTO->classType == 'Distribution'){
      $outArray[] = $descriptionElementSTO->area->term;
      $glue = ', ';
      $sortOutArray = true;
      $enclosingHtml = 'p';
    }else{
      $outArray[] = '<li>No method for rendering unknown description class: '.$descriptionElementSTO->classType.'</li>';
    }
  }

  return theme('cdm_descriptionElementArray', $outArray, $feature, $glue, $sortOutArray, $enclosingHtml);
}

function theme_cdm_descriptionElementArray($elementArray, $feature, $glue = '', $sortArray = false, $enclosingHtml = 'ul'){
  $out = '<'.$enclosingHtml.' class="description" id="'.$feature.'">';
  
  if($sortArray) sort($elementArray);
  
  $out .= join($elementArray, $glue);
  
  $out .= '</'.$enclosingHtml.'>';
  return $out;
}

function theme_cdm_descriptionElementTextData($element){

  $description = str_replace("\n", "<br/>", $element->description);
  $referenceCitation = '';
  if($element->reference){
    $referenceCitation = '; '.theme('cdm_fullreference', $element->reference, TRUE);
  }
  return '<li class="descriptionText">' . $description . $referenceCitation.'</li>';
}

function theme_cdm_search_results($resultPageSTO, $path, $parameters){

  drupal_set_title(t('Search Results'));

  $out = '';
  if(count($resultPageSTO->results) > 0){
    $out .= theme('cdm_list_of_taxa', $resultPageSTO->results);
    $out .= theme('cdm_pager', $resultPageSTO,  $path, $parameters);
  } else {
    $out = '<h4 class="error">Sorry, no matching entries found.</h4>';
  }
  return $out;
}

function theme_cdm_pager(&$resultPageSTO, $path, $parameters, $neighbors = 2){
  $out = '';

  if ($resultPageSTO->totalPageCount > 1) {

    $viewportsize = $neighbors * 2 + 1;
    if($resultPageSTO->totalPageCount <= $viewportsize){
      $viewportsize = $resultPageSTO->totalPageCount;
    }

    $out .= '<div class="pager">';
    if($resultPageSTO->pageNumber > 1){
      $out .= theme('cdm_pager_link', t('« first'), 1,  $resultPageSTO, $path, $parameters, array('class' => 'pager-first'));
      $out .= theme('cdm_pager_link', t('‹ previous'), $resultPageSTO->pageNumber - 1, $resultPageSTO, $path, $parameters, array('class' => 'pager-previous'));
    }

    if($resultPageSTO->totalPageCount <= $viewportsize || $resultPageSTO->pageNumber <= $neighbors){
      $first_number = 1;
    } else if($resultPageSTO->pageNumber >= $resultPageSTO->totalPageCount - $neighbors){
      $first_number = $resultPageSTO->totalPageCount - $viewportsize;
    } else {
      $first_number = $resultPageSTO->pageNumber - $neighbors;
    }

    if($first_number > 1){
      $out .= '<div class="pager-list-dots-left">...</div>';
    }


    for($i = $first_number; ($i == $resultPageSTO->totalPageCount) || ($i < $first_number + $viewportsize); $i++){
      $out .= theme('cdm_pager_link', $i, $i,  $resultPageSTO, $path, $parameters, array('class' => 'pager-first'));
    }
    if($i < $resultPageSTO->totalPageCount){
      $out .= '<div class="pager-list-dots-right">...</div>';
    }

    if($resultPageSTO->pageNumber < $resultPageSTO->totalPageCount){
      $out .= theme('cdm_pager_link', t('next ›'), $resultPageSTO->pageNumber + 1, $resultPageSTO, $path, $parameters, array('class' => 'pager-next'));
      $out .= theme('cdm_pager_link', t('last »'), $resultPageSTO->totalPageCount, $resultPageSTO, $path, $parameters, array('class' => 'pager-last'));
    }
    $out .= '</div>';

    return $out;
  }
}

function theme_cdm_pager_new(&$pager, $path, $parameters, $neighbors = 2){
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
