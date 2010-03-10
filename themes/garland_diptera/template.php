<?php
// $Id: template.php,v 1.4.2.1 2007/04/18 03:38:59 drumm Exp $

/**
 * Sets the body-tag class attribute.
 *
 * Adds 'sidebar-left', 'sidebar-right' or 'sidebars' classes as needed.
 */
function phptemplate_body_class($sidebar_left, $sidebar_right) {
  if ($sidebar_left != '' && $sidebar_right != '') {
    $class = 'sidebars';
  }
  else {
    if ($sidebar_left != '') {
      $class = 'sidebar-left';
    }
    if ($sidebar_right != '') {
      $class = 'sidebar-right';
    }
  }

  if (isset($class)) {
    print ' class="'. $class .'"';
  }
}

/**
 * Return a themed breadcrumb trail.
 *
 * @param $breadcrumb
 *   An array containing the breadcrumb links.
 * @return a string containing the breadcrumb output.
 */
function phptemplate_breadcrumb($breadcrumb) {
  if (!empty($breadcrumb)) {
    return '<div class="breadcrumb">'. implode(' › ', $breadcrumb) .'</div>';
  }
}

/**
 * Allow themable wrapping of all comments.
 */
function phptemplate_comment_wrapper($content, $type = null) {
  static $node_type;
  if (isset($type)) $node_type = $type;

  if (!$content || $node_type == 'forum') {
    return '<div id="comments">'. $content . '</div>';
  }
  else {
    return '<div id="comments"><h2 class="comments">'. t('Comments') .'</h2>'. $content .'</div>';
  }
}

/**
 * Override or insert PHPTemplate variables into the templates.
 */
function _phptemplate_variables($hook, $vars) {
  if ($hook == 'page') {

    if ($secondary = menu_secondary_local_tasks()) {
      $output = '<span class="clear"></span>';
      $output .= "<ul class=\"tabs secondary\">\n". $secondary ."</ul>\n";
      $vars['tabs2'] = $output;
    }

    // Hook into color.module
    if (module_exists('color')) {
      _color_page_alter($vars);
    }
    return $vars;
  }
  return array();
}

/**
 * Returns the rendered local tasks. The default implementation renders
 * them as tabs.
 *
 * @ingroup themeable
 */
function phptemplate_menu_local_tasks() {
  $output = '';

  if ($primary = menu_primary_local_tasks()) {
    $output .= "<ul class=\"tabs primary\">\n". $primary ."</ul>\n";
  }

  return $output;
}

function garland_diptera_cdm_descriptionElements($descriptionElements){

  $outArray = array();
  $glue = '';
  $sortOutArray = false;
  $enclosingHtml = 'ul';
  
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
      $asListElement = true;
      $outArray[] = theme('cdm_descriptionElementTextData', $element, $asListElement);
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

  return theme('cdm_descriptionElementArray', $outArray, $feature, $glue, $sortOutArray, $enclosingHtml);
}

/**
 * Allows theaming of the taxon page tabs
 * 
 * @param $tabname
 * @return unknown_type
 */
function garland_diptera_cdm_taxonpage_tab($tabname){
  switch($tabname){
    case 'Synonymy' : return t('Nomenclature'); break;
    default : return t($tabname); 
  }
}

function garland_diptera_cdm_feature_name($feature_name){
  switch($feature_name){
    case "Citation": return t("Citations");
    default: return t(ucfirst($feature_name));
  }
}


function garland_diptera_get_partDefinition($nameType){
  
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

function garland_diptera_get_nameRenderTemplate($renderPath){
  
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

