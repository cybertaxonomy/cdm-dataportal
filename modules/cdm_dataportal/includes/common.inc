<?php
/**
 * @file
 * Functions for dealing with CDM entities from the package model.common
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
 * @defgroup compose Compose functions
 * @{
 * Functions which are composing Drupal render arays
 *
 * The cdm_dataporal module needs to compose rather complex render arrays from
 * the data returned by the CDM REST service. The compose functions are
 * responsible for creating the render arrays.
 *
 * All these functions are also implementations of the compose_hook()
 * which is used in the proxy_content() function.
 * @}
 */

/**
 * Compose an render array from a CDM Marker object.
 *
 * compose_hook() implementation
 *
 * @param object $marker
 *   CDM instance of type Marker
 * @return array
 *   A drupal render array
 *
 * @ingroup compose
 */
function compose_cdm_marker($marker) {

  $render_array = array(
      // ---- generic
      //  these entries should be common to all cdm enitiy render arrays
      '#theme' => 'cdm_marker', // TODO   add alternative theme funcitons: 'cdm_marker_' . marker.type.label
      '#attributes' => array('class' => html_class_attribute_ref($marker)),

      // ---- individual
      '#label' => $marker->markerType->representation_L10n . ': ' . (($marker->flag !== TRUE ? t('yes') : t('no'))),
  );

  return $render_array;
}

/**
 * Checks if the given $cdm_entitiy has a marker the type references by the
 * $marker_type_uuid and returns TRUE if a matching marker has been found.
 *
 * @param object $cdm_entitiy A CDM Entity
 * @param string $marker_type_uuid
 */
function cdm_entity_has_marker($cdm_entitiy, $marker_type_uuid) {
  if(isset($cdm_entitiy->markers[0]) && !is_uuid($marker_type_uuid)){
    foreach ($cdm_entitiy->markers as $marker) {
      if(isset($marker->markerType) && $marker->markerType->uuid == $marker_type_uuid){
        return TRUE;
      }
    }
  }
  return FALSE;
}

/**
 * Sorts an array of CDM IdentifiableSource instances by 1. by the
 * author teams family names and 2. by the publication date.
 *
 * @param array $sources
 *    The array of CDM IdentifiableSource instances
 * @param bool $do_theme if set TRUE the sources will be themed
 *        by theme_cdm_OriginalSource
 * @return multitype:
 */
function oder_sources($sources, $do_theme = false){
    $sort_array = array();
    foreach ($sources as $source) {

      $order_key = '';

      // find the familynames
      if(isset($source->citation->uuid) && !isset($source->citation->authorship)){
        $authorteam = cdm_ws_get(CDM_WS_REFERENCE_AUTHORTEAM, $source->citation->uuid);

        $persons = array();
        if($authorteam->class == 'Team'){
          if(isset($authorteam->teamMembers)){
            $persons = $authorteam->teamMembers;
          }
        } else {
          $persons[] = $authorteam;
        }

        foreach($persons as $person){
          if(!empty($person->lastname)){
            $order_key .= $person->lastname;
          } else {
            $order_key .= $person->titleCache;
          }
        }
        if(empty($order_key)){
          $order_key = $authorteam->titleCache;
        }

      }
      $order_key = str_pad($order_key, 50);

      // add publication date to the key
      if(isset($source->citation->datePublished)){
        $order_key .= '_' . timePeriodAsOrderKey($source->citation->datePublished);
      } else {
        $order_key .= '_' . "0000";
      }

      // padd key until unique
      while(array_key_exists($order_key, $sort_array)){
        $order_key .= "_";
      }


      if($do_theme) {
        $sort_array[$order_key] = theme('cdm_OriginalSource', array('source' => $source));
      } else {
        $sort_array[$order_key] = $source;
      }
    }
    ksort($sort_array);
    return array_values ($sort_array);
}

/**
 * Compare callback to be used in usort to sort image sources of CDM OriginalSource instances.
 *
 * TODO the compare strategy implemented in oder_sources() is probably better but is not taking the
 * originalName into account.
 *
 * @param $a
 * @param $b
 */
