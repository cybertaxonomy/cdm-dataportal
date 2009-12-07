<?php
// $Id$

/*
 * Copyright (C) 2007 EDIT
 * European Distributed Institute of Taxonomy
 * http://www.e-taxonomy.eu
 */

function _add_js_thickbox(){
	// ---- jQuery thickbox:
	/*
	* bug: compat-1.0.js && thickbox.js line 237 .trigger("unload")
	* -> event is not triggered because of problems with compat-1.0.js'
	* see INSTALL.txt
	*
	*/
	//drupal_add_js(drupal_get_path('module', 'cdm_dataportal').'/js/jquery.imagetool.min.js');
	drupal_add_js(drupal_get_path('module', 'cdm_dataportal').'/js/thickbox/thickbox.js');
	drupal_add_css(drupal_get_path('module', 'cdm_dataportal').'/js/thickbox/cdm_thickbox.css');
}

function _add_js_lightbox($galleryID){
	$lightBoxBasePath = drupal_get_path('module', 'cdm_dataportal') .'/js/jquery-lightbox-0.5';
	drupal_add_js($lightBoxBasePath.'/js/jquery.lightbox-0.5.js');
	drupal_add_css($lightBoxBasePath.'/css/jquery.lightbox-0.5.css');
	drupal_add_js ('$(document).ready(function() {
      $(\'#'.$galleryID.' a.lightbox\').lightBox({
        fixedNavigation:  true,
        imageLoading:     \''.$lightBoxBasePath.'/images/lightbox-ico-loading.gif\', 
        imageBtnPrev:     \''.$lightBoxBasePath.'/images/lightbox-btn-prev.gif\',    
        imageBtnNext:     \''.$lightBoxBasePath.'/images/lightbox-btn-next.gif\',   
        imageBtnClose:    \''.$lightBoxBasePath.'/images/lightbox-btn-close.gif\',  
        imageBlank:       \''.$lightBoxBasePath.'/images/lightbox-blank.gif\'
      });
    });', 'inline');
}


function _add_js_cluetip(){

	//TODO replace by http://www.socialembedded.com/labs/jQuery-Tooltip-Plugin/jQuery-Tooltip-Plugin.html
	drupal_add_js(drupal_get_path('module', 'cdm_dataportal').'/js/cluetip/jquery.cluetip.js');
	drupal_add_js(drupal_get_path('module', 'cdm_dataportal').'/js/jquery.dimensions.js');
	drupal_add_js(drupal_get_path('module', 'cdm_dataportal').'/js/cluetip/jquery.hoverIntent.js');
	drupal_add_css(drupal_get_path('module', 'cdm_dataportal').'/js/cluetip/jquery.cluetip.css');
	drupal_add_js ("$(document).ready(function(){
      $('.cluetip').css({color: '#0062C2'}).cluetip({
        splitTitle: '|',
        showTitle: true,
        activation: 'hover',
        sicky: true,
        arrows: true,
        dropShadow: false,
        cluetipClass: 'rounded'
      });
    });", 'inline');
}

/**
 * TODO if getting fragment from request is possible remove $_REQUEST['highlite'] HACK
 * NOT WORKING since fragments are not available to the server
 function fragment(){
 global $fragment;
 if(!$fragment){
 $fragment = substr($_SERVER['REQUEST_URI'], strrpos($_SERVER['REQUEST_URI'], '#'));
 }
 return $fragment;
 }
 */

function uuid_anchor($uuid, $innerHTML){
	$highlite = $_REQUEST['highlite'] == $uuid;
	return '<a name="'.$uuid.'" ></a><span class="'.($highlite ? 'highlite' : '').'">'.$innerHTML.'</span>';
}

/**
 * Enter description here...
 *
 * @param unknown_type $name
 * @param unknown_type $numOfNameTokens
 * @return unknown
 * @deprecated looks like this is not used anymore
 */
function tagNameParts($name, $numOfNameTokens){

	$out = '<span class="name">';

	$token = strtok($name, " \n\t");
	$i = 0;
	$noSpace = true;
	while($token != false){
		if($i == $numOfNameTokens){
			$out .= '</span> <span class="authors">';
			$noSpace = true;
		}
		$out .= ($noSpace?'':' ').$token;
		$noSpace = false;
		$token = strtok(" \n\t");
		$i++;
	}
	return $out.'</span>';
}

/**
 * Converts an array of TagedText items into a sequence of corresponding html tags whereas
 * each item will provided with a class attribute which set to the key of the TaggedText item.
 *
 * @param array $taggedtxt
 * @param String $tag
 * @param String $glue the string by which the chained text tokens are concatenated together.
 *       Default is a blank character
 * @return String of HTML
 */
function theme_cdm_taggedtext2html(array &$taggedtxt, $tag = 'span', $glue = ' ', $skiptags = array()){
	$out = '';
	$i = 0;
	foreach($taggedtxt as $tt){
		if(!in_array($tt->type, $skiptags) && strlen($tt->text) > 0){
			$out .= (strlen($out) > 0 && ++$i < count($taggedtxt)? $glue : '').'<'.$tag.' class="'.$tt->type.'">'.t($tt->text).'</'.$tag.'>';
		}
	}
	return $out;
}

/**
 * Almost any cdmObject may be annotated. Therefore we provide a generic way to display
 * as well as create or update annotations.
 *
 * TODO it should be configurable which objects can be annotated as this might differ in dataportals
 *
 */
function theme_cdm_annotation($cdmBase){
	if(!$cdmBase->uuid){
		return;
	}else{

		drupal_add_js(drupal_get_path('module', 'cdm_dataportal').'/js/cdm_annotations.js');
		drupal_add_js(drupal_get_path('module', 'cdm_dataportal').'/js/jquery.form.js');

		$annotatableUuid = $cdmBase->uuid;
		//FIXME annotations only available as property of e.g. taxon, name, ...
		$annotationUrl = cdm_compose_url(CDM_WS_ANNOTATIONS, array($annotatableUuid));

		$annotationProxyUrl = url('cdm_api/proxy/'. urlencode($annotationUrl).'/cdm_annotation_content');

		$out = ' <span class="annotation">';
		$out .= '<span class="annotation_toggle" rel="'.$annotationProxyUrl.'">+</span>';
			

		$out .= '<div class="annotation_box"></div>';
		$out .= '</span>';

		return $out;

	}
}

function theme_cdm_annotation_content($AnnotationTO){

	drupal_add_js(drupal_get_path('module', 'cdm_dataportal').'/js/cdm_annotations.js');
	drupal_add_js(drupal_get_path('module', 'cdm_dataportal').'/js/jquery.form.js');

	$out .= theme('cdm_list_of_annotations', $AnnotationTO->annotationElements);

	$annotationUrl = cdm_compose_url(CDM_WS_ANNOTATIONS, array($AnnotationTO->uuid));
	$annotationProxyUrl = url('cdm_api/proxy/'. urlencode($annotationUrl).'/cdm_annotation_post');

	// TODO users have to be authenticated to the dataportal to be able to write annotations
	$out .= '
  			<div class="annotation_create">
  				<form action="'.$annotationProxyUrl.'" method="POST">
  					<textarea name="annotation"></textarea>
  					<input type="hidden" name="commentator" value="">
  					<input type="submit" value="'.t('Save annotation').'" />
  				</form>
 			</div>
	';

	return $out;
}

function theme_cdm_list_of_annotations($annotationElements){

	$out = '<ul class="annotation_list">';

	foreach ($annotationElements as $key => $row){
		$created[$key] = $row;
	}
	array_multisort($created, SORT_ASC, $annotationElements);

	foreach ($annotationElements as $annotation){
		$out .= '<li>' . $annotation->text . '</li>';
	}

	$out .= '</ul>';

	return $out;

}

function theme_cdm_media($descriptionElement, $mimeTypePreference){
	$out = "";

	_add_js_thickbox();

	$uuid = $descriptionElement->uuid;
	$feature = $descriptionElement->feature;
	$medias = $descriptionElement->media;

	foreach($medias as $media){
		$prefRepresentations = cdm_preferred_media_representations($media, $mimeTypePreference, 300, 400);
		$mediaRepresentation = array_shift($prefRepresentations);
		if($mediaRepresentation) {

			$contentTypeDirectory = substr($mediaRepresentation->mimeType, 0, stripos($mediaRepresentation->mimeType, '/'));

			$out .= theme('cdm_media_mime_' . $contentTypeDirectory,  $mediaRepresentation, $feature);

			//			$attributes = array('class'=>'thickbox', 'rel'=>'descriptionElement-'.$uuid, 'title'=>$feature->term);
			//		    for($i = 0; $part = $mediaRepresentation->representationParts[$i]; $i++){
			//		    	if($i == 0){
			//		    	    $image_url = drupal_get_path('module', 'cdm_dataportal').'/images/'.$feature->term.'-media.png';
			//		    	    $media = '<img src="'.$image_url.'" height="14px" alt="'.$feature->term.'" />';
			//		    	    $out .= l($media, $part->uri, $attributes, NULL, NULL, TRUE, TRUE);
			//		    	} else {
			//		    		$out .= l('', $part->uri, $attributes, NULL, NULL, TRUE);
			//		    	}
			//		  	}
		} else {
			// no media available, so display just the type term
			$out .=  $feature->representation_L10n;
		}
	}
	return $out;

}

function theme_cdm_mediaTypeTerm($feature){
	$icon_url = drupal_get_path('module', 'cdm_dataportal').'/images/'.$feature->representation_L10n.'-media.png';
	return '<img src="'.$icon_url.'" height="14px" alt="'.$feature->representation_L10n.'" />';
}

function theme_cdm_media_mime_application($mediaRepresentation, $feature){

	foreach($mediaRepresentation->parts as $part){
		$attributes = array('title'=> theme('cdm_feature_name', $feature->representation_L10n), 'target'=>'_blank');
		//$attributes = array('title'=>$feature->representation_L10n, 'target'=>'_blank');
		//$attributes = array('title'=>'original publication', 'target'=>'_blank');
		$out .= l(theme('cdm_mediaTypeTerm', $feature), $part->uri, $attributes, NULL, NULL, TRUE, TRUE);
	}
	return $out;
}

function theme_cdm_media_mime_image($mediaRepresentation, $feature){
	$out = '';
	$attributes = array('class'=>'thickbox', 'rel'=>'representation-'.$representation->uuid, 'title'=>$feature->representation_L10n);
	for($i = 0; $part = $representation->representationParts[$i]; $i++){
		if($i == 0){

			$out .= l(theme('cdm_mediaTypeTerm', $feature), $part->uri, $attributes, NULL, NULL, TRUE, TRUE);
		} else {
			$out .= l('', $part->uri, $attributes, NULL, NULL, TRUE);
		}
	}
	return $out;
}

function theme_cdm_media_mime_text($representation, $feature){

	foreach($representation->parts as $part){
		$attributes = array('title'=> theme('cdm_feature_name', $feature->representation_L10n), 'target'=>'_blank');
		//$attributes = array('title'=>t('original publication'), 'target'=>'_blank');
		$out .= l(theme('cdm_mediaTypeTerm', $feature), $part->uri, $attributes, NULL, NULL, TRUE, TRUE);
	}
	return $out;
}


function theme_cdm_media_caption($media, $elements = array('title', 'description', 'file', 'filename', 'rights'), $fileUri = null){

	$out = '<div class="media_caption">';
	if( array_search('title', $elements)!== false && $media->titleCache){
		$out .= '<span class="title">'.$media->titleCache.'</span>';
	}
	if(array_search('description', $elements)!== false && $media->description_L10n){
		$out .= '<span class="description">'.$media->description_L10n.'</span>';
	}
	if(array_search('file', $elements)!== false){
		$out .= '<span class="file">'.$media->titleCache.'</span>';
	}
	if(array_search('filename', $elements)!== false && $fileUri){
		$filename = substr($fileUri, strrpos($fileUri, "/")+1);
		$out .= '<span class="filename">'.$filename.'</span>';
	}

	if(array_search('rights', $elements)!== false && $media->rights){
		$out .= '<ul class="rights">';
		foreach($media->rights as $right){
			$out .= '<li>'.theme('cdm_right', $right).'</li>';
		}
		$out .= '</ul>';
	}

	$out .= '</div>';

	return $out;
}

