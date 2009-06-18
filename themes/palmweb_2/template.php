<?php

/**
 * The description page is supposed to be the front page for a taxon.
 *
 * @param TaxonTO $taxonTO
 * @return
 */
function palmweb_2_cdm_taxon_page_description($taxon, $mergedTrees){

  // preferred image
  // hardcoded for testing;
  $defaultPreferredImage = drupal_get_path('theme', 'palmweb_2').'/images/no_picture.png';
  $out .= '<div class="preferredImage">'.theme('cdm_preferredImage', $mergedTrees, $defaultPreferredImage, '&width=333&height=220&quality=95&format=jpeg').'</div>';
  
   // description TOC
  $out .= theme('cdm_featureTreeTOCs', $mergedTrees);
  // description
  $out .= theme('cdm_featureTrees', $mergedTrees, $taxon);
  
  return $out;
}

function palmweb_2_cdm_taxon_page_images($taxon, $taxonDescriptions){
  
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
        //$part = $taggedText->text == "subsp." ? "s" : $taggedText->text;
        $nameArray[] = $taggedText->text;
      }
    }
       
    $query = join("%5F", $nameArray) . '%20AND%20jpg';
    
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
          'name' => true,
          'authorTeam' => true,   
        ),
        'authorshipPart' => array(
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
      case 'acceptedFor':
        $template = array(
          'namePart' => array('#uri'=>true),
          'authorshipPart' => true
        );
        break;
      case 'taxon_page_title':
      case 'list_of_taxa':
      case 'taxon_page_synonymy':
      case 'typedesignations': //TODO correct template for typedesignations?
      default:
        $template = array(
          'namePart' => array('#uri'=>true),
          'authorshipPart' => true,
          'referencePart' => array('#uri'=>true),
          'descriptionPart' => true
        );
  }
  return $template;
}
