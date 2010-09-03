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
function garland_cichorieae_cdm_taxon_page_description($taxon, $mergedTrees, $media, $hideImages = false){

	RenderHints::pushToRenderStack('taxon_page_description');
	// description TOC
	$out = theme('cdm_featureTreeTOCs', $mergedTrees);

	// preferred image
	// 2 lines hard coded for testing
	if( variable_get('cdm_dataportal_show_default_image', false) && !$hideImages){

		//$defaultPreferredImage = drupal_get_path('theme', 'garland_cichorieae').'/images/nopic_400x300.jpg';
		$defaultRepresentationPart = false;
		$defaultRepresentationPart->width = 400;
		$defaultRepresentationPart->height = 300;
		$defaultRepresentationPart->uri = drupal_get_path('theme', 'garland_cichorieae').'/images/nopic_400x300_4x3cm.jpg';

		$imageUriParams = '&width=400&height=300&quality=95&format=jpeg';

		$imageMaxExtend = 400;
		$out .= '<div class="preferredImage">'.theme('cdm_preferredImage', $media, $defaultRepresentationPart, $imageMaxExtend, $imageUriParams).'</div>';
	}

	// description
	$out .= theme('cdm_featureTrees', $mergedTrees, $taxon);
	RenderHints::popFromRenderStack();

	return $out;
}

/*
 function garland_cichorieae_cdm_descriptionElementTextData($element){

 $description = str_replace("\n", "<br/>", $element->multilanguageText_L10n->text);
 $referenceCitation = '';
 $sourceRefs = '';

 if($element->reference){
 // disabling references for cichorieae description Elements because they all have faulty references
 $referenceCitation = '; '.theme('cdm_fullreference', $element->reference, TRUE);
 }
 //return '<p class="descriptionText">' . $description . $referenceCitation.'</p>';

 foreach($element->sources as $source){
 $referenceCitation = theme('cdm_DescriptionElementSource', $source);
 if($description && strlen($description) > 0 && $referenceCitation ){
 $sourceRefs .= ' ('.$referenceCitation.')' ;
 /*
 * TODO: why does not belongs this code to the cichorieae theme ??
 *
 }else if ($referenceCitation){
 $sourceRefs = $referenceCitation;
 //var_dump('Cichorieae:');
 //var_dump('sourceRefs => ' . $sourceRefs);
  

 }
 }
 return '<p class="descriptionText">' . $description . $sourceRefs . '</p>';
 }
 */