// TODO DELETE !!!
///*
// * This function return a string wicht contains the metadata information from a media (e.g. jpeg file)
// * The argument is the media where we want to get the metadata
// * */
//function theme_cdm_media_metadata_caption ($media){
//	$media_metadata = cdm_ws_get(CDM_WS_MEDIA_METADATA, array($media->uuid));
//
//	//Getting the location metadata for showing it in the lihgtbox
//	$location = '';
//	if($media_metadata->Country || $media_metadata->Province || $media_metadata->City){
//		//adding sublocation
//		$location .= ($media_metadata->Sublocation ? '<br>Location: ' .$media_metadata->Sublocation : '');
//		//adding city
//		if ($location == '' && $media_metadata->City)
//		$location .= '<br>Location: ' . $media_metadata->City;
//		elseif (!($location == '') && $media_metadata->City)
//		$location .= ', ' . $media_metadata->City;
//		//adding Province
//		if ($location == '' && $media_metadata->Province)
//		$location .= '<br>Location: ' . $media_metadata->Province;
//		elseif (!($location == '') && $media_metadata->Province)
//		$location .= ', ' . $media_metadata->Province;
//		//adding Country
//		if ($location == '' && $media_metadata->Country)
//		$location .= '<br>Location: ' . $media_metadata->Country;
//		elseif (!($location == '') && $media_metadata->Country)
//		$location .= ' (' . $media_metadata->Country . ')';
//	}
//
//	$metadata_caption = ($media->titleCache ? $media->titleCache : '')
//	.($media->titleCache && $media->description_L10n ? ' - ' : '')
//	.($media->description_L10n ? $media->description_L10n : '')
//	.($media_metadata->Artist ? '<br>Artist: '.$media_metadata->Artist : '<br> No artist')
//	.($media_metadata->Copyright ? '<br>CopyRight: ' .$media_metadata->Copyright : '<br> No copyright')
//	.$location;
//
//	return $metadata_caption;
//}



