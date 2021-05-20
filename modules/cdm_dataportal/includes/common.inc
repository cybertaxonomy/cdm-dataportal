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
 * The cdm_dataportal module needs to compose rather complex render arrays from
 * the data returned by the CDM REST service. The compose functions are
 * responsible for creating the render arrays.
 *
 * All these functions are also implementations of the compose_hook()
 * which is used in the proxy_content() function.
 * @}
 */


/**
 * Composes an HTML element class attribute value composed of
 * the short-name of the cdm class and the uuid of the entity.
 * This class attribute should be used wherever an cdm-entity is rendered.
 *
 * In case of Taxon entities or TaxonNodeDTOs the secReference is also added
 * to the class attributes as 'sec_uuid:<uuid>'. In case of TaxonNodeDTOs the
 * Taxon uuid is added also as taxon_uuid:<uuid>
 *
 * These according class selectors in css must be escaped, eg:
 *    .cdm\:TextData
 *
 * @param $cdm_entity
 *    A CDM entity, TaxonNodeDTO or TypedEntityReference
 */
function html_class_attribute_ref($cdm_entity) {

  $attributes = '';
  if (is_cdm_entity($cdm_entity)) {
    // Entities
    $attributes =  "cdm:" . $cdm_entity->class . " uuid:" . $cdm_entity->uuid;
    switch ($cdm_entity->class){
      case 'Taxon':
        if(isset($cdm_entity->secSource->uuid)){
          $attributes .= " sec_uuid:"  . $cdm_entity->secSource->citation->uuid;
        }
        break;
    }
  } elseif (isset($cdm_entity->class)) {
    // DTOs
    if(isset_not_empty($cdm_entity->type) && isset_not_empty($cdm_entity->uuid)){
        $attributes =  "cdm:" . $cdm_entity->type . " uuid:" . $cdm_entity->uuid;
    }
    // additional data of specific DTOs
    switch ($cdm_entity->class){
      case 'TaxonNodeDto':
        $attributes .= "taxon_uuid:"  . $cdm_entity->taxonUuid . " sec_uuid:"  . $cdm_entity->secUuid;;
      break;
    }
  }

  return $attributes;
}


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
  $label = $marker->markerType->representation_L10n . ': ' . (($marker->flag !== TRUE ? t('yes') : t('no')));
  return markup_to_render_array('<span class="' . html_class_attribute_ref($marker) . '">' . $label . '</span>');
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
 * @return array
 *  An array of drupal render arrays
 */
function oder_and_render_original_sources($sources){
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

      $sort_array[$order_key] = render_original_source($source);
    }
    ksort($sort_array);
    return array_values ($sort_array);
}

/**
 * Compare callback to be used in usort to sort image sources of CDM OriginalSource instances.
 *
 * TODO the compare strategy implemented in oder_and_render_original_sources() is probably better but is not taking the
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
 * Provides an explanatory text on the statistical values representation as generated by statistical_values()
 *
 * @return string
 *     the text
 */
  function statistical_values_explanation(){
    return t("A single or the first number in square brackets denotes sample size");
  }

/**
 * Creates an array suitable to be used in statistical_values()
 * The min max structure is suitable for being used in the context
 * of GatheringEvents and StatisticalMeasures (see render_quantitative_statistics()).
 *
 * The order of the items is important for the display and must not be changed.
 *
 * @return array
 */
function statistical_values_array() {

  $min_max = [
    'ExactValue' => NULL,
    'Min' => NULL,
    'TypicalLowerBoundary' => NULL,
    'TypicalUpperBoundary' => NULL,
    'Max' => NULL,
    'SampleSize' => NULL,
    'Average' => NULL,
    'Variance' => NULL,
    'StandardDeviation' => NULL
  ];
  return $min_max;
}

/**
 * Creates markup from a min max array.
 *
 * NOTE: use  statistical_values_array() to create an appropriate array
 *
 * Internally Min will be translated to TypicalLowerBoundary if no such value is present.
 * The same also accounts for Max and TypicalUpperBoundary.
 *
 * For further details see #3742, #8766
 *
 * @param $stat_vals_arr
 *  the statistical values array as produced by statistical_values_array()
 * @param $unit
 *  Defaults to no unit
 * @return string
 */
