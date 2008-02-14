<?php
// $Id$

/*
 * Copyright (C) 2007 EDIT
 * European Distributed Institute of Taxonomy 
 * http://www.e-taxonomy.eu
 * 
 * The contents of this file are subject to the Mozilla Public License Version 1.1
 * See LICENSE.TXT at the top of this package for the full license terms.
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
 * Renders the full name string (complete scientific name including the author team)
 *  
 * @param NameTO $nameTO the taxon name
 */
function theme_cdm_name($nameTO, $displayNomRef = true){
  
    //TODO: - take the different subtypes of eu.etaxonomy.cdm.model.name.TaxonNameBase into account?
    $class = 'name'; //($nameTO->secUuid ? 'taxon' : 'taxonname');  
  
    if($nameTO){
      if(!$nameTO->taggedName || !count($nameTO->taggedName)){
        $out .= '<span class="'.$class.'">'.$nameTO->fullname.'</span>';
      } else {
        $out .= '<span class="'.$class.'">'.cdm_taggedtext2html($nameTO->taggedName).'</span>';
      }
    } else {
      $out .= '<span class="error">Invalid NameTO</span>';
    }
    
        
    if($displayNomRef && $nameTO->nomenclaturalReference->citation){
      $out .= (str_beginsWith($nameTO->nomenclaturalReference->citation, 'in') ? '&nbsp;':',&nbsp;');
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
function theme_cdm_taxon($taxonTO, $displayNomRef = true, $noSecundum = true, $enclosingTag = 'span'){

    
    $refSecundum = false;
    if(!$noSecundum){  
      $ref_sec = cdm_ws_get_reference($taxonTO->secUuid);
      if($ref_sec){
        $refSecundum = str_trunk($ref_sec->fullcitation, 40, '...');
      }
    }
    
    $out  = theme('cdm_name', $taxonTO->name, $displayNomRef);
    
	  $out .=($refSecundum ? '&nbsp;<span class="secundum">sec. '.$refSecundum.'</span>': '');

	  //TODO:   .$ptaxon->namePhrase; 
	  
    if($enclosingTag){
        $out = '<'.$enclosingTag.' class="taxon'.($taxonTO->isAccepted === true ? ' accepted':'').'">'.$out.'</'.$enclosingTag.'>';
    }

    return $out;    
}

/**
 * Renders a link to the taxon detail page for the given $taxon 
 *
 * @param TaxonTO $taxon
 */
function theme_cdm_taxon_link($taxonTO, $fragment = NULL, $showNomRef = false){
    
    if($fragment){
        $fragment = '#'.$fragment;
    }

    if(!$taxon->isAccepted) { 
        $out = 'ERROR: theme_cdm_taxon_link() - taxon is not accepted';
    }
    
    $name_html = theme('cdm_taxon', $taxonTO, false, true, '');
    $out = l($name_html, cdm_dataportal_taxon_path($taxonTO->uuid), array('class'=>'accepted'), '', $fragment, FALSE, TRUE);
    
    if($showNomRef){
       $out .=' '.theme('cdm_nomRef', $taxonTO);
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

function theme_cdm_dynabox($label, $content_url, $theme, $enclosingtag = 'li'){
  $cdm_proxy_url = url('cdm_api/proxy/'.urlencode($content_url)."/$theme");
  $out .= '<li class="dynabox"><span class="label">'.$label.'</span>';
  $out .= '<ul class="dynabox_content" title="'.$cdm_proxy_url.'"><li><img class="loading" src="'.drupal_get_path('module', 'cdm_dataportal').'/images/loading_circle_grey_16.gif" style="display:none;"></li></ul>';
  return $out;
}

function theme_cdm_dataportal_names_list($taxonSTOs){
  
  drupal_add_js(drupal_get_path('module', 'cdm_dataportal').'/js/cdm_dynabox.js');
  drupal_add_css(drupal_get_path('module', 'cdm_dataportal').'/cdm_dataportal.css');
  
  $out = '<ul class="cdm_names" style="background-image: none;">';
  foreach($taxonSTOs as $taxon){
    if($taxon->isAccepted){
      $out .= '<li>'.theme('cdm_taxon_link', $taxon).'</li>';
    } else {
      $out .= theme('cdm_dynabox', theme('cdm_name', $taxon->name), cdm_ws_get_accepted_url($taxon->uuid), 'cdm_taxon_link');
    }
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
  
  if($referenceTO->citation){
    $out .= ' '.$referenceTO->citation;
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
    $nomref_citation = $referenceSTO->citation; 
  }

  $module_path = drupal_get_path('module', 'cdm_dataportal');
  drupal_add_js($module_path.'/js/jquery_lightbox/js/jlightbox.uncompressed.js');
  drupal_add_css($module_path.'/js/jquery_lightbox/css/jlightbox.css', 'module', 'screen');
  
  if( count($referenceSTO->mediaURI) > 0 ){
    $attributes = array('rel'=>'lightbox[protologues]');
    $out = l($nomref_citation, $referenceSTO->mediaURI[0]->value, $attributes, NULL, NULL, TRUE);
    for($i = 1;  $i < count($referenceSTO->mediaURI); $i++) {
      $out .= l('', $referenceSTO->mediaURI[$i]->value, $attributes, NULL, NULL, TRUE);
    }
  } else {
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
  
  // aggregate misapplied named having the same fullname:
  $misapplied = array();
  foreach($TaxonRelationshipTOs as $taxonRelation){
    if(true || $taxonRelation->type->uuid == UUID_MISAPPLIED_NAME_FOR || $taxonRelation->type->uuid == UUID_INVALID_DESIGNATION_FOR ){
      
      $sensu_reference = cdm_ws_get_reference($taxonRelation->secUuid);
      $name = $taxonRelation->name->fullname;
      if(!isset($misapplied[$name])){
        $misapplied[$name] = '<span class="misapplied">'.theme('cdm_related_taxon',$taxonRelation, UUID_MISAPPLIED_NAME_FOR, false).'<span>&nbsp;'
        .'<span class="sensu">sensu '.theme('cdm_fullreference',$sensu_reference ).'</span>';
      } else {
        $misapplied[$name] .= ';&nbsp;<span class="sensu">sensu '.theme('cdm_fullreference',$sensu_reference ).'</span>';
      }
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
    $out .= '<li class="nameTypeDesignation"><span class="status">'.$ntd->status->value.'</span> - '.theme('cdm_name', $ntd->typeSpecies, false);
    $out .= theme('cdm_typedesignations', $ntd->typeSpecimens);
    $out .= '</li>';
  }    
  
  foreach($specimenTypeDesignations as $std){
    $out .= '<li class="specimenTypeDesignation"><span class="status">'.$std->status->value.'</span> - '.$std->typeSpecimen->specimenLabel;
    if(is_array($std->typeSpecimen->mediaURI)){
      $image_url = drupal_get_path('module', 'cdm_dataportal').'/images/external_link.gif';
      foreach($std->typeSpecimen->mediaURI as $uri){
        $out .= ' <a href="'.$uri->value.'" target="'.$uri->uuid.'"><img src="'.$image_url.'" /></a>';
      }
    }
    $out .= '</li>';
  }
  
  $out .= '</ul>';
  
  return $out;
}

//TODO: port everything below

/**
 * Renders a list of synonyms including misapplied names which are related to the 
 * given TAXIC_PTaxon.
 *
 * @param TAXIC_PTaxon $ptaxon
 * @param boolean $recursive default is false. Whether to crawl recursively into the
 * three of relations
 * 
 * usage taxon_detail.php.inc
 */
function render_synonyms(TAXIC_PTaxon $ptaxon, $recursive = false, $showNotes = false){

    $_debug = false;
    
    $is_higher_rank = $ptaxon->rankId < 60;
  
    
    $out = false;
    $last_relQualifierId = false;
    $last_was_heterotypic = false;
    $last_basionym_nameId = false;
    $is_other_basionym = false;
    
    $synonyms = findSynonyms($ptaxon->nameId, $ptaxon->refId, false, $recursive);

    foreach($synonyms AS $relpt){ // -- relations loop
             
      // whether this is the first entry in the lsit
      $is_first_entry = $out === false;
      
      // whether the current taxon has another basionym than the last one
      $is_other_basionym = $last_basionym_nameId !== false  
        && ($last_basionym_nameId !== $relpt->basionymNameId)
        && $relpt->relQualifierId != 3;
      
      // whether the current taxon has another name ralation than the last one
      $is_other_relQualifierId = $last_relQualifierId !== false && ($last_relQualifierId !== $relpt->relQualifierId);

      //is here the end of list of homotypic end of heterotypic subsection?  
      //$end_of_section = ($last_relQualifierId !== false && $is_other_basionym);
      $end_of_section = !$is_first_entry && $is_other_basionym || $is_other_relQualifierId && $relpt->relQualifierId == 3 /* misaplied */;

      // name relations declared as synomyn (id=2) are assumed to be heterotypic
      $is_heterotypic = $relpt->relQualifierId == 6 || ($relpt->relQualifierId == 2 && !$is_other_basionym && $last_was_heterotypic);
      // DELETE ME? $is_heterotypic = $relpt->relQualifierId == 6 || ($relpt->relQualifierId == 2 && !$is_other_basionym);
      
      $is_homotypic = $relpt->relQualifierId == 7 || $relpt->relQualifierId == 101 || $relpt->relQualifierId == 103;
      
      // is this taxon homotypic to a previously printed heterotypic taxon, so is should be
      // an intented subelement and re-taged as homotypic 
      $is_subelement = $is_heterotypic && !$is_other_basionym && !$is_first_entry;
      
      $is_the_basionym = $is_homotypic && !$last_basionym_nameId;
      
     
      if($is_first_entry){ 
        $out = '<ul>'.chr(10).'<li class="blank_line"></li>'.chr(10); 
           if($is_heterotypic){
             if($_debug) $out .= '<li><span style="color:blue; font-size: 80%;">renderTypeDesignations before first entry which is heterotypic]</span></li>';
             $out .= renderTypeDesignations($ptaxon->nameId, '', $is_higher_rank ); 
             $out .= '<li class="blank_line"></li>'.chr(10);
           }
      }
      /*$out .= '<li><span style="color:blue; font-size: 80%;">last: '
				.$last_basionym_nameId.' !== '.$relpt->basionymNameId.' '.$relpt->fullName
                .'</span></li>';
		*/      
      //DEBUG $out .= '<pre>'.print_r($relpt, true).'</pre>';
      
      if($is_other_basionym){
        if($_debug) $out .= '<li><span style="color:blue; font-size: 80%;">renderTypeDesignations 1 (relID: '.$last_relQualifierId.' for '.$last_basionym_nameId.')</span></li>';
        $out .= renderTypeDesignations($last_basionym_nameId, ($last_was_heterotypic ? 'subelement' : ''), $is_higher_rank ); 
        //$out .= renderTypeDesignations($last_basionym_nameId, '', $is_higher_rank ); 
      }
      
      if($end_of_section){
          // find type information          
          $out .= '<li class="blank_line"></li>'.chr(10);
      }
      
      if($is_subelement){
          // all those taxa which are homotypic to the preceeding basionym are to be
          // displayed as homotypic
          $relpt->relQualifierId = 7;
          $cssClass = 'subelement';
      } else {
          $cssClass = '';
      }
      
      $out .= render_related_ptname($relpt, 'li', $cssClass, $showNotes).chr(10);
      
       // remember last synonyms properties for next round
      $last_relQualifierId = $relpt->relQualifierId;
      $last_basionym_name = $relpt->basionymFullName;
      $last_basionym_nameId = $relpt->basionymNameId;
      $last_was_heterotypic = $is_heterotypic;
      
    } // END relations loop
    
    
    if($is_the_basionym){
       if($_debug) $out .= '<li><span style="color:blue; font-size: 80%;">renderTypeDesignations 0 for '.$relpt->nameId.'</span></li>';
        $out .= renderTypeDesignations($relpt->nameId, '', $is_higher_rank);
    } else 
    // TypeDesignations for the last item
    if($is_other_basionym || !$last_was_heterotypic){
        if($_debug) $out .= '<li><span style="color:blue; font-size: 80%;">renderTypeDesignations 2 for '.$last_basionym_nameId.'</span></li>';
        $out .= renderTypeDesignations($last_basionym_nameId, '', $is_higher_rank);
    } else
    
    // In case the given PTaxon had no synonyms ...
     if($last_basionym_nameId === false || $last_relQualifierId == 7){
        if($_debug) $out .= '<li><span style="color:blue; font-size: 80%;">renderTypeDesignations 3 for'.$ptaxon->nameId.'; last_basionym_nameId = '.$last_basionym_nameId.'</span></li>';
        $out .= renderTypeDesignations($ptaxon->nameId, '', $is_higher_rank);
    } else {
      $out .= '<li class="blank_line"></li>'.chr(10).'</ul>';
    }
    
    return $out;
}
// -------------------- END function render_synonyms()

function render_homonyms(TAXIC_TaxonName $taxon){
  $homonyms = getLaterHomonyms($taxon);
  $out = '';

  foreach($homonyms as $homnym){
    $out .= '<li class="homonym">[non '.$homnym->authors.', '.$homnym->nomRef.']'.render_notes($homnym).'</li>';
  }
  
  if($out){
    $out = '<ul>'.$out.'</ul>';
  }
  return $out;
}

/**
 * render_related_ptname() is used by render_synonyms()
 * @param TAXIC_PTaxon $relPTaxon
 * @param String $enclosingTag 
 */
function render_related_ptname($relPTaxon, $enclosingTag = 'li', $cssClass = '', $showNotes = false){
    
    $cssClass .= ' '.str_replace(' ', '_', $relPTaxon->relQualifier);
  
    switch($relPTaxon->relQualifierId){
        case 7  :
        case 101  :
        case 103  : // homotypic-synonym
            $relSign = '≡';
            break;
        case 3  :   // misapplied_name
            $relSign = '-';
            break;
        case 8  :   // invalid_designation
            $relSign = '-';
            break;
        default : 
            $relSign = '=';
            break;
    }
    
    //$relSign .= $relPTaxon->relQualifierId;

    $out = '';
    
    if($relPTaxon->relQualifierId == 3){
        // special look for misapplied_names
        $relatedToName = ''; // only used in HTML comment
        $sensuPart = ' sensu '.$relPTaxon->ref;
        if($relPTaxon->relInversion){
            $namePart = ($relPTaxon->isAccepted()? render_ptname_link($relPTaxon) : render_ptname($relPTaxon));
            $out = $relPTaxon->relQualifier.' '.$namePart.' '.$sensuPart;
        } else {
            $namePart = '„'.trim($relPTaxon->name).'“';
            if($relPTaxon->relPTaxon->name){
                // only used in HTML comment!!
                $relatedToName = trim($relPTaxon->relPTaxon->name); 
            }
            $out .= $namePart.'</span>'.$sensuPart;
        }
        
    } else {
        $relatedToName = ($relPTaxon->relPTaxon ? $relPTaxon->relPTaxon->name : false);
        if($relPTaxon->isAccepted()) {
            $out .= render_ptname_link($relPTaxon, null, true);
        } else {
            $out .= render_ptname($relPTaxon, true ,'span', true);
        }
            
    } // END else special look for misapplied_names

    $notes = ($showNotes && showNotes() ? render_notes($relPTaxon) : '');
    
    $out =  '<'.$enclosingTag.' class="'.$cssClass.'"><!-- '.$relPTaxon->relQualifier.' '.$relatedToName.'-->'
			.'<a name="'.$relPTaxon->nameId.'_'.$relPTaxon->refId.'" ></a><span class="relation_sign">'.$relSign.'</span> '
            .$out . $notes
            .'</'.$enclosingTag.'>';
    return $out;
}

/**
 * @param TAXIC_PTaxon $ptname
 */
$NOMREFSET_COUNTER = 0;

function render_nomRef($ptname){
  global $conf;
  global $NOMREFSET_COUNTER;
  $NOMREFSET_COUNTER += 1;
    if(count($protoploges = $ptname->getProtologues()) > 0){
		$prot = $protoploges[0];
		// FIXME: protologues are really a list of links. treat only the first
		if(is_file($conf['abs_media_path'].$conf['protologue_path'].$prot.".png")){
    	  $nomrefLink = '<span class="nomref"><a href="'.$conf['protologue_path'].$prot.'.png" rel="lightbox[nomrefset'.$NOMREFSET_COUNTER.']" title="&lt;a href=&quot;'.$conf['protologue_quality_path'].$prot.'.tif&quot;&gt;High Quality Scan&lt;/a&gt;">'.trim($ptname->nomRef).'</a></span>';
		} else {
		  $nomrefLink = '<span class="nomref"><a href="'.$conf['protologue_path'].$prot.'001.png" rel="lightbox[nomrefset'.$NOMREFSET_COUNTER.']" title="&lt;a href=&quot;'.$conf['protologue_quality_path'].$prot.'.tif&quot;&gt;High Quality Scan&lt;/a&gt;">'.trim($ptname->nomRef).'</a></span>';	  
          // check in filesystem if there are more pages as JPGs. TIFFs can cope with 8 pages per file, so no need for that
          $i=2;
          while (is_file($conf['abs_media_path'].$conf['protologue_path'].$prot.str_pad($i, 3, '0', STR_PAD_LEFT).".png")){
          	$nomrefLink = $nomrefLink."   <a href='".$conf['protologue_path'].$prot.str_pad($i, 3, '0', STR_PAD_LEFT).".png' rel='lightbox[nomrefset".$NOMREFSET_COUNTER."]'></a>";
          	$i++;
          }
		}
        return $nomrefLink;
    } else {
        return '<span class="nomref">'.trim($ptname->nomRef).'</span>';
    }
    
}


/**
 * render a numeric pager
 */
function renderNumPager($numOfPages, $activePage, $maxPagerItems, $linkUrlBase){
	
    if ($activePage > $maxPagerItems - 2){
        // shift visible pager items
        $begin = $activePage - floor($maxPagerItems / 2);
        $end   = min($activePage + ceil($maxPagerItems / 2), $numOfPages);
    } else {
        $begin = 1; 
        $end   = min($numOfPages, $maxPagerItems);
    }
    // hide the pager if there is only one page:
    if($begin == $end)
        return;
        
    echo '<ul class="paging numeric">';
    if ($activePage > 10){
	    echo '<li><a href="', $linkUrlBase, $activePage - 10, '" title="backward 10 pages">&laquo;</a></li>';
    }
    if ($activePage > 1){
	    echo '<li><a href="', $linkUrlBase, $activePage - 1, '" title="previuos">&lt;</a></li>';
    }
	for ($pn = $begin; $pn <= $end ; ++$pn) {
		if($pn != $activePage){
		     echo '<li><a href="'.$linkUrlBase.$pn.'">'. $pn. '</a></li>';
		} else {
			echo '<li class="active">'.$pn.'</li>';
		}
	} 
    if ($activePage < $numOfPages){
	    echo '<li><a href="', $linkUrlBase, $activePage + 1, '" title="next">&gt;</a></li>';
    }
    if ($activePage < $numOfPages - 9){
	    echo '<li><a href="', $linkUrlBase, $activePage + 10, '" title="forward 10 pages">&raquo;</a></li>';
    }
    if($numOfPages > 0){
        echo '<li> (of '.$numOfPages.' pages)</li>';
    }
    echo '</ul>';
}

/**
 * @param TAXIC_TaxonName $taxon
 */
function render_notes(TAXIC_TaxonName $taxon){
  
    if(!showNotes()){
      return;
    }
    
    $notes = array();
    if($taxon->nameNotes && trim($taxon->nameNotes)){
      $notes['Name Notes'] = $taxon->nameNotes;
    }
    if($taxon->nomRefNotes && trim($taxon->nomRefNotes)){
      $notes['Nomenclatural Ref. Notes'] = $taxon->nomRefNotes;
    }
    
    if($taxon instanceof TAXIC_PTaxon){
      if($taxon->nameNotes && trim($taxon->conceptNotes)){
        $notes['Taxon Concept Notes'] = $taxon->conceptNotes;
      }
      if($taxon->refNotes && trim($taxon->refNotes)){
        $notes['Concept Reference Notes'] = $taxon->refNotes;
      }
    }
    if(count($notes) > 0){
      $out = '<span class="note_toggler"><img src="themes/cichorieae/note_gray.gif" /><div class="note">';
      $out .= '<div class="title"><div class="close" title="close"><img src="themes/cichorieae/close.gif" /></div>Notes on '.render_name($taxon).'</div><div class="content">';
      foreach(array_keys($notes) as $title){
        $out .= render_notes_entry($title, $notes[$title]);
      }
      return $out.'</div></div></span>';
    } else {
      return;
    }
}

function render_notes_entry($title, $note){
    
    return '<h4>'.$title.'</h4><p>'.htmlspecialchars($note).'</p>'.chr(10);
}

