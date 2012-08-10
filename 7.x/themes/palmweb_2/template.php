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
        $out .= $descriptionElement->area->representation_L10n . $annotationFootnoteKeys . $sourcesFootnoteKeyList;
        if(++$itemCnt < count($descriptionElements)){
          $out .=  $separator;
        }
        $out .= "</" . $enclosingTag . ">";
  }
  $taxonTrees =  cdm_ws_get(CDM_WS_PORTAL_TAXONOMY);
  foreach($taxonTrees as $taxonTree){
    if ($taxonTree->uuid == variable_get('cdm_taxonomictree_uuid', FALSE)){
      $reference = $taxonTree->reference;
      break;
    }
  }

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

function palmweb_2_cdm_feature_nodesTOC($variables){
  $featureNodes = $variables['featureNodes'];
  
  global $theme;
  $out .= '<ul>';
  $countFeatures = 0;
  $numberOfChildren = count(cdm_ws_get(CDM_WS_PORTAL_TAXONOMY_CHILDNODES_OF_TAXON, array (get_taxonomictree_uuid_selected(), substr(strrchr($_GET["q"], '/'), 1))));
  if ($numberOfChildren != 0) {
    $out .= '<li>'.l(t(theme('cdm_feature_name', 'Number of Taxa')), $_GET['q'], array("class"=>"toc"), NULL, generalizeString('Number Of Taxa')).'</li>';
  }
  foreach($featureNodes as $node){

    if(hasFeatureNodeDescriptionElements($node)){

      $featureRepresentation = isset($node->feature->representation_L10n) ? $node->feature->representation_L10n : 'Feature';
      // HACK to implement images for taxa, should be removed
      if($node->feature->uuid != UUID_IMAGE && $node->feature->uuid != UUID_USE ){
      	$countFeatures++;
      	$countFeatures++;
        $out .= '<li>'.l(t(theme('cdm_feature_name', $featureRepresentation)), $_GET['q'], array("class"=>"toc"), NULL, generalizeString($featureRepresentation)).'</li>';
      }
    }
  }
  //Setting the Anchor to the Bibliography section if the option is enabled
  $show_bibliography = variable_get('cdm_show_bibliography', 1);
  
  $markerTypes['markerTypes'] = UUID_MARKERTYPE_USE;
  $useDescriptions = cdm_ws_get(CDM_WS_PORTAL_TAXON_DESCRIPTIONS, substr(strrchr($_GET["q"], '/'), 1), queryString($markerTypes));
  if(!empty($useDescriptions)) {
  		$out .= '<li>'.l(t(theme('cdm_feature_name', 'Uses')), $_GET['q'], array("class"=>"toc"), NULL, generalizeString('UseRecords')).'</li>';
  }
  
  if ($show_bibliography && $countFeatures != 0) {
  	$out .= '<li>'.l(t(theme('cdm_feature_name', 'Bibliography')), $_GET['q'], array("class"=>"toc"), NULL, generalizeString('Bibliography')).'</li>';
  }
  $out .= '</ul>';
  return $out;
}