function statistical_values($stat_vals_arr, $unit = '') {

  static $xbar_equals = 'x&#772;='; // x&#772; is x-bar (http://www.personal.psu.edu/ejp10/blogs/gotunicode/2010/03/dealing-with-x-bar-x-and-p-hat.html)

  $exact_markup = '';
  $min_max_markup = '';
  $other_vals_array = [];

  // --- sanitize values
  if(statistical_values_num_equals($stat_vals_arr, 'Min', 'TypicalLowerBoundary')){
    $stat_vals_arr['Min'] = NULL;
  }

  if(statistical_values_num_equals($stat_vals_arr, 'Max', 'TypicalUpperBoundary')){
    $stat_vals_arr['Max'] = NULL;
  }

  if($stat_vals_arr['TypicalLowerBoundary'] === null && $stat_vals_arr['Min'] !== null){
    $stat_vals_arr['TypicalLowerBoundary'] = $stat_vals_arr['Min'];
    $stat_vals_arr['Min'] = NULL;
  }

  if($stat_vals_arr['TypicalUpperBoundary'] === null && $stat_vals_arr['Max']  !== null){
    $stat_vals_arr['TypicalUpperBoundary'] = $stat_vals_arr['Max'];
    $stat_vals_arr['Max'] = NULL;
  }

  if (statistical_values_num_equals($stat_vals_arr, 'TypicalUpperBoundary', 'TypicalLowerBoundary')) {
    $stat_vals_arr['Average'] = $stat_vals_arr['TypicalUpperBoundary'];
    $stat_vals_arr['TypicalLowerBoundary'] = NULL;
    $stat_vals_arr['TypicalUpperBoundary'] = NULL;
  }

  // --- check for inconsistent cases, eg. only Max and average given
  if ($stat_vals_arr['TypicalLowerBoundary'] === NULL && $stat_vals_arr['TypicalUpperBoundary']  !== null) {
    // min missing
    $stat_vals_arr['TypicalLowerBoundary'] = '?';
  }
  if ($stat_vals_arr['TypicalLowerBoundary'] !== null && $stat_vals_arr['TypicalUpperBoundary'] === NULL) {
    // max missing
    $stat_vals_arr['TypicalUpperBoundary'] = '?';
  }

  if(statistical_values_is_numeric($stat_vals_arr['Average']) && statistical_values_is_numeric($stat_vals_arr['TypicalUpperBoundary']) && statistical_values_is_numeric($stat_vals_arr['TypicalLowerBoundary'])) {
    statistical_values_adjust_significant_figures($stat_vals_arr['Average'], $stat_vals_arr['TypicalLowerBoundary'], $stat_vals_arr['TypicalUpperBoundary']);
  }

  foreach ($stat_vals_arr as $key => $statistical_val) {

    if ($statistical_val !== NULL) {
      if ($statistical_val == '?') {
        $val_markup = $statistical_val;
      }
      else {
        if (is_numeric($statistical_val->_value)) {
          $val_markup = '<span class="'
            . html_class_attribute_ref($statistical_val) . ' '
            . (isset($statistical_val->type) ? $statistical_val->type->termType : '') . ' ' . $key . '" title="' . $key . '">'
            . $statistical_val->_value . '</span>';

        }
        else {
          $val_markup = NULL;
        }
      }

      if ($val_markup) {
        switch ($key) {
          case 'ExactValue':
            $exact_markup = $val_markup;
            break;
          // ---- min_max_element
          case 'Min':
            $min_max_markup .= "($val_markup&ndash;)";
            break;
          case 'Max':
            $min_max_markup .= "(&ndash;$val_markup)";
            break;
          case 'TypicalLowerBoundary':
            $min_max_markup .= "$val_markup";
            break;
          case 'TypicalUpperBoundary':
            $min_max_markup .= "&ndash;$val_markup";
            break;
          // ---- other values
          case 'SampleSize':
            $other_vals_array[$key] = $val_markup;
            break;
          case 'Average':
            $other_vals_array[$key] = $xbar_equals . $val_markup;
            break;
          case 'Variance':
            $other_vals_array[$key] = 'σ²=' . $val_markup;
            break;
          case 'StandardDeviation':
            $other_vals_array[$key] = 'σ=' . $val_markup;
            break;
        }
      }
    }
  }

  $full_markup = $exact_markup . ($exact_markup ? ' ' : '') . $min_max_markup;

  if(!$full_markup && !empty($other_vals_array['Average'])){
    // this could be the case in which we only have one value for Average
    // this trivial case needs to be displayed a simpler way
    $full_markup = str_replace($xbar_equals, '' , $other_vals_array['Average']);
    unset($other_vals_array['Average']);
  }
  if($unit){
    $full_markup .= ' ' . $unit;
  }
  if(count($other_vals_array)){
    $full_markup .= ' [' . join(';', $other_vals_array) . ']';
  }

  return $full_markup;
}

