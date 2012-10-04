<?php

/**
 * The description page is supposed to be the front page for a taxon.
 *
 * @param TaxonTO $taxonTO
 * @return
 */
function palmweb_2_cdm_taxon_page_profile($variables){

  $taxon = $variables['taxon'];
  $mergedTrees = $variables['mergedTrees'];
  $media = $variables['media'];
  $hideImages = $variables['hideImages'];
  
  $out = '';
      
  if(!$hideImages){
    // preferred image
    // hardcoded for testing;
    $defaultRepresentationPart = false;
    $defaultRepresentationPart->width = 184;
    $defaultRepresentationPart->height = 144;
    $defaultRepresentationPart->uri = drupal_get_path('theme', 'palmweb_2').'/images/no_picture.png';

    // preferred image size 184px × 144
    $imageMaxExtend = 184;
    $out .= '<div id="taxonProfileImage">'.theme('cdm_preferredImage', array('media' => $media, 'defaultRepresentationPart' => $defaultRepresentationPart, 'imageMaxExtend' => $imageMaxExtend)).'</div>';
  }

  // description TOC
  $out .= theme('cdm_featureTreeTOCs', array('mergedTrees' => $mergedTrees));
  // description
  $out .= theme('cdm_featureTrees', array('mergedTrees' => $mergedTrees, 'taxon' => $taxon));

  return $out;
}


function palmweb_2_cdm_descriptionElementDistribution($variables) {
  $descriptionElements = $variables['descriptionElements'];
  $enclosingTag = $variables['enclosingTag'];
  
  $out = '';
  $separator = ', ';

  RenderHints::pushToRenderStack('descriptionElementDistribution');
  RenderHints::setFootnoteListKey(UUID_DISTRIBUTION);

  $itemCnt = 0;
  foreach($descriptionElements as $descriptionElement){
  	//$out .= ($descriptionElement->class);
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
		
        $out .= '<' . $enclosingTag . ' class="DescriptionElement DescriptionElement-' . $descriptionElement->class .'">';
        //$out .= $descriptionElement->area->representation_L10n . $annotationFootnoteKeys . $sourcesFootnoteKeyList;
        $out .= $descriptionElement->area->representation_L10n;
        if(++$itemCnt < count($descriptionElements)){
          $out .=  $separator;
        }
        $out .= "</" . $enclosingTag . ">";
  }
  $taxonTrees =  cdm_ws_get(CDM_WS_PORTAL_TAXONOMY);
  $reference = new stdClass();
  foreach($taxonTrees as $taxonTree){
    if ($taxonTree->uuid == variable_get('cdm_taxonomictree_uuid')){
      if(isset($taxonTree->reference)) $reference = $taxonTree->reference;
      break;
    }
  }
  $referenceCitation = '';
  if(isset($reference->uuid)) {
    $referenceCitation = '(<span class="reference">'.l('World Checklist of Monocotyledons', path_to_reference($reference->uuid), array("class"=>"reference"), NULL, NULL, FALSE ,TRUE).'</span>)';
  }

  $sourceRefs = '';
  if($out && strlen($out) > 0 ){
    $sourceRefs = ' '.$referenceCitation;
  }

  if(strlen($sourceRefs) > 0){
    $sourceRefs = '<span class="sources">' . $sourceRefs . '</span>';
  }

  RenderHints::popFromRenderStack();
  return $out. $sourceRefs ;

}

