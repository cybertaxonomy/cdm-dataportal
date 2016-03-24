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
        if (isset($right->agent[0]) ){
          $agent_names = array();
          foreach ($right->agent as $agent) {
            $agent_names[] = $agent->titleCache;
          }
          $copyrights[] = implode(', ', $agent_names);
        }
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
        $licenses[] = $license_str;
        break;

      case UUID_RIGHTS_ACCESS_RIGHTS:
        $access_rights[] = $right->text . $right->uuid;
        break;

      default:
        $unknown_groups[] = $right->text . $right->uuid; // TODO !
    }
  }

  $groups = array();
  if (count($copyrights) > 0) {
    _description_list_group_add($groups, t('Copyright'), $copyrights);
  }
  if (count($licenses) > 0) {
    _description_list_group_add($groups, t('Licenses'), $licenses);
  }
  if (count($access_rights) > 0) {
    _description_list_group_add($groups, t('Access rights'), $access_rights);
  }
  if (count($unknown) > 0) {
    _description_list_group_add($groups, t('Rights (untyped)'), $unknown);
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
 * Gets the metadata info such as title or artist of a media file.
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

  $metadata_caption = array(
    'title' => '',// Media_metadata and media.
    'artist' => '',// Media_metadata and media.
    'rights',// Media_metadata and media.
    'location',// Media_metadata.
    'filename' => '',// Media.
    'mediacreated' => '',// Media.
    'description' => '',
  );// Media.

  // Getting the media metadata.
  $media_metadata = cdm_ws_get(CDM_WS_MEDIA_METADATA, array($media->uuid));
  $media_metadata_aux = (array) $media_metadata;

  // Filename.
  if (!empty($media->representations[0]->parts[0]->uri)) {
    $fileUri = $media->representations[0]->parts[0]->uri;
    $filename = substr($fileUri, strrpos($fileUri, "/") + 1);
    $metadata_caption['filename'] = $filename;
  }
  else {
    $metadata_caption['filename'] = '';
  }

  // Title.
  if (!empty($media_metadata->ObjectName)) {
    $metadata_caption['title'] = $media_metadata->ObjectName;
  }
  elseif (!empty($media_metadata_aux['Object Name'])) {
    $metadata_caption['title'] = $media_metadata_aux['Object Name'];
  }
  elseif (!empty($media->title_L10n)) {
    $metadata_caption['title'] = $media->title_L10n;
  }
  elseif (!empty($media->titleCache)) {
    $metadata_caption['title'] = $media->titleCache;
  }

  // Append description to title.
  if (!empty($media->description_L10n)) {
    $metadata_caption['title'] .= '<span class="media-description">' . $media->description_L10n . '<span>';
  }

  // Artist.
  if (!empty($media_metadata->Artist)) {
    $metadata_caption['artist'] = '' . $media_metadata->Artist;
  }
  elseif (!empty($media->artist->titleCache)) {
    $metadata_caption['artist'] = $media->artist->titleCache;
  }

  // Copyright.
  $metadata_caption['rights'] = array(
    'copyright' => array('agentNames' => array()),
    'license' => array(
      'agentNames' => array(),
      'types' => array(),
      'abbreviatedTexts' => array(),
      'uris' => array(),
    ),
  );
  if (!empty($media_metadata->Copyright)) {
    $metadata_caption['rights']['copyright']['agentNames'][] = $media_metadata->Copyright;
  }
  elseif (isset($media->rights) && is_array($media->rights)) {
    foreach ($media->rights as $right) {
      if(isset($right->term)){
        switch ($right->type->uuid) {
          case UUID_RIGHTS_LICENCE:
            $metadata_caption['rights']['license']['agentNames'][] = ($right->agent ? '' . $right->agent->firstname . ' ' . $right->agent->lastname : '');
            $metadata_caption['rights']['license']['types'][] = ($right->representation_L10n ? '' . $right->representation_L10n : '');
            $metadata_caption['rights']['license']['abbreviatedTexts'][] = ($right->abbreviatedText ? '' . $right->abbreviatedText : '');
            $metadata_caption['rights']['license']['uris'][] = ($right->uri ? '' . $right->uri : '');
            break;
          case UUID_RIGHTS_COPYRIGHT:
            $metadata_caption['rights']['copyright']['agentNames'][] = $right->agent->firstname . ' ' . $right->agent->lastname;
            break;
        }
      }
    }
  }
  else {
    $metadata_caption['rights']['agentNames'][] = '';
  }

  // Filling the description (though there is no description in the db???).
  // $metadata_caption['description'] = $media->description_L10n;

  // Location.
  $metadata_caption['location'] = array();
  $metadata_caption['location']['sublocation'] = !empty($media_metadata->Sublocation) ? $media_metadata->Sublocation : FALSE;
  $metadata_caption['location']['city'] = !empty($media_metadata->City) ? $media_metadata->City : FALSE;
  $metadata_caption['location']['province'] = !empty($media_metadata->Province) ? $media_metadata->Province : FALSE;
  $metadata_caption['location']['country'] = !empty($media_metadata->Country)? $media_metadata->Country : FALSE;

  /*
   // Creation date.
   if($media_metadata["Modify Date"])
   $metadata_caption['mediacreated'] = $media_metadata["Modify Date"];
   else
   $metadata_caption['mediacreated'] = $media->created;
   */

  // Returned value.
  return $metadata_caption;
}
