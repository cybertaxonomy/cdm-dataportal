<?php
/**
 * @file
 * Functions for dealing with CDM entities of type SpecimenOrOccurrences
 * This file contains new functions which are to replace functions in occurrences.inc
 * and should be merged with the latter one.
 *
 * @copyright
 *   (C) 2007-2021 EDIT
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
 * Composes the view on specimens and occurrences as derivate tree
 * starting from the field unit including all derivatives.
 *
 * @see CDM_SPECIMEN_LIST_VIEW_MODE_OPTION_DERIVATE_TREE
 *
 * @param array $root_unit_dtos
 *   list of SpecimenOrObservationDTOs
 * @return array
 *   The Drupal render array
 *
 * @ingroup compose
 */
function compose_specimen_table_top_down_new(array $root_unit_dtos){
  // add icons
  $expand_icon = font_awesome_icon_markup(
    'fa-plus-square-o',
    array(
      'alt' => 'Show details',
      'class' => array('expand_icon')
    )
  );
  $collapse_icon = font_awesome_icon_markup(
    'fa-minus-square-o',
    array(
      'alt' => 'Show details',
      'class' => array('collapse_icon')
    )
  );

  $derivation_tree = derived_units_tree($root_unit_dtos);

  $render_array = [];
  $render_array['derived-unit-tree'] = $derivation_tree;

  _add_js_derivation_tree('.derived-unit-tree');

  return $render_array;
}

/**
 * Creates the root levels and trees for all subordinate derivatives.
 *
 * See derived_units_sub_tree()
 *
 * @param array $root_unit_dtos
 *     list of SpecimenOrObservationDTOs
 * @return array
 *    An array which can be used in render arrays to be passed to the
 * theme_table() and theme_list().
 */
function derived_units_tree(array $root_unit_dtos){

  $root_items = [];
  //we need one more item to contain the items of one level (fieldunit, derivate data etc.)
  foreach ($root_unit_dtos as &$sob_dto) {
    $root_item = [
      '#prefix' => '<div class="derived-unit-tree">',
      '#suffix' => '</div>',
      '#type' => 'container',
      '#attributes' => ['class' => ['derived-unit-item derived-unit-tree-root',  html_class_attribute_ref($sob_dto)]],
      'div-container' => ['root-item-and-sub-tree' => [markup_to_render_array('<div class="unit-header"><div class="unit-label">' . $sob_dto->label .'</div></div>'
        .'<div class="unit-content-wrapper">' // allows to apply the borders between .derived-unit-tree-root and .unit-content
        .'<div class="unit-content">' . compose_cdm_specimen_or_observation_tree_entry($sob_dto) . '</div>'
      . '</div>')]],

    ];
    if (isset($sob_dto->derivatives) && sizeof($sob_dto->derivatives) > 0){
      usort($sob_dto->derivatives, 'compare_specimen_or_observation_dtos');
      // children are displayed in a nested list.
      $root_item['div-container']['root-item-and-sub-tree'][] = derived_units_sub_tree($sob_dto->derivatives);
    }
    $root_items[] = $root_item;
  }

  return $root_items;
}

/**
 * @param array $unit_dtos
 *
 * @return array
 */
function derived_units_sub_tree(array $unit_dtos) {

  $list_items = derived_units_as_list_items($unit_dtos);

  $derivation_tree = [
    '#theme' => 'item_list',
    '#type' => 'ul',
    '#attributes' => [
      'class' => CDM_SPECIMEN_LIST_VIEW_MODE_OPTION_DERIVATE_TREE . ' derived-unit-sub-tree',
    ],
    '#items' => $list_items,
  ];
  return $derivation_tree;
}

/**
 * Creates render array items for FieldUnitDTO or DerivedUnitDTO.
 *
 * @param array $root_unit_dtos
 *     list of SpecimenOrObservationDTOs
 * @return array
 *    An array which can be used in render arrays to be passed to the
 * theme_table() and theme_list().
 */
function derived_units_as_list_items(array $root_unit_dtos){

  $list_items = [];
  //we need one more item to contain the items of one level (fieldunit, derivate data etc.)
  foreach ($root_unit_dtos as &$sob_dto) {
    $item = [];
    $item['class'] = ['derived-unit-item ',  html_class_attribute_ref($sob_dto)];
    // data" element of the array is used as the contents of the list item
    $item['data'] = [];
    $item['data'] = '<div class="unit-header"><div class="unit-label">' . $sob_dto->label .'</div></div>'
      . '<div class="unit-content">' . compose_cdm_specimen_or_observation_tree_entry($sob_dto) . '</div>';
    if (isset($sob_dto->derivatives) && sizeof($sob_dto->derivatives) > 0){
      usort($sob_dto->derivatives, 'compare_specimen_or_observation_dtos');
      // children are displayed in a nested list.
      $item['children'] = derived_units_as_list_items($sob_dto->derivatives);
    }
    $list_items[] = $item;
  }

  return $list_items;
}