function palmweb_2_cdm_feature_nodesTOC($variables){
  $featureNodes = $variables['featureNodes'];
  $out = '';
  
  global $theme;
  
  $out .= '<ul>';
  $countFeatures = 0;
  $numberOfChildren = count(cdm_ws_get(CDM_WS_PORTAL_TAXONOMY_CHILDNODES_OF_TAXON, array (get_taxonomictree_uuid_selected(), substr(strrchr($_GET["q"], '/'), 1))));
  if ($numberOfChildren != 0) {
    $out .= '<li>'.l(t(theme('cdm_feature_name', array('feature_name' => 'Number of Taxa'))), $_GET['q'], 
        array('attributes' => array('class' => array('toc')),'fragment' => generalizeString('Number Of Taxa'))) .'</li>';
  }
  foreach($featureNodes as $node){

    if(hasFeatureNodeDescriptionElements($node)){

      $featureRepresentation = isset($node->feature->representation_L10n) ? $node->feature->representation_L10n : 'Feature';
      // HACK to implement images for taxa, should be removed
      if($node->feature->uuid != UUID_IMAGE && $node->feature->uuid != UUID_USE ){
      	$countFeatures++;
      	$countFeatures++;
        $out .= '<li>'.l(t(theme('cdm_feature_name',  array('feature_name' => $featureRepresentation))), $_GET['q'], 
            array('attributes' => array('class' => array('toc')),'fragment' => generalizeString($featureRepresentation))).'</li>';
      }
    }
  }
  //Setting the Anchor to the Bibliography section if the option is enabled
  $show_bibliography = variable_get('cdm_show_bibliography', 1);
  
  $markerTypes['markerTypes'] = UUID_MARKERTYPE_USE;
  $useDescriptions = cdm_ws_get(CDM_WS_PORTAL_TAXON_DESCRIPTIONS, substr(strrchr($_GET["q"], '/'), 1), queryString($markerTypes));
  if(!empty($useDescriptions)) {
  		$out .= '<li>'.l(t(theme('cdm_feature_name',  array('feature_name' => 'Uses'))), $_GET['q'], array('attributes' => array('class' => array('toc')), 'fragment' => 'userecords')) . '</li>';
  }
  
  if ($show_bibliography && $countFeatures != 0) {
  	$out .= '<li>'.l(t(theme('cdm_feature_name',  array('feature_name' => 'Bibliography'))), $_GET['q'], array('attributes' => array('class' => array('toc')), 'fragment' => 'bibliography')) . '</li>';
  }
  $out .= '</ul>';
  return $out;
}

