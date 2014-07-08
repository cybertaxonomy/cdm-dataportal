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
 *    array of CDM Rights entities
 */
function cdm_rights_as_dl_groups($rights_list) {

  $copyrights = array();
  $licenses = array();
  $access_rights = array();
  $unknown = array();

  foreach ($rights_list as $right) {
    if (!is_object($right)){
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
          } else {
            $license_str = l('link', $right->uri);
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
        $unknown_groups[] =  $right->text . $right->uuid; //TODO
    }
  }

  $groups = array();
  if(count($copyrights) > 0) {
    _description_list_group_add($groups, t('Copyright'), $copyrights);
  }
  if(count($licenses) > 0) {
    _description_list_group_add($groups, t('Licenses'), $licenses);
  }
  if(count($access_rights) > 0) {
    _description_list_group_add($groups, t('Access rights'), $access_rights);
  }
  if(count($unknown) > 0) {
    _description_list_group_add($groups, t('Rights (untyped)'), $unknown);
  }

  return $groups;

}