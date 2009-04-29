<?php
// $Id: $

/**
 * Overrides of generic themeing functions in cdm_datportal.theme.php
 */
function diptera_cdm_taxon_page_title($nameTO){
  
  if(variable_get('cdm_dataportal_nomref_in_title', 1)){
    // taxon name only with author and year
    // $displayAuthor = true, $displayNomRef = true, $displayStatus = true, $displayDescription = true, $nomRefLink = true
    return theme('cdm_name', $nameTO, /*$displayAuthor*/ TRUE, /*$displayStatus*/ false, /*$displayDescription*/ false, TRUE);
  } else {
    return theme('cdm_name', $nameTO);
  }
}

/**
 * for diptera show only author, year as nom.ref
 *
 * @param unknown_type $referenceSTO a referenceSTO or referenceTO instance
 * @param unknown_type $cssClass
 * @param unknown_type $separator
 * @param unknown_type $enclosingTag
 * @return unknown
 */
function diptera_cdm_nomenclaturalReferenceSTO($referenceSTO, $doLink = FALSE, $cssClass = '', $separator = '<br />' , $enclosingTag = 'li', $showPage = false){

  $nomref_citation = $referenceSTO->authorship.', '.$referenceSTO->year;
  if($showPage){
    $referenceTO = cdm_ws_get(CDM_WS_REFERENCE, $referenceSTO->uuid);
    $nomref_citation .= ': '.$referenceTO->pages;
  }
  
  if($doLink){
    $nomref_citation = l($nomref_citation, "/cdm_dataportal/reference/".$referenceSTO->uuid, array("title"=>$referenceSTO->citation), NULL, NULL, FALSE, FALSE);
  }
  
  if(!empty($nomref_citation)){
    $nomref_citation = (str_beginsWith($nomref_citation, 'in') ? '&nbsp;':',&nbsp;') . $nomref_citation;
  }
  
  return $nomref_citation;
}

function diptera_cdm_related_taxon($taxonSTO, $reltype_uuid = '', $displayNomRef = true){

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
  
  $taxon_str  = theme('cdm_name', $taxonSTO->name, true, false, false, false, false);
  
  $out = '<span class="relation_sign">'.$relsign.'</span>'.$name_prefix.$taxon_str;
  if($taxonSTO->name->taggedName){
    $authorsStr = cdm_taggedtext_value($taxonSTO->name->taggedName, "authors");
    $authorsHtml = '<span class="authors">'.$authorsStr.'</span>';
    if(isset($taxonSTO->name->nomenclaturalReference)){
      $authorsHtml = l($authorsHtml, "/cdm_dataportal/reference/".$taxonSTO->name->nomenclaturalReference->uuid, array(), NULL, NULL, FALSE, TRUE);
    }

    $out .= ( str_beginsWith($authorsStr,'(') ? ' ' : ', ') . $authorsHtml;
  }else{
    if($taxonSTO->name->nomenclaturalReference){
      $out .= ' '.theme('cdm_nomenclaturalReferenceSTO', $taxonSTO->name->nomenclaturalReference, TRUE, '', '<br />', 'li', TRUE);
    }
  }
  $out .= $name_postfix;
  return $out;

}

function diptera_cdm_taxon_page_images($taxon){

  $descriptions = $taxon->featureTree->descriptions;
  foreach($descriptions as $description){
    $features = $description->features;
    foreach($features as $feature){
      if($feature->feature->term == 'Image'){
        $descriptionElements = $feature->descriptionElements;
        if(count($descriptionElements) > 0){
          $imagesExist = true;
          // display image
          
          foreach($descriptionElements as $descriptionElement){
            $medias = $descriptionElement->media;
            foreach($medias as $media){
              $representations = $media->representations;
              foreach($representations as $representation){
                $representationParts = $representation->respresentationParts;
                foreach($representationParts as $representationPart){
                  $out .= '<img src="' . $representationPart->uri . '" alt=""/>';
                }
              }
            }
          }
          
        }
      }
    }
  }
  
  return $imagesExist ? $out : 'No images available.';
}

?>