function palmweb_2_cdm_feature_nodes($variables){
  $mergedFeatureNodes = $variables['mergedFeatureNodes'];
  $taxon = $variables['taxon'];
  
  $out = '';
  RenderHints::pushToRenderStack('feature_nodes');

  $gallery_settings = getGallerySettings(CDM_DATAPORTAL_DESCRIPTION_GALLERY_NAME);
  //Creating an array to place the description elements in 
  $bibliographyOut = array();
  $countFeatures = 0;
  $numberOfChildren = count(cdm_ws_get(CDM_WS_PORTAL_TAXONOMY_CHILDNODES_OF_TAXON, array (get_taxonomictree_uuid_selected(), $taxon->uuid)));
  if ($taxon->name->rank->titleCache == "Genus") {
  	$subRank = "species";
  }
  if($taxon->name->rank->titleCache == "Species") {
  	$subRank = "infraspecific taxa";
  }
  if ($numberOfChildren != 0) {
  	
  	$out .= '<a name="number_of_taxa"> </a><H2>Number of Taxa</H2><div class="content"> <ul class="description">';
	$out .= '<li class=\"descriptionText DescriptionElement\">' . $numberOfChildren . " " . $subRank . '</li></ul>';
  }

  foreach($mergedFeatureNodes as $node){

    if(hasFeatureNodeDescriptionElements($node)) {
     
      $featureRepresentation = isset($node->feature->representation_L10n) ? $node->feature->representation_L10n : 'Feature';
      $block->module = 'cdm_dataportal';
      //if the option is enabled the description elements will be added to the array
      $show_bibliography = variable_get('cdm_show_bibliography', 1);
  	  if ($show_bibliography) {
      	$bibliographyOut[] =  $node->descriptionElements;
  	  }
      $media_list = array();
      if($node->feature->uuid != UUID_IMAGE && $node->feature->uuid != UUID_USE ) {
      	$countFeatures++;
      	$countFeatures++;
        $block->delta = generalizeString($featureRepresentation);
        $block->subject = '<span class="'. html_class_atttibute_ref($node->feature) . '">' . theme('cdm_feature_name',  array('feature_name' => $featureRepresentation)) . '</span>';
        $block->module = "cdm_dataportal-feature";
        $block->content = '';
        
        /*
         * Content/DISTRIBUTION
         */
        if($node->feature->uuid == UUID_DISTRIBUTION){

          if(variable_get(DISTRIBUTION_TEXTDATA_DISPLAY_ON_TOP, 0)){
            $distributionTextDataList = array();
            $distributionElementsList = array();
            foreach($node->descriptionElements as $descriptionElement){
            	
              if($descriptionElement->class == "TextData"){
                $distributionTextDataList[] = $descriptionElement;
              } else {
                $distributionElementsList[] = $descriptionElement;
              }
            }
            if(count($distributionTextDataList) > 0){
              $node->descriptionElements = $distributionElementsList;
              $block->content .= theme('cdm_descriptionElements', array('descriptionElements' => $distributionTextDataList, 'featureUuid' => $node->feature->uuid, 'taxon_uuid' => $taxon->uuid));
            }
          }

          // Display cdm distribution map TODO this is a HACK to a proper generic implementation?
          $block->content .= theme('cdm_distribution_map', array('taxon' => $taxon));
          $block->content .= theme('cdm_descriptionElements', array('descriptionElements' => $node->descriptionElements, 'featureUuid' => $node->feature->uuid, 'taxon_uuid' => $taxon->uuid));
        }

        /*
         * Content/COMMON_NAME
         */
        else if ($node->feature->uuid == UUID_COMMON_NAME){
          //TODO why is theme_cdm_descriptionElement_CommonTaxonName not beeing used???
          $block->content .= theme('cdm_common_names', array('elements' => $node->descriptionElements));
        /*
        }else if($node->feature->uuid == UUID_IMAGE_SOURCES) {
          $block->content .= theme('cdm_image_sources', $node->descriptionElements);
        */
        }

        /*
         * Content/ALL OTHER FEATURES
         */
        else if($node->feature->uuid == UUID_USE_RECORD) { 
        	$block->content .= theme('cdm_block_Uses', $taxon->uuid);
          	//$block->content .= theme('cdm_descriptionElements', $node->descriptionElements, $node->feature->uuid, $taxon->uuid),
        }
        else {
          $block->content .= theme('cdm_descriptionElements', array('descriptionElements' => $node->descriptionElements, 'featureUuid' => $node->feature->uuid, 'taxon_uuid' => $taxon->uuid));

          /*
           *  Content/ALL OTHER FEATURES/Subordinate Features
           *
           *  subordinate features are printed inline in one floating text,
           *  it is expected that supordinate features only "contain" TextData elements
           */
          // TODO move into own theme
          if(count($node->children) > 0){

            //TODO support more than one level of childen http://dev.e-taxonomy.eu/trac/ticket/2393
            $text = '';
            foreach ($node->children as $child){
            
             if (is_array($child->descriptionElements)){
               foreach ($child->descriptionElements as $element) {

                 if(is_array($element->media)){
                   // append media of supordinate emelents to list of main feature
                   $media_list = array_merge($media_list, $element->media);
                 }

                 $description = str_replace("\n", "<br/>", $element->multilanguageText_L10n->text);
                //TODO use localized version of feature name, the locale must match the locale of the multilanguage text (http://dev.e-taxonomy.eu/trac/ticket/2394)
                 $description = str_replace($element->feature->titleCache, '<em>' . $element->feature->titleCache . '</em>', $description);
               }
               $text .= " " . $description;
               $description = '';
             }

            }
            $block->content .= $text;
          }

        }

        /*
         * Media/ALL FEATURES
         */
        $media_list = array_merge($media_list, cdm_dataportal_media_from_descriptionElements($node->descriptionElements));

        $gallery = theme('cdm_media_gallerie', array(
           'mediaList' => $media_list, 
           'galleryName' => CDM_DATAPORTAL_DESCRIPTION_GALLERY_NAME . '_' . $node->feature->uuid,
           'maxExtend' => isset($gallery_settings['cdm_dataportal_media_maxextend']) ? $gallery_settings['cdm_dataportal_media_maxextend'] : NULL ,
           'cols' => isset($gallery_settings['cdm_dataportal_media_cols']) ? $gallery_settings['cdm_dataportal_media_cols'] : NULL ,
           'maxRows' => isset($gallery_settings['cdm_dataportal_media_maxRows']) ? $gallery_settings['cdm_dataportal_media_maxRows'] : NULL ,
           'captionElements' => isset($captionElements) ? $captionElements : NULL ,
           )
        );

        $block->content .= $gallery;
        $block->content .= theme('cdm_footnotes', array('footnoteListKey' => $node->feature->uuid));
        $block->content .= theme('cdm_annotation_footnotes', array('footnoteListKey' => $node->feature->uuid));
        // add anchor to subject
        $block->subject = '<a name="'.$block->delta.'"></a>'.  $block->subject;

        $block->region = false;
        $out .= theme('block', array( 'elements' => array('#block' => $block, '#children' => $block->content )));
      }
    
      
      
      
    }
    
  }
  //calling the theme function for Bibliography to add it to the output
  
  //Add the display of the number of taxa in the selected genus
  $out .= theme('cdm_block_Uses', array('taxonUuid' => $taxon->uuid));

	
  $show_bibliography = variable_get('cdm_show_bibliography', 1);
  if ($show_bibliography && $countFeatures !=0) {    
  	$out .= theme('cdm_descriptionElementBibliography', array('descriptionElementsBibliography' => $bibliographyOut));
  }

  RenderHints::popFromRenderStack();
  return $out;
}


