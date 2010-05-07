<?php

/**
 * The description page is supposed to be the front page for a taxon.
 *
 * @param TaxonTO $taxonTO
 * @return
 */
function palmweb_2_cdm_taxon_page_description($taxon, $mergedTrees, $media, $hideImages = false){

  
  if(!$hideImages){
    // preferred image
    // hardcoded for testing;
    $defaultRepresentationPart = false;
    $defaultRepresentationPart->width = 184;
    $defaultRepresentationPart->height = 144;
    $defaultRepresentationPart->uri = drupal_get_path('theme', 'palmweb_2').'/images/no_picture.png';
    
    // preferred image size 184px × 144
    $imageMaxExtend = 184;
    $out .= '<div class="preferredImage">'.theme('cdm_preferredImage', $media, $defaultRepresentationPart, $imageMaxExtend).'</div>';
  }
  
  // description TOC
  $out .= theme('cdm_featureTreeTOCs', $mergedTrees);
  // description
  $out .= theme('cdm_featureTrees', $mergedTrees, $taxon);
  
  return $out;
}

function _disabled_palmweb_2_cdm_taxon_page_images($taxon, $media){

  $flashLink = isset($media[0]);
  
  if($flashLink){
    
    $taggedName = $taxon->name->taggedName;
    
    $nameArray = array();
    foreach($taggedName as $taggedText){
      if($taggedText->type == 'name'){
        $nameArray[] = $taggedText->text;
      }
    }
    
   $query = join("%5F", $nameArray) . '%20AND%20EditWP6%20AND%20jpg';
    
  $out = '
  
  <script type="text/javascript" src="http://media.bgbm.org/erez/js/fsiwriter.js"></script>

<script type="text/javascript">
<!--
	writeFlashCode( "http://media.bgbm.org/erez/fsi/fsi.swf?&cfg=showcase_presets/showcase_info.fsi&effects=%26quality%3D95&showcase_query='.$query.'&skin=silver&showcase_labeltextheight=50&textbox_textfrom=IPTC_WP6&textbox_height=50&param_backgroundcolor=454343&publishwmode=opaque&showcase_hscroll=true&showcase_basecolor=454343&plugins=PrintSave,textbox",
		"http://media.bgbm.org/erez/erez?src=erez-private/flashrequired.svg&tmp=Large&quality=97&width=500&height=323",
		"width=500;height=323;bgcolor=454343;wmode=opaque");
// -->
</script>
<noscript>
	<object classid="clsid:D27CDB6E-AE6D-11cf-96B8-444553540000" codebase="http://download.macromedia.com/pub/shockwave/cabs/flash/swflash.cab#version=6,0,65,0" width="500" height="323">
		<param name="movie" value="http://media.bgbm.org/erez/fsi/fsi.swf?&cfg=showcase_presets/showcase_info.fsi&effects=%26quality%3D95&showcase_query='.$query.'&skin=silver&showcase_labeltextheight=50&textbox_textfrom=IPTC_WP6&textbox_height=50&param_backgroundcolor=454343&publishwmode=opaque&showcase_hscroll=true&showcase_basecolor=454343plugins=PrintSave,textbox"/>
		<param name="bgcolor" value="454343" />
		<param name="wmode" value="opaque" />
		<param name="allowscriptaccess" value="always" />
		<param name="allowfullscreen" value="true" />
		<param name="quality" value="high" />
		<embed src="http://media.bgbm.org/erez/fsi/fsi.swf?&cfg=showcase_presets/showcase_info.fsi&effects=%26quality%3D95&showcase_query='.$query.'&skin=silver&showcase_labeltextheight=50&textbox_textfrom=IPTC_WP6&textbox_height=50&param_backgroundcolor=454343&publishwmode=opaque&showcase_hscroll=true&showcase_basecolor=454343plugins=PrintSave,textbox"
			width="500"
			height="323"
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
//            $sourcesFootnoteKeyList .= theme('cdm_footnote_key', $_fkey, UUID_DISTRIBUTION, ($sourcesFootnoteKeyList ? $separator : ''));
//        }
//        if($annotationFootnoteKeys && $sourcesFootnoteKeyList){
//            $annotationFootnoteKeys .= $separator;
//        }
        $out .= $descriptionElement->area->representation_L10n . $annotationFootnoteKeys . $sourcesFootnoteKeyList . $separator;
    }
  
  $out = substr($out, 0, strlen($out)-2);
  $taxonTrees =  cdm_ws_get(CDM_WS_PORTAL_TAXONOMY);
  foreach($taxonTrees as $taxonTree){
    if ($taxonTree -> uuid == variable_get('cdm_taxonomictree_uuid', FALSE)){
      $reference = $taxonTree-> reference;
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
      case '#DEFAULT':
        $template = array(
          'namePart' => array('#uri'=>true),
          'referencePart' => true,
          'descriptionPart' => true
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
