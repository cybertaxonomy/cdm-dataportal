<?php
/**
 * @file
 * Functions for dealing with CDM entities of type SpeciemenOrOccurrences
 *
 * @copyright
 *   (C) 2007-2012 EDIT
 *   European Distributed Institute of Taxonomy
 *   http://www.e-taxonomy.eu
 *
 *   The contents of this module are subject to the Mozilla
 *   Public License Version 1.1.
 * @see http://www.mozilla.org/MPL/MPL-1.1.html
 *
 * @author
 *   - Andreas Kohlbecker <a.kohlbecker@BGBM.org>
 */

/**
 * Returns an array of render array entries for a HTML description list.
 *
 * @see theme_description_list()
 *
 * @param array $rights_list
 *   array of CDM Rights entities
 *
 * @return array
 *   the render array of the groups for the HTML description list
 */
function cdm_rights_as_dl_groups($rights_list) {
  $copyrights = array();

  $licenses = array();
  $access_rights = array();
  $unknown = array();

  foreach ($rights_list as $right) {
    if (!is_object($right)) {
      continue;
    }
    $type_uuid = isset($right->type->uuid) ? $right->type->uuid : 'UNKNOWN';
    switch ($type_uuid) {

      case UUID_RIGHTS_COPYRIGHT:
        $text = '';
        if (isset($right->text) ){
          // sanitize potential '(c)' away
          $text  = preg_replace('/^\(c\)/', '', $right->text);
        } if (isset($right->agent) ){
          $text  .= (strlen($text) > 0 ? ', ' : '') . $right->agent;
        }

        $copyrights[] = array('#markup' => '&copy; ' . $text);
        break;

      case UUID_RIGHTS_LICENCE:
        $license_str = '';
        if (isset($right->abbreviatedText)) {
          $license_str .= $right->abbreviatedText;
        }
        if (isset($right->uri)) {
          if (strlen($license_str) > 0) {
            $license_str = l($license_str, $right->uri);
          }
          else {
            $license_str = l(t('link'), $right->uri);
          }
        }
        if (strlen($license_str) > 0 && isset($right->text)) {
          $license_str .= ': ' . $right->text;
        }
        $licenses[] = array('#markup' => $license_str);
        break;

      case UUID_RIGHTS_ACCESS_RIGHTS:
        $access_rights[] = array('#markup' => $right->text);
        break;

      default:
        $unknown_groups[] = array('#markup' => $right->text);
    }
  }

  $groups = array();
  if (count($copyrights) > 0) {
    _description_list_group_add($groups, t('Copyright') . ':', $copyrights);
  }
  if (count($licenses) > 0) {
    _description_list_group_add($groups, t('Licenses') . ':', $licenses);
  }
  if (count($access_rights) > 0) {
    _description_list_group_add($groups, t('Access rights') . ':', $access_rights);
  }
  if (count($unknown) > 0) {
    _description_list_group_add($groups, t('Rights') . ':', $unknown);
  }

  return $groups;

}


/**
 * Provides the markup for an icon to represent a media which is associated with the given $feature.
 *
 * @param $feature
 *   the cdm Feature term
 * @param $media_url
 *   Optional, currently unused. May be used in future to display different
 *   icons for different media urls, like the fav-icon of the referenced
 * @return string
 *   The markup for the icon
 */
function media_feature_icon($feature, $media_url = NULL) {
  return font_awesome_icon_markup('fa-book', array('alt' => $feature->representation_L10n));
}

/**
 * Gets the metadata info such as title or artist and source references of a media file.
 *
 * The function tries at first to get all the info from the file metadata
 * and if it is not available look at the media file info stored at the database.
 *
 * @param mixed $media
 *   The media file object for which to get the metadata.
 *
 * @return array
 *   The array with the available specified metadata info.
 */