function compare_original_sources($a, $b){

  $a_string = '';
  if(isset($a->citation->titleCache)) {
    $a_string = $a->citation->titleCache;
  }
  if((isset($a->nameUsedInSource))){
    $a_string .= $a->nameUsedInSource->titleCache;
  } elseif (isset($a->originalNameString)){
    $a_string .= $a->originalNameString;
  }

  $b_string = '';
  if(isset($b->citation->titleCache)) {
    $b_string = $b->citation->titleCache;
  };
  if((isset($b->nameUsedInSource))){
    $b_string .= $b->nameUsedInSource->titleCache;
  } elseif (isset($b->originalNameString)){
    $b_string .= $b->originalNameString;
  }

  if ($a_string == $b_string) {
    return 0;
  }
  return ($a_string < $b_string) ? -1 : 1;
}

/**
 * Compare callback to be used in usort to sort image sources of CDM Media instances.
 *
 * @param $a
 * @param $b
 */
function compare_text_data($a, $b) {

  if ($a->multilanguageText_L10n->text == $b->multilanguageText_L10n->text) {
    return 0;
  }
  return ($a->multilanguageText_L10n->text < $b->multilanguageText_L10n->text) ? -1 : 1;
}

  /**
   * Compare two different footnotes objects.
   *
   * The comparison is based on the footnote key. The one which is
   * displayed as footnote number.
   *
   * @param mixed $a
   *   Footnote object $a.
   * @param mixed $b
   *   Footnote object $b.
   */
  function footnotes_key_compare($a, $b) {
    $res = 0;
    if (empty($a) || empty($b)) {
      return $res;
    }
    if ($a->keyStr < $b->keyStr) {
      $res = -1;
    }
    elseif ($a->keyStr > $b->keyStr) {
      $res = 1;
    }
    return $res;
  }


/**
 * Creates an array suitable to be used in min_max_markup()
 *
 * @return array
 */
function min_max_array()
{
// FIXME use UUIDs instead? how about idInVocab?
  $min_max = array(
      'Extreme Min' => NULL,
      'Min' => NULL,
      'Average' => NULL,
      'Max' => NULL,
      'Extreme Max' => NULL,
  );
  return $min_max;
}

/**
 * Creates markup from a min max array.
 *
 * NOTE: use  min_max_array() to create an appropriate array
 *
 * @param $min_max
 *  the min-max array
 * @param $unit
 *  Defaults to no unit
 * @return string
 */
function min_max_markup($min_max, $unit = '') {

  $min_max_markup = '';
  // create min-max string
  if ($min_max['Min'] !== NULL && $min_max['Max'] !== NULL && $min_max['Min']->_value == $min_max['Max']->_value) {
    // min and max are identical
    $min_max['Average'] = $min_max['Min'];
    $min_max['Min'] = NULL;
    $min_max['Max'] = NULL;
  }

  // check for inconsistent cases, eg. only Max and average given
  if ($min_max['Min'] === NULL && $min_max['Max'] !== NULL) {
    // min missing
    $min_max['Min'] = '?';
  }
  if ($min_max['Min'] !== NULL && $min_max['Max'] === NULL) {
    // max missing
    $min_max['Max'] = '?';
  }


  foreach ($min_max as $key => $statistical_val) {
    if ($statistical_val !== NULL) {

      if ($statistical_val == '?') {
        $val_markup = $statistical_val;
      } else {
        $val_markup = '<span class="'
            . html_class_attribute_ref($statistical_val) . ' '
            . (isset($statistical_val->type) ? $statistical_val->type->termType : '') . ' ">'
            . $statistical_val->_value . '</span>';
      }

      if (strlen($min_max_markup)) {
        $min_max_markup .= '–';
      }
      if (str_beginsWith($key, 'Extreme')) {
        $val_markup = "($val_markup)";
      }
      $min_max_markup .= $val_markup;
    }
  }
  return $min_max_markup . ' ' . $unit;
}

/**
 * Creates min max markup to represent a min-average-max measure optionally with an error value.
 *
 * The fields that are taken into account are:
 * - field_base_name = min
 * - field_base_nameMax = max
 * - field_base_nameText = free text
 * - field_base_nameError = error value
 *
 * @param $object
 *    The object having min max measurement fields
 * @param string $field_base_name
 *    The base name for all measurement fields. This name is at the same time the full name of the
 *    min value.
 * @return array
 *   the resulting min max array
 */
