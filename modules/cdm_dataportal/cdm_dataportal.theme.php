<?php
// $Id$

/*
 * Copyright (C) 2007 EDIT
 * European Distributed Institute of Taxonomy 
 * http://www.e-taxonomy.eu
 */

function _add_js_thickbox(){
   // ---- jQuery ThickBox:
  /*
   * bug: compat-1.0.js && thickbox.js line 237 .trigger("unload")
   * -> event is not triggered because of problems with compat-1.0.js'
   * see INSTALL.txt
   * 
   */
  
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
 * Renders the full name string (complete scientific name including the author team)
 *  
 * @param NameTO $nameTO the taxon name
 */
function theme_cdm_name($nameTO, $displayNomRef = true){
  
    //TODO: - take the different subtypes of eu.etaxonomy.cdm.model.name.TaxonNameBase into account?
    $class = ''; //name'; //($nameTO->secUuid ? 'taxon' : 'taxonname');  

    if(!$nameTO){
      return '<span class="error">Invalid NameTO</span>';      
    }

    $hasNomRef = $nameTO->nomenclaturalReference->fullCitation;
    //FIXME class="'.$class.' below seems to be unused
    if(!$nameTO->taggedName || !count($nameTO->taggedName)){
      $out .= '<span class="'.$class.'">'.$nameTO->fullname.'</span>';
    } else {
      $skip = $hasNomRef ? array('reference') : array();
      $out .= '<span class="'.$class.'">'.cdm_taggedtext2html($nameTO->taggedName, 'span', ' ', $skip).'</span>';
    }
        
    if($displayNomRef && $hasNomRef){
      $out .= (str_beginsWith($nameTO->nomenclaturalReference->fullCitation, 'in') ? '&nbsp;':',&nbsp;');
      $out .= theme('cdm_nomenclaturalReferenceSTO', $nameTO->nomenclaturalReference);
    }
    
    if($nameTO->status){
      $out .= ', '.cdm_dataportal_t($nameTO->status);
    }
    return $out;
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
function theme_cdm_taxon($taxonTO, $displayNomRef = true, $noSecundum = true, $enclosingTag = 'span', $uuidAnchor = TRUE){

    $refSecundum = false;
    if(!$noSecundum){  
      $ref_sec = cdm_ws_get(CDM_WS_SIMPLE_REFERENCE ,$taxonTO->secUuid);
      if($ref_sec){
        $refSecundum = str_trunk($ref_sec->fullCitation, 40, '...');
      }
    }
   
    
    
    $out  = theme('cdm_name', $taxonTO->name, $displayNomRef);
    // append secundum information
	  $out .=($refSecundum ? '&nbsp;<span class="secundum">sec. '.$refSecundum.'</span>' : '');
	  // add uuid anchor
	  if($uuidAnchor){
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
function theme_cdm_taxon_link($taxonTO, $fragment = NULL, $showNomRef = true){
    
    if($fragment){
        $fragment = '#'.$fragment;
    }

    if(!$taxon->accepted) { 
      $out = 'ERROR: theme_cdm_taxon_link() - taxon is not accepted';
    }
    
    $name_html = theme('cdm_taxon', $taxonTO, false, true, '', FALSE);
    $out = l($name_html, cdm_dataportal_taxon_path($taxonTO->uuid), array('class'=>'accepted'), null, $fragment, FALSE, TRUE);
    
    if($showNomRef){
       $out .=' '.theme('cdm_nomenclaturalReferenceSTO', $taxonTO->name->nomenclaturalReference);
    }
	
	return $out;
}

/**
 * Renders a link to the taxon detail page for the given $taxon 
 *
 * @param TaxonTO $taxon
 */
function theme_cdm_synonym_link($taxonTO, $accepted_uuid, $showNomRef = true){
    
    $name_html = theme('cdm_taxon', $taxonTO, false, true, '');
    $out = l($name_html, cdm_dataportal_taxon_path($accepted_uuid), array('class'=>'synonym'), 'highlite='.$taxonTO->uuid, $taxonTO->uuid, FALSE, TRUE);
    if($showNomRef){
       $out .=' '.theme('cdm_nomenclaturalReferenceSTO', $taxonTO->name->nomenclaturalReference);
    }

     
  return $out;
}

function theme_cdm_related_taxon($taxonSTO, $reltype_uuid = '', $displayNomRef = true){
  
  $relsign = '';
  $name_prefix = '';
  $name_postfix = '';
  switch ($reltype_uuid){
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
      $relsign = '=';
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

function theme_cdm_listof_taxa($taxonSTOs){
  
//  drupal_add_js(drupal_get_path('module', 'cdm_dataportal').'/js/cdm_dynabox.js');
//  drupal_add_css(drupal_get_path('module', 'cdm_dataportal').'/cdm_dataportal.css');
//  
  $out = '<ul class="cdm_names" style="background-image: none;">';
  
  $synonym_uuids = array();
  foreach($taxonSTOs as $taxon){
    if(!_cdm_dataportal_acceptetByCurrentView($taxon)){
      if(!array_key_exists($taxon->uuid, $synonym_uuids)){
        $synonym_uuids[$taxon->uuid] = $taxon->uuid;
      }
    }
  }
  $acceptedTable = cdm_ws_get(CDM_WS_ACCEPTED_TAXON, join(',',$synonym_uuids));
  
  foreach($taxonSTOs as $taxon){
    if(_cdm_dataportal_acceptetByCurrentView($taxon)){
      $out .= '<li>'.theme('cdm_taxon_link', $taxon).'</li>';
    } else {
      $acceptedTaxa = array();
      if(is_array($acceptedTable)){
        foreach($acceptedTable as $uuid=>$t){
          if($uuid == $taxon->uuid){
            $acceptedTaxa[] = $t;
          }
        }
      }
      if(count($acceptedTaxa) == 1){
        $out .= '<li>'.theme('cdm_synonym_link', $taxon, $acceptedTaxa[0]->uuid ).'<li>';        
      } else {
        $out .= theme('cdm_dynabox', theme('cdm_name', $taxon->name), cdm_compose_url(CDM_WS_ACCEPTED_TAXON, array($taxon->uuid)), 'cdm_listof_taxa');        
      }
      
    }
  }
  $out .= '</ul>';
  return $out;
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


function theme_cdm_fullreference($referenceTO){
  $out = $referenceTO->authorship;
  
  if($referenceTO->fullCitation){
    $out .= ' '.$referenceTO->fullCitation;
  }
  if($referenceTO->microReference){
    $out .= ' : '.$referenceTO->microReference;
  }
  if($referenceTO->year){
    $out .= '. '.$referenceTO->year;
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
function theme_cdm_nomenclaturalReferenceSTO($referenceSTO, $cssClass = '', $separator = '<br />' , $enclosingTag = 'li'){
  
  if(isset($referenceSTO->microReference)){
    // well it is a ReferenceTO
    $nomref_citation = theme('cdm_fullreference', $referenceSTO);
  } else {
    // it is ReferenceSTO
    $nomref_citation = $referenceSTO->fullCitation;
  }

  _add_js_thickbox();
  
  // find media representations for inline display and high quality for download
  // assuming that there is only one protologue per name only the first media is used 
  
//  if( count($referenceSTO->media[0]->representations) > 0 ){
//    foreach($referenceSTO->media[0]->representations as $representation){
//      $mimeType = $representation->mimeType;
//      if(($mimeType == 'image/png' || $mimeType == 'image/jpeg' || $mimeType == 'image/gif')
//        && count($representation->representationParts) > 0){
//        $representation_inline = $representation;
//      } else if($representation->mimeType == 'image/tiff'
//        && count($representation->representationParts) > 0){
//        $representation_highquality = $representation;
//      }
//    }
//  }
  $prefRepresentations = cdm_preferred_media_representations($referenceSTO->media[0], array('image/gif', 'image/jpeg', 'image/png'), 300, 400);
  $representation_inline = array_shift($prefRepresentations);
  if($representation_inline) {
    $attributes = array('class'=>'thickbox', 'rel'=>'protologues-'.$referenceSTO->uuid);
    for($i = 0; $part = $representation_inline->representationParts[$i]; $i++){
      if($i == 0){        
        $out = l($nomref_citation, $part->uri, $attributes, NULL, NULL, TRUE);
      } else {
        $out .= l('', $part->uri, $attributes, NULL, NULL, TRUE);              
      }
    }
  } else {
    // no media available, so display just the citation string
    $out =  $nomref_citation;
  }
  return $out;  
}

function theme_cdm_taxon_page($taxonTO){
  $out = '';
  $out .= theme('cdm_homotypicSynonyms', $taxonTO->homotypicSynonyms, $taxonTO->typeDesignations, $taxonTO->nameTypeDesignations);
  foreach($taxonTO->heterotypicSynonymyGroups as $hs_group){
    $out .= theme('cdm_heterotypicSynonymyGroup', $hs_group);
  }
  $out .= theme('cdm_taxonRelations', $taxonTO->taxonRelations);
  $out .= theme('cdm_descriptions', $taxonTO->descriptions);
  return $out;
}

function theme_cdm_homotypicSynonyms($synonymRelationshipTOs, $specimenTypeDesignations = false, $nameTypeDesignations = false){
  
  $out = '';
  $out = '<ul class="homotypicSynonyms">';
  
  foreach($synonymRelationshipTOs as $synonym){
    $out .= '<li class="synonym">'.theme('cdm_related_taxon', $synonym->synoynm, UUID_HOMOTYPIC_SYNONYM_OF).'</li>';
  }
  if($specimenTypeDesignations){
    $out .= theme('cdm_typedesignations', $specimenTypeDesignations, $nameTypeDesignations);
  }
  
  $out .= '</ul>';
  return $out;
}

function theme_cdm_heterotypicSynonymyGroup($homotypicGroupTO){
  $out = '';  
  $out = '<ul class="heterotypicSynonymyGroup">';
  
  $is_first_entry = true;
  foreach($homotypicGroupTO->taxa as $taxonSTO){
    if($is_first_entry){
      $is_first_entry = false;
      // is first list entry
      $out .= '<li class="firstentry synonym">'.theme('cdm_related_taxon',$taxonSTO, UUID_HETEROTYPIC_SYNONYM_OF).'</li>';
    } else {
      $out .= '<li class="synonym">'.theme('cdm_related_taxon',$taxonSTO, UUID_HOMOTYPIC_SYNONYM_OF).'</li>';
    }
  }
  
  if(isset($homotypicGroupTO->typeDesignations)){
    $out .= theme('cdm_typedesignations', $homotypicGroupTO->typeDesignations);
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
  
  // aggregate misapplied named having the same fullname:
  $misapplied = array();
  foreach($TaxonRelationshipTOs as $taxonRelation){
    if(true || $taxonRelation->type->uuid == UUID_MISAPPLIED_NAME_FOR || $taxonRelation->type->uuid == UUID_INVALID_DESIGNATION_FOR ){
      
      $sensu_reference_list = cdm_ws_get(CDM_WS_SIMPLE_REFERENCE ,$taxonRelation->taxon->secUuid);
      $sensu_reference = $sensu_reference_list[0];
      $name = $taxonRelation->taxon->name->fullname;
      if(!isset($misapplied[$name])){
        $misapplied[$name] = '<span class="misapplied">'.theme('cdm_related_taxon',$taxonRelation->taxon, UUID_MISAPPLIED_NAME_FOR, false).'</span>';
      } else {
        $misapplied[$name] .= ';';
      }
      $misapplied[$name] .= '&nbsp;<span class="sensu cluetip no-print" title="|sensu '.htmlspecialchars(theme('cdm_fullreference',$sensu_reference )).'|">sensu '
      .$sensu_reference->authorship.'</span>'
      .'<span class="reference only-print">sensu '.theme('cdm_fullreference',$sensu_reference ).'</span>'
      ;
    }
  }
  // generate output
  $out = '<ul class="misapplied">';
  foreach($misapplied as $misapplied_name){
    $out .= '<li class="synonym">'.$misapplied_name.'</li>';
  }
  $out .= '</ul>';
  return $out;
}


function theme_cdm_typedesignations($specimenTypeDesignations, $nameTypeDesignations = array()){
  
  $out = '<ul class="typeDesignations">';

  foreach($nameTypeDesignations as $ntd){
    $out .= '<li class="nameTypeDesignation"><span class="status">'.$ntd->status->text.'</span> - '.theme('cdm_name', $ntd->typeSpecies, false);
    $out .= theme('cdm_typedesignations', $ntd->typeSpecimens);
    $out .= '</li>';
  }    
  
  foreach($specimenTypeDesignations as $std){
    $out .= '<li class="specimenTypeDesignation">';
    $out .= '<span class="status">'.$std->status->text.'</span> - '.$std->typeSpecimen->specimenLabel;
    $out .= theme('cdm_specimen', $std->typeSpecimen);
    $out .= '</li>';
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


function theme_cdm_descriptions(array $descriptionTOs){
  /*
   * ->label
   * ->sources{}
   * ->elements{
   *      ->type
   *      ->media{}
   *      ->description
   *      ->language
   *      }
   */  

  $out = '';
  $i = 0;
  foreach($descriptionTOs as $descriptionTO){
    if($i++ > 0){
      $out .= '<hr />';
      
    }
    $elementSTOsByType = array();
    foreach($descriptionTO->elements as $descriptionElementSTO){
      $elementType = isset($descriptionElementSTO->type->term) && $descriptionElementSTO->type->term ? $descriptionElementSTO->type->term : '';
      if(!array_key_exists($elementType, $elementSTOsByType)){
        $elementSTOsByType[$elementType] = array();
      }
      $elementSTOsByType[$elementType][$descriptionElementSTO->description] = $descriptionElementSTO;
    }
    
    //sort all 
    foreach(array_keys($elementSTOsByType) as $key){
  
      ksort($elementSTOsByType[$key]);
      
      $block->module = 'cdm_dataportal';
      $block->delta  = isset($key) ? $key : t('Description');
      $block->subject = t(ucfirst($block->delta));
      $block->delta = str_replace(' ', '_', strtolower($block->delta));
      $block->content = '<table>';
      
      $i = 0;
      foreach($elementSTOsByType[$key] as $descriptionElementSTO){
        $block->content .= '<tr class="'.($i++%2?'odd':'even').'"><td>'.$descriptionElementSTO->description.'</td><td>'.theme('cdm_fullreference', $descriptionElementSTO->reference).'</td></tr>';
        //TODO show media etc
      }
      $block->content .= '</table>';
      $out .= theme('block', $block);
    }
    
  }
  return $out;
}



function theme_cdm_search_results($resultPageSTO, $path, $parameters){
  
  drupal_set_title(t('Search Results'));
  
  $out = '';
  if(count($resultPageSTO->results) > 0){
    $out = theme('cdm_listof_taxa', $resultPageSTO->results);
    $out .= theme('cdm_pager', $resultPageSTO,  $path, $parameters);
  } else {
    $out = '<h4 calss="error">Sorry, no matching entries found.</h4>';
  }
  return $out;
}

function theme_cdm_pager(&$resultPageSTO, $path, $parameters, $neighbors = 2){
  $out = '';

  if ($resultPageSTO->totalPageCount > 1) {
    
    $viewportsize = $neighbors * 2 + 1; 
    

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
    for($i = $first_number; $i < $first_number + $viewportsize; $i++){
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