function read_media_metadata($media) {

  $metadata = array(
    'title' => '',// Media_metadata and media.
    'artist' => '',// Media_metadata and media.
    'rights',// Media_metadata and media.
    'location',// Media_metadata.
    'filename' => '',// Media.
    'mediacreated' => '', // Media.
    'description' => ''
  );

  // Getting the media metadata.
  $media_metadata = cdm_ws_get(CDM_WS_MEDIA_METADATA, array($media->uuid));
  $media_metadata_aux = (array) $media_metadata;


  // Filename.
  if (!empty($media->representations[0]->parts[0]->uri)) {
    $fileUri = $media->representations[0]->parts[0]->uri;
    $filename = substr($fileUri, strrpos($fileUri, "/") + 1);
    $metadata['filename'] = $filename;
  }
  else {
    $metadata['filename'] = '';
  }

  // Title.
  if (!empty($media_metadata->ObjectName)) {
    $metadata['title'] = $media_metadata->ObjectName;
  }
  elseif (!empty($media_metadata_aux['Object Name'])) {
    $metadata['title'] = $media_metadata_aux['Object Name'];
  }
  elseif (!empty($media->title_L10n)) {
    $metadata['title'] = $media->title_L10n;
  }
  elseif (!empty($media->titleCache)) {
    $metadata['title'] = $media->titleCache;
  }

  // Append description to title.
  if (!empty($media->description_L10n)) {
    $metadata['title'] .= '<span class="media-description">' . $media->description_L10n . '<span>';
  }

  // Artist.
  if (!empty($media_metadata->Artist)) {
    $metadata['artist'] = '' . $media_metadata->Artist;
  }
  elseif (!empty($media->artist->titleCache)) {
    $metadata['artist'] = $media->artist->titleCache;
  }

  // Rights
  $metadata['rights'] = array();
  if (!empty($media_metadata->Copyright)) {
    $rightsObj =  new stdClass();
    $rightsObj->type = new stdClass();
    $rightsObj->type->uuid = UUID_RIGHTS_COPYRIGHT;
    $rightsObj->type->titleCache = 'copyright';
    $rightsObj->type->representation_L10n = 'copyright';
    $rightsObj->type->text = $media_metadata->Copyright;
    $metadata['rights'][] = $rightsObj;
  }
  if(isset($media->rights) && is_array($media->rights)) {
    $metadata['rights'] = array_merge($metadata['rights'], $media->rights);
  }

  // Filling the description (though there is no description in the db???).
  // $metadata_caption['description'] = $media->description_L10n;

  // Location.
  $metadata['location'] = array();
  $metadata['location']['sublocation'] = !empty($media_metadata->Sublocation) ? $media_metadata->Sublocation : FALSE;
  $metadata['location']['city'] = !empty($media_metadata->City) ? $media_metadata->City : FALSE;
  $metadata['location']['province'] = !empty($media_metadata->Province) ? $media_metadata->Province : FALSE;
  $metadata['location']['country'] = !empty($media_metadata->Country)? $media_metadata->Country : FALSE;

  /*
   // Creation date.
   if($media_metadata["Modify Date"])
   $metadata_caption['mediacreated'] = $media_metadata["Modify Date"];
   else
   $metadata_caption['mediacreated'] = $media->created;
   */

  return $metadata;
}


/**
 * Return HTML for a media gallery
 *
 * @param array $configuration
 *   An associative array containing:
 *   - mediaList: An array of Media entities.
 *   - maxExtend
 *   - cols
 *   - maxRows
 *   - showCaption:  boolean value, whether to show captions or not.
 *   - captionElements: An array of caption elements to be shown. In case the array
 *        is empty of NULL  all available caption elements will be show. In order to
 *        supress all captions  set 'showCaption' to FALSE
 *        example:
 *          1) Show 'title', 'description', 'file', 'filename' in the caption:
 *            array('title', 'description', 'file', 'filename'),
 *          2) To add an addtional link at the bottom of  the caption:
 *            array('titlecache', '#uri'=>t('Open Image')) this will cause a link
 *            to be rendered with label 'Open image' which will open the according
 *            media object.
 *   - mediaLinkType: Valid values:
 *      - "NONE": do not link the images,
 *      - "LIGHTBOX": open the link in a light box,
 *      - "NORMAL": link to the image page or to the $alternativeMediaUri if
 *        it is defined.
 *   - alternativeMediaUri: A fix string or an array of alternative URIs to link the images
 *     which will overwrite the URIs of the media parts.
 *     The order of URI in this array must correspond with the order of
 *     images in $mediaList.
 *   - galleryLinkUri: An URI to link the the hint on more images to;
 *     if NULL no link is created.
 *
 * @return array
 *   A render array for the image gallery
 * @ingroup: compose
 */
