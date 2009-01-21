<?php

/**
 * Overrides of generic themeing functions in cdm_datportal.theme.php
 */

/**
 * @param NameTO $nameTO
 * @return taxon name without author and nomencaltural reference
 */
function garland_cichorieae_cdm_taxon_page_title($nameTO){
  return theme('cdm_name', $nameTO, false, false, false, false);
}


/**
 * The description page is supposed to be the front page for a taxon.
 *
 * @param TaxonTO $taxonTO
 * @return
 */
function garland_cichorieae_cdm_taxon_page_description($taxonTO){
  // preferred image
  // hardcoded for testing
  $defaultPreferredImage = drupal_get_path('theme', 'garland_cichorieae').'/images/nopic.jpg';
  
  $out = theme('cdm_preferredImage', $taxonTO, $defaultPreferredImage, '&width=333&height=220&quality=95&format=jpeg');
  
  // description TOC
  $out .= theme('cdm_featureTreeToc', $taxonTO->featureTree);
  
  // descriptions
  $out .= theme('cdm_featureTree', $taxonTO->featureTree);
  
  return $out;
}


function garland_cichorieae_cdm_taxon_page_images($taxonTO){
  
  $descriptions = $taxonTO->featureTree->descriptions;
  
  foreach($descriptions as $descriptionTo){
    $features = $descriptionTo->features;
    foreach($features as $featureTo){
      if($featureTo->feature->term == 'Image'){
        
        $flashLink = $featureTo->descriptionElements[0]->description;
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
  $out = '<div class="description" id="'.$feature.'">';
  
  if($sortArray) sort($elementArray);
  
  $out .= join($elementArray, $glue);
  
  $out .= '</div>';
  return $out;
}

function garland_cichorieae_cdm_descriptionElementTextData($element){
  $description = str_replace("\n", "<br/>", $element->description);
  return '<p class="descriptionText">' . $description . '</p>';
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