function garland_cichorieae_cdm_descriptionElementTextData($element, $asListElement, $block_subject){
	$description = str_replace("\n", "<br/>", $element->multilanguageText_L10n->text);
	$sourceRefs = '';
	$result = array();
	$out;
	$res_author;
	$res_date;

	$default_theme = variable_get('theme_default', 'garland_cichorieae');	
	if (($default_theme == 'flora_malesiana' || $default_theme == 'flore_afrique_centrale')
	    && $element->feature->titleCache == 'Citation'){
		$asListElement = true;
	}else{
		$asListElement = false;
	}  
	if (strcmp($block_subject, 'Name Usage') == 0){
		foreach($element->sources as $source){
			$referenceCitation = cdm_ws_get(CDM_WS_NOMENCLATURAL_REFERENCE_CITATION, 
			                                array($source->citation->uuid), 
								            "microReference=".urlencode($source->citationMicroReference));
			$referenceCitation = $referenceCitation->String; 
			//var_dump($referenceCitation->String);
			if($description && strlen($description) > 0 && $referenceCitation ){
				$sourceRefs .= ' ('.$referenceCitation.')' ;
			}else if ($referenceCitation){
				$sourceRefs = $referenceCitation;
			}			
		}
	}else{
		foreach($element->sources as $source){
			$referenceCitation = theme('cdm_DescriptionElementSource', 
		                               $source, 
								       (strcmp($block_subject, 'Name Usage') == 0) ? false : true);
			if($description && strlen($description) > 0 && $referenceCitation ){
				$sourceRefs .= ' ('.$referenceCitation.')' ;
			}else if ($referenceCitation){
				$sourceRefs = $referenceCitation;
			}
		}
	}
	/*
	foreach($element->sources as $source){ //var_dump($source); var_dump(' ++++++++ ');
		$referenceCitation = theme('cdm_DescriptionElementSource', 
		                           $source, 
								   (strcmp($block_subject, 'Name Usage') == 0) ? false : true);
		if($description && strlen($description) > 0 && $referenceCitation ){
			$sourceRefs .= ' ('.$referenceCitation.')' ;
		}else if ($referenceCitation){
			$sourceRefs = $referenceCitation;
		}
	}
	*/
	if(strlen($sourceRefs) > 0){
		$sourceRefs = '<span class="sources">' . $sourceRefs . '</span>';
	}

	if ($source->nameUsedInSource->uuid && (strcmp($block_subject, 'Name Usage') != 0)){ //do a link to name page
		$name_used_in_source_link_to_show = l($source->nameUsedInSource->titleCache,
											  path_to_name($source->nameUsedInSource->uuid),
											  array(), NULL, NULL, FALSE ,TRUE);
	}else if ($source->nameUsedInSource->uuid && (strcmp($block_subject, 'Name Usage') == 0)){
		$name_used_in_source_link_to_show = $source->nameUsedInSource->titleCache;
	}else if (strlen($source->nameUsedInSource->originalNameString) > 0){ //show a text without link
		$name_used_in_source_link_to_show = $source->nameUsedInSource->originalNameString;
	}

	if($asListElement && (strcmp($block_subject, 'Name Usage') == 0)){	 
		$out = '<li class="descriptionText">' . $name_used_in_source_link_to_show;
		//adding ":" if necesary
		if ($name_used_in_source_link_to_show && ($description || $sourceRefs)){
			$out .= ': ';
		}
		$out .= $description . $sourceRefs . theme('cdm_annotations_as_footnotekeys', $element) . '</li>';
	}else if ($asListElement){
		$out = '<li class="descriptionText">' . $name_used_in_source_link_to_show;
		//adding ":" if necesary
		if ($name_used_in_source_link_to_show && ($description || $sourceRefs)){
			$out .= ': ';
		}
		$out .= $description . $sourceRefs . theme('cdm_annotations_as_footnotekeys', $element) . '</li>';
	//special handling for flora malesiana TODO: possible better way to implement this case?
	}else{
		if ($name_used_in_source_link_to_show){
			$name_used_in_source_link_to_show = ' (name in source: '. $name_used_in_source_link_to_show . ')';
		}
		$out = $description . $sourceRefs . $name_used_in_source_link_to_show;
		$out .= theme('cdm_annotations_as_footnotekeys', $element);
	}

	// add annotations as footnote key
	//$out .= theme('cdm_annotations_as_footnotekeys', $element); move above
	return $out;
}






