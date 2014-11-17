<?php

  /**
   * Returns the localized representations of the modifiers hold by the
   * supplied cdm instance concatenated into one string.
   *
   * @param object $iModifieable
   *   cdm instance of an class implementing the interface IModifieable:
   *   DescriptionElementBase, StateDate, State
   *
   * @return String
   *    localized representations of the modifiers hold by the
   *    supplied cdm instance concatenated into one string
   */
  function cdm_modifers_representations($iModifieable, $glue = ', ') {
    $modifiers_strings = array();
    if (isset($iModifieable->modifiers)) {
      foreach ($iModifieable->modifiers as $modifier) {
        $modifiers_strings[] = cdm_term_representation($modifier);
      }
    }
    return implode(', ', $modifiers_strings);
  }

  /**
   * Filters the given set of description elements and prefers
   * computed elements over others. Computed description elements
   * are identified by the MarkerType.COMPUTED()
   *
   * If the given set contains at least one computed element only
   * the computed elements are returned.
   *
   * @param $description_elements
   *   An array of CDM DescriptionElementBase instances
   *
   * @return
   *  only the computed description elements otherwise all others.
   *
   * @deprecated this is replaced by the cdmlib DistributionUtil class!!!
   */
  function cdm_description_elements_prefer_computed($description_elements){

    $computed_elements = array();
    $other_elements = array();

    if(!empty($description_elements)){
      foreach ($description_elements as $element) {
        if(cdm_entity_has_marker($element, UUID_MARKERTYPE_COMPUTED)){
          $computed_elements[] = $element;
        } else {
          $other_elements[] = $element;
        }
      }
    }

    if(count($computed_elements) > 0) {
      return $computed_elements;
    } else {
      return $other_elements;
    }
  }

  /**
   * @return array
   */
  function cdm_distribution_filter_query(){
    $cdm_distribution_filter = get_array_variable_merged(CDM_DISTRIBUTION_FILTER, CDM_DISTRIBUTION_FILTER_DEFAULT);
    $query = array();

    if($cdm_distribution_filter['filter_rules']['statusOrderPreference']){
      $query['statusOrderPreference'] = 1;
    }
    if($cdm_distribution_filter['filter_rules']['subAreaPreference']){
      $query['subAreaPreference'] = 1;
    }
    if(is_array($cdm_distribution_filter['hideMarkedAreas']) && count($cdm_distribution_filter['hideMarkedAreas']) > 0){
      $query['hideMarkedAreas'] = '';
      foreach ($cdm_distribution_filter['hideMarkedAreas'] as $marker_type => $enabled)
        if($enabled){
          $query['hideMarkedAreas'] .= ($query['hideMarkedAreas'] ? ',' : '') . $marker_type;
        }
    }

    return $query;
  }

  /**
   * Merge the fields 'annotations', 'markers', 'sources', 'media' from the source CDM DescriptionElement into  the target.
   *
   * @param $target
   *     The source CDM DescriptionElement
   * @param $source
   *     The target CDM DescriptionElement
   */
  function cdm_merge_description_elements(&$target, &$source){
    static $fields_to_merge = array('annotations', 'markers', 'sources', 'media');

    foreach ($fields_to_merge as $field){
      if(is_array($source->$field)) {
        if(!is_array($target->$field)){
          $target->$field = $source->$field;
        } else {
          $target->$field = array_merge($target->$field, $source->$field);
        }
      }
    }
  }

  /**
   * Adds an entry to the end of the table of content items list
   *
   * The  table of content items are crated internally by calling
   * toc_list_item() the resulting item is added to the statically cached
   * list of toc elements
   *
   * @param $label
   *  The label of toc entry
   * @param $class_attribute_suffix
   *  The suffix to be appended to the class attribute prefix: "feature-toc-item-"
   * @param $fragment
   *  Optional parameter to define a url fragment different from the $label,
   *  if the $fragment is not defined the $label will be used
   */
  function cdm_toc_list_add_item($label, $class_attribute_suffix, $fragment = NULL, $as_first_element = FALSE){
    $toc_list_items = &cdm_toc_list();

    if(!$fragment){
      $fragment = $label;
    }
    $fragment = generalizeString($fragment);

    $class_attributes = 'feature-toc-item-' . $class_attribute_suffix;

    $new_item = toc_list_item(
      theme(
        'cdm_feature_name',
        array('feature_name' => $label))
      ,
      array('class' => $class_attributes),
      $fragment
    );

    if($as_first_element){
      array_unshift($toc_list_items, $new_item);
    } else {
      $toc_list_items[] = $new_item;
    }

  }

  /**
   * Returns the statically cached table of content items as render array
   *
   * @see see also cdm_toc_list_add_item()
   *
   * @return array
   *   a render array of table of content items suitable for theme_item_list()
   */
  function &cdm_toc_list(){
    $toc_list_items = &drupal_static('toc_list_items', array());

    return $toc_list_items;
  }