function min_max_measure($object, $field_base_name)
{
  static $default_unit = 'm';

  $min_max_array = min_max_array();
  if (@is_numeric($object->$field_base_name)) {
    $min_max_array['Min'] = new stdClass();
    $min_max_array['Min']->_value = $object->$field_base_name;
  }

  $field_name = $field_base_name . 'Max';
  if (@is_numeric($object->$field_name)) {
    $min_max_array['Max'] = new stdClass();
    $min_max_array['Max']->_value = $object->$field_name;
  }
  $min_max_markup = min_max_markup($min_max_array, $default_unit);

  $field_name = $field_base_name . 'Error';
  if (@is_numeric($object->$field_name )) {
    $min_max_markup .= ' +/- ' . $object->$field_name . ' ' . $default_unit;
  }
  $field_name = $field_base_name . 'Text';
  if (@is_string($object->$field_name)) {
    $min_max_markup .= ' ' . $object->$field_name;
  }
  return $min_max_markup;
}

// TODO  move below code into new file: agent.inc

/*
 * Compose an render array from a CDM TaxonNodeAgentRelation object as Taxon Expert.
 *
 * compose_hook() implementation
 *
 * @param object $taxon_node_agent_relation
 *   CDM instance of type TaxonNodeAgentRelation
 * @return array
 *   A drupal render array
 *
 * @ingroup compose
 */
function compose_cdm_taxon_expert($taxon_node_agent_relation) {

  $label_suffix = ':';

  if($taxon_node_agent_relation->class == 'DefaultPagerImpl'){
    // oops this is a pager
    // this situation will occur when this compose is executed
    // through the proxy_content() method
    $taxon_node_agent_relation = $taxon_node_agent_relation->records[0];

  }

  if(is_object($taxon_node_agent_relation->agent)) {
    $agent_details = compose_cdm_team_or_person_base($taxon_node_agent_relation->agent);
    // all data will be added to the groups of the agent_details render array
    $groups = &$agent_details[0]['#groups'];

    @_description_list_group_add($groups, t('Role'). $label_suffix, $taxon_node_agent_relation->type->representation_L10n);

    $family_tnars = cdm_ws_fetch_all(CDM_WS_PORTAL_AGENT . '/' . $taxon_node_agent_relation->agent->uuid . '/taxonNodeAgentRelations', array("rank"=>"Familia"));

    $taxa_markup = array(
      '#theme_wrappers' => array('container'),
      '#attributes' => array('class' => array('managed_taxa')),
      '#wrapper_attributes' => array('class' => 'sublist-container')
      );
    foreach($family_tnars as $tnar){
      if(is_object($tnar->taxonNode->taxon)){
        $taxa_markup[$tnar->taxonNode->taxon->titleCache] = markup_to_render_array(render_taxon_or_name($tnar->taxonNode->taxon, url(path_to_taxon($tnar->taxonNode->taxon->uuid))));
      }
    }
    ksort($taxa_markup);

    @_description_list_group_add($groups, t('Families'). $label_suffix, array($taxa_markup));

  }

  return $agent_details;
}


/*
 * Compose an render array from a CDM TeamOrPersonBase object.
 *
 * compose_hook() implementation
 *
 * TODO: currently mainly implemented for Agent, add Team details
 *
 * @param object $team_or_person
 *   CDM instance of type TeamOrPersonBase
 * @return array
 *   A drupal render array
 *
 * @ingroup compose
 */