function garland_cichorieae_cdm_taxon_page_images($taxon, $media){

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
	writeFlashCode( "http://media.bgbm.org/erez/fsi/fsi.swf?&cfg=showcase_presets/showcase_info.fsi&effects=%26quality%3D95&showcase_query='.$query.'&skin=silver&showcase_labeltextheight=50&textbox_textfrom=IPTC_WP6&textbox_height=50&param_backgroundcolor=454343&publishwmode=opaque&showcase_hscroll=true&showcase_basecolor=454343&plugins=textbox,fullscreen",
		"http://media.bgbm.org/erez/erez?src=erez-private/flashrequired.svg&tmp=Large&quality=97&width=620&height=400",
		"width=620;height=400;bgcolor=454343;wmode=opaque");
// -->
</script>
<noscript>
	<object classid="clsid:D27CDB6E-AE6D-11cf-96B8-444553540000" codebase="http://download.macromedia.com/pub/shockwave/cabs/flash/swflash.cab#version=6,0,65,0" width="470" height="400">
		<param name="movie" value="http://media.bgbm.org/erez/fsi/fsi.swf?&cfg=showcase_presets/showcase_info.fsi&effects=%26quality%3D95&showcase_query='.$query.'&skin=silver&showcase_labeltextheight=50&textbox_textfrom=IPTC_WP6&textbox_height=50&param_backgroundcolor=454343&publishwmode=opaque&showcase_hscroll=true&showcase_basecolor=454343plugins=textbox,fullscreen"/>
		<param name="bgcolor" value="454343" />
		<param name="wmode" value="opaque" />
		<param name="allowscriptaccess" value="always" />
		<param name="allowfullscreen" value="true" />
		<param name="quality" value="high" />
		<embed src="http://media.bgbm.org/erez/fsi/fsi.swf?&cfg=showcase_presets/showcase_info.fsi&effects=%26quality%3D95&showcase_query='.$query.'&skin=silver&showcase_labeltextheight=50&textbox_textfrom=IPTC_WP6&textbox_height=50&param_backgroundcolor=454343&publishwmode=opaque&showcase_hscroll=true&showcase_basecolor=454343plugins=PrintSave,textbox,fullscreen"
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

function garland_cichorieae_cdm_taxon_page_images_cichorieae_copyright(){
	$out = '<div id="cichorieae-copyright">';
	$out .= '<p>';
	$out .= '&copy; Images used on this website remain copyright of the individual photographer.<br> To obtain permission to use images in publications or websites please contact the network at <a href="edit-wp6-cichorieae@bgbm.org">edit-wp6-cichorieae@bgbm.org</a>. Images may be used for personal use, such as PowerPoint presentations, without permission.';
	$out .= '</p>';
	$out .= '</div>';

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

function garland_cichorieae_get_partDefinition($nameType){
	if($nameType == 'BotanicalName'){
		return array(
        'namePart' => array(
          'name' => true
		),
        'nameAuthorPart' => array(
          'name' => true,
          'authors' => true
		),
        'referencePart' => array(
          'reference' => true,
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

function garland_cichorieae_get_nameRenderTemplate($renderPath){

	switch($renderPath) {
		case 'taxon_page_title':
			$template = array(
            'namePart' => array('#uri'=>true)
			);
			break;
		case 'taxon_page_synonymy':
		case 'related_taxon':
			$template = array(
          'nameAuthorPart' => array('#uri'=>true),
          'referencePart' => true,
          'statusPart' => true,
          'descriptionPart' => true
			);
			break;
		case 'acceptedFor':
			$template = array(
            'nameAuthorPart' => array('#uri'=>true),
            'referencePart' => true
			);
			break;
		case 'typedesignations':
		case 'list_of_taxa':
		case '#DEFAULT':
			$template = array(
            'nameAuthorPart' => array('#uri'=>true),
            'referencePart' => true
			);
	}
	return $template;
}

/**
 */
function garland_cichorieae_cdm_taxon_list_thumbnails($taxon){

	$gallery_name = $taxon->uuid;

	$showCaption = variable_get('cdm_dataportal_findtaxa_show_thumbnail_captions', 0);
	$prefMimeTypeRegex = 'image:.*';
	$prefMediaQuality = '*';
	$cols = variable_get('cdm_dataportal_findtaxa_media_cols', 3);
	$maxRows = variable_get('cdm_dataportal_findtaxa_media_maxRows', 1);
	$maxExtend = variable_get('cdm_dataportal_findtaxa_media_maxextend', 120);

	if($showCaption){
		$captionElements = array('title', '#uri'=>t('open Image'));
	}

	$galleryLinkUri = path_to_taxon($taxon->uuid).'/images';
	$mediaList = cdm_ws_get(CDM_WS_PORTAL_TAXON_MEDIA, array($taxon->uuid, $prefMimeTypeRegex, $prefMediaQuality));
	$out .= theme('cdm_media_gallerie', $mediaList, $gallery_name ,$maxExtend, $cols, $maxRows, $captionElements, 'NORMAL', $galleryLinkUri, null);

	return $out;
}