function palmweb_2_cdm_feature_nodes($variables){
  $mergedFeatureNodes = $variables['mergedFeatureNodes'];
  $taxon = $variables['taxon'];

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
        $block->subject = '<span class="'. html_class_atttibute_ref($node->feature) . '">' . theme('cdm_feature_name', $featureRepresentation) . '</span>';
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
              $block->content .= theme('cdm_descriptionElements', $distributionTextDataList, $node->feature->uuid, $taxon->uuid);
            }
          }

          // Display cdm distribution map TODO this is a HACK to a proper generic implementation?
          $block->content .= theme('cdm_distribution_map', $taxon);
          $block->content .= theme('cdm_descriptionElements', $node->descriptionElements, $node->feature->uuid, $taxon->uuid);
        }

        /*
         * Content/COMMON_NAME
         */
        else if ($node->feature->uuid == UUID_COMMON_NAME){
          //TODO why is theme_cdm_descriptionElement_CommonTaxonName not beeing used???
          $block->content .= theme('cdm_common_names', $node->descriptionElements);
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
          $block->content .= theme('cdm_descriptionElements', $node->descriptionElements, $node->feature->uuid, $taxon->uuid);

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
        $captionElements = array('title', 'rights');

        $gallery = theme('cdm_media_gallerie', $media_list, CDM_DATAPORTAL_DESCRIPTION_GALLERY_NAME.'_'.$node->feature->uuid,
              $gallery_settings['cdm_dataportal_media_maxextend'],
              $gallery_settings['cdm_dataportal_media_cols'],
              $gallery_settings['cdm_dataportal_media_maxRows'],
              $captionElements
        );

        $block->content .= $gallery;
        $block->content .= theme('cdm_footnotes', $node->feature->uuid);
        $block->content .= theme('cdm_annotation_footnotes', $node->feature->uuid);
        // add anchor to subject
        $block->subject = '<a name="'.$block->delta.'"></a>'.  $block->subject;
       $out .= theme('block', $block);

        
      }
    
      
      
      
    }
    
  }
  //calling the theme function for Bibliography to add it to the output
  
  //Add the display of the number of taxa in the selected genus
  $out .= theme('cdm_block_Uses', $taxon->uuid);

	
  $show_bibliography = variable_get('cdm_show_bibliography', 1);
  if ($show_bibliography && $countFeatures !=0) {
  	$out .= theme('cdm_descriptionElementBibliography', $bibliographyOut);
  }

  RenderHints::popFromRenderStack();
  return $out;
}


function palmweb_2_cdm_search_results($variables){
  $pager = $variables['pager'];
  $path = $variables['path'];
  $query_parameters = $variables['query_parameters'];

	$showThumbnails = $_SESSION['pageoptions']['searchtaxa']['showThumbnails'];
	if( !is_numeric($showThumbnails)){
		//AT RBG KEW - 14/11/2011 - Set the show thumbnails to 0 by default 
		$showThumbnails = 0;
	}
	$setSessionUri = url('cdm_api/setvalue/session', "var=[pageoption][searchtaxa][showThumbnails]&val=");
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
	if(count($pager->records) > 0){
	    $out .= '<div id="search_results">';
		$out .= theme('cdm_list_of_taxa', $pager->records);
		$out .= '</div>';
		$out .= theme('cdm_pager', $pager, $path, $query_parameters);
	} else {
		$out = '<h4 class="error">Sorry, no matching entries found.</h4>';
	}
	return $out;
}

