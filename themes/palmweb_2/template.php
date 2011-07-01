<?php

/**
 * The description page is supposed to be the front page for a taxon.
 *
 * @param TaxonTO $taxonTO
 * @return
 */
function palmweb_2_cdm_taxon_page_profile($taxon, $mergedTrees, $media, $hideImages = false){


  if(!$hideImages){
    // preferred image
    // hardcoded for testing;
    $defaultRepresentationPart = false;
    $defaultRepresentationPart->width = 184;
    $defaultRepresentationPart->height = 144;
    $defaultRepresentationPart->uri = drupal_get_path('theme', 'palmweb_2').'/images/no_picture.png';

    // preferred image size 184px × 144
    $imageMaxExtend = 184;
    $out .= '<div id="taxonProfileImage">'.theme('cdm_preferredImage', $media, $defaultRepresentationPart, $imageMaxExtend).'</div>';
  }

  // description TOC
  $out .= theme('cdm_featureTreeTOCs', $mergedTrees);
  // description
  $out .= theme('cdm_featureTrees', $mergedTrees, $taxon);

  return $out;
}


function palmweb_2_cdm_descriptionElementDistribution($descriptionElements) {

  $out = '';
  $separator = ', ';

    RenderHints::pushToRenderStack('descriptionElementDistribution');
    RenderHints::setFootnoteListKey(UUID_DISTRIBUTION);
    foreach($descriptionElements as $descriptionElement){
        // annotations as footnotes
//        $annotationFootnoteKeys = theme('cdm_annotations_as_footnotekeys', $descriptionElement);
//        // source references as footnotes
//        $sourcesFootnoteKeyList = '';
//        foreach($descriptionElement->sources as $source){
//            $_fkey = FootnoteManager::addNewFootnote(UUID_DISTRIBUTION, theme('cdm_DescriptionElementSource', $source, false));
//            $sourcesFootnoteKeyList .= theme('cdm_footnote_key', $_fkey, ($sourcesFootnoteKeyList ? $separator : ''));
//        }
//        if($annotationFootnoteKeys && $sourcesFootnoteKeyList){
//            $annotationFootnoteKeys .= $separator;
//        }
        $out .= $descriptionElement->area->representation_L10n . $annotationFootnoteKeys . $sourcesFootnoteKeyList . $separator;
    }

  $taxonTrees =  cdm_ws_get(CDM_WS_PORTAL_TAXONOMY);
  foreach($taxonTrees as $taxonTree){
    if ($taxonTree->uuid == variable_get('cdm_taxonomictree_uuid', FALSE)){
      $reference = $taxonTree->reference;
      break;
    }
  }
  $out = substr($out, 0, strlen($out)-strlen($separator) );

  $referenceCitation = '('.l('<span class="reference">World Checklist of Monocotyledons</span>', path_to_reference($reference->uuid), array("class"=>"reference"), NULL, NULL, FALSE ,TRUE).')';

  if($out && strlen($out) > 0 ){
    $sourceRefs .= ' '.$referenceCitation;
  }

  if(strlen($sourceRefs) > 0){
    $sourceRefs = '<span class="sources">' . $sourceRefs . '</span>';
  }

    RenderHints::popFromRenderStack();
  return $out. $sourceRefs ;

}


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

function palmweb_2_get_partDefinition($nameType){
  if($nameType == 'BotanicalName'){
    return array(
        'namePart' => array(
          'name' => true,
          'authors' => true,
        ),
        'authorshipPart' => array(
        ),
        'referencePart' => array(
          'reference' => true,
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

function palmweb_2_get_nameRenderTemplate($renderPath){

  switch($renderPath) {
      case 'acceptedFor':
        $template = array(
          'namePart' => array('#uri'=>true),
        );
        break;
      case 'typedesignations':
        $template = array(
          'namePart' => array('#uri'=>true),
          'referencePart' => true
        );
      case 'taxon_page_title':
      case 'list_of_taxa':
      case 'taxon_page_synonymy':
      case 'related_taxon':
      case 'polytomousKey':
      case '#DEFAULT':
        $template = array(
          'namePart' => array('#uri'=>true),
          'referencePart' => true,
          'descriptionPart' => true,
          'statusPart' => true
        );
  }
  return $template;
}

function palmweb_2_cdm_feature_name($feature_name){
  switch($feature_name){
    case "Protologue": return t("Original Publication");
    default: return t(ucfirst($feature_name));
  }
}

function palmweb_2_cdm_taxon_page_title($taxon, $uuid, $synonym_uuid){
	RenderHints::pushToRenderStack('taxon_page_title');
	$synonym = cdm_ws_get(CDM_WS_PORTAL_TAXON, $synonym_uuid);
	if(isset($taxon->name->nomenclaturalReference)){
		$referenceUri = url(path_to_reference($taxon->name->nomenclaturalReference->uuid));
	}
	$out = theme('cdm_taxonName', $taxon->name, null, $referenceUri, false);

	RenderHints::popFromRenderStack();
	if ($synonym->name->titleCache){
	$result = '<span class = "synonym_title">' .$synonym->name->titleCache . ' is synonym of ' .'</span>'.
		   '<span class="'.$taxon->class.'">'.$out.'</span>';
	}else{
		$result = '<span class="'.$taxon->class.'">'.$out.'</span>';
	}
	return $result;

}

function palmweb_2_cdm_uri_to_synonym($synonymUuid, $acceptedUuid, $pagePart = null) {
	$acceptedPath = path_to_taxon($acceptedUuid, true);
	return url($acceptedPath . ($pagePart ? '/'.$pagePart : '') . '/'.$synonymUuid, 'highlite='.$synonymUuid);
	//return url($acceptedPath.($pagePart ? '/'.$pagePart : ''), 'highlite='.$synonymUuid, $synonymUuid."/$synonymUuid");
	//return url("$acceptedPath/$synonymUuid".($pagePart ? '/'.$pagePart : ''), 'highlite='.$synonymUuid);
}