/**
 * Calculates the required precision for the target value to be significantly different from min and may and rounds it.
 *
 * @param $target
 *    The statistical value to be rounded to the least significant precision
 * @param $min
 *    The lower bound to calculate the least significant precision
 * @param $max
 *    The upper bound to calculate the least significant precision
 * @param int $threshold
 *    Per default 1, but can be set to any other value > 0 and < 1 to define
 *    another threshold for the transition to the next precision level.
 *    E.g. A value of 0.5 will cause values > 50 and <= 500 to be shown with
 *    a precision of 2, whereas with a threshold of 1 the values > 10 and <= 100
 *    will be shown with a precision of 2 figures
 */
function statistical_values_adjust_significant_figures(&$target, $min,  $max, $threshold = 1){

  $precision = 1;
  if($min->_value !== $max->_value){
    $precision = floor(log10(abs($max->_value - $min->_value) * (1 / $threshold)));
    // increase precision by one
    $precision += $precision < 0 ? - 1 : 1;
  }

  $target->_value = sigFig($target->_value, $precision);
}

/**
 * based on an idea taken from https://stackoverflow.com/questions/37618679/format-number-to-n-significant-digits-in-php#answer-48283297
 *
 * @param $value
 * @param $digits
 *
 * @return float|string
 */
function sigFig($value, $digits, $round_only = true)
{
  if ($value == 0) {
    $decimalPlaces = $digits - 1;
  } elseif ($value < 0) {
    $decimalPlaces = $digits - floor(log10($value * -1)) - 1;
  } else {
    $decimalPlaces = $digits - floor(log10($value)) - 1;
  }

  $answer = ($decimalPlaces > 0) && !$round_only ?
    number_format($value, $decimalPlaces) : round($value, abs($decimalPlaces));
  return $answer;
}



/**
 * Used internally in statistical_values() do determine numerically equality of stat_vals_arr values
 *
 * @param $stat_vals_arr
 * @param $key1
 * @param $key2
 *
 * @return bool
 */
function statistical_values_num_equals($stat_vals_arr, $key1, $key2){

  return $stat_vals_arr[$key1] !== NULL && $stat_vals_arr[$key2] !== NULL && $stat_vals_arr[$key1]->_value == $stat_vals_arr[$key2]->_value && is_numeric($stat_vals_arr[$key1]->_value);
}