//Bibliography theming function
//@WA this theme function does not exist..
/*
function theme_cdm_descriptionElementBibliography($descriptionElementsBibliogragphy) {
	$listOfReferences = array();
	//$useDescriptions = cdm_ws_get()
	$markerTypes['markerTypes'] = UUID_MARKERTYPE_USE;
	$useDescriptions = cdm_ws_get(CDM_WS_PORTAL_TAXON_DESCRIPTIONS, substr(strrchr($_GET["q"], '/'), 1), queryString($markerTypes));
	 //= substr(strrchr($_GET["q"], '/'), 1);
	//$descout = print_r($useDescriptions);
    foreach ($descriptionElementsBibliogragphy as $descriptionElementsBiblio) {
		foreach ($descriptionElementsBiblio as $descriptionElementBiblio) {
			if(is_array($descriptionElementBiblio->sources)){
				foreach($descriptionElementBiblio->sources as $source){
					$isAlreadySelected = false;
					if(empty($listOfReferences)) {
						$listOfReferences[] = $source;
					}
					else {
						foreach ($listOfReferences as $selectedReference) {
							if ($selectedReference->citation->uuid == $source->citation->uuid) {
								$isAlreadySelected = true;
							}
						}
						//add the source in the list of reference/ This is to remove duplicates from the Bibliography section.
						if (!$isAlreadySelected) {
							$listOfReferences[] = $source;
						}
					}
					
				}
			}
		}
		
	}
	foreach($useDescriptions as $useDescription) {
		if (is_array($useDescription->sources)) {
			foreach ($useDescription->sources as $source) {
				$isAlreadySelected = false;
				if(empty($listOfReferences)) {
					$listOfReferences[] = $source;
				}
				else {
					foreach ($listOfReferences as $selectedReference) {
						if ($selectedReference->citation->uuid == $source->citation->uuid) {
							$isAlreadySelected = true;
						}
					}
					if (!$isAlreadySelected) {
						$listOfReferences[] = $source;
					}
				}
			}
		}
	}
	
	//Call the reference formatting function, it will do the heavy lifting
	$out = formatReference_for_Bibliogrpahy($listOfReferences);
	return $out;
}
*/
//@WA not used..
/*
function formatReference_for_Bibliogrpahy($references) {
	$out = '<div id="block-cdm_dataportal-feature-discussion"><a name="bibliography"> </a><H2>Bibliography</H2><div class="content"> <ul class="description">';
    $outTemp= array();
  foreach ($references as $reference) {
    $referenceString = '';
		switch ($reference->citation->type) {
			case "Journal":
				$referenceString .= "<li class=\"descriptionText DescriptionElement\">";
				$numberOfTeamMembers = count($reference->citation->authorTeam->teamMembers);
				$currentRecord = 1;
				if (!empty($reference->citation->authorTeam->teamMembers)) {
					foreach ($reference->citation->authorTeam->teamMembers as $teamMember) {
						if(!empty($teamMember->lastname) && !empty($teamMember->firstname)) {
							if ($currentRecord == 1) {
								$referenceString .= $teamMember->lastname . ", " . $teamMember->firstname;
							}
							else if ($numberOfTeamMembers != $currentRecord) {
								$referenceString .= " , " . $teamMember->lastname . ", " . $teamMember->firstname;	
							}
							else {
								$referenceString .= " & " . $teamMember->lastname . ", " . $teamMember->firstname;
								$referenceString .= ((str_endsWith($referenceString, ".") || str_endsWith($referenceString, ". ")) ? ' ' : ". ");
							}
							$currentRecord += 1;
						}
						else {
							if ($numberOfTeamMembers != $currentRecord) {
								$referenceString .= $teamMember->titleCache. " & ";	
							}
							else {
								$referenceString .= $teamMember->titleCache;
								$referenceString .= ((str_endsWith($referenceString, ".") || str_endsWith($referenceString, ". ")) ? ' ' : ". ");
							}
							$currentRecord += 1;
						}
					}
				}
				else {
					$referenceString .= $reference->citation->authorTeam->titleCache;
					$referenceString .= ((str_endsWith($referenceString, ".") || str_endsWith($referenceString, ". ")) ? " " : ". ");
				}
				//else {
					//$referenceString .= $teamMember->lastname . ", " . $teamMember->firstname . " ";
				//}
				if (!empty($reference->citation->datePublished->start)) {
					$referenceString .= substr($reference->citation->datePublished->start,0,4);
					$referenceString .= ((str_endsWith($referenceString, ".") || str_endsWith($referenceString, ". ")) ? "" : ". ");
				}
				$referenceString .= $reference->citation->title . ". " . $reference->citation->publisher;
				$referenceString .= ((str_endsWith($referenceString, ".") || str_endsWith($referenceString, ". ")) ? "" : ". ");
				$referenceString .= "</li>";
				break;
				

			case "Article":
				$referenceString .= "<li class=\"descriptionText DescriptionElement\">";
				$numberOfTeamMembers = count($reference->citation->authorTeam->teamMembers);
				$currentRecord = 1;
				if (!empty($reference->citation->authorTeam->teamMembers)) {
					foreach ($reference->citation->authorTeam->teamMembers as $teamMember) {
						if(!empty($teamMember->lastname) && !empty($teamMember->firstname)) {
							if ($currentRecord == 1) {
								$referenceString .= $teamMember->lastname . ", " . $teamMember->firstname;
							}
							else if ($numberOfTeamMembers != $currentRecord) {
								$referenceString .= " , " . $teamMember->lastname . ", " . $teamMember->firstname;	
							}
							else {
								$referenceString .= " & " . $teamMember->lastname . ", " . $teamMember->firstname;
								$referenceString .= ((str_endsWith($referenceString, ".") || str_endsWith($referenceString, ". ")) ? ' ' : ". ");
							}
							$currentRecord += 1;
						}
						else {
							if ($numberOfTeamMembers != $currentRecord) {
								$referenceString .= $teamMember->titleCache. " & ";	
							}
							else {
								$referenceString .= $teamMember->titleCache;
								$referenceString .= ((str_endsWith($referenceString, ".") || str_endsWith($referenceString, ". ")) ? ' ' : ". ");
							}
							$currentRecord += 1;
						}
					}
				}
				else {
					$referenceString .= $reference->citation->authorTeam->titleCache;
					$referenceString .= ((str_endsWith($referenceString, ".") || str_endsWith($referenceString, ". ")) ? " " : ". ");
				}
				//else {
					//$referenceString .= $teamMember->lastname . ", " . $teamMember->firstname . " ";
				//}
				if (!empty($reference->citation->datePublished->start)) {
					$referenceString .= substr($reference->citation->datePublished->start,0,4);
					$referenceString .= ((str_endsWith($referenceString, ".") || str_endsWith($referenceString, ". ")) ? "" : ". ");
				}
				$referenceString .= $reference->citation->title . ". " . $reference->citation->publisher;
				$referenceString .= ((str_endsWith($referenceString, ".") || str_endsWith($referenceString, ". ")) ? "" : ". ");
				$referenceString .= "</li>";
				break;
				
				
			case "Book":
				$referenceString .= "<li class=\"descriptionText DescriptionElement\">";
				$numberOfTeamMembers = count($reference->citation->authorTeam->teamMembers);
				$currentRecord = 1;
				if (!empty($reference->citation->authorTeam->teamMembers) && $reference->citation->authorTeam->titleCache != "-empty team-") {
					foreach ($reference->citation->authorTeam->teamMembers as $teamMember) {
						if(!empty($teamMember->lastname) && !empty($teamMember->firstname)) {
							if ($numberOfTeamMembers != $currentRecord) {
								$referenceString .= $teamMember->lastname . ", " . $teamMember->firstname. " & ";	
							}
							else {
								$referenceString .= $teamMember->lastname . ", " . $teamMember->firstname;
								$referenceString .= ((str_endsWith($referenceString, ".") || str_endsWith($referenceString, ". ")) ? "" : ". ");
							}
							$currentRecord += 1;
						}
						else {
							if ($numberOfTeamMembers != $currentRecord) {
								$referenceString .= $teamMember->titleCache. " & ";	
							}
							else {
								$referenceString .= $teamMember->titleCache;
								$referenceString .= ((str_endsWith($referenceString, ".") || str_endsWith($referenceString, ". ")) ? "" : ". ");
							}
							$currentRecord += 1;
						}
					}
					
					
				}
				else if ($reference->citation->authorTeam->titleCache != "-empty team-"){
					$referenceString .= $reference->citation->authorTeam->titleCache;
					$referenceString .= ((str_endsWith($referenceString, ".") || str_endsWith($referenceString, ". ")) ? "" : ". ");
				}
				 else {
				 	$isCitationTitleCache  = true;
				 	$referenceString .=  $reference->citation->titleCache;
				 }
				if (!empty($reference->citation->datePublished->start)) {
					$referenceString .= substr($reference->citation->datePublished->start,0,4);
					$referenceString .= ((str_endsWith($referenceString, ".") || str_endsWith($referenceString, ". ")) ? "" : ". ");
				}
				if ($isCitationTitleCache == false && !empty($reference->citation->title)) {
					$referenceString .= $reference->citation->title; 
					$referenceString .= ((str_endsWith($referenceString, ".") || str_endsWith($referenceString, ". ")) ? "" : ". ");
				}
				if (!empty($reference->citation->publisher)) {
					$referenceString .= $reference->citation->publisher;
					$referenceString .= ((str_endsWith($referenceString, ".") || str_endsWith($referenceString, ". ")) ? "" : ". ");
				}
				$referenceString .= ((str_endsWith($referenceString, ".") || str_endsWith($referenceString, ". ")) ? "" : ". ");
				$referenceString .= "</li>";
				break;
			case "BookSection":
				$referenceString .= "<li class=\"descriptionText DescriptionElement\">";
				$numberOfTeamMembers = count($reference->citation->authorTeam->teamMembers);
				$currentRecord = 1;
				if (!empty($reference->citation->authorTeam->teamMembers)) {
					foreach ($reference->citation->authorTeam->teamMembers as $teamMember) {
						if(!empty($teamMember->lastname) && !empty($teamMember->firstname)) {
							if ($numberOfTeamMembers != $currentRecord) {
								$referenceString .= $teamMember->lastname . ", " . $teamMember->firstname. " & ";	
							}
							else {
								$referenceString .= $teamMember->lastname . ", " . $teamMember->firstname;
								$referenceString .= ((str_endsWith($referenceString, ".") || str_endsWith($referenceString, ". ")) ? "" : ". ");
							}
							$currentRecord += 1;
						}
						else {
							if ($numberOfTeamMembers != $currentRecord) {
								$referenceString .= $teamMember->titleCache. " & ";	
							}
							else {
								$referenceString .= $teamMember->titleCache;
								$referenceString .= ((str_endsWith($referenceString, ".") || str_endsWith($referenceString, ". ")) ? "" : ". ");
							}
							$currentRecord += 1;
						}
					}
				}
				$referenceString .= substr($reference->citation->inReference->datePublished->start,0,4) . ". " . $reference->citation->title . ". " . "Pages ". $reference->citation->pages . ". In ";
				$numberOfTeamMembersInReference = count($reference->citation->inReference->authorTeam->teamMembers);
				$currentRecordinReference = 1;
				if (!empty($reference->citation->inReference->authorTeam->teamMembers)) {
					foreach ($reference->citation->inReference->authorTeam->teamMembers as $teamMember) {
						if(!empty($teamMember->lastname) && !empty($teamMember->firstname)) {
							if ($numberOfTeamMembers != $currentRecord) {
								$referenceString .= $teamMember->lastname . ", " . $teamMember->firstname. " & ";	
							}
							else {
								$referenceString .= $teamMember->lastname . ", " . $teamMember->firstname;
								$referenceString .= ((str_endsWith($referenceString, ".") || str_endsWith($referenceString, ". ")) ? "" : ". ");
							}
							$currentRecord += 1;
						}
						else {
							if ($numberOfTeamMembers != $currentRecord) {
								$referenceString .= $teamMember->titleCache. " & ";	
							}
							else {
								$referenceString .= $teamMember->titleCache;
								$referenceString .= ((str_endsWith($referenceString, ".") || str_endsWith($referenceString, ". ")) ? "" : ". ");
							}
							$currentRecord += 1;
						}
					}
				}
				
				$referenceString .= $reference->citation->inReference->title . ". " . $reference->citation->inReference->publisher . ". " . $reference->citation->inReference->placePublished;
				$referenceString .= ((str_endsWith($referenceString, ".") || str_endsWith($referenceString, ". ")) ? "" : ". ");
					
				
				$referenceString .= "</li>";
				break;
				
				
			case "WebPage" :
				$referenceString .= "<li class=\"descriptionText DescriptionElement\">" . $reference->citation->titleCache . "</li>";
				break;
			case "Generic" :
				$referenceString .= "<li class=\"descriptionText DescriptionElement\">";
				$numberOfTeamMembers = count($reference->citation->authorTeam->teamMembers);
				$currentRecord = 1;
				if (!empty($reference->citation->authorTeam->teamMembers)) {
					foreach ($reference->citation->authorTeam->teamMembers as $teamMember) {
						if(!empty($teamMember->lastname) && !empty($teamMember->firstname)) {
							if ($currentRecord == 1) {
								$referenceString .= $teamMember->lastname . ", " . $teamMember->firstname;
							}
							else if ($numberOfTeamMembers != $currentRecord) {
								$referenceString .= " , " . $teamMember->lastname . ", " . $teamMember->firstname;	
							}
							else {
								$referenceString .= " & " . $teamMember->lastname . ", " . $teamMember->firstname;
								$referenceString .= ((str_endsWith($referenceString, ".") || str_endsWith($referenceString, ". ")) ? ' ' : ". ");
							}
							$currentRecord += 1;
						}
						else {
							if ($numberOfTeamMembers != $currentRecord) {
								$referenceString .= $teamMember->titleCache. " & ";	
							}
							else {
								$referenceString .= $teamMember->titleCache;
								$referenceString .= ((str_endsWith($referenceString, ".") || str_endsWith($referenceString, ". ")) ? ' ' : ". ");
							}
							$currentRecord += 1;
						}
					}
				}
				else if(!empty($reference->citation->authorTeam->titleCache)) {
					$referenceString .= $reference->citation->authorTeam->titleCache;
					$referenceString .= ((str_endsWith($referenceString, ".") || str_endsWith($referenceString, ". ")) ? " " : ". ");
				}
				else {
					$referenceString .= $reference->citation->titleCache;
					$referenceString .= ((str_endsWith($out, ".") || str_endsWith($out, ". ")) ? " " : ". ");
				}
				//else {
					//$referenceString .= $teamMember->lastname . ", " . $teamMember->firstname . " ";
				//}
				if (!empty($reference->citation->datePublished->start)) {
					$referenceString .= substr($reference->citation->datePublished->start,0,4);
					$referenceString .= ((str_endsWith($referenceString, ".") || str_endsWith($referenceString, ". ")) ? " " : ". ");
				}
				$referenceString .= $reference->citation->title . ". " . $reference->citation->publisher;
				$referenceString .= ((str_endsWith($referenceString, ".") || str_endsWith($referenceString, ". ")) ? " " : ". ");
				$referenceString .= ((str_endsWith($referenceString, ".") ) ? " " : "");
				$referenceString .= "</li>";
				break;
			default:
				
				//$author_team = cdm_ws_get(CDM_WS_REFERENCE_AUTHORTEAM, $reference->citation->uuid);
				
				//if(!empty($author_team->titleCache)) {
					//$referenceString.= print_r($reference->citation);
					//$referenceString .= '<li class="descriptionText DescriptionElement">' . "<b>" . $reference->citation->title . ":" . "</b>" . $author_team->titleCache .   '</li>';
				//}
				//else {
					//$referenceString .= '<li class="descriptionText DescriptionElement">' ."<b>" . $reference->citation->titleCache . "</b>" . '</li>';
				//}
				//if ($referenceCitation){
					//$sourceRefs = $referenceCitation;
					////$referenceString .= "[titleccache] " . $descriptionElementBiblio->feature->titleCache . "[/titlecache]";
					////$referenceString .= "[Class] " . $descriptionElementBiblio->class . "[/class]";
					////$referenceString .= "[sourceref]" . $sourceRefs . "[/sourceRef]";
				//}
				break;
		}
		$outTemp[] = $referenceString;
	}
	sort($outTemp);
  	
	foreach ($outTemp as $refString) {
		$out .= $refString;
	}
	
	$out .= "</ul></div></div>";
	return $out;
}
*/
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
    $out = l('<span class="reference">'.$citation.'</span>'
   	, path_to_reference($reference->uuid) 
    //, $_GET['q']
    , array("class"=>"reference")
    //, NULL, generalizeString('Bibliography'), FALSE ,TRUE);
    , NULL, NULL, FALSE ,TRUE);
  } else {
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
  switch($variables['feature_name']){
    case "Protologue": return t("Original Publication");
    default: return t(ucfirst($feature_name));
  }
}

function palmweb_2_cdm_taxon_page_title($taxon, $uuid, $synonym_uuid){
  $taxon = $variables['taxon']; 
  $uuid = $variables['uuid'];
  $synonym_uuid = $variables['synonym_uuid'];
  
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

//@WA this theme function does not exist..
/*
function palmweb_2_cdm_uri_to_synonym($synonymUuid, $acceptedUuid, $pagePart = null) {
	$acceptedPath = path_to_taxon($acceptedUuid, true);
	return url($acceptedPath . ($pagePart ? '/'.$pagePart : '') . '/'.$synonymUuid, 'highlite='.$synonymUuid);
	//return url($acceptedPath.($pagePart ? '/'.$pagePart : ''), 'highlite='.$synonymUuid, $synonymUuid."/$synonymUuid");
	//return url("$acceptedPath/$synonymUuid".($pagePart ? '/'.$pagePart : ''), 'highlite='.$synonymUuid);
}
*/

/* assign the css classes primary-links and secondary-links to the menus */
function palmweb_2_preprocess_page(&$vars) {

  if (isset($vars['main_menu'])) {
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
    $body =  $vars['node']->body['und'][0]['value'];
    
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