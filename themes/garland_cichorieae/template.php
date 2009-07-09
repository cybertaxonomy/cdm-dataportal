<?php
// $Id$

/**
 * Overrides of generic themeing functions in cdm_datportal.theme.php
 */


/**
 * The description page is supposed to be the front page for a taxon.
 *
 * @param TaxonTO $taxonTO
 * @return
 */
function garland_cichorieae_cdm_taxon_page_description($taxon, $mergedTrees){

  // description TOC
  $out .= theme('cdm_featureTreeTOCs', $mergedTrees);

  // preferred image
  // hardcoded for testing
  $defaultPreferredImage = drupal_get_path('theme', 'garland_cichorieae').'/images/nopic_400x300.jpg';
  $out .= '<div class="preferredImage">'.theme('cdm_preferredImage', $mergedTrees, $defaultPreferredImage, '&width=400&height=300&quality=95&format=jpeg').'</div>';

  // description
  $out .= theme('cdm_featureTrees', $mergedTrees, $taxon);

  return $out;
}

function garland_cichorieae_cdm_descriptionElementTextData($element){

  $description = str_replace("\n", "<br/>", $element->multilanguageText_L10n->text);
  $referenceCitation = '';
  if($element->reference){
    // disabling references for cichorieae description Elements because they all have faulty references
    //$referenceCitation = '; '.theme('cdm_fullreference', $element->reference, TRUE);
  }
  return '<p class="descriptionText">' . $description . $referenceCitation.'</p>';
}

function garland_cichorieae_cdm_taxon_page_images($taxon, $taxonDescriptions){

  if($taxonDescriptions){
    foreach($taxonDescriptions as $descriptionElements){
      foreach($descriptionElements->elements as $element){
        if($element->feature->uuid == UUID_IMAGE){
          $flashLink = true;
          break;
        }
      }
    }
  }

  if($flashLink){

    $taggedName = $taxon->name->taggedName;

    $nameArray = array();
    foreach($taggedName as $taggedText){
      if($taggedText->type == 'name' || $taggedText->type == 'rank'){
        // replacing of "subsp." with "s" is cichorieae specific
        $part = $taggedText->text == "subsp." ? "s" : $taggedText->text;
        $nameArray[] = $part;
      }
    }

    $query = join("%5F", $nameArray) . '%20AND%20jpg';

    $out = '
  
  <script type="text/javascript" src="http://media.bgbm.org/erez/js/fsiwriter.js"></script>

<script type="text/javascript">
<!--
	writeFlashCode( "http://media.bgbm.org/erez/fsi/fsi.swf?&cfg=showcase_presets/showcase_info.fsi&effects=%26quality%3D95&showcase_query='.$query.'&skin=silver&showcase_labeltextheight=50&textbox_textfrom=IPTC_WP6&textbox_height=50&param_backgroundcolor=454343&publishwmode=opaque&showcase_hscroll=true&showcase_basecolor=454343&plugins=PrintSave,textbox",
		"http://media.bgbm.org/erez/erez?src=erez-private/flashrequired.svg&tmp=Large&quality=97&width=620&height=400",
		"width=620;height=400;bgcolor=454343;wmode=opaque");
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
			width="620"
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

/**
 * @overrides theme_cdm_taggedtext2html in order to replace t.infr and t.infgen. with '[unranked]'
 */
function garland_cichorieae_cdm_taggedtext2html(array &$taggedtxt, $tag = 'span', $glue = ' ', $skiptags = array()){
  $out = '';
  $i = 0;
  foreach($taggedtxt as $tt){
    if(!in_array($tt->type, $skiptags) && strlen($tt->text) > 0){
      $out .= (strlen($out) > 0 && ++$i < count($taggedtxt)? $glue : '').'<'.$tag.' class="'.$tt->type.'">';
      if($tt->type == "rank" && ($tt->text == "t.infr." || $tt->text == "t.infgen.")){
        $out .= t('[unranked]');
      }else{
        $out .= t($tt->text);
      }
      $out .= '</'.$tag.'>';
    }
  }
  return $out;
}

function garland_cichorieae_cdm_descriptionElementArray($elementArray, $feature, $glue = '', $sortArray = false, $enclosingHtml = 'ul'){
  $enclosingHtml = 'div';
  $out = '<'.$enclosingHtml.' class="description" id="'.$feature->representation_L10n.'">';

  if($sortArray) sort($elementArray);

  $out .= join($elementArray, $glue);

  $out .= '</'.$enclosingHtml.'>';
  return $out;
}


/**
 * all reference links switched of
 */
function garland_cichorieae_cdm_nomenclaturalReferenceSTO($referenceSTO, $doLink = FALSE, $cssClass = '', $separator = '<br />' , $enclosingTag = 'li'){

  $doLink = FALSE;

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


/***** GARLAND OVERRIDES ******/

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

function theme_get_partDefinition($nameType){
  if($nameType == 'BotanicalName'){
    return array(
        'namePart' => array(
          'name' => true
    ),
        'authorshipPart' => array(
          'authorTeam' => true,   
    ),
        'referencePart' => array(
          'reference' => true      
    ),
        'microreferencePart' => array(
          'microreference' => true,
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

function theme_get_nameRenderTemplate($renderPath){

  switch($renderPath) {
    case 'taxon_page_title':
      $template = array(
            'namePart' => array('#uri'=>true)
      );
      break;
    case 'taxon_page_synonymy':
      $template = array(
          'namePart' => array('#uri'=>true),
          'referencePart' => array('#uri'=>true),
          'microreferencePart' => true,
          'statusPart' => true,
          'descriptionPart' => true
      );
      break;
    case 'acceptedFor':
      $template = array(
            'namePart' => array('#uri'=>true),
            'authorshipPart' => true,
            'referencePart' => array('#uri'=>true)
      );
      break;
    default:
    case 'list_of_taxa':
    case 'typedesignations':
      $template = array(
            'namePart' => array('#uri'=>true),
            'referencePart' => true,
            'microreferencePart' => true
      );
  }
  return $template;
}