function compose_cdm_media_gallerie($configuration) {

  $mediaList = $configuration['mediaList'];

  // Do not show an empty gallery.
  if (count($mediaList) == 0) {
    return '';
  }

  // merge with default
  $configuration = array_merge(
    array(
      'mediaList' => NULL,
      'galleryName' => NULL,
      'maxExtend' => 150,
      'cols' => 4,
      'maxRows' => FALSE,
      'captionElements' => array('title'),
      'mediaLinkType' => 'LIGHTBOX',
      'alternativeMediaUri' => NULL,
      'galleryLinkUri' => NULL,
      'showCaption' => TRUE,
    ),
    $configuration);

  $galleryName = $configuration['galleryName'];
  $maxExtend = $configuration['maxExtend'];
  $cols = $configuration['cols'];
  $maxRows = $configuration['maxRows'];
  $captionElements = $configuration['captionElements'];
  $mediaLinkType = $configuration['mediaLinkType'];
  $alternativeMediaUri = $configuration['alternativeMediaUri'];
  $galleryLinkUri = $configuration['galleryLinkUri'];
  $showCaption = $configuration['showCaption'];

  $caption_link_uri = '';
  if(isset($captionElements['#uri'])){
    $caption_link_uri = $captionElements['#uri'];
    unset($captionElements['#uri']);
  }
  if (!is_array($captionElements) || count($captionElements) == 0) {
    $captionElements = NULL;
  }

  // TODO correctly handle multiple media representation parts
  $_SESSION['cdm']['last_gallery'] = current_path();
  // Prevent from errors.
  if (!isset($mediaList[0])) {
    // return;
  }

  // --- Duplicate supression: media can be reused but should only be shown
  // once.
  $tempMediaList = array();
  $tempMediaUuids = array();
  foreach ($mediaList as $media) {
    if (!in_array($media->uuid, $tempMediaUuids)) {
      $tempMediaList[] = $media;
      $tempMediaUuids[] = $media->uuid;
    }
  }
  $mediaList = $tempMediaList;

  // ---
  $galleryID = "media_gallery_" . $galleryName;

  $mediaPartLinkAttributes = array();
  $openMediaLinkAttributes = array();

  // Prepare media links.
  $doLink = FALSE;
  if ($mediaLinkType != 'NONE') {
    $doLink = TRUE;
  }
  if ($mediaLinkType == 'LIGHTBOX') {
    $doLink = TRUE;
    _add_js_lightbox($galleryID);
  }

  // Render the media gallery grid.
  $out = '<table id="' . $galleryID . '" class="media_gallery">';
  $out .= '<colgroup>';
  for ($c = 0; $c < $cols; $c++) {
    $out .= '<col style="width:' . (100 / $cols) . '%;">';
  }
  $out .= '</colgroup>';

  for ($r = 0; ($r < $maxRows || !$maxRows) && count($mediaList) > 0; $r++) {
    $captionParts = array();
    $mediaIndex = 0;
    $out .= '<tr>';
    for ($c = 0; $c < $cols; $c++) {
      $media = array_shift($mediaList);

      if (isset($media->representations[0]->parts[0])) {

        //
        // Find preferred representation.
        //
        $preferred_media_representations_list = cdm_preferred_media_representations($media, array(
          'image/jpg',
          'image/jpeg',
          'image/png',
          'image/gif',
        ), $maxExtend, $maxExtend);
        if (count($preferred_media_representations_list) == 0) {
          // Fallback to using the first one in the list.
          $preferred_media_representations_list = $media->representations;
        }
        $preferred_media_representation = array_shift($preferred_media_representations_list);

        // $preferred_media_representation->parts[0]->uri =
        // "http://127.0.0.1/images/palmae/palm_tc_14415_1.jpg";
        $contentTypeDirectory = media_content_type_dir($preferred_media_representation, 'application');

        $mediaPartHtml = call_user_func_array(
          'cdm_media_gallerie_' . $contentTypeDirectory,
          array( $preferred_media_representation->parts[0], $maxExtend, TRUE)
        );
        // --- Compose Media Link.
        $mediaLinkUri = FALSE;
        if ($alternativeMediaUri) {
          if (isset($alternativeMediaUri[$mediaIndex])) {
            $mediaLinkUri = $alternativeMediaUri[$mediaIndex];
          }
          if (is_string($alternativeMediaUri)) {
            $mediaLinkUri = $alternativeMediaUri;
          }
        }
        else {
          $mediaLinkUri = $preferred_media_representation->parts[0]->uri;
        }
        $mediaIndex++;

        // media captions will be loaded via AHAH
        _add_js_ahah();
        $content_url = cdm_compose_url(CDM_WS_PORTAL_MEDIA, $media->uuid);
        $cdm_proxy_url_caption = url('cdm_api/proxy/' . urlencode($content_url) . "/cdm_media_caption/" . serialize($captionElements));
        $ahah_media_caption =  '<div class="ahah-content" data-cdm-ahah-url="' . $cdm_proxy_url_caption . '">'
          . '<span class="loading" style="display: none;">' . loading_image_html() . '</span></div>';

        // preparing the part link (= click on image iteself) which can be handled in two ways
        //
        //  1. open image in lightbox, the captions in the lightbox will be loaded via AHAH
        //  2. open the media in a new window with target 'specimen'
        if ($mediaLinkType == 'LIGHTBOX' && $contentTypeDirectory == 'image') {
          $mediaPartLinkAttributes['class'] = array('lightbox');
        }
        else {
          $mediaPartLinkAttributes['target'] = "specimen";
          $openMediaLinkAttributes['target'] = "specimen";
        }
        $mediaPartLinkAttributes['alt'] = htmlentities($ahah_media_caption);

        // --- preparing the media caption

        /* old comment: "no caption elements to show up here except the $caption_link_uri, if at all"
         *
         * a.kohlbecker 2013-03-14 :
         *   It is unclear why no caption elements should be shown, Was it a technical reason?
         *   see commit r16723 740177eb-a1d8-4ec3-a630-accd905eb3da
         *   If not problems arise with this remove it after some weeks
         */
        $captionPartHtml = $ahah_media_caption;

        if ($caption_link_uri) {
          if ($contentTypeDirectory == 'image') {
            // it is an image, so open it in the media page
            $captionPartHtml .= '<div class="media-caption-link">' . l($caption_link_uri, path_to_media($media->uuid), array(
                'attributes' => array(), 'html' => TRUE,
              )) . '</div>';
          }
          else {
            // otherwise open it directly and let the the browser handle the media type
            $openMediaLinkAttributes['absolute'] = TRUE;
            $captionPartHtml .= '<div class="media-caption-link">' . l($caption_link_uri, $mediaLinkUri, array(
                'attributes' => $openMediaLinkAttributes, 'html' => TRUE,
              )) . '</div>';
          }
        }

        $captionParts[] = $captionPartHtml;

        // --- Surround imagePart with link, this .
        if ($doLink) {
          $mediaPartHtml = l($mediaPartHtml, $mediaLinkUri, array(
            'attributes' => $mediaPartLinkAttributes, 'html' => TRUE,
          ));
        }
      }
      else {
        $mediaPartHtml = '';
        $captionParts[] = '';
      }
      $out .= '<td class="media">' . $mediaPartHtml . '</td>';
    }
    $out .= '</tr>'; // End of media parts.
    if ($showCaption) {
      if ( (is_array($captionElements) && count($captionElements) > 0) || $caption_link_uri) {
        $out .= '<tr>';
        // Add caption row.
        foreach ($captionParts as $captionPartHtml) {
          $out .= '<td class="caption">' . $captionPartHtml . '</td>';
        }
        $out .= '</tr>';
      }
    }
  }

  if ($galleryLinkUri) {
    if (count($mediaList) > 0) {
      $moreHtml = count($mediaList) . ' ' . t('more in gallery');
    }
    else {
      $moreHtml = t('open gallery');
    }
    $moreHtml = l($moreHtml, $galleryLinkUri);
    $out .= '<tr><td colspan="' . $cols . '">' . $moreHtml . '</td></tr>';
  }
  $out .= '</table>';

  return $out;
}