/**
 * Prepares an empty Drupal block for displaying description elements of a specific CDM Feature.
 *
 * The block can also be used for pseudo Features like a bibliography. Pseudo features are
 * derived from other data on the fly and so not exist as such in the cdm data. In case
 * of pseudo features the $feature is left empty
 *
 * @param $feature_name
 *    A label describing the feature, usually the localized feature representation.
 * @param $feature
 *    The CDM Feature for which the block is created. (optional)
 * @return object
 *    A Drupal block object
 */
function feature_block($feature_name, $feature = null) {
  $block = new stdclass(); // Empty object.
  $block->module = 'cdm_dataportal';
  $block->delta = generalizeString($feature_name);
  $block->region = null;
  $block->subject = '<a name="' . $block->delta . '"></a><span class="' . html_class_attribute_ref($feature) . '">'
    . theme('cdm_feature_name', array('feature_name' => $feature_name))
    . '</span>';
  $block->module = "cdm_dataportal-feature";
  $block->content = '';
  return $block;
}

  /**
   * Creates a list item for a table of content, suitable as data element for a themed list
   *
   * @see theme_list()
   *
   * @param $label
   * @param $http_request_params
   * @param $attributes
   * @return array
   */
  function toc_list_item($label, $attributes = array(), $fragment = null) {

    // we better cache here since drupal_get_query_parameters has no internal static cache variable
    $http_request_params = drupal_static('http_request_params', drupal_get_query_parameters());

    $item =  array(
      'data' => l(
        $label,
        $_GET['q'],
        array(
          'attributes' => array('class' => array('toc')),
          'fragment' => generalizeString($label),
          'query' => $http_request_params
        )
      )
    );
    $item['attributes'] = $attributes;
    return $item;
  }

  /**
   * Creates the footnotes for the given CDM DescriptionElement instance.
   *
   * Footnotes are created for annotations and original sources,
   * optionally the sources are put into a separate bibliography.
   *
   * @param $descriptionElement
   *     A CDM DescriptionElement instance
   * @param $separator
   *     Optional parameter. The separator string to concatenate the footnote ids, default is ','
   * @param $footnote_list_key_suggestion
   *     will be overridden for original sources if the bibliography block is enabled
   *
   * @return String
   *    The foot note keys
   */
  function cdm_create_description_element_footnotes($description_element, $separator = ',',
            $footnote_list_key_suggestion = null, $do_link_to_reference = FALSE,
            $do_link_to_name_used_in_source = FALSE
      ){


    // Annotations as footnotes.
    $footNoteKeys = cdm_annotations_as_footnotekeys($description_element, $footnote_list_key_suggestion);

    // Source references as footnotes.
    $bibliography_settings = get_bibliography_settings();
    $original_source_footnote_tag = $bibliography_settings['enabled'] == 1 ? 'div' : null; // otherwise the default

    foreach ($description_element->sources as $source) {
      if (_is_original_source_type($source)) {
        $fn_key = FootnoteManager::addNewFootnote(
          original_source_footnote_list_key($footnote_list_key_suggestion),
          theme('cdm_OriginalSource', array(
            'source' => $source,
            'doLink' => $do_link_to_reference,
            'do_link_to_name_used_in_source' => $do_link_to_name_used_in_source

          )),
          $original_source_footnote_tag
        );
        // Ensure uniqueness of the footnote keys.
        cdm_add_footnote_to_array($footNoteKeys, $fn_key);
      }
    }
    // Sort and render footnote keys.
    $footnoteKeyListStr = '';
    asort($footNoteKeys);
    foreach ($footNoteKeys as $footNoteKey) {
      $footnoteKeyListStr .= theme('cdm_footnote_key',
        array(
          'footnoteKey' => $footNoteKey,
          'separator' => ($footnoteKeyListStr ? $separator : '')));
    }
    return $footnoteKeyListStr;
  }

  /**
   *
   * @return string the footnote_list_key
   */
  function original_source_footnote_list_key($key_suggestion = null) {
    if(!$key_suggestion){
      $key_suggestion = RenderHints::getFootnoteListKey();
    }
    $bibliography_settings = get_bibliography_settings();
    $footnote_list_key = $bibliography_settings['enabled'] == 1 ? 'BIBLIOGRAPHY' : $key_suggestion;
    return $footnote_list_key;
  }

  /**
   * Provides the according tag name for the description element markup which fits the  $feature_block_settings['as_list'] value
   *
   * @param $feature_block_settings
   *   A feature_block_settings array, for details, please see get_feature_block_settings($feature_uuid = 'DEFAULT')
   */
  function cdm_feature_block_element_tag_name($feature_block_settings){
    switch ($feature_block_settings['as_list']){
      case 'ul':
      case 'ol':
        return 'li';
      case 'div':
        if(isset($feature_block_settings['element_tag'])){
          return $feature_block_settings['element_tag'];
        }
        return 'span';
      case 'dl':
        return 'dd';
      default:
        return 'div'; // should never happen, throw error instead?
    }
  }