function statistical_values_is_numeric($stat_val){
  return $stat_val !== null && is_numeric($stat_val->_value);
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
 *    The object having min max measurement fields e.g.: GatheringEvent
 * @param string $field_base_name
 *    The base name for all measurement fields. This name is at the same time the full name of the
 *    min value.
 * @return string
 *   The markup for the min max
 *
 * @see statistical_values()
 */
function statistical_values_from_gathering_event($object, $field_base_name)
{
  static $default_unit = 'm';

  $field_name = $field_base_name . 'Text';
  if (isset_not_empty($object->$field_name)) {
    // Freetext overrides all other data
    $min_max_markup = ' ' . $object->$field_name;
  } else {
    // create markup for the atomized min max data
    $min_max_array = statistical_values_array();
    if (isset_numerical($object->$field_base_name)) {
      $min_max_array['TypicalLowerBoundary'] = new stdClass();
      $min_max_array['TypicalLowerBoundary']->_value = $object->$field_base_name;
    }
    $field_name = $field_base_name . 'Max';
    if (isset_numerical($object->$field_name)) {
      $min_max_array['TypicalUpperBoundary'] = new stdClass();
      $min_max_array['TypicalUpperBoundary']->_value = $object->$field_name;
    } else {
      $min_max_array['Average'] = $min_max_array['TypicalLowerBoundary'];
      $min_max_array['TypicalLowerBoundary'] = null;
    }
    $min_max_markup = statistical_values($min_max_array, $default_unit);
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

  $agent_details = null;

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

function visible_extensions_sorted($cdm_entity) {

  $visible_extensions_sorted = [];
  $extension_types_visible = variable_get(EXTENSION_TYPES_VISIBLE, unserialize(EXTENSION_TYPES_VISIBLE_DEFAULT));
  if (isset($cdm_entity->extensions)) {
    foreach ($cdm_entity->extensions as $ext) {
      if (array_search($ext->type->uuid, $extension_types_visible)) {
        if (!array_key_exists($ext->type->uuid, $visible_extensions_sorted)) {
          $visible_extensions_sorted[$ext->type->uuid] = [];
        }
        $visible_extensions_sorted[$ext->type->uuid][] = $ext;
      }
    }
  }
  return $visible_extensions_sorted;
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
  $render_array= null;
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
    $render_array = array(
      array('#theme' => 'description_list', '#groups' => $sub_dl_groups)
    );
    return $render_array;
  }
  return $render_array;
}

/**
 * Compose an render array from a CDM Identifier objects.
 *
 * @param $extensions
 *    An array of CDM Identifier objects
 * @return array
 *   A render array containing the fields of the supplied $sequence
 *
 * @ingroup compose
 */
function compose_identifiers($identifiers){

  $render_array = [];
  //     // urn:lsid:ipni.org:authors:20009164-1 UUID_IDENTIFIER_TYPE_LSID
  foreach($identifiers as $identifier){
    if($identifier->identifier){
      $render_array[] = markup_to_render_array('<span class="label">' . $identifier->type->representation_L10n . ':</span> ' . $identifier->identifier, null, '<span class="'. html_class_attribute_ref($identifier) . '">', '</span>');
    }
  }

  return $render_array;

}

function formatParams($params) {
    $paramString = null;
    if (is_array($params)){
        $keys =array_keys($params);
        $paramString = '';
        foreach ($keys as $k ){
            if ($k != 'pageNumber' && $k != 'pageSize'){
                $paramString .= ' -'.$k.'='.urlencode($params[$k]);
            }
        }
    }
    return $paramString;
}

function formatWSParams($params) {
  $paramString = null;
    if (is_array($params)){
        $keys =array_keys($params);
        $paramString = '';
        foreach ($keys as $k ){
            if ($k != 'pageNumber' && $k != 'pageSize'){
                $paramString .= '&'.$k.'='.urlencode($params[$k]);
            }
        }
    }
    return $paramString;
}

/**
 *
 * @param $cdm_entity
 *
 * @return string the markup
 */
function render_cdm_entity_link($cdm_entity) {

  switch ($cdm_entity->class) {
    case 'TaxonDescription':
    case 'NameDescription':
    case 'SpecimenDescription':
      $link =  '<span class="' . html_class_attribute_ref($cdm_entity) . '">' . $cdm_entity->titleCache . '</span> ' . icon_link(path_to_description($cdm_entity->uuid));
      break;
    default:
      $link = '<span class="error">UNSUPPORTED CDM ENTITY TYPE</span>';
  }
  return $link;
}

/**
 * @param $point
 * @param bool $do_link_to_geohack
 * @param string $geohack_link_pagename
 *
 * @return string
 */
function render_point($point, $do_link_to_geohack = true, $geohack_link_pagename = ''){
  if(!isset($point->sexagesimalString)){
    return '';
  }
  $markup = $point->sexagesimalString;
  if($do_link_to_geohack) {
    // see https://www.mediawiki.org/wiki/GeoHack
    $geohack_url = sprintf(
      "https://geohack.toolforge.org/geohack.php?language=en&params=%f;%f&pagename=%s",
      $point->latitude,
      $point->longitude,
      $geohack_link_pagename);
    $markup = l($markup, $geohack_url, ['attributes' => ['target' => 'geohack']]);
  }
  if(isset($point->errorRadius) && $point->errorRadius){
    $markup .= ' +/-' . $point->errorRadius . ' m';
  }
  if(isset($point->referenceSystem) && $point->referenceSystem){
    $refsystem = '(' . $point->referenceSystem->representation_L10n . ')';
    $markup = str_replace($refsystem, '', $markup);
    $markup .= ' ' . $refsystem;
  }
  return $markup;
}

/**
 * Creates an icon which links to the given path
 *
 * @param $path
 *
 * @param string $fragment
 *   The URL fragment to link to
 * @param bool $superscript
 *   If TRUE, the Icon will be be displayed in superscript position. Default is TRUE.
 *
 * @return string
 */
function icon_link($path, $fragment = '', $superscript = FALSE) {
  if($superscript){
    $attributes = ['class' => ['superscript']];
  } else {
    $attributes = [];
  }
  $icon_link = l(custom_icon_font_markup('icon-interal-link-alt-solid', $attributes), $path, ['html' => TRUE, 'fragment' => $fragment] );
  return $icon_link;
}

/**
 * Determine if a variable is set and is not NULL an is not empty.
 *
 * @param $var
 *  The variable to test
 *
 * @return bool
 *  True if the variable is set and is not NULL an is not empty.
 */
function isset_not_empty(&$var, $numerical_non_zero = FALSE){
  if (!isset($var)){
    return false;
  }
  if(is_numeric($var)) {
    if($numerical_non_zero){
      return $var;
    } else {
      true;
    };
  } else if(is_array($var)){
    return count($var) > 0;
  }else if(is_string($var)){
    return !!trim($var);
  } else {
    return !!$var;
  }
}

function isset_numerical(&$var){
  if (!isset($var)){
    return false;
  }
  return is_numeric($var);
}

function normalize_to_class_attribute($string){
  $string = strtolower($string);
  $string = preg_replace('/\s/', '_', $string);
  $string = preg_replace('/[^a-z0-9\-_]/', '', $string);
  return $string;
}