/**
 * Creates markup for a CDM MediaRepresentation which is referencing an image.
 *
 * @param $mediaRepresentationPart
 * @param $maxExtend
 * @param $addPassePartout
 * @param $attributes
 * @return string
 *   The markup for the media representation
 */
function cdm_media_gallerie_image($mediaRepresentationPart, $maxExtend, $addPassePartout, $attributes = array()) {

  $out = '';

  // TODO merge with theme_cdm_media_mime_image?
  if (isset($mediaRepresentationPart)) {

    $h = $mediaRepresentationPart->height;
    $w = $mediaRepresentationPart->width;
    if ($w == 0 || $h == 0) {
      // Take url and replace spaces.
      $image_uri = str_replace(' ', '%20', $mediaRepresentationPart->uri);
      $imageDimensions = getimagesize_remote($image_uri);
      if (!$imageDimensions) {
        return '<div>' . t('Image unavailable, uri: ') . $mediaRepresentationPart->uri . '</div>';
      }
      $w = $imageDimensions[0];
      $h = $imageDimensions[1];
    }

    $ratio = $w / $h;
    if ($ratio > 1) {
      $displayHeight = round($maxExtend / $ratio);
      $displayWidth = $maxExtend;
      $m = round(($maxExtend - $displayHeight) / 2);
      $margins = 'margin:' . $m . 'px 0 ' . $m . 'px 0;';
    }
    else {
      $displayHeight = $maxExtend;
      $displayWidth = round($maxExtend * $ratio);
      $m = round(($maxExtend - $displayWidth) / 2);
      $margins = 'margin:0 ' . $m . 'px 0 ' . $m . 'px;';
    }

    // Turn attributes array into string.
    if(!is_array($attributes)){
      $attributes = array();
    }
    if(!isset($attributes['alt'])){
      $attributes['alt'] = check_plain($mediaRepresentationPart->uri);
    }
    $attrStr = ' ';
    // $attributes['title'] = 'h:'.$h.', w:'.$w.',ratio:'.$ratio;
    if (is_array($attributes)) {
      foreach ($attributes as $name => $value) {
        $attrStr .= $name . '="' . $value . '" ';
      }
    }

    if ($addPassePartout) {
      $out .= '<div class="image-passe-partout" style="width:' . $maxExtend . 'px; height:' . $maxExtend . 'px;">';
    }
    else {
      // Do not add margins if no pass partout is shown.
      $margins = '';
    }
    $out .= '<img src="' . $mediaRepresentationPart->uri . '" width="' . $displayWidth . '" height="' . $displayHeight . '" style="' . $margins . '"' . $attrStr . ' />';

    if ($addPassePartout) {
      $out .= '</div>';
    }
  }

  return $out;
}

