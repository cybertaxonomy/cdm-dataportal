<?php


class DerivationTreeComposer {

  private $root_unit_dtos;
  private $focused_unit_uuid = null;
  private $with_details = false;

  /**
   * @return bool
   */
  public function isWithDetails() {
    return $this->with_details;
  }

  /**
   * @param bool $with_details
   */
  public function setWithDetails($with_details) {
    $this->with_details = $with_details;
  }

  /**
   * DerivationTreeComposer constructor.
   */
  public function __construct($root_unit_dtos ) {
    $this->root_unit_dtos = $root_unit_dtos;
  }

  /**
   * @return array
   *     list of SpecimenOrObservationDTOs
   */
  public function getRootUnitDtos() {
    return $this->root_unit_dtos;
  }

  /**
   * @param array $root_unit_dtos
   *     list of SpecimenOrObservationDTOs
   */
  public function setRootUnitDtos($root_unit_dtos) {
    $this->root_unit_dtos = $root_unit_dtos;
  }

  /**
   * @return null
   */
  public function getFocusedUnitUuid() {
    return $this->focused_unit_uuid;
  }

  /**
   * @param null $focused_unit_uuid
   */
  public function setFocusedUnitUuid($focused_unit_uuid) {
    $this->focused_unit_uuid = $focused_unit_uuid;
  }

  public function compose() {
    $derivation_tree = $this->derived_units_tree($this->root_unit_dtos);

    $render_array = [];
    $render_array['derived-unit-tree'] = $derivation_tree;

    _add_js_derivation_tree('.derived-unit-tree');

    return $render_array;
  }

  /* =======  member compose  methods ========== */


  /**
   * Creates the root levels and trees for all subordinate derivatives.
   *
   * See derived_units_sub_tree()
   *
   * @return array
   *    An array which can be used in render arrays to be passed to the
   * theme_table() and theme_list().
   */
  private function derived_units_tree() {

    RenderHints::pushToRenderStack('derived-unit-tree');
    RenderHints::setFootnoteListKey('derived-unit-tree');

    $root_items = [];
    //we need one more item to contain the items of one level (fieldunit, derivate data etc.)
    foreach ($this->root_unit_dtos as &$sob_dto) {

      $content_wrapper_markup = '';
      if($this->with_details){
        $field_unit_dto_render_array = compose_cdm_specimen_or_observation_dto_details_grid($sob_dto);
        $content_wrapper_markup = '<div class="unit-content-wrapper">' // allows to apply the borders between .derived-unit-tree-root and .unit-content
          . '<div class="unit-content derived-unit-details-grid">' . drupal_render($field_unit_dto_render_array) . '</div>'
          . '</div>';
      }
      $root_item = [
        '#prefix' => '<div class="derived-unit-tree">',
        '#suffix' => '</div>',
        '#type' => 'container',
        '#attributes' => [
          'class' => [
            'derived-unit-item derived-unit-tree-root',
            html_class_attribute_ref($sob_dto),
          ],
        ],
        'div-container' => [
          'root-item-and-sub-tree' => [
            markup_to_render_array($this->derived_units_tree_node_header($sob_dto)
              . $content_wrapper_markup),
          ],
        ],

      ];
      if (isset($sob_dto->derivatives) && sizeof($sob_dto->derivatives) > 0) {
        usort($sob_dto->derivatives, 'compare_specimen_or_observation_dtos');
        // children are displayed in a nested list.
        $root_item['div-container']['root-item-and-sub-tree'][] = $this->derived_units_sub_tree($sob_dto->derivatives);
      }
      $root_items[] = $root_item;
    }

    $root_items['footnotes'] = markup_to_render_array(render_footnotes());
    RenderHints::popFromRenderStack();

    return $root_items;
  }

  /**
   * @param array $unit_dtos
   *
   * @return array
   */
   private function derived_units_sub_tree(array $unit_dtos) {

    $list_items = $this->derived_units_as_list_items($unit_dtos);

    $derivation_tree = [
      '#theme' => 'item_list',
      '#type' => 'ul',
      '#attributes' => [
        // NOTE: class attribute "derived-unit-item" is important for consistency with subordinate <ul> elements produced by the drupal theme function
        'class' => CDM_SPECIMEN_LIST_VIEW_MODE_OPTION_DERIVATE_TREE . ' derived-unit-item derived-unit-sub-tree',
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
   *
   * @return array
   *    An array which can be used in render arrays to be passed to the
   * theme_table() and theme_list().
   */
   private function derived_units_as_list_items(array $root_unit_dtos) {

    $list_items = [];
    //we need one more item to contain the items of one level (fieldunit, derivate data etc.)
    foreach ($root_unit_dtos as &$sob_dto) {
      $item = [];
      $item['class'] = ['derived-unit-item ', html_class_attribute_ref($sob_dto)];
      // data" element of the array is used as the contents of the list item
      $item['data'] = [];
      $unit_content_markup = '';
      if($this->with_details){
        $units_render_array = compose_cdm_specimen_or_observation_dto_details_grid($sob_dto);
        $unit_content_markup = '<div class="unit-content derived-unit-details-grid">' . drupal_render($units_render_array) . '</div>';
      }
      $item['data'] = $this->derived_units_tree_node_header($sob_dto)
        . $unit_content_markup;
      if (isset($sob_dto->derivatives) && sizeof($sob_dto->derivatives) > 0) {
        usort($sob_dto->derivatives, 'compare_specimen_or_observation_dtos');
        // children are displayed in a nested list.
        $item['children'] = $this->derived_units_as_list_items($sob_dto->derivatives);
      }
      $list_items[] = $item;
    }

    return $list_items;
  }

  /**
   * @param $sob_dto
   *
   * @return string
   */
  function derived_units_tree_node_header($sob_dto) {
    $link = '';
    $focused_attribute = '';
    $hover_effect_attribute = '';
    if(is_uuid($this->getFocusedUnitUuid()) & $sob_dto->uuid == $this->getFocusedUnitUuid()) {
      $focused_attribute = " focused_item";
    } else {
      $link = cdm_internal_link(path_to_specimen($sob_dto->uuid), NULL);
    }
    if($this->isWithDetails()){
      $hover_effect_attribute = ' unit-label-hover-effect';
    }
    $icon_link_markup = '';
    if($link) {
      $icon_link_markup = '<span class="page-link">' . $link . '</span>';
    }
    return '<div class="unit-header ' . $focused_attribute .'"><div class="unit-label' . $hover_effect_attribute .' "><span class="">' . symbol_for_base_of_record($sob_dto->recordBase->uuid). '</span> ' . $sob_dto->label . $icon_link_markup . '</div></div>';
  }

}