function compose_cdm_team_or_person_base($team_or_person, $data = array()) {

  $groups = array();

  $label_suffix = ':';

  // $weight = 0;
  if($team_or_person){

    if(is_object($team_or_person->lifespan)){
      // ToDo render as date* - date† ?
      @_description_list_group_add($groups, t('Lifespan'). $label_suffix, timePeriodToString($team_or_person->lifespan) /*, '' , $weight++ */);
    }

    // nomenclaturalTitle
    @_description_list_group_add($groups, "Nomenclatural Title". $label_suffix, $team_or_person->nomenclaturalTitle);
    // collectorTitle
    @_description_list_group_add($groups, "Collector Title". $label_suffix, $team_or_person->collectorTitle);

    // institutionalMemberships
    if(is_array($team_or_person->institutionalMemberships)){

      $institutes_ra =  array();
      foreach($team_or_person->institutionalMemberships as $membership) {
        $membership_groups = array();
        @_description_list_group_add($membership_groups, t('Department'). $label_suffix, $membership->department);
        @_description_list_group_add($membership_groups, t('Role'). $label_suffix, $membership->role);
        if(is_object($membership->period)){
          @_description_list_group_add($membership_groups, t('Period'). $label_suffix, timePeriodToString($membership->period));
        }
        if(is_object($membership->institute->contact)){
          $institute_contact_details = compose_cdm_contact($membership->institute->contact, $membership->institute->titleCache);
          if(is_array($institute_contact_details[0]['#groups'])){
            $membership_groups = array_merge($membership_groups, $institute_contact_details[0]['#groups']);
          }
        }
        if(count($membership_groups) > 0){
          $institutes_ra[]  = array(
            '#title' => $membership->institute->titleCache,
            '#theme' => 'description_list',
            '#groups' => $membership_groups,
            '#attributes' => array('class' => html_class_attribute_ref($membership)),
            '#wrapper_attributes' => array('class' => 'sublist-container')
          );
        } else {
          // no further details for the membership, display the title
          $institutes_ra[] = markup_to_render_array('<h3>' . $membership->institute->titleCache . '</h3>');
        }

      }

      $label = count($institutes_ra) > 1 ? t('Institutes'):  t('Institute');
      @_description_list_group_add($groups, $label. $label_suffix, $institutes_ra /*, '' , $weight++ */);
    }


    // Contact
    $agent_contact_details = compose_cdm_contact($team_or_person->contact, $team_or_person->titleCache);
    if(is_array($agent_contact_details[0]['#groups'])){
      $groups = array_merge($groups, $agent_contact_details[0]['#groups']);
    }

    // additional data
    foreach($data as $key=>$value){
      @_description_list_group_add($sub_dl_groups, t('@key', array('@key' => $key)), $value /*, '' , $weight++ */);
    }

  }

  $team_or_person_details = array(
    '#title' => $team_or_person->titleCache,
    '#theme' => 'description_list',
    '#groups' => $groups,
    '#attributes' => array('class' => html_class_attribute_ref($team_or_person)),
  );
  return array($team_or_person_details);
}


/*
 * Compose an render array from a CDM Contact object.
 *
 * compose_hook() implementation
 *
 * TODO: currently mainly implemented for Agent, add Team details
 *
 * @param object $contact
 *   CDM instance of type Contact
 * @param $title
 *   The title for the description list header
 * @param $weight
 *   Optional weight for the description list entries
 * @return array
 *   A drupal render array
 *
 * @ingroup compose
 */
function compose_cdm_contact($contact, $title, $weight = 0)
{

  $groups = array();

  $contact_details = null;

  $label_suffix = ':';

  $contact_field_names_map = array(
    'emailAddresses' => t('Email'),
    'urls' => t('Urls'),
    'phoneNumbers' => t('Phone'),
    'faxNumbers' => t('Fax'),
  );

  // Contact
  if(is_object($contact)){
    if(isset($contact->addresses)){
      // TODO ....
      // $sub_groups = array();
      // foreach($contact->addresses as $address){
      //   @_description_list_group_add($sub_groups, $label, $contact->$fieldName, '', $weight++);
      // }
    }
    foreach($contact_field_names_map as $fieldName => $label){
      if(is_array($contact->$fieldName)){
        @_description_list_group_add($groups, $label . $label_suffix, $contact->$fieldName, '', $weight++);
      }
    }
    $contact_details = array(
      '#title' => $title,
      '#theme' => 'description_list',
      '#groups' => $groups
    );


  } else if(is_string($title)) {
    // if the contact entity is empty but the title is given anyway
    // we are only adding the title, using the description_list
    // structure is not possible since it would be empty due to
    // missing group data
    $contact_details = array('#markup' => '<h3>' . $title . '</h3>');
  }

  return array($contact_details);

}

/**
 * Compose an render array from a CDM Extension objects.
 *
 * @param $extensions
 *    An array of CDM Extension objects
 * @return array
 *   A render array containing the fields of the supplied $sequence
 *
 * @ingroup compose
 */
function compose_extensions($extensions)
{
  $extensions_render_array= null;
  $extensions_by_type = array();
  foreach ($extensions as $extension) {
    if (@is_string($extension->value)) {
      if (!isset($extensions_by_type[$extension->type->representation_L10n])) {
        $extensions_by_type[$extension->type->representation_L10n] = array();
      }
      $extensions_by_type[$extension->type->representation_L10n][] = markup_to_render_array($extension->value);
    }
  }

  if (count($extensions_by_type)) {
    $sub_dl_groups = array();
    foreach ($extensions_by_type as $type_label => $text_list) {
      @_description_list_group_add($sub_dl_groups, $type_label . ':', $text_list);
    }
    $extensions_render_array = array(
      array('#theme' => 'description_list', '#groups' => $sub_dl_groups)
    );
    return $extensions_render_array;
  }
  return $extensions_render_array;
}
