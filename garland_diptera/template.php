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

/**
 * This function sort an array of description elements where all the elements
 * are of type UUID_CITATION. The sorting is done based at first on the title
 * cache of the author and second of the date of the publication.
 * @param $x An element of the array
 * @param $y An element of the array
 * @return array The sortered description elements array
 */
function compare_citations($x, $y)
{

	if( !$x->sources[0]->citation->uuid){// && !$y->sources[0]->citation->uuid){
		$res = -1;
		//var_dump($y->sources[0]->citation->uuid);
	}elseif(!$y->sources[0]->citation->uuid){
		$res = 1;
		//var_dump($x->sources[0]->citation->uuid);
	}
	else{


  $author_team_x = cdm_ws_get(CDM_WS_REFERENCE_AUTHORTEAM, $x->sources[0]->citation->uuid);
  $author_team_y = cdm_ws_get(CDM_WS_REFERENCE_AUTHORTEAM, $y->sources[0]->citation->uuid);

	 //same author, and different year
	if($author_team_x->titleCache == $author_team_y->titleCache){
		$x_year = substr(
		        $x->sources[0]->citation->datePublished->start,
		        0,
		        strpos($x->sources[0]->citation->datePublished->start,'-'));
		$y_year = substr(
		          $y->sources[0]->citation->datePublished->start,
		          0,
		          strpos($y->sources[0]->citation->datePublished->start,'-'));
		if ($x_year < $y_year){//the year of the first publication is smaller
			$res = -1;
		}
		else if($x_year == $y_year){ //if same year check the page
			$x_page = $x->sources[0]->citationMicroReference;
			$y_page = $y->sources[0]->citationMicroReference;
			if($x_page < $y_page){
				$res = -1;
			}
			else{
				$res = 1;
			}
		}else
		$res = 1;
	}
	//different author and the first one is alphabetically smaller
	//else if($x->sources[0]->citation->authorTeam->teamMembers[0]->lastname <
	//$y->sources[0]->citation->authorTeam->teamMembers[0]->lastname){
	else if ($author_team_x->titleCache < $author_team_y->titleCache)	{
		$res = -1;
	}
	//different author and the second one is alphabetically smaller
	else{
		$res = 1;
	}

  }
	//var_dump($res);
	//var_dump(' ============ ');
	return $res;
}


function garland_diptera_cdm_descriptionElements($descriptionElements){

  $outArray = array();
  $glue = '';
  $sortOutArray = false;
  $enclosingHtml = 'ul';

	$citations = array();

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
	usort($descriptionElements, 'compare_citations');
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
		case 'Has orthographic variant': return t("Misspellings");
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
    case '#DEFAULT':
      $template = array(
        'namePart' => array('#uri'=>true),
        'referencePart' => array('#uri'=>true),
        'statusPart' => true,
        'descriptionPart' => true
      );
  }
  return $template;
}

function garland_diptera_cdm_OriginalSource($descriptionElementSource, $doLink = TRUE){

    //ev. delegate to theme_cdm_ReferencedEntityBase
    $out = '';
    if($descriptionElementSource->citation){
      $datePublished = $descriptionElementSource->citation->datePublished;
      if (strlen($datePublished->start) >0){
        $year=substr($datePublished->start,0,strpos($datePublished->start,'-'));
      }

        $author_team = cdm_ws_get(CDM_WS_REFERENCE_AUTHORTEAM, $descriptionElementSource->citation->uuid);
        $author_team_titlecache = $author_team->titleCache;
        if (strlen($year)>0){
          $reference = $author_team_titlecache.' '. $year;
        }else {
          $reference = $author_team_titlecache ;
        }

      if($doLink){
        $out = l('<span class="reference">'.$reference.'</span>'
          , path_to_reference($descriptionElementSource->citation->uuid)
          , array("class"=>"reference")
          , NULL, NULL, FALSE ,TRUE);
      } else {
       $out = $reference;
      }
      if($descriptionElementSource->citationMicroReference){
        $out .= ': '. $descriptionElementSource->citationMicroReference;
      }
    }
    return $out;
}