function palmweb_2_cdm_search_results($variables){
  $pager = $variables['pager'];
  $path = $variables['path'];
  $query_parameters = $variables['query_parameters'];

	$showThumbnails = isset($_SESSION['pageoptions']['searchtaxa']['showThumbnails']) ? $_SESSION['pageoptions']['searchtaxa']['showThumbnails'] : 0;
	if( !is_numeric($showThumbnails)){
		//AT RBG KEW - 14/11/2011 - Set the show thumbnails to 0 by default 
		$showThumbnails = 0;
	}
	$setSessionUri = url('cdm_api/setvalue/session', array('query' => array('var' => '[pageoption][searchtaxa][showThumbnails]', 'val' => '')));
	drupal_add_js('$(document).ready(function() {

        // init
        if('.$showThumbnails.' == 1){
              $(\'.media_gallery\').show(20);
        } else {
          $(\'.media_gallery\').hide(20);
        }
        // add change hander
        $(\'#showThumbnails\').change(
          function(event){
            var state = 0;
            if($(this).is(\':checked\')){
              $(\'.media_gallery\').show(20);
              state = 1;
            } else {
              $(\'.media_gallery\').hide(20);
            }
            // store state in session variable
            var uri = \''.$setSessionUri.'\' + state;
            jQuery.get(uri);
          });
        });', "inline");

	drupal_set_title(t('Search results'));

	$out = ''; //l('Advanced Search', '/cdm_dataportal/search');
	//AT RBG KEW - 14/11/2011 - Changed the wording of the Show Thumbnails tick box text 
	$out = '<div class="page_options"><form name="pageoptions"><input id="showThumbnails" type="checkbox" name="showThumbnails" '.($showThumbnails == 1? 'checked="checked"': '').'> '.t('Show Image Thumbnails').'</form></div>';
	if(!empty($pager) && count($pager->records) > 0){
	    $out .= '<div id="search_results">';
		$out .= theme('cdm_list_of_taxa', array('records' => $pager->records));
		$out .= '</div>';
		$out .= theme('cdm_pager', array('pager' => $pager, 'path' => $path, 'parameters' => $query_parameters));
	} else {
		$out = '<h4 class="error">Sorry, no matching entries found.</h4>';
	}
	return $out;
}

//@WA: theme function moved to cdm_dataportal module, theme/cdm_dataportal.bibliography.theme
// so this can be used by other portals as well.
//@TODO: should this not be part of the palmweb_2 featuretree and be treated as a normal description feature?
//function theme_cdm_descriptionElementBibliography
//function formatReference_for_Bibliography($references) {

function palmweb_2_cdm_media_caption($variables){
  $media = $variables['media'];
  $elements = $variables['elements'];
  $fileUri = $variables['fileUri'];
  
	$media_metadata = cdm_read_media_metadata($media);

	$doTitle= !$elements || array_search('title', $elements)!== false;
	$doDescription = !$elements || array_search('description', $elements)!== false;
	$doArtist = !$elements || array_search('artist', $elements)!== false;
	$doLocation = !$elements || array_search('location', $elements)!== false;
	$doRights = !$elements || array_search('rights', $elements)!== false;

	$descriptionPrefix = "";

	$out = '<dl class="media-caption">';
	//title
	if($doTitle) {
	    if($media_metadata['title']){
		  $out .= '<dt class = "title">' . t('Title') . '</dt> <dd class = "title">' . $media_metadata['title'] . '</dd>';
		  $descriptionPrefix = "- ";
	    } else if(!($doDescription && $media_metadata['description'])) {
	      // use filename as fallbackoption if no description will be shown
          $out .= '<dt class = "title">' . t('Title') . '</dt> <dd class = "title">' . $media_metadata['filename'] . '</dd>';
          $descriptionPrefix = "- ";
	    }
	}
	//description
	if($media_metadata['description'] && $doDescription) {
		$out .= '<dt class = "description">' . t('Description') . '</dt> <dd class = "description">' . $descriptionPrefix . $media_metadata['description'] . '</dd>';
	}
	//artist
	if($media_metadata['artist'] && $doArtist) {
		$out .= '<dt class = "artist">' . t('Artist') . '</dt> <dd class = "astist">' . str_replace("'","", $media_metadata['artist']) . '</dd>';
	}
	//location
	if($doLocation){
		$location = '';
		$location .= $media_metadata['location']['sublocation'];
		if ($location && $media_metadata['location']['city']){
			$location .= ', ';
		}
		$location .= $media_metadata['location']['city'];
		if ($location && $media_metadata['location']['province']){
			$location .= ', ';
		}
		$location .= $media_metadata['location']['province'];
		if ($location && $media_metadata['location']['country']){
			$location .= ' (' . $media_metadata['location']['country'] . ')';
		} else {
			$location .= $media_metadata['location']['country'];
		}
		if ($location){
			$out .= '<dt class = "location">' . t('Location') . '</dt> <dd class = "location">' . $location  . '</dd>';
		}
	}
	//rights
	if($doRights){
		$rights = '';
		//copyrights
		$cnt = count($media_metadata['rights']['copyright']['agentNames']);
		if($cnt > 0){
			$rights .= '<dt class="rights">&copy;</dt> <dd class="rights"> ';
			for($i = 0; $i < $cnt; $i++){
				$rights .= str_replace("'","", $media_metadata['rights']['copyright']['agentNames'][$i]);
				if($i+1 < $cnt){
					$rights .= ' / ';
				}
			}
			$rights .= '</dd>';
		}
		//license
		$cnt = count($media_metadata['rights']['license']['agentNames']);
		if($cnt > 0){
			$rights .= '<dt class ="license">' . t('License') . '</dt> <dd class = "license">';
			for($i = 0; $i < $cnt; $i++){
				$rights .= $media_metadata['rights']['license']['agentNames'][$i];
				if ($i+1 < $cnt){
					$rights .= ' / ';
				}
			}
			$rights .= '</dd>';
		}
		if($rights){
			$out .=  $rights . '</dt>';
		}
	}
	//TODO add all other metadata elemenst generically
	$out .= '</dl>';
	//return value
	return $out;
}






/**
 *
 * Enter description here ...
 * @param unknown_type $reference
 * @param unknown_type $microReference
 * @param unknown_type $doLink
 * @param unknown_type $referenceStyle
 */
function palmweb_2_cdm_reference($variables ){
  $reference = $variables['reference'];
  $microReference = $variables['microReference'];
  $doLink = $variables['doLink'];
  $referenceStyle = $variables['referenceStyle'];
  
  $author_team = cdm_ws_get(CDM_WS_REFERENCE_AUTHORTEAM, $reference->uuid);

  $year = partialToYear($reference->datePublished->start);
  $citation = _short_form_of_author_team ($author_team->titleCache) . ($year ? '. '.$year : '');
  $citation = str_replace('..', '.', $citation);

  if($doLink){
    $out ='<span class="reference">';
    $out .= l($citation, path_to_reference($reference->uuid), array(
    'attributes' => array(
    "class" => "reference",
    ),
    'absolute' => TRUE,
    'html' => TRUE,
    ));
    $out .= '</span>';
  }
  else {
    $out = '<span class="reference">'.$citation.'</span>';
  }
  //FIXME use microreference webservice instead
  if(!empty($descriptionElementSource->citationMicroReference)){
    $out .= ': '. $descriptionElementSource->citationMicroReference;
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

function palmweb_2_get_partDefinition($variables){
   
  if($variables['nameType'] == 'BotanicalName'){
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

function palmweb_2_get_nameRenderTemplate($variables){

  switch($variables['renderPath']) {
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

function palmweb_2_cdm_feature_name($variables){
  $feature_name = $variables['feature_name'];
  switch($feature_name){
    case "Protologue": return t("Original Publication");
    default: return t(ucfirst($feature_name));
  }
}

function palmweb_2_cdm_taxon_page_title($variables){
  $taxon = $variables['taxon']; 
  $uuid = $variables['uuid'];
  $synonym_uuid = $variables['synonym_uuid'];
  
	RenderHints::pushToRenderStack('taxon_page_title');
	$synonym = cdm_ws_get(CDM_WS_PORTAL_TAXON, $synonym_uuid);
	if(isset($taxon->name->nomenclaturalReference)){
		$referenceUri = url(path_to_reference($taxon->name->nomenclaturalReference->uuid));
	}
	
	$out = theme('cdm_taxonName', array('taxonName' => $taxon->name, 'nameLink' => NULL, 'refenceLink' => $referenceUri, 'show_annotations' => FALSE));

	RenderHints::popFromRenderStack();
	if (isset($synonym->name->titleCache)){
	$result = '<span class = "synonym_title">' .$synonym->name->titleCache . ' is synonym of ' .'</span>'.
		   '<span class="'.$taxon->class.'">'.$out.'</span>';
	} else {
		$result = '<span class="'.$taxon->class.'">'.$out.'</span>';
	}
	return $result;

}

//@WA this theme function does not exist..
/*
function palmweb_2_cdm_uri_to_synonym($synonymUuid, $acceptedUuid, $pagePart = null) {
	$acceptedPath = path_to_taxon($acceptedUuid, true);
	return url($acceptedPath . ($pagePart ? '/'.$pagePart : '') . '/'.$synonymUuid, 'highlite='.$synonymUuid);
	//return url($acceptedPath.($pagePart ? '/'.$pagePart : ''), 'highlite='.$synonymUuid, $synonymUuid."/$synonymUuid");
	//return url("$acceptedPath/$synonymUuid".($pagePart ? '/'.$pagePart : ''), 'highlite='.$synonymUuid);
}
*/

/* 
 * Hook prepocess_page
 * 
 * Assign the css classes primary-links and secondary-links to the menus and
 * process the 'Login' menu item, to change into 'My account' after login and
 * change the tab title for the IMCE file browser
 * 
 * @author W.Addink <w.addink@eti.uva.nl>
 * @return void
 */
function palmweb_2_preprocess_page(&$vars) {

  if (isset($vars['main_menu'])) {
    //For the Palmae theme we want to change the menu item 'Login' into 'My account' if a user is logged in
    global $user;
    foreach ($vars['main_menu'] as $key => $value) {
        if($value['href'] == 'user' && !empty($user->name)){
            $vars['main_menu'][$key]['title'] = t('My account');
            $vars['main_menu'][$key]['href'] = 'user/' . $user->uid;
        }
    }
    // theme the main menu with the desired css classes
    $vars['primary_nav'] = theme('links__system_main_menu', array(
      'links' => $vars['main_menu'],
      'attributes' => array(
        'class' => array('links', 'inline', 'main-menu', 'primary-links'),
      ),
      'heading' => array(
        'text' => t('Main menu'),
        'level' => 'h2',
        'class' => array('element-invisible'),
      )
    ));
  }
  else {
    $vars['primary_nav'] = FALSE;
  }
  if (isset($vars['secondary_menu'])) {
    $vars['secondary_nav'] = theme('links__system_secondary_menu', array(
      'links' => $vars['secondary_menu'],
      'attributes' => array(
        'class' => array('links', 'inline', 'secondary-menu', 'secondary-links'),
      ),
      'heading' => array(
        'text' => t('Secondary menu'),
        'level' => 'h2',
        'class' => array('element-invisible'),
      )
    ));
  }
  else {
    $vars['secondary_nav'] = FALSE;
  }
  
  // Change IMCE tab to 'Personal Files'
  if(!empty($vars['tabs']['#primary'] )){
    foreach($vars['tabs']['#primary'] as $key => $value){     
      if($value['#link']['path'] == 'user/%/imce'){
        $vars['tabs']['#primary'][$key]['#link']['title'] = t('Personal Files');
      }
    }
  }
}
/*
 * Fix file urls in nodes
 *
 * In nodes, relative urls are used to include files like <img src="/files/..
 * Portals can be installed in configurations with sub-directories however in which case these urls need to be adjusted.
 * Examples: mysite.org, mysite.org/myportal, mysite.org/portals/myportal
 * Therefore preprocess nodes and replace these urls with a the appropriate url for the current setup.
 *
 * @author W.Addink <w.addink@eti.uva.nl>
 * @return void
 */
function palmweb_2_preprocess_node(&$vars) {
  $body = '';
  // Warning: use #markup value, for which filters like php, html etc are applied!
  if (isset($vars['content']['body'][0]['#markup'])) {
    $body = $vars['content']['body'][0]['#markup']; 
  }
  else {
    $vars['fixed_body'] = '';
    return;
  }

  $file_path = '/' . variable_get('file_public_path', conf_path() . '/files');
  global $base_url;
  $fixed_file_path = $base_url . '/' . $file_path ;
  $preg_file_path = preg_quote($file_path, '/');
  $body = preg_replace ('/src\s*=\s*["]\s*' . $preg_file_path.'/', 'src="'.$fixed_file_path , $body);
  $body = preg_replace ('/src\s*=\s*[\']\s*' . $preg_file_path.'/', 'src=\''.$fixed_file_path , $body);
  $body = preg_replace ('/href\s*=\s*["]\s*' . $preg_file_path.'/', 'href="'.$fixed_file_path , $body);
  $body = preg_replace ('/href\s*=\s*[\']\s*' . $preg_file_path.'/', 'href=\''.$fixed_file_path , $body);

  $vars['fixed_body'] = $body;    
}
