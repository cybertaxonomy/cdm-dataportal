<?php
// $Id: $

function diptera_cdm_descriptionElements($descriptionElements){

  $outArray = array();
  $glue = '';
  $sortOutArray = false;
  $enclosingHtml = 'ul';
  
  
   RenderHints::pushToRenderStack('cdm_descriptionElements');
    
  
  // only for diptera
  if(isset($descriptionElements[0]) && $descriptionElements[0]->feature->uuid == UUID_CITATION ) {
    foreach($descriptionElements as $element){
      $tokens = split(":", $element->multilanguageText_L10n->text);
      if(count($tokens) == 2){
        // token[0]: taxon name; token[1]: note; 
        $element->multilanguageText_L10n->text = $tokens[1] . ' [<span class="name">' . $tokens[0] . '</span>]';
      }
      if(isset($element->citation->datePublished->start)){
        $elementMap[partialToYear($element->citation->datePublished->start)] = $element;
      } else {
        $elementMap[] = $element;
      }
    }
    $success = ksort($elementMap);
    $descriptionElements = $elementMap;
  }
  // ---
  
  foreach($descriptionElements as $element){
    if($element->class == 'TextData'){
      $outArray[] = theme('cdm_descriptionElementTextData', $element);
    }else if($element->class == 'Distribution'){
      if( !array_search($element->area->representation_L10n, $outArray)){
        $outArray[] = $element->area->representation_L10n;
        $glue = ', ';
        $sortOutArray = true;
        $enclosingHtml = 'p';
      }
    }else{
      $outArray[] = '<li>No method for rendering unknown description class: '.$element->classType.'</li>';
    }
  }

      $out = theme('cdm_descriptionElementArray', $outArray, $feature, $glue, $sortOutArray, $enclosingHtml);
      RenderHints::popFromRenderStack();
      return  $out;
}

/**
 * Allows theaming of the taxon page tabs
 * 
 * @param $tabname
 * @return unknown_type
 */
function diptera_cdm_taxonpage_tab($tabname){
  switch($tabname){
    case 'Synonymy' : return t('Nomenclature'); break;
    default : return t($tabname); 
  }
}

function diptera_cdm_feature_name($feature_name){
  switch($feature_name){
    case "Citation": return t("Citations");
    default: return t(ucfirst($feature_name));
  }
}


function diptera_get_partDefinition($nameType){
  
  if($nameType == 'ZoologicalName'){
    return array(
        'namePart' => array(
          'name' => true
        ),
        'nameAuthorPart' => array(
          'name' => true,
          'authors' => true
        ),
        'referencePart' => array(
         'authors' => true,
         'microreference' => true
        ),
        'statusPart' => array(
          'status' => true,
        ),
        'descriptionPart' => array(
          'description' => true,
        ),
      );
  }
  return false;
}

function diptera_get_nameRenderTemplate($renderPath){
  
  switch ($renderPath){
    case 'taxon_page_title': 
      $template = array(
          'namePart' => array('#uri'=>true),
        );
      break;
    case  'acceptedFor':
    case 'list_of_taxa': 
      $template = array(
        'namePart' => array('#uri'=>true),
        'referencePart' => array('#uri'=>true),
      );
      break;
    case 'typedesignations': 
      $template = array(
        'namePart' => array('#uri'=>true),
        'referencePart' => array('#uri'=>true)
      );
      break;
    case 'taxon_page_synonymy':
    case 'related_taxon':
    default: 
      $template = array(
        'namePart' => array('#uri'=>true),
        'referencePart' => array('#uri'=>true),
        'statusPart' => true,
        'descriptionPart' => true
      );
  }
  return $template;
}

?>