/**
 * Creates markup for a CDM MediaRepresentation which is referencing an web application.
 *
 * @param $mediaRepresentationPart
 * @param $maxExtend
 * @param $addPassePartout
 * @param $attributes
 * @return string
 *   The markup for the media representation
 */
function cdm_media_gallerie_application($mediaRepresentationPart, $maxExtend, $addPassePartout, $attributes = array()) {

  $out = '';

  if (isset($mediaRepresentationPart)) {

    if ($addPassePartout) {
      $out .= '<div class="image-passe-partout" style="width:' . $maxExtend . 'px; height:' . $maxExtend . 'px;">';
    }
    $out .= '<div class="application">Web Application</div>';

    if ($addPassePartout) {
      $out .= '</div>';
    }
  }

  return $out;
}

/**
 * Creates markup for a CDM MediaRepresentation which is referencing an web application.
 *
 * @param $mediaRepresentationPart
 * @param $maxExtend
 * @param $addPassePartout
 * @param $attributes
 * @return string
 *   The markup for the media representation
 */
function cdm_media_gallerie_text($mediaRepresentationPart, $maxExtend, $addPassePartout, $attributes = array()) {

  $out = '';

  if (isset($mediaRepresentationPart)) {
    if ($addPassePartout) {
      $out .= '<div class="image-passe-partout" style="width:' . $maxExtend . 'px; height:' . $maxExtend . 'px;">';
    }

    $out .= '<div class="application">Web Application</div>';

    if ($addPassePartout) {
      $out .= '</div>';
    }
  }

  return $out;
}

/**
 * Adds the OpenLayers based image viewer to the page.
 *
 * The OpenLayers based image viewer allows to zoom and pan the displayed image.
 *
 * Documentation related to using Openlayers in order to view images is found here:
 *  - @see http://trac.openlayers.org/wiki/UsingCustomTiles#UsingTilesWithoutaProjection
 *  - @see http://trac.openlayers.org/wiki/SettingZoomLevels
 *
 * @param array $variables
 *   An associative array of theme variables:
 *   - mediaRepresentationPart: The CDM MediaRepresentationPart instance to be displayed.
 *   - maxExtend: The maximum extend of the image viewer view port.
 */
function cdm_openlayers_image($mediaRepresentationPart, $maxExtend) {

  _add_js_openlayers();

  // TODO merge code below with code from theme_cdm_media_gallerie_image
  // var_dump("MEDIA URI: " . $mediaRepresentationPart->uri);
  // TODO merge code below with code from theme_cdm_media_gallerie_image
  $w = $mediaRepresentationPart->width;
  $h = $mediaRepresentationPart->height;

  if ($w == 0 || $h == 0) {
    // Take url and replace spaces.
    $image_uri = str_replace(' ', '%20', $mediaRepresentationPart->uri);
    $imageDimensions = getimagesize_remote($image_uri);
    if (!$imageDimensions) {
      return '<div>' . t('Image unavailable, uri:') . $mediaRepresentationPart->uri . '</div>';
    }
    $w = $imageDimensions[0];
    $h = $imageDimensions[1];
  }

  // Calculate maxResolution
  if ($w > $h) {
    $maxRes = $w / $maxExtend;
  }
  else {
    $maxRes = $h / $maxExtend;
  }

  $maxRes *= 1;

  drupal_add_js('
 var map;

 var imageLayerOptions={
     maxResolution: ' . $maxRes . ',
     maxExtent: new OpenLayers.Bounds(0, 0, ' . $w . ', ' . $h . ')
  };
  var mapOptions={
      controls:
       [
         new OpenLayers.Control.PanZoom(),
         new OpenLayers.Control.Navigation({zoomWheelEnabled: false, handleRightClicks:true, zoomBoxKeyMask: OpenLayers.Handler.MOD_CTRL})
       ],
     restrictedExtent:  new OpenLayers.Bounds(0, 0, ' . $w . ', ' . $h . ')
  };

 var graphic = new OpenLayers.Layer.Image(
          \'Image Title\',
          \'' . $mediaRepresentationPart->uri . '\',
          new OpenLayers.Bounds(0, 0, ' . $w . ', ' . $h . '),
          new OpenLayers.Size(' . $w . ', ' . $h . '),
          imageLayerOptions
          );

 function init() {
   map = new OpenLayers.Map(\'openlayers_image\', mapOptions);
   map.addLayers([graphic]);
   map.setCenter(new OpenLayers.LonLat(0, 0), 1);
   map.zoomToMaxExtent();
 }

jQuery(document).ready(function(){
  init();
});', array('type' => 'inline'));
  $out = '<div id="openlayers_image" class="image_viewer" style="width: ' . $maxExtend . 'px; height:' . ($maxExtend) . 'px"></div>';
  return $out;
}