function theme_cdm_metadata_caption($metadata_caption, $elements = array('title', 'description', 'artist', 'location', 'rights'), $fileUri = null){
	$out = '<dl class="media-caption">';
	//title
	if($metadata_caption['title'] && (!$elements || array_search('title', $elements)!== false)){
	   $out .= '<dt class = "title">' . t('Title') . '</dt> <dd class = "title">' . $metadata_caption['title'] . '</dd>';
	   //unset($metadata_caption['title']);
	}
	   //description   
	if($metadata_caption['description'] && (!$elements || array_search('description', $elements)!== false)){
	   $out .= '<dt class = "description">' . t('Description') . '</dt> <dd class = "description">' . $metadata_caption['description'] . '</dd>';
	   //unset($metadata_caption['description']);
	}
    //artist
	if($metadata_caption['artist'] && (!$elements || array_search('artist', $elements)!== false)){
	   //$out .= '<span class = "artist">' . ($metadata_caption['artist'] ? 'Artist: ' . $metadata_caption['artist'] . '</span>' . '<br>' : '');
	   $out .= '<dt class = "artist">' . t('Artist') . '</dt> <dd class = "astist">' . $metadata_caption['artist'] . '</dd>';
	}
	//location   
	if(!$elements || array_search('location', $elements)!== false){
	   $location = '';
	   $location .= $media_metadata['location']['sublocation'];
	   if ($location && $metadata_caption['location']['city']){
	       $location .= ', ';
	   }
	   $location .= $metadata_caption['location']['city'];
	   if ($location && $metadata_caption['location']['province']){
           $location .= ', ';
	   }
	   $location .= $metadata_caption['location']['province'];
       if ($location && $metadata_caption['location']['country']){
           $location .= ' (' . $metadata_caption['location']['country'] . ')';
       } else {
        $location .= $metadata_caption['location']['country'];
       }
       if ($location){
        $out .= '<dt class = "location">' . t('Location') . '</dt> <dd class = "location">' . $location  . '</dd>';
       }
	}
	//rights
	if(!$elements || array_search('rights', $elements)!== false){
	   $rights = '';
       //copyrights
	   $cnt = count($metadata_caption['rights']['copyright']['agentNames']);
	   if($cnt > 0){
           $rights .= '<dt class="rights">&copy;</dt> <dd class="rights"> '; 
	       for($i = 0; $i < $cnt; $i++){
            $rights .= $metadata_caption['rights']['copyright']['agentNames'][$i];
            if($i+1 < $cnt){
                $rights .= ' / ';
            }
	       }
	       $rights .= '</dd>';
	   }
	   //license
       $cnt = count($metadata_caption['rights']['license']['agentNames']);
       if($cnt > 0){
           $rights .= '<dt class ="license">' . t('License') . '</dt> <dd class = "license">'; 
	       for($i = 0; $i < $cnt; $i++){
            $rights .= $metadata_caption['rights']['license']['agentNames'][$i];
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
 */
function theme_cdm_taxon_list_thumbnails($taxon){

	$gallery_name = $taxon->uuid;

	$showCaption = variable_get('cdm_dataportal_findtaxa_show_thumbnail_captions', 1);
	$prefMimeTypeRegex = 'image:.*';
	$prefMediaQuality = '*';
	$cols = variable_get('cdm_dataportal_findtaxa_media_cols', 3);
	$maxRows = variable_get('cdm_dataportal_findtaxa_media_maxRows', 1);
	$maxExtend = variable_get('cdm_dataportal_findtaxa_media_maxextend', 120);

	if($showCaption){
		//$captionElements = array('title', '#uri'=>t('open Image'));
		$captionElements = array('title', 'rights');
	}

	$galleryLinkUri = path_to_taxon($taxon->uuid).'/images';
	$mediaList = cdm_ws_get(CDM_WS_TAXON_MEDIA, array($taxon->uuid, $prefMimeTypeRegex, $prefMediaQuality));
	$out .= theme('cdm_media_gallerie', $mediaList, $gallery_name ,$maxExtend, $cols, $maxRows, $captionElements, 'LIGHTBOX', null, $galleryLinkUri);

	return $out;
}


/**
 * @param $mediaList an array of Media entities
 * @param $maxExtend
 * @param $cols
 * @param $maxRows
 * @param $captionElements an array possible values are like in the following example: array('title', 'description', 'file', 'filename'), 
 *         to add a link to the caption: array('titlecache', '#uri'=>t('open Image'));
 * @param $mediaLinkType valid values:
 *      "NONE": do not link the images,
 *      "LIGHTBOX": open the link in a light box,
 *      "NORMAL": link to the image page or to the $alternativeMediaUri if it is defined
 * @param $alternativeMediaUri an array of alternative URIs to link the images wich will overwrite the URIs of the media parts.
 *     The order of URI in this array must correspond with the order of images in $mediaList
 * @param $galleryLinkUri an URI to link the the hint on more images to; if null no link is created
 * @return unknown_type
 */
function theme_cdm_media_gallerie($mediaList, $galleryName, $maxExtend = 150, $cols = 4, $maxRows = false, $captionElements = array('title'),
$mediaLinkType = 'LIGHTBOX', $alternativeMediaUri = null, $galleryLinkUri = null ){

	//TODO correctly handle multiple media representation parts
	$_SESSION['cdm']['last_gallery']= substr($_SERVER['REQUEST_URI'],strpos($_SERVER['REQUEST_URI'], "?q=")+3);
	// prevent from errors
	if(!isset($mediaList[0])){
		return;
	}

	$galleryID = "media_gallery_".$galleryName;

	// prepare media links
	$doLink = false;
	$linkAttributes = null;
	if($mediaLinkType != 'NONE'){
		$doLink = true;
	}
	if($mediaLinkType == 'LIGHTBOX'){
		$doLink = true;
		//_add_js_thickbox();
		//$linkAttributes = array("class"=>"thickbox", "rel"=>"media_gallerie".$galleryName);
		_add_js_lightbox($galleryID);
		$linkAttributes = array("class"=>"lightbox");
	}

	// render the media gallery grid
	$out = '<table id="'.$galleryID.'" class="media_gallery">';
	$out .= '<colgroup>';
	for($c = 0; $c < $cols; $c++){
		$out .= '<col width="'.(100 / $cols).'%">';
	}
	$out .= '</colgroup>';

	$mediaIndex = 0;
	for($r = 0; ($r < $maxRows || !$maxRows) && count($mediaList) > 0; $r++){
		$captionParts = array();
		$out .= '<tr>';
		for($c = 0; $c < $cols; $c++){
			$media = array_shift($mediaList);
			if(isset($media->representations[0]->parts[0])){
				$contentTypeDirectory = substr($media->representations[0]->mimeType, 0, stripos($media->representations[0]->mimeType, '/'));
				$mediaIndex++;
				$mediaPartHtml = theme('cdm_media_gallerie_'.$contentTypeDirectory, $media->representations[0]->parts[0], $maxExtend, TRUE);

				// --- compose Media Link
				$mediaLinkUri = false;
				if($alternativeMediaUri){
					if(isset($alternativeMediaUri[$mediaIndex])){
						$mediaLinkUri = $alternativeMediaUri[$mediaIndex];
					}
					if(is_string($alternativeMediaUri)){
						$mediaLinkUri = $alternativeMediaUri;
					}
				} else {
					$mediaLinkUri = $media->representations[0]->parts[0]->uri;
				}

			   
				$metadataMap = cdm_read_media_metadata($media);
				
				// generate gallery caption
				$captionPartHtml = theme('cdm_metadata_caption', $metadataMap, $captionElements);
				// generate & add caption to lightbox
				$lightBoxCaptionElements = null;
                $linkAttributes['title'] = theme('cdm_metadata_caption', $metadataMap, $lightBoxCaptionElements);

				//$mediaList = cdm_ws_get(CDM_WS_TAXON_MEDIA, array($taxon->uuid, $prefMimeTypeRegex, $prefMediaQuality)); define('CDM_WS_TAXON_MEDIA', 'portal/taxon/$0/media/$1/$2');
				//$prefMimeTypeRegex = 'image:.*';
					
				// --- assemble captions
//				if(isset($media->representations[0]->parts[0]->uri)){
//					$fileUri = $media->representations[0]->parts[0]->uri;
//				}
//				$captionPartHtml = theme('cdm_media_caption', $media, $captionElements, $fileUri);
				if(isset($captionElements['#uri'])){
					$captionPartHtml .= '<div>'.l($captionElements['#uri'], path_to_media($media->uuid), null, null, null, FALSE, TRUE).'</div>';
				}
				$captionParts[] = $captionPartHtml;

				// --- surround imagePart with link
				if($doLink){
					$mediaPartHtml = l($mediaPartHtml, $mediaLinkUri, $linkAttributes, null, null, FALSE, TRUE);
				}

			} else {
				$mediaPartHtml = '';
				$captionParts[] = '';
			}
			$out .= '<td>'.$mediaPartHtml.'</td>';
		}
		$out .= '</tr>'; // end of media parts
		if(isset($captionElements[0])){
			$out .= '<tr>';
			// add caption row
			foreach($captionParts as $captionPartHtml){
				$out .= '<td>'.$captionPartHtml.'</td>';
			}
			$out .= '</tr>';
		}
	}
	if($galleryLinkUri){
		if(count($mediaList) > 0){
			$moreHtml = count($mediaList).' '.t('more in gallery');
		} else {
			$moreHtml = t('open gallery');
		}
		$moreHtml = l($moreHtml, $galleryLinkUri);
		$out .= '<tr><td colspan="'.$cols.'">'.$moreHtml.'</td></tr>';
	}
	$out .= '</table>';
	return $out;
}

function theme_cdm_media_gallerie_image($mediaRepresentationPart, $maxExtend, $addPassePartout = FALSE, $attributes = null){
	//TODO merge with theme_cdm_media_mime_image?

	if(isset($mediaRepresentationPart)){
		$h = $mediaRepresentationPart->height;
		$w = $mediaRepresentationPart->width;
		$margins = '0 0 0 0';
		$ratio = $w / $h;
		if($ratio > 1){
			$displayHeight = round($maxExtend / $ratio);
			$displayWidth = $maxExtend;
			$m = round(($maxExtend - $displayHeight) / 2);
			$margins = 'margin:'.$m.'px 0 '.$m.'px 0;';
		} else {
			$displayHeight = $maxExtend;
			$displayWidth = round($maxExtend * $ratio);
			$m = round(($maxExtend - $displayWidth) / 2);
			$margins = 'margin:0 '.$m.'px 0 '.$m.'px;';
		}

		// turn attributes array into string
		$attrStr = ' ';
		//$attributes['title'] = 'h:'.$h.', w:'.$w.',ratio:'.$ratio;
		if(is_array($attributes)){
			foreach($attributes as $name=>$value){
				$attrStr .= $name.'="'.$value.'" ';
			}
		}

		//return  '<img src="'."http://wp5.e-taxonomy.eu/dataportal/cichorieae/media/photos/Lapsana_communis_A_01.jpg".'" width="'.$maxExtend.'" height="'.$maxExtend.'" />';
		if($addPassePartout){
			$out .= '<div class="image-passe-partout" style="width:'.$maxExtend.'px; height:'.$maxExtend.'px;">';
		} else {
			// do not add margins if no pass partout is shown
			$margins = '';
		}
		$out .= '<img src="'.$mediaRepresentationPart->uri.'" width="'.$displayWidth.'" height="'.$displayHeight.'" style="'.$margins.'"'.$attrStr.' />';

		if($addPassePartout){
			$out .= '</div>';
		}
		return $out;
	}

}

function theme_cdm_openlayers_image($mediaRepresentationPart, $maxExtend){

	// see http://trac.openlayers.org/wiki/UsingCustomTiles#UsingTilesWithoutaProjection
	// and http://trac.openlayers.org/wiki/SettingZoomLevels

	$w = $mediaRepresentationPart->width;
	$h = $mediaRepresentationPart->height;

	// calculate  maxResolution (default is 360 deg / 256 px) and the bounds
	if($w > $h){
		$lat = 90;
		$lon = 90 * ($h / $w);
		$maxRes = $w / $maxExtend;
	} else {
		$lat = 90 * ($w / $h);
		$lon = 90;
		$maxRes =  $h / $maxExtend ;
	}

	$maxRes *= 1;
	drupal_add_js('
 var map;

 var imageLayerOptions={
     maxResolution: '.$maxRes.',
     maxExtent: new OpenLayers.Bounds(0, 0, '.$w.', '.$h.')
  };
  var mapOptions={
     restrictedExtent:  new OpenLayers.Bounds(0, 0, '.$w.', '.$h.')
  };
 
 var graphic = new OpenLayers.Layer.Image(
          \'Image Title\',
          \''.$mediaRepresentationPart->uri.'\',
          new OpenLayers.Bounds(0, 0, '.$w.', '.$h.'),
          new OpenLayers.Size('.$w.', '.$h.'),
          imageLayerOptions
          );
  
 function init() {
   map = new OpenLayers.Map(\'openlayers_image\', mapOptions);
   map.addLayers([graphic]);
   map.setCenter(new OpenLayers.LonLat(0, 0), 1);
   map.zoomToMaxExtent();
 }
 
$(document).ready(function(){
  init();

});'
	, 'inline');
	$out = '<div id="openlayers_image" class="image_viewer" style="width: '.$maxExtend.'px; height:'.($maxExtend).'px"></div>';
	return $out;

}

function theme_cdm_media_page($media, $mediarepresentation_uuid = false, $partId = false){
	$out = '';
	// determine which reprresentation and which part to show
	$representationIdx = 0;
	if($mediarepresentation_uuid){
		$i = 0;
		foreach($media->representations as $representation) {
			if($representation->uuid == $mediarepresentation_uuid){
				$representationIdx = $i;
			}
			$i++;
		}
	} else {
		$mediarepresentation_uuid = $media->representations[0]->uuid;
	}

	$partIdx  = 0;
	if(!is_numeric($partId)){
		// assuming it is an uuid
		$i = 0;
		foreach($media->representations[$representationIdx]->parts as $part) {
			if($part->uuid == $partId){
				$partIdx = $i;
			}
			$i++;
		}
	} else {
		// assuming it is an index
		$partIdx = $partId;
	}


	$title = $media->titleCache;

	$imageMaxExtend = variable_get('image-page-maxextend', 400);

	if(!$title){
		$title = 'Media### '.$media->uuid.'';
	}

	drupal_set_title($title);


	$out .= '<div class="media">';

	//$out .= '<div class="viewer">';
	$out .= theme(cdm_back_to_image_gallery_button);
	$out .= '<div class="viewer">';
	//$out .= theme('cdm_media_gallerie_image', $representation->parts[$partIdx], $imageMaxExtend);
	$out .= theme('cdm_openlayers_image', $media->representations[$representationIdx]->parts[$partIdx], $imageMaxExtend);
	$out .= '</div>';

	// general media metadata
	$metadataToPrint = cdm_read_media_metadata($media);
    $metadataToPrint = theme('cdm_metadata_caption', $metadataToPrint);
    $out .= $metadataToPrint;
	//$out .= '<div class="metadata_caption">' . theme('cdm_media_metadata_caption', $media) . '</div><br>';
	/*
	 $out .= '<h4 class="title">'.$media->titleCache.'</h4>';
	 $out .= '<div class="description">'.$media->description_L10n.'</div>';
	 $out .= '<div class="artist">'.$media->artist->titleCache.'</div>';
	 */
	//$out .= theme(cdm_back_to_image_gallery_button);
	/*
	$out .= '<ul class="rights">';
	foreach($media->rights as $right){
	$out .= '<li>'.theme('cdm_right', $right).'</li>';
	}
	$out .= '</ul>';
	*/

	//tabs for the different representations
	//ul.secondary
	$out .= '<ul class="primary">';
	foreach($media->representations as $representation){
		$out .= '<li>'.l($media->representations[$representationIdx]->mimeType, path_to_media($media->uuid, $mediarepresentation_uuid, $partIdx)).'</li>';
	}
	$out .= '</ul>';

	// representation(-part) specific metadata
	$thumbnailMaxExtend = 100;
	$out .= '<table>';
	//$out .= '<tr><th colspan="3">'.t('MimeType').': '.$media->representations[$representationIdx]->mimeType.'</th></tr>';
	$i = 0;
	foreach($media->representations[$representationIdx]->parts as $part){
		$out .= '<tr><th>'.t('Part').' '.($i + 1).'</th><td>';
		switch($part->class){
			case 'ImageFile': $out .= $part->width.' x '.$part->height.' - '.$part->size.'k'; break;
			case 'AudioFile':
			case 'MovieFile': $out .= t('Duration').': '.$part->duration.'s - '.$part->size.'k'; break;
			default: $out .= $part->size.'k';
		}
		$out .= '</td><td><a href="'.url(path_to_media($media->uuid, $mediarepresentation_uuid, $i)).'">'.theme('cdm_media_gallerie_image', $part, $thumbnailMaxExtend, true);'</a></td><tr>';
		$i++;
	}
	$out .= '</table>';
	$out .= '</div>';

	return $out;
}

function theme_cdm_right($right){

	$funcSubName = false;
	switch($right->term->uuid){
		case UUID_RIGHTS_LICENCE: $funcSubName = 'licence'; break;
		case UUID_RIGHTS_COPYRIGHT: $funcSubName = 'copyright'; break;
		default: $funcSubName = 'copyright';
	}
	if($funcSubName){
		$out = '<div class="right">';
		$out .= theme('cdm_right_'.$funcSubName, $right);
		$out .= '</div>';
		return $out;
	}
}


/**
 * Theme for rights of type UUID_RIGHTS_COPYRIGHT
 * @param $right
 * @return unknown_type
 */
function theme_cdm_right_copyright($right){
	$out .= '<span class="type">&copy;</span>';
	if($right->agent){
		$out .= ' <span class="agent"> '.$right->agent->firstname . ' ' .$right->agent->lastname .'</span>';
	}
	return $out;
}


/**
 * Theme for rights of type UUID_RIGHTS_LICENCE
 * @param $right
 * @return unknown_type
 */
function theme_cdm_right_licence($right){
	$out .= '<span class="type">'.$right->type->representation_L10n.' </span>';
	if($right->uri){
		$out .= '<a href="'.$right->uri.'>'.$right->abbreviatedText.'</a>';
	} else {
		$out .= $right->abbreviatedText;
	}
	if($right->agent){
		$out .= '<span class="agent"> '.$right->agent->firstname . ' ' .$right->agent->lastname .'</span>';
	}
	return $out;
}


/**
 * TODO
 * Quick-and-dirty solution to show distribution service to exemplar groups
 *
 * @param unknown_type $featureTo
 * @return unknown
 */
function theme_cdm_descriptionElements_distribution($taxon){


	$server = variable_get('cdm_dataportal_geoservice_access_point', false);

	if(!server){
		return "<p>No geoservice specified</p>";
	}else{
		$map_data_parameters = cdm_ws_get(CDM_WS_GEOSERVICE_DISTRIBUTIONMAP, $taxon->uuid);

		$display_width = variable_get('cdm_dataportal_geoservice_display_width', false);
		$bounding_box = variable_get('cdm_dataportal_geoservice_bounding_box', false);
		$labels_on = variable_get('cdm_dataportal_geoservice_labels_on', 0);

		$query_string = ($display_width ? '&ms=' . $display_width: '')
		. ($bounding_box ? '&bbox=' .  $bounding_box : '')
		. ($labels_on ? '&labels=' .  $labels_on : '');

		$query_string .= '&img=false&legend=1&mlp=3';

		if(variable_get('cdm_dataportal_map_openlayers', 1)){
			// embed into openlayers viewer
			//$server = 'http://edit.csic.es/v1/areas_sld.php';
			$server = 'http://edit.csic.es/v1/test.php';
			$map_tdwg_Uri = url($server. '?' .$map_data_parameters->String, $query_string);

			//#print($map_tdwg_Uri.'<br>');

			//$map_tdwg_Uri ='http://edit.csic.es/v1/areas3_ol.php?l=earth&ad=tdwg4:c:UGAOO,SAROO,NZSOO,SUDOO,SPAAN,BGMBE,SICSI,TANOO,GEROO,SPASP,KENOO,SICMA,CLCBI,YUGMA,GRCOO,ROMOO,NZNOO,CLCMA,YUGSL,CLCLA,ALGOO,SWIOO,CLCSA,MDROO,HUNOO,ETHOO,BGMLU,COROO,BALOO,POROO,BALOO|e:CZESK,GRBOO|g:AUTAU|b:LBSLB,TUEOO|d:IREIR,AUTLI,POLOO,IRENI|f:NETOO,YUGCR|a:TUEOO,BGMBE,LBSLB||tdwg3:c:BGM,MOR,SPA,SIC,ITA,MOR,SPA,FRA|a:YUG,AUT&as=a:8dd3c7,,1|b:fdb462,,1|c:4daf4a,,1|d:ffff33,,1|e:bebada,,1|f:ff7f00,,1|g:377eb8,,1&&ms=610&bbox=-180,-90,180,90';
			//$tdwg_sldFile = cdm_http_request($map_tdwg_Uri);

			// get the respone from the map service
			$responseObj = cdm_ws_get($map_tdwg_Uri, null, null, "GET", TRUE);
			$responseObj = $responseObj[0];

			// get the sld files from the response object
			if(isset($responseObj->layers)){
				$layerSlds = $responseObj->layers;
				foreach($layerSlds as $layer){
					$tdwg_sldUris[$layer->tdwg] = "http://edit.csic.es/v1/sld/".$layer->sld;
					//#print($tdwg_sldUris[$layer->tdwg].'<br>');
				}
			}
			// get the bbox from the response object
			$zoomto_bbox = ($bounding_box ? $bounding_box : ($responseObj->bbox ? $responseObj->bbox :'-180, -90, 180, 90') );

			$add_tdwg1 = (isset($tdwg_sldUris['tdwg1']) ? "
          tdwg_1.params.SLD = '".$tdwg_sldUris['tdwg1']."';
          map.addLayers([tdwg_1]);" : '');
			$add_tdwg2 = (isset($tdwg_sldUris['tdwg2']) ? "
          tdwg_2.params.SLD = '".$tdwg_sldUris['tdwg2']."';
          map.addLayers([tdwg_2]);" : '');
			$add_tdwg3 = (isset($tdwg_sldUris['tdwg3']) ? "
          tdwg_3.params.SLD = '".$tdwg_sldUris['tdwg3']."';
          map.addLayers([tdwg_3]);" : '');
			$add_tdwg4 = (isset($tdwg_sldUris['tdwg4']) ? "
          tdwg_4.params.SLD = '".$tdwg_sldUris['tdwg4']."';
          map.addLayers([tdwg_4]);" : '');

			//      $googleMapsApiKey_localhost = 'ABQIAAAAFho6eHAcUOTHLmH9IYHAeBRi_j0U6kJrkFvY4-OX2XYmEAa76BTsyMmEq-tn6nFNtD2UdEGvfhvoCQ';
			//      drupal_set_html_head(' <script src="http://maps.google.com/maps?file=api&amp;v=2&amp;key='.$googleMapsApiKey_localhost.'"></script>');

			/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			 * OpenLayers.js must be loaded BEFORE jQuery.
			 * If jQuery loaded before $.something will fail in IE8.
			 * Therefore we add OpenLayers.js it in the page.tpl.php
			 */
			//drupal_add_js(drupal_get_path('module', 'cdm_dataportal').'/js/OpenLayers/OpenLayers.js', 'core', 'header');
			drupal_add_js('
 var map;
 
 var layerOptions = {
     maxExtent: new OpenLayers.Bounds(-180, -90, 180, 90),
     isBaseLayer: false,
     displayInLayerSwitcher: false
  };
 
 var tdwg_1 = new OpenLayers.Layer.WMS.Untiled( 
    "tdwg level 1", 
    "http://edit.csic.es/geoserver/wms",
    {layers:"topp:tdwg_level_1",transparent:"true", format:"image/png"},
    layerOptions
  );
  
 var tdwg_2 = new OpenLayers.Layer.WMS.Untiled( 
    "tdwg level 2", 
    "http://edit.csic.es/geoserver/wms",
    {layers:"topp:tdwg_level_2",transparent:"true", format:"image/png"},
    layerOptions
  );
  
 var tdwg_3 = new OpenLayers.Layer.WMS.Untiled( 
    "tdwg level 3", 
    "http://edit.csic.es/geoserver/wms",
    {layers:"topp:tdwg_level_3", transparent:"true", format:"image/png"},
    layerOptions
  );
  
  var tdwg_4 = new OpenLayers.Layer.WMS.Untiled( 
    "tdwg level 4", 
    "http://edit.csic.es/geoserver/wms",
    {layers:"topp:tdwg_level_4",transparent:"true", format:"image/png"},
    layerOptions
  );
  
 // make baselayer
 layerOptions[\'isBaseLayer\'] = true; 
 
 var ol_wms = new OpenLayers.Layer.WMS( 
    "OpenLayers WMS",
    "http://labs.metacarta.com/wms/vmap0",
    {layers: \'basic\'}, 
    layerOptions
  );
  
  
  // ------------------------------
  
  
 function init() {
 
   var mapOptions={
// controls break openlayers in IE8 !!!!!!!!!!!!!!
//     controls: 
//       [
//         new OpenLayers.Control.LayerSwitcher({\'ascending\':false}),
//         new OpenLayers.Control.PanZoomBar(),
//         //new OpenLayers.Control.PanZoom(),
//         //new OpenLayers.Control.MouseToolbar(),
//         //new OpenLayers.Control.MousePosition(),
//         //new OpenLayers.Control.KeyboardDefaults()
//       ],
       maxExtent: new OpenLayers.Bounds(-180, -90, 180, 90),
       maxResolution: '.(360 / $display_width).',
       restrictedExtent: new OpenLayers.Bounds(-180, -90, 180, 90),
       projection: new OpenLayers.Projection("EPSG:4326")
    };
    
   
   map = new OpenLayers.Map(\'openlayers_map\', mapOptions);
   map.addLayers([ol_wms]);
   '.$add_tdwg1.'
   '.$add_tdwg2.'
   '.$add_tdwg3.'
   '.$add_tdwg4.'
   map.zoomToExtent(new OpenLayers.Bounds('.$zoomto_bbox.'), false);
 }
 
$(document).ready(function(){
  init();

});'
			, 'inline');
			$out = '<div id="openlayers_map" class="smallmap" style="width: '.$display_width.'px; height:'.($display_width / 2).'px"></div>';
			$out .= '<div class="distribution_map_caption">' . variable_get('cdm_dataportal_geoservice_map_caption', '') . '</div>' . '<br>';

		} else {
			// simple image
			$mapUri = url($server. '?' .$map_data_parameters->String, $query_string);
			$out .= '<img class="distribution_map" src="'.$mapUri.'" alt="Distribution Map" />';
		}

		// add a simple legend
		if(variable_get('cdm_dataportal_geoservice_legend_on', TRUE)){
			$legenddata = array(
        'native' => "4daf4a",
        'native_doubtfully_native' => "377eb8",
        'cultivated' => "984ea3",
        'introduced' => "ff7f00",
        'introduced adventitious' => "ffff33",
        'introduced cultivated' => "a65628",
        'introduced naturalized' => "f781bf"
        );

        $out .= '<div class="distribution_map_legend">';
        foreach($legenddata as $term => $color){
        	$out .= '<img style="width: 3em; height: 1em; background-color: #'.$color.'" src="'.
        	drupal_get_path('module', 'cdm_dataportal').'/images/clear.gif" />'.t($term).' ';
        }
        $out .= '</div>';

		}

		return $out;
	}
}

function theme_cdm_taxonName($taxonName, $nameLink = NULL, $refenceLink = NULL, $renderPath = null){

	$renderTemplate = get_nameRenderTemplate($renderPath, $nameLink, $refenceLink);

	$partDefinition = get_partDefinition($taxonName->class);

	// apply defintions to template
	foreach($renderTemplate as $part=>$uri){
		if(isset($partDefinition[$part])){
			$renderTemplate[$part] = $partDefinition[$part];
		}
		if(is_array($uri)){
			$renderTemplate[$part]['#uri'] = $uri['#uri'];
		}
	}

	if(isset($taxonName->taggedName)){

		$taggedName = $taxonName->taggedName;
		// due to a bug in the cdmlib the taggedName alway has a lst empty element, we will remove it:
		array_pop($taggedName);

		$lastAuthorElementString = false;
		$hasNamePart_with_Authors = isset($renderTemplate['namePart']) && isset($renderTemplate['namePart']['authors']);
		$hasNameAuthorPart_with_Authors = isset($renderTemplate['nameAuthorPart']) && isset($renderTemplate['nameAuthorPart']['authors']);

		if(!($hasNamePart_with_Authors || $hasNameAuthorPart_with_Authors)){
			//      // find author and split off from name
			//      // TODO expecting to find the author as the last element
			//      if($taggedName[count($taggedName)- 1]->type == 'authors'){
			//        $authorTeam = $taggedName[count($taggedName)- 1]->text;
			//        unset($taggedName[count($taggedName)- 1]);
			//      }

			// remove all authors
			$taggedNameNew = array();
			foreach($taggedName as $element){
				if($element->type != 'authors'){
					$taggedNameNew[] = $element;
				} else {
					$lastAuthorElementString = $element->text;
				}
			}
			$taggedName = $taggedNameNew;

		}
		$name = '<span class="'.$taxonName->class.'">'.theme('cdm_taggedtext2html', $taggedName).'</span>';
	} else {
		$name = '<span class="'.$taxonName->class.'_titleCache">'.$taxonName->titleCache.'</span>';
	}

	// fill name into $renderTemplate
	array_setr('name', $name, $renderTemplate);

	//  // fill with authorTeam
	//  if($authorTeam){
	//    $authorTeamHtml = ' <span class="authorTeam">'.$authorTeam.'</span>';
	//    array_setr('authorTeam', $authorTeamHtml, $renderTemplate);
	//  }


	// fill with reference
	if(isset($renderTemplate['referencePart'])){

		// [Eckhard]:"Komma nach dem Taxonnamen ist grunsätzlich falsch,
		// Komma nach dem Autornamen ist überall dort falsch, wo ein "in" folgt."
		if(isset($renderTemplate['referencePart']['reference']) && $taxonName->nomenclaturalReference){
			$microreference = null;
			if(isset($renderTemplate['referencePart']['microreference'])){
				$microreference = $taxonName->nomenclaturalMicroReference;
			}
			$citation = cdm_ws_get(CDM_WS_NOMENCLATURAL_REFERENCE_CITATION, array($taxonName->nomenclaturalReference->uuid, $microreference));
			$citation = $citation->String;
			// find preceding element of the refrence
			$precedingKey = get_preceding_contentElementKey('reference', $renderTemplate);
			if(str_beginsWith($citation, ", in")){
				$citation = substr($citation, 2);
				$separator = ' ';
			} else if(!str_beginsWith($citation, "in") && $precedingKey == 'authors'){
				$separator = ', ';
			} else {
				$separator = ' ';
			}

			$referenceArray['#separator'] = $separator;
			$referenceArray['#html'] = '<span class="reference">'.$citation.'</span>';
			array_setr('reference', $referenceArray, $renderTemplate);
		}

		// if authors have been removed from the name part the last named authorteam
		// should be added to the reference citation, otherwise, keep the separator
		// out of the reference
		if(isset($renderTemplate['referencePart']['authors']) && $lastAuthorElementString){
			// if the nomenclaturalReference cintation is not included in the reference part but diplay of the microreference
			// is whanted append the microreference to the authorTeam
			if(!isset($renderTemplate['referencePart']['reference']) && isset($renderTemplate['referencePart']['microreference'])){
				$separator = ": ";
				$citation = $taxonName->nomenclaturalMicroReference;
			}
			$referenceArray['#html'] = ' <span class="reference">'.$lastAuthorElementString.$separator.$citation.'</span>';
			array_setr('authors', $referenceArray, $renderTemplate);
		}

	}

	// fill with status
	if(array_setr('status', true, $renderTemplate)){
		if(isset($taxon->name->status[0])){
			foreach($taxon->name->status as $status){
				$statusHtml .= ', '.$status->type->representation_L10n;
			}
		}
		array_setr('status', ' <span class="nomenclatural_status">'.$statusHtml.'</span>', $renderTemplate);
	}

	// fill with protologues etc...
	if(array_setr('description', true, $renderTemplate)){
		$descriptions = cdm_ws_get(CDM_WS_NAME_DESCRIPTIONS, $taxonName->uuid);
		foreach($descriptions as $description){
			if(!empty($description)){
				foreach($description->elements as $description_element){
					$descriptionHtml .= theme("cdm_media", $description_element, array('application/pdf', 'image/png', 'image/jpeg', 'image/gif', 'text/html'));
				}
			}
		}
		array_setr('description', $descriptionHtml, $renderTemplate);
	}

	// render
	$out = '';
	foreach($renderTemplate as $partName=>$part){
		$separator = '';
		$partHtml = '';
		$uri = false;
		if(!is_array($part)){
			continue;
		}
		if(isset($part['#uri']) && is_string($part['#uri'])){
			$uri = $part['#uri'];
			unset($part['#uri']);
		}
		foreach($part as $key=>$content){
			$html = '';
			if(is_array($content)){
				$html = $content['#html'];
				$separator = $content['#separator'];
			} else if(is_string($content)){
				$html = $content;
			}
			$partHtml .= '<span class="'.$key.'">'.$html.'</span>';
		}
		if($uri){
			$out .= $separator.'<a href="'.$uri.'" class="'.$partName.'">'.$partHtml.'</a>';
		} else {
			$out .= $separator.$partHtml;
		}
	}

	return $out;
}

/**
 * Recursively searches the array for the $key and sets the given value
 * @param $key
 * @param $value
 * @param $array
 * @return true if the key has been found
 */
function &array_setr($key, $value, array &$array){
	foreach($array as $k=>&$v){
		if($key == $k){
			$v = $value;
			return $array;
		} else if(is_array($v)){
			$innerArray = array_setr($key, $value, $v);
			if($innerArray){
				return $array;
			}
		}
	}
	return null;
}

function &get_preceding_contentElement($contentElementKey, array &$renderTemplate){
	$precedingElement = null;
	foreach($renderTemplate as &$part){
		foreach($part as $key=>&$element){
			if($key == $contentElementKey){
				return $precedingElement;
			}
			$precedingElement = $element;
		}
	}
	return null;
}

function &get_preceding_contentElementKey($contentElementKey, array &$renderTemplate){
	$precedingKey = null;
	foreach($renderTemplate as &$part){
		foreach($part as $key=>&$element){
			if($key == $contentElementKey){
				return $precedingKey;
			}
			if(!str_beginsWith($key, '#')){
				$precedingKey = $key;
			}
		}
	}
	return null;
}


function theme_cdm_related_taxon($taxon, $reltype_uuid = '', $displayNomRef = true){

	$relsign = '';
	$name_prefix = '';
	$name_postfix = '';
	switch ($reltype_uuid){
		case UUID_HETEROTYPIC_SYNONYM_OF:
		case UUID_SYNONYM_OF:
			$relsign = '=';
			break;
		case UUID_HOMOTYPIC_SYNONYM_OF:
			$relsign = '≡';
			break;
		case UUID_MISAPPLIED_NAME_FOR:
		case UUID_INVALID_DESIGNATION_FOR:
			$relsign = '&ndash;'; // &ndash; &mdash; &minus;
			$name_prefix = '"';
			$name_postfix = '"';
			break;
		default :
			$relsign = '&ndash;';
	}

	$renderPath = 'related_taxon';

	//$taxonUri = url(path_to_taxon($taxon->uuid));
	if($taxon->name->nomenclaturalReference){
		$referenceUri = url(path_to_reference($taxon->name->nomenclaturalReference->uuid));
	}
	$nameHtml = theme('cdm_taxonName', $taxon->name, $taxonUri, $referenceUri, $renderPath);

	$out = '<span class="relation_sign">'.$relsign.'</span>'.$name_prefix . $nameHtml . $name_postfix;
	return $out;

}

/**
 * will theme form elements of type 'select_secuuid'
 * see $form['cdm_dataportal']['secuuid_widget']
 * @param FormElement $element
 */
function theme_select_secuuid($element) {

	$default_uuid = variable_get($element['#varname'], false);

	$tree = cdm_taxontree_build_tree(null, false); // get root nodes
	$secUuids = array();
	foreach($tree as $node){
		$secUuids[] = $node->secUuid;
	}
	cdm_api_secref_cache_prefetch($secUuids);

	theme('cdm_taxontree_add_scripts');
	drupal_add_js('$(document).ready(function() {$(\'ul.cdm_taxontree\').cdm_taxontree(
  {
    widget:                 true,
    element_name:           \''.$element['#varname'].'\',  //
    multiselect:            '.($element['#multiple']?'true':'false').',         //
  }
  );});', 'inline');

	$out  = '<div class="cdm_taxontree_widget">';
	$out .= '<div class="taxontree">'.theme('cdm_taxontree', $tree, NULL, FALSE, 'cdm_taxontree_node_reference').'</div>';
	$out .= $element['#children'].'<div style="clear: both;" /></div>';

	return theme(
    'form_element',
	array(
      '#title' => $element['#title'],
      '#description' => $element['#description'],
      '#id' => $element['#id'],
      '#required' => $element['#required'],
      '#error' => $element['#error'],
	),
	$out
	);
}

function theme_cdm_dynabox($label, $content_url, $theme, $enclosingtag = 'li'){
	drupal_add_js(drupal_get_path('module', 'cdm_dataportal').'/js/cdm_dynabox.js');

	$cdm_proxy_url = url('cdm_api/proxy/'.urlencode($content_url)."/$theme");
	$out .= '<li class="dynabox"><span class="label" alt="'.t('Click for accepted taxon').'">'.$label.'</span>';
	$out .= '<ul class="dynabox_content" title="'.$cdm_proxy_url.'"><li><img class="loading" src="'.drupal_get_path('module', 'cdm_dataportal').'/images/loading_circle_grey_16.gif" style="display:none;"></li></ul>';
	return $out;
}

function theme_cdm_list_of_taxa($records, $showMedia = false){

	$renderPath = 'list_of_taxa';

	$showMedia_taxa = variable_get('cdm_dataportal_findtaxa_show_taxon_thumbnails', 1);
	$showMedia_synonyms = variable_get('cdm_dataportal_findtaxa_show_synonym_thumbnails', 0);

	// .. well, for sure not as performant as before, but better than nothing.
	$synonym_uuids = array();
	foreach($records as $taxon){
		if($taxon->class != "Taxon"){
			if(!array_key_exists($taxon->uuid, $synonym_uuids)){
				$synonym_uuids[$taxon->uuid] = $taxon->uuid;
			}
		}
	}
	// batch service not jet implemented:
	// $table_of_accepted = cdm_ws_property(CDM_WS_TAXON_ACCEPTED, join(',', $synonym_uuids));
	// thus ...
	$table_of_accepted = array();
	foreach($synonym_uuids as $synUuid){
		$table_of_accepted[$synUuid] = cdm_ws_get(CDM_WS_TAXON_ACCEPTED, $synUuid);
	}

	$out = '<ul class="cdm_names" style="background-image: none;">';

	foreach($records as $taxon){
		// its a Taxon
		if($taxon->class == "Taxon"){
			$taxonUri = url(path_to_taxon($taxon->uuid));
			if(isset($taxon->name->nomenclaturalReference)){
				$referenceUri = url(path_to_reference($taxon->name->nomenclaturalReference->uuid));
			}
			$out .= '<li class="Taxon">'.theme('cdm_taxonName', $taxon->name, $taxonUri, $referenceUri, $renderPath);
			if($showMedia_taxa){
				$out .= theme('cdm_taxon_list_thumbnails', $taxon);
			}
			$out .= '</li>';
		} else {
			// its a synonym
			$uuid = $taxon->uuid;
			$acceptedTaxa = $table_of_accepted[$uuid];
			if(count($acceptedTaxa) == 1){
				$acceptedTaxon = $acceptedTaxa[0];
				$taxonUri = uri_to_synonym($taxon->uuid, $acceptedTaxon->uuid, 'synonymy');
				if(isset($acceptedTaxon->name->nomenclaturalReference)){
					$referenceUri = url(path_to_reference($acceptedTaxon->name->nomenclaturalReference->uuid));
				}
				$out .= '<li class="Synonym">'.theme('cdm_taxonName', $taxon->name, $taxonUri, $referenceUri, $renderPath);
				if($showMedia_synonyms){
					$out .= theme('cdm_taxon_list_thumbnails', $acceptedTaxon);
				}
				$out .= '</li>';
			} else {
				//TODO avoid using AHAH in the cdm_dynabox
				//TODO add media
				$out .= theme('cdm_dynabox', theme('cdm_taxonName', $taxon->name, null, null, $renderPath), cdm_compose_url(CDM_WS_TAXON_ACCEPTED, array($taxon->uuid)), 'cdm_list_of_taxa');
			}
		}
	}
	$out .= '</ul>';
	return $out;
}


function theme_cdm_credits(){
	return null;
	$secRef_array = _cdm_dataportal_currentSecRef_array();
	return '<span class="sec_reference_citation">'.$secRef_array['citation'].'</span>'
	.( $secRef_array['period'] ? ' <span class="year">'.partialToYear($secRef_array['period']).'</span>' : '')
	.( $secRef_array['authorTeam'] ? '<div class="author">'.$secRef_array['authorTeam']['titleCache'].'</div>' : '');
}


function theme_cdm_print_button(){

	drupal_add_js ('$(document).ready(function() {
         $(\'#print_button img\').click(function () { 
         window.print();
     });
  });', 'inline');

	$output = '<div id="print_button"><img src="'
	.drupal_get_path('module', 'cdm_dataportal').'/images/print_icon.gif'
	.'" alt="'.t('Print this page').'" title="'.t('Print this page').'" />'.t(' Print this page');
	$output .= '</div>';

	return $output;
}

/**
 * @deprecated ??
 */
function theme_cdm_reference($reference, $linkToReference = FALSE, $style = NULL ){

	if($style == "ZoologicalName"){
		$year = partialToYear($reference->datePublished->start);
		$citation = $reference->authorTeam->titleCache.($year ? ', '.$year : '');
	} else {
		$citation = $reference->titleCache;
	}
	if($linkToReference){
		return l('<span class="reference">'.$citation.'</span>', "/cdm_dataportal/reference/".$reference->uuid, array("class"=>"reference"), NULL, NULL, FALSE ,TRUE);
	} else {
		return '<span class="reference">'.$citation.'</span>';
	}
}

/**
 * Enter description here...
 *
 * @param unknown_type $referenceSTO a referenceSTO or referenceTO instance
 * @param unknown_type $cssClass
 * @param unknown_type $separator
 * @param unknown_type $enclosingTag
 * @return unknown
 */
function theme_cdm_nomenclaturalReference($name, $doLink = FALSE, $displayMicroreference = TRUE, $displayPages = FALSE, $enclosingTag = 'span'){

	if(!$name->nomenclaturalReference){
		return;
	}

	$reference = $name->nomenclaturalReference;

	// different display for ZoologicalNames and others
	if($name->class == ZoologicalName){
		$year = partialToYear($reference->datePublished->start);
		$citation = $reference->authorTeam->titleCache.($year ? ', '.$year : '');
	} else {
		// BotanicalName and others
		$citation = $reference->titleCache;
	}

	if($doLink){
		$citationHtml = l($citation, "/cdm_dataportal/reference/".$reference->uuid, array(), NULL, NULL, FALSE, TRUE);
	} else {
		$citationHtml = $citation;
	}

	if(!empty($citation)){
		$citationHtml = (str_beginsWith($citation, 'in') ? '&nbsp;':',&nbsp;') . $citationHtml;
		if($displayMicroreference && $name->nomenclaturalMicroReference){
			$microreference = '<span class="pages">:&nbsp;' . $name->nomenclaturalMicroReference . '</span>';
		}
		if($displayPages){ //TODO substitute for microreference in dipterea? -> check theme.php#L85
		$citation .= ': '.$reference->pages;
		}
		$citationHtml .= $microreference;
	}


	return '<'.$enclosingTag.' class="reference">'.$citationHtml.'</'.$enclosingTag.'>'; ;
}

/**
 * Allows theaming of the taxon page tabs
 *
 * @param $tabname
 * @return unknown_type
 */
function theme_cdm_taxonpage_tab($tabname){
	//TODO replace by using translations or theme the menue tabs itself instead?
	switch($tabname){
		default: return t($tabname);
	}
}

/**
 * default title for a taxon page
 *
 * @param NameTO $nameTO
 * @return the formatted taxon name
 */
function theme_cdm_taxon_page_title($taxon){

	$renderPath = 'taxon_page_title';
	if(isset($taxon->name->nomenclaturalReference)){
		$referenceUri = url(path_to_reference($taxon->name->nomenclaturalReference->uuid));
	}
	return '<span class="'.$taxon->class.'">'.theme('cdm_taxonName', $taxon->name, null, $referenceUri, $renderPath).'</span>';
}


function theme_cdm_acceptedFor($renderPath = false){
	$out = '';
	if(!$renderPath){
		$renderPath = 'acceptedFor';
	}
	if(isset($_REQUEST['acceptedFor'])){
			
		$synonym = cdm_ws_get(CDM_WS_TAXON, $_REQUEST['acceptedFor']);
			
		if($synonym){
			$out .= '<span class="acceptedFor">';
			$out .= t('is accepted for ');
			if(isset($synonym->name->nomenclaturalReference)){
				$referenceUri = url(path_to_reference($synonym->name->nomenclaturalReference->uuid));
			}
			$out .= theme('cdm_taxonName', $synonym->name, null, $referenceUri, $renderPath);
			$out .= '</span>';
		}
	}

	return $out;
}

function theme_cdm_back_to_search_result_button(){
	$out = '';
	if($_SESSION['cdm']['search']){
		/*['cdm']['last_search']*/
		//$out .= '<div id="backButton">'.l(t('Back to search result'), $_SESSION ).'</div>';
		$out .= '<div id="backButton">'.l(t('Back to search result'), "http://" . $_SERVER['SERVER_NAME'] . $_SESSION['cdm']['last_search'] ).'</div>';

	}
	return $out;
}

function theme_cdm_back_to_image_gallery_button(){
	//$galleryLinkUri = path_to_taxon($taxon->uuid).'/images';
	//$gallery_name = $taxon->uuid;
	//$mediaList = cdm_ws_get(CDM_WS_TAXON_MEDIA, array($taxon->uuid, $prefMimeTypeRegex, $prefMediaQuality));

	$out = '<div id="backToGalleryButton">'.l(t('Back to Images'), $_SESSION['cdm']['last_gallery'] ).'</div>';

	return $out;
}


/**
 * A wrapper function that groups available information to show by default, when
 * a taxon page is requested by the browser.
 * Individual themeing has to decide what this page should include (see methods beneath)
 * and what information should go into tabs or should not be shown at all.
 *
 * It is headed by the name of the accepted taxon without author and reference.
 * @param $taxonTO the taxon object
 * @param $page_part name of the part to display,
 *         valid values are: 'description', 'images', 'synonymy', 'all'
 */
function theme_cdm_taxon_page_general($taxon, $page_part = 'description') {

	$page_part = variable_get('cdm_dataportal_taxonpage_tabs', 1) ? $page_part : 'all';
	$hideTabs = array();


	// get images
	$prefMimeTypeRegex = 'image:.*';
	$prefMediaQuality = '*';
	$media = cdm_ws_get(CDM_WS_TAXON_MEDIA, array($taxon->uuid, $prefMimeTypeRegex, $prefMediaQuality));
	if(!isset($media[0])) {
		$hideTabs[] = theme('cdm_taxonpage_tab', 'Images');
	}

	// hideImage flag depending on administative preset
	$hideImages = false;
	if(variable_get('image_hide_rank', '0') != '0'){
		$rankCompare = rank_compare($taxon->name->rank->uuid, variable_get('image_hide_rank', '-99'));
		$hideImages =  ($rankCompare > -1);
	}
	// $hideTabs[] = theme('cdm_taxonpage_tab', 'General');
	// $hideTabs[] = theme('cdm_taxonpage_tab', 'Synonymy')

	// hide tabs
	$tabhide_js = '';
	foreach($hideTabs as $tabText) {
		$tabhide_js .= "$('.tabs.primary').children('li').children('a:contains(\"$tabText\")').hide();\n";
	}
	drupal_add_js("
  $(document).ready(function(){
  $tabhide_js
    });", 'inline');

  $out = '';
  $out .= theme('cdm_back_to_search_result_button');
  $out .= theme('cdm_acceptedFor', 'page_general');

  // --- DESCRIPTION --- //
  if($page_part == 'description' || $page_part == 'all'){

  	$featureTree = cdm_ws_get(CDM_WS_FEATURETREE, variable_get('cdm_dataportal_featuretree_uuid', false));
  	$taxonDescriptions = cdm_ws_get(CDM_WS_TAXON_DESCRIPTIONS, $taxon->uuid);
  	$mergedTrees = cdm_ws_descriptions_by_featuretree($featureTree, $taxonDescriptions, variable_get('cdm_dataportal_descriptions_separated', FALSE));

  	$out .= '<div id="general">';
  	$out .= theme('cdm_taxon_page_description', $taxon, $mergedTrees, $media, $hideImages);
  	$out .= '</div>';
  }
  // --- IMAGES --- //
  if(!$hideImages && $page_part == 'images' || $page_part == 'all'){
  	$out .= '<div id="images">';
  	if($page_part == 'all'){
  		$out .= '<h2>'.t('Images').'</h2>';
  	}
  	$out .= theme('cdm_taxon_page_images', $taxon, $media);
  	$out .= '</div>';
  }
  // --- SYNONYMY --- //
  if($page_part == 'synonymy' || $page_part == 'all'){
  	$out .= '<div id="synonymy">';
  	if($page_part == 'all'){
  		$out .= '<h2>'.t('Synonymy').'</h2>';
  	}
  	$addAcceptedTaxon = !variable_get('cdm_dataportal_nomref_in_title', 1);
  	$out .= theme('cdm_taxon_page_synonymy', $taxon, $addAcceptedTaxon);

  	if(variable_get('cdm_dataportal_display_name_relations', 1)){

  		$nameRelationships = cdm_ws_get(CDM_WS_TAXON_NAMERELATIONS, $taxon->uuid);
  		// TODO is it correct to skip relationsFromThisName since all relationships are to be understood as 'is .... of'
  		if(variable_get('cdm_dataportal_name_relations_skiptype_basionym', 1)){
  			$skip = array(UUID_BASIONYM);
  		}
  		$out .= theme('cdm_nameRelations', $nameRelationships, $skip);
  	}
  	$out .= '</div>';
  }

  return $out;
}

/**
 * Outputs all descriptive data and shows the preferred picture of the
 * accepted taxon.
 *
 */
function theme_cdm_taxon_page_description($taxon, $mergedTrees, $media = null, $hideImages = false){

	//  if(!$hideImages){
	//    // preferred image
	//    // hardcoded for testing;
	//    $defaultRepresentationPart = false;
	//    $defaultRepresentationPart->width = 184;
	//    $defaultRepresentationPart->height = 144;
	//    $defaultRepresentationPart->uri = drupal_get_path('theme', 'palmweb_2').'/images/no_picture.png';
	//
	//    // preferred image size 184px × 144
	//    $imageMaxExtend = 184;
	//    $out .= '<div class="preferredImage">'.$defaultRepresentationPart->uri.theme('cdm_preferredImage', $media, $defaultRepresentationPart, $imageMaxExtend).'</div>';
	//  }

	// description TOC
	$out .= theme('cdm_featureTreeTOCs', $mergedTrees);
	// description
	$out .= theme('cdm_featureTrees', $mergedTrees, $taxon);
	return $out;
}

/**
 * Show whole synonymy for the accepted taxon. Synonymy list is headed by the complete scientific name
 * of the accepted taxon with nomenclatural reference.
 *
 */
function theme_cdm_taxon_page_synonymy($taxon, $addAcceptedTaxon){

	$renderPath = 'taxon_page_synonymy';
	$synomymie = cdm_ws_get(CDM_WS_TAXON_SYNONYMY, $taxon->uuid);
	$taxonRelationships = cdm_ws_get(CDM_WS_TAXON_RELATIONS, $taxon->uuid);
	$skip = array(UUID_BASIONYM);

	if($addAcceptedTaxon){
		if(isset($taxon->name->nomenclaturalReference)){
			$referenceUri = url(path_to_reference($taxon->name->nomenclaturalReference->uuid));
		}
		$out .= theme('cdm_taxonName', $taxon->name, null, $referenceUri, $renderPath);
	}

	if($addAcceptedTaxon && !isset($synomymie->homotypicSynonymsByHomotypicGroup[0])){
		// display the type information for the added taxon
		$typeDesignations = cdm_ws_get(CDM_WS_TAXON_NAMETYPEDESIGNATIONS, $taxon->uuid);
		if($typeDesignations){
			$out .= theme('cdm_typedesignations', $typeDesignations);
		}
	} else {
		// reder the homotypicSynonymyGroup including the type information
		$out .= theme('cdm_homotypicSynonymyGroup', $synomymie->homotypicSynonymsByHomotypicGroup);
	}
	if($synomymie->heterotypicSynonymyGroups) {
		foreach($synomymie->heterotypicSynonymyGroups as $homotypicalGroup){
			$out .= theme('cdm_heterotypicSynonymyGroup', $homotypicalGroup);
		}
	}

	$out .= theme('cdm_taxonRelations', $taxonRelationships, $skip);

	return $out;
}

/**
 * Show the collection of images stored with the accepted taxon
 *
 */
function theme_cdm_taxon_page_images($taxon, $media){

	$hasImages = isset($media[0]);

	if($hasImages){
		//
		$maxExtend = 150;
		$cols = 3;
		$maxRows = false;
		$alternativeMediaUri = null;
		$captionElements = array('title', 'rights', '#uri'=>t('open Image'));
		$gallery_name = $taxon->uuid;
		$mediaLinkType = 'LIGHTBOX';
		$out = '<div class="image-gallerie">';
		$out .= theme('cdm_media_gallerie', $media, $gallery_name, $maxExtend, $cols, $maxRows, $captionElements, $mediaLinkType, null);
		$out .= '</div>';
	}else{
		$out = 'No images available.';

	}
	return $out;

}

function theme_cdm_reference_pager($referencePager, $path, $parameters = array()){
	drupal_set_title(t('Bibliographic Index'));
	$out = '';
	if(count($referencePager->records) > 0){
		$out .= '<ul>';
		foreach($referencePager->records as $reference){
			$reference->fullCitation = $reference->titleCache; //FIXME remove hack for matching cdm entity to STO
			$out .= '<li>'.theme('cdm_reference', $reference, TRUE).'</li>';
		}
		$out .= '</ul>';
		$out .= theme('cdm_pager', $referencePager,  $path, $parameters);
	} else {
		$out = '<h4 class="error">Sorry, this page contains not entries.</h4>';
	}
	return $out;
}

/**
 * Show a reference in it's atomized form
 */
function theme_cdm_reference_page($referenceTO){
	if($referenceTO->titleCache) {
		drupal_set_title($referenceTO->titleCache);
	} else {
		drupal_set_title($referenceTO->fullCitation);
	}
	$field_order = array(
    "title",
	//"titleCache",
	//"citation",
    "authorTeam",
    "editor",
    "publisher",
    "placePublished",
    "datePublished",
    "year",
    "edition",      // class Book
    "volume",       // class Article
    "seriesPart",
    "inSeries",
    "inJournal",     // class Article
    "inBook",        // class BookSection
    "nomRefBase",    // class BookSection, Book, Article
    "inProceedings", // class InProceedings
    "pages",         // class Article
    "series",        // class Article, PrintSeries
    "school",        // class Thesis
    "institution",   // class Report
    "organization",  // class Proceedings
    "nextVersion",
    "previousVersion",
    "isbn",         // class Book
    "issn",         // class Journal
    "uri",
	);

	$table_rows = array();
	foreach($field_order as $fieldname){
		if(isset($referenceTO->$fieldname)){
			if($fieldname == "datePublished") {
				$partial = $referenceTO->$fieldname;
				$datePublished = '';
				if($partial->start){
					$datePublished = substr($partial->start, 0, 4).'-'.substr($partial->start, 4, 2).'-'.substr($partial->start, 6, 2);
				}
				if($partial->end){
					$datePublished = (strlen($datePublished) > 0 ? ' '.t('to').' ' : '').substr($partial->end, 0, 4).'-'.substr($partial->end, 4, 2).'-'.substr($partial->end, 6, 2);
				}
				$table_rows[] = array(t(ucfirst(strtolower($fieldname))), $datePublished);
			} else if(is_object($referenceTO->$fieldname)){
				$table_rows[] = array(t(ucfirst(strtolower($fieldname))), $referenceTO->$fieldname->titleCache);
			} else {
				$table_rows[] = array(t(ucfirst(strtolower($fieldname))), $referenceTO->$fieldname);
			}
		}
	}
	return theme("table", array("","") , $table_rows);
}

/**
 * Show a synonym page
 *
 * TODO what should show on this page exactly?
 *
 */
function theme_cdm_synonym_page(){

}

function theme_cdm_preferredImage($media, $defaultRepresentationPart, $imageMaxExtend, $parameters = ''){

	if(isset($media[0])){
		$representationPart = $media[0]->representations[0]->parts[0];
		if($parameters){
			$representationPart->uri.$parameters;
		}
	} else {
		$representationPart = $defaultRepresentationPart;
	}

	//$widthAndHeight = ($imageWidth ? ' width="'.$imageWidth : '').($imageHeight ? '" height="'.$imageHeight : '');
	//  $imageUri = $preferredMedia ? $preferredMedia->representations[0]->parts[0]->uri . $parameters : $defaultImage;
	$attributes = array('alt'=>($preferredMedia ? $preferredMedia->representations[0]->parts[0]->uri : "no image available"));
	$out .= theme('cdm_media_gallerie_image', $representationPart, $imageMaxExtend, false, $attributes);
	// $out = '<img class="left" '.$widthAndHeight.' " src="'.$imageUri.'" alt="'.$altText.'" />';
	return $out;
}

function theme_cdm_homotypicSynonymyGroup($synonymList, $prependedSynonyms = array()){

	if(! is_array($synonymList) || count($synonymList) == 0){
		return;
	}

	$out = '<ul class="homotypicSynonyms">';

	if(!empty($prependedSynonyms)){
		foreach($prependedSynonyms as $taxon){
			$out .= '<li class="synonym">'.theme('cdm_related_taxon', $taxon, UUID_HOMOTYPIC_SYNONYM_OF).'</li>';
		}
	}

	foreach($synonymList as $synonym){
		$out .= '<li class="synonym">'.theme('cdm_related_taxon', $synonym, UUID_HOMOTYPIC_SYNONYM_OF).'</li>';
	}

	$typeDesignations = cdm_ws_get(CDM_WS_TAXON_NAMETYPEDESIGNATIONS, $synonymList[0]->uuid);
	if($typeDesignations){
		$out .= theme('cdm_typedesignations', $typeDesignations);
	}

	$out .= '</ul>';
	return $out;
}

function theme_cdm_homotypicSynonymLine($taxon){
	$out = '';
	$out .= '<li class="synonym">'.theme('cdm_related_taxon', $taxon, UUID_HOMOTYPIC_SYNONYM_OF).'</li>';
	return $out;
}

function theme_cdm_heterotypicSynonymyGroup($homotypicalGroup){
	$out = '';
	$out = '<ul class="heterotypicSynonymyGroup">';

	$is_first_entry = true;
	$typeDesignations = null;
	foreach($homotypicalGroup as $synonym){
		if($is_first_entry){
			$is_first_entry = false;
			//$typeDesignations = cdm_ws_get(CDM_WS_NAME_TYPEDESIGNATIONS, $synonym->name->uuid);
			$typeDesignations = cdm_ws_get(CDM_WS_TAXON_NAMETYPEDESIGNATIONS, $synonym->uuid);
			// is first list entry
			$out .= '<li class="firstentry synonym">'.theme('cdm_related_taxon',$synonym, UUID_HETEROTYPIC_SYNONYM_OF).'</li>';
		} else {
			$out .= '<li class="synonym">'.theme('cdm_related_taxon',$synonym, UUID_HOMOTYPIC_SYNONYM_OF).'</li>';
		}
	}

	if($typeDesignations){
		$out .= theme('cdm_typedesignations', $typeDesignations);
	}

	$out .= '</ul>';

	return $out;
}

/**
 * renders misapplied names and invalid designations.
 * Both relation types are currently treated the same!
 *
 * @param unknown_type $taxonRelationships
 * @return unknown
 */
function theme_cdm_taxonRelations($taxonRelationships){

	if(!$taxonRelationships){
		return;
	}

	_add_js_cluetip();

	// aggregate misapplied names having the same fullname:
	$misapplied = array();
	foreach($taxonRelationships as $taxonRelation){
		if(true || $taxonRelation->type->uuid == UUID_MISAPPLIED_NAME_FOR || $taxonRelation->type->uuid == UUID_INVALID_DESIGNATION_FOR ){

			$name = $taxonRelation->fromTaxon->name->titleCache;
			$authorteam = $taxonRelation->fromTaxon->sec->authorTeam->titleCache;

			if(!isset($misapplied[$name])){
				$misapplied[$name]['out'] = '<span class="misapplied">'.theme('cdm_related_taxon',$taxonRelation->fromTaxon, UUID_MISAPPLIED_NAME_FOR, false).'</span>';
			}

			// collect all authors for this fullname
			if(isset($authorteam)){
				$misapplied[$name]['authorteam'][$authorteam] = '&nbsp;<span class="sensu cluetip no-print" title="|sensu '.htmlspecialchars(theme('cdm_reference',$taxonRelation->fromTaxon->sec )).'|">sensu '
				.$authorteam.'</span>'
				.'<span class="reference only-print">sensu '.theme('cdm_reference', $taxonRelation->fromTaxon->sec ).'</span>';
			}

		}
	}

	// generate output
	$out = '<ul class="misapplied">';
	foreach($misapplied as $misapplied_name){
		$out .= '<li class="synonym">'.$misapplied_name['out'] . " ";
		// sorting authors
		ksort($misapplied_name['authorteam']);
		$out .= join('; ', $misapplied_name['authorteam']);
		$out .= '</li>';
	}
	$out .= '</ul>';
	return $out;
}

function theme_cdm_nameRelations($nameRelationships, $skipTypes = false){

	// group by relationship type
	$relationshipGroups = array();
	foreach($nameRelationships as $nameRelationship){
		if(!array_key_exists($nameRelationship->type->uuid, $relationshipGroups)){
			$relationshipGroups[$nameRelationship->type->uuid] = array();
		}
		$relationshipGroups[$nameRelationship->type->uuid][] = $nameRelationship;
	}

	// generate output
	$out = '';
	foreach($relationshipGroups as $group){
		$type = $group[0]->type;

		if(is_array($skipTypes) && in_array($type->uuid, $skipTypes)){
			continue;
		}

		$block->module = 'cdm_dataportal';
		$block->subject = t(ucfirst($type->inverseRepresentation_L10n));
		$block->delta = generalizeString(strtolower($type->inverseRepresentation_L10n));

		foreach($group as $relationship){
			$relatedNames[] = cdm_taggedtext2html($relationship->fromName->taggedName);
		}

		$block->content .= implode('; ', $relatedNames);
		$out .= theme('block', $block);
	}
	return $out;
}

/**
 * FIXME this definitively has to be in another spot. just didn't know where to put it right now.
 * Compares the status of two SpecimenTypeDesignations
 * @param String $a 	a SpecimenTypeDesignations
 * @param String $b		another SpecimenTypeDesignations
 */
function compare_specimenTypeDesignationStatus($a, $b){
	/* this is the desired sort oder as of now:
	 * 	Holotype
	 * 	Isotype
	 * 	Lectotype
	 * 	Isolectotype
	 * 	Syntype
	 *
	 * TODO
	 * Basically, what we are trying to do is, we define an ordered array of TypeDesignation-states
	 * and use the index of this array for comparison. This array has to be filled with the cdm-
	 * TypeDesignation states and the order should be parameterisable inside the dataportal.
	 */
	// make that static for now
	$typeOrder = array('Holotype', 'Isotype', 'Lectotype', 'Isolectotype', 'Syntype');

	$aQuantifier = array_search($a->typeStatus->label, $typeOrder);
	$bQuantifier = array_search($b->typeStatus->label, $typeOrder);

	if ($aQuantifier == $bQuantifier) {
		// sort alphabetically
		return ($a->typeStatus->label < $b->typeStatus->label) ? -1 : 1;
	}
	return ($aQuantifier < $bQuantifier) ? -1 : 1;

}

function theme_cdm_typedesignations($typeDesignations = array()){

	_add_js_cluetip();
	$renderPath = 'typedesignations';
	$out = '<ul class="typeDesignations">';

	$specimenTypeDesignations = array();
	foreach($typeDesignations as $typeDesignation){
		if($typeDesignation->class == 'SpecimenTypeDesignation'){
			// SpecimenTypeDesignations should be ordered. collect theme here only
			$specimenTypeDesignations[] = $typeDesignation;
		}else {

			// it's a NameTypeDesignation
			if($typeDesignation->notDesignated){
				$out .= '<li class="nameTypeDesignation"><span class="status">Type</span>: '.t('not designated'). '</li>';
			}else if($typeDesignation->typeName){

				$out .= '<li class="nameTypeDesignation"><span class="status">Type</span>: ';

				if($typeDesignation->typeName->nomenclaturalReference){
					$referenceUri = url(path_to_reference($typeDesignation->typeName->nomenclaturalReference->uuid));
				}
				$out .= theme('cdm_taxonName', $typeDesignation->typeName, null, $referenceUri, $renderPath);

				//        if($typeDesignation->typeName->class == 'ZoologicalName') {
				//          // appending authorTeam which has been skipped in cdm_name
				//          $authorTeam = cdm_taggedtext_value($typeDesignation->typeName->taggedName, 'authors');
				//          $authorTeamPart = l('<span class="authors">'.$authorTeam.'</span>', "/cdm_dataportal/reference/".$typeDesignation->typeName->nomenclaturalReference->uuid, array(), NULL, NULL, FALSE, TRUE);
				//          $out .= (str_endsWith($authorTeam, ')') ? '' : ', ').$authorTeamPart;
				//        } else {
				//          $out .= ' '.theme('cdm_reference', $typeDesignation->citation, true, $referenceStyle);
				//          $out .= '</li>';
				//        }
			}
		}
	}

	if(!empty($specimenTypeDesignations)){
		// sorting might be different for dataportals so this has to be parameterized
		usort($specimenTypeDesignations, "compare_specimenTypeDesignationStatus");
		foreach($specimenTypeDesignations as $std){

			$typeReference = '';
			//show citation only for Lectotype or Neotype
			$showCitation = isset($std->typeStatus) && ($std->typeStatus->uuid == UUID_NEOTYPE || $std->typeStatus->uuid == UUID_LECTOTYPE);
			if($showCitation && !empty($std->citation)){
				$shortCitation = $std->citation->authorTeam->titleCache;
				$shortCitation .= (strlen($shortCitation) > 0 ? ' ' : '' ). partialToYear($std->citation->datePublished->start);
				if(strlen($shortCitation) == 0){
					$shortCitation = theme('cdm_reference',$std->citation );
					$missingShortCitation = true;
				}
				$typeReference .= '&nbsp;(' . t('designated by');
				$typeReference .= '&nbsp;<span class="typeReference '.($missingShortCitation ? '' : 'cluetip').' no-print" title="'. htmlspecialchars('|'.theme('cdm_reference',$std->citation ).'|') .'">';
				$typeReference .= $shortCitation.'</span>';
				$typeReference .= ')';
				//$typeReference .= '<span class="reference only-print">(designated by '.theme('cdm_reference',$std->citation ).')</span>';
			}

			$out .= '<li class="specimenTypeDesignation">';
			$out .= '<span class="status">'.(($std->typeStatus->representation_L10n) ? $std->typeStatus->representation_L10n : t('Type')) .$typeReference.'</span>: '.$std->typeSpecimen->titleCache;
			$out .= theme('cdm_specimen', $std->typeSpecimen);
			$out .= '</li>';
		}
	}

	$out .= '</ul>';

	return $out;
}

function theme_cdm_specimen($specimen){

	_add_js_thickbox();

	$out = '';
	if(isset($specimen->media[0])){

		$image_url = drupal_get_path('module', 'cdm_dataportal').'/images/external_link.gif';
		// thickbox has problems reading the first url parameter, so a litte hack is needed here:
		// adding a meaningless patameter &tb_hack=1& ....
		$out .= '&nbsp;<a href="#TB_inline?tb_hack=1&width=300&amp;height=330&amp;inlineId=specimen_media_'.$specimen->uuid.'" class="thickbox">'
		.'<img src="'.$image_url.'" title="'.t('Show media').'" /></a>';

		$out .= '<div id="specimen_media_'.$specimen->uuid.'" class="tickbox_content"><table>';

		$media_row = '<tr class="media_data">';
		$meta_row = '<tr class="meta_data">';

		foreach($specimen->media as $media){
			foreach($media->representations as $representation){

				//TODO this this is PART 2/2 of a HACK - select preferred representation by mimetype and size
				//
				if(true || $representation->mimeType == 'image/jpeg'){
					foreach($representation->parts as $part){
						// get media uri conversion rules if the module is installed and activated
						if(module_exists('cdm_mediauri')){
							$muris = cdm_mediauri_conversion($part->uri);
						}
						// --- handle media preview rules
						if(isset($muris['preview'])){

							$a_child = '<img src="'.$muris['preview']['uri'].'" class="preview" '
							.($muris['preview']['size_x'] ? 'width="'.$muris['preview']['size_x'].'"' : '')
							.($muris['preview']['size_y'] ? 'width="'.$muris['preview']['size_y'].'"' : '')
							.'/>';
						} else {
							$a_child = '<img src="'.$part->uri.'" />';
						}

						// --- handle web application rules
						$webapp = '';
						if(isset($muris['webapp'])){
							if($muris['webapp']['embed_html']){
								// embed in same page
								$webapp = $muris['webapp']['embed_html'];
							} else {
								$webapp = l(t('web application'), $muris['webapp']['uri']);
							}
						}
						$media_row .= '<td><a href="'.$part->uri.'" target="'.$part->uuid.'">'.$a_child.'</a></td>';
						$meta_row .= '<td><span class="label">'.check_plain($specimen->titleCache).'</span><div class="webapp">'.$webapp.'</div></td>';
					} // END parts
					//TODO this is PART 2/2 of a hack
					break;
				} // END representations
			} // END media
		}
		$out .= $media_row.'</tr>';
		$out .= $meta_row.'</tr>';

		$out .= '</div></table>';
	}
	return $out;
}

function theme_cdm_featureTrees($mergedTrees, $taxon){

	if(!$mergedTrees){
		return;
	}

	foreach($mergedTrees as &$mTree){
		//TODO diplay title and reference in case of multiple $mergedTrees -> theme
		$out .= theme('cdm_feature_nodes', $mTree->root->children, $taxon);
	}
	return $out;
}

function theme_cdm_featureTreeTOCs($mergedTrees){

	if(!$mergedTrees){
		return;
	}
	//FIXME
	$out = '<div class="featureTOC">';
	$out .= '<h2>' . t('Content') .'</h2>';

	//TODO diplay title and reference in case of multiple $mergedTrees -> theme

	foreach($mergedTrees as &$mTree){
		$out .= theme('cdm_feature_nodesTOC', $mTree->root->children);
	}

	$out .= '</div>';
	return $out;
}

function theme_cdm_feature_nodes($featureNodes, $taxon){

	foreach($featureNodes as $node){
		// process $descriptionElements with content only
		if(is_array($node->descriptionElements) && count($node->descriptionElements) > 0){

			$featureRepresentation = isset($node->feature->representation_L10n) ? $node->feature->representation_L10n : 'Feature';

			$block->module = 'cdm_dataportal';

			if($node->feature->uuid != UUID_IMAGE){
				$block->delta = generalizeString($featureRepresentation);
				$block->subject = theme('cdm_feature_name', $featureRepresentation);
				$block->module = "cdm_dataportal-feature";

				$block->content = theme('cdm_descriptionElements', $node->descriptionElements, $block->delta);

				// set anchor; FIXME put anchor in $block->subject
				$out .= '<a name="'.$block->delta.'"></a>';
				$out .= theme('block', $block);

				// TODO HACK
				if($node->feature->uuid == UUID_DISTRIBUTION){
					$out .= theme('cdm_descriptionElements_distribution', $taxon);
				}
			}
		}
		// theme
		if(count($node->children) > 0){
			$out .= '<div class="nested_description_elements">';
			$out .= theme('cdm_feature_nodes', $node->children, $taxon);
			$out .= '</div>';
		}
	}
	return $out;
}


function theme_cdm_feature_nodesTOC($featureNodes){

	$out .= '<ul>';

	foreach($featureNodes as $node){
		// process $descriptionElements with content only
		if(is_array($node->descriptionElements) && count($node->descriptionElements) > 0){

			$featureRepresentation = isset($node->feature->representation_L10n) ? $node->feature->representation_L10n : 'Feature';
			// HACK to implement images for taxa, should be removed
			if($node->feature->uuid != UUID_IMAGE){
				$out .= '<li>'.l(t(ucfirst($featureRepresentation)), $_GET['q'], array("class"=>"toc"), NULL, generalizeString($featureRepresentation)).'</li>';
			}
		}
	}

	$out .= '</ul>';
	return $out;
}

function theme_cdm_feature_name($feature_name){
	//TODO replace by using translations ?
	switch($feature_name){
		default: return t(ucfirst($feature_name));
	}
}

function theme_cdm_descriptionElements($descriptionElements){

	$outArray = array();
	$glue = '';
	$sortOutArray = false;
	$enclosingHtml = 'ul';

	foreach($descriptionElements as $descriptionElement){

		if($descriptionElement->feature->uuid == UUID_DISTRIBUTION){
			if($descriptionElement->class == 'Distribution'){
				$repr = $descriptionElement->area->representation_L10n;
			} else if($descriptionElement->class == 'TextData'){
				$repr = $descriptionElement->multilanguageText_L10n->text;
			}
			if( !array_search($repr, $outArray)){
				$outArray[] = $repr;
				$glue = ', ';
				$sortOutArray = true;
				$enclosingHtml = 'p';
			}
		} else if($descriptionElement->class == 'TextData'){
			$outArray[] = theme('cdm_descriptionElementTextData', $descriptionElement);
		} else {
			$outArray[] = '<li>No method for rendering unknown description class: '.$descriptionElement->classType.'</li>';
		}

	}
	// take the feature of the last $descriptionElement
	$feature = $descriptionElement->feature;
	return theme('cdm_descriptionElementArray', $outArray, $feature, $glue, $sortOutArray, $enclosingHtml);
}

function theme_cdm_descriptionElementArray($elementArray, $feature, $glue = '', $sortArray = false, $enclosingHtml = 'ul'){
	$out = '<'.$enclosingHtml.' class="description" id="'.$feature->representation_L10n.'">';

	if($sortArray) sort($elementArray);

	$out .= join($elementArray, $glue);

	$out .= '</'.$enclosingHtml.'>';
	return $out;
}

function theme_cdm_descriptionElementTextData($element){

	$description = str_replace("\n", "<br/>", $element->multilanguageText_L10n->text);
	$sourceRefs = '';
	foreach($element->sources as $source){
		$referenceCitation = '';

		if($source->citation){
			$authorTeam = $source->citation->authorTeam->teamMembers;
			if (count($authorTeam) > 2){
				$authorA = $authorTeam[0]->lastname;
				$authorA .= " et al.";
			}
			elseif (count($authorTeam = 2)){
				$authorA = $authorTeam[0] -> lastname . " & " . $authorTeam[1] ->lastname;
			}
			else
			$authorA = $authorTeam[0]-> lastname;


			//$authorTeam = $source->citation->authorTeam->titleCache;

			$referenceCitation = l('<span class="reference">'.$authorA.'</span>', path_to_reference($source->citation->uuid), array("class"=>"reference"), NULL, NULL, FALSE ,TRUE);
			if($source->citationMicroReference){
				$referenceCitation .= ': '. $source->citationMicroReference;
			}
			if($description && strlen($description) > 0 ){
				$sourceRefs .= '; '.$referenceCitation;
			}
		}
	}

	/*
	 if (count($authorTeam) > 2){
		$authorA = $authorTeam[0];
		$authorA .= "et al.";
		}
		elseif (count($authorTeam = 2)){
		$authorA = $authorTeam[0] . " & " . $authorTeam[1];
		}
		else
		$authorA = $authorTeam[0];

		$referenceCitation = l('<span class="reference">'.$authorA.'</span>', path_to_reference($source->citation->uuid), array("class"=>"reference"), NULL, NULL, FALSE ,TRUE);
		if($source->citationMicroReference){
		$referenceCitation .= ': '. $source->citationMicroReference;
		}
		if($description && strlen($description) > 0 ){
		$sourceRefs .= '; '.$referenceCitation;
		}
		}
		*/
	if(strlen($sourceRefs) > 0){
		$sourceRefs = '<span class="sources">' . $sourceRefs . '</span>';
	}
	return '<li class="descriptionText">' . $description . $sourceRefs. '</li>';
}

function theme_cdm_search_results($pager, $path, $parameters){


	$showThumbnails = $_SESSION['pageoptions']['searchtaxa']['showThumbnails'];
	if( !is_numeric($showThumbnails)){
		$showThumbnails = 1;
	}
	$setSessionUri = url('cdm_api/setvalue/session').'/pageoptions|searchtaxa|showThumbnails/';
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

	drupal_set_title(t('Search Results'));

	$out = ''; //l('Advanced Search', '/cdm_dataportal/search');

	$out = '<div class="page_options"><form name="pageoptions"><input id="showThumbnails" type="checkbox" name="showThumbnails" '.($showThumbnails == 1? 'checked="checked"': '').'> '.t('Show Thumbnails').'</form></div>';
	if(count($pager->records) > 0){
		$out .= theme('cdm_list_of_taxa', $pager->records);
		$out .= theme('cdm_pager', $pager, $path, $parameters);
	} else {
		$out = '<h4 class="error">Sorry, no matching entries found.</h4>';
	}
	return $out;
}


function theme_cdm_pager(&$pager, $path, $parameters){
	$out = '';

	if ($pager->pagesAvailable > 1) {

		$out .= '<div class="pager">';
		if($pager->currentIndex > 0){
			$out .= theme('cdm_pager_link', t('« first'), 0,  $pager, $path, $parameters, array('class' => 'pager-first'));
			$out .= theme('cdm_pager_link', t('‹ previous'), $pager->currentIndex - 1, $pager, $path, $parameters, array('class' => 'pager-previous'));
		}

		if($pager->indices[0] > 0){
			$out .= '<div class="pager-list-dots-left">...</div>';
		}

		foreach($pager->indices as $index){
			$label = $index + 1;
			$out .= theme('cdm_pager_link', $label, $index,  $pager, $path, $parameters, array('class' => 'pager-first'));
		}
		if($pager->indices[count($pager->indices) - 1] < $pager->pagesAvailable - 1){
			$out .= '<div class="pager-list-dots-right">...</div>';
		}

		if($pager->nextIndex){
			$out .= theme('cdm_pager_link', t('next ›'), $pager->nextIndex, $pager, $path, $parameters, array('class' => 'pager-next'));
			$out .= theme('cdm_pager_link', t('last »'), $pager->pagesAvailable - 1, $pager, $path, $parameters, array('class' => 'pager-last'));
		}
		$out .= '</div>';

		return $out;
	}
}

function theme_cdm_pager_link($text, $linkIndex, &$pager, $path, $parameters = array(), $attributes) {

	$out = '';
	$parameters['search']['page'] = $linkIndex;
	if ($linkIndex == $pager->currentIndex) {
		$out = '<strong>'.$text.'</strong>';
	} else {
		$queryString = drupal_query_string_encode($parameters);
		$out = l($text, $path, $attributes, $queryString);
	}
	return $out;
}
