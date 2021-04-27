<?php


class DerivationTreeComposer {

  private $root_unit_dtos;
  private $focused_unit_uuid = null;
  private $with_details = false;
  private $collapsible = false;

  /**
   * @return bool
   */
  public function isCollapsible() {
    return $this->collapsible;
  }

  /**
   * @param bool $collapsible
   */
  public function setCollapsible($collapsible) {
    $this->collapsible = $collapsible;
  }

  /**
   * @return bool
   */
  public function isWithDetails() {
    return $this->with_details;
  }

  public function collapsibleItemClassAttribute($has_sub_derivatives) {
    return $this->isCollapsible() & $has_sub_derivatives === TRUE ? ' tree-item-collapsible' : '';

  }

  public function subItemsClassAttribute($has_sub_derivatives) {
    return $has_sub_derivatives === true ? ' with-sub-items' : '';
  }

  public function itemCollapsedStateClassAttribute() {
    return $this->isCollapsible() ? ' collapsed' : '';
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

    _add_js_derivation_tree('.item-tree');

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

    $list_items = $this->derived_units_as_list_items($this->root_unit_dtos);

    $root_items = [
      '#theme' => 'item_list',
      '#type' => 'ul',
      '#prefix' => '<div class="item-tree">',
      '#suffix' => '</div>',
      '#attributes' => [
      ],
      '#items' => $list_items,
    ];

    $root_items['footnotes'] = markup_to_render_array(render_footnotes());
    RenderHints::popFromRenderStack();

    return $root_items;

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
      $has_sub_derivatives = isset($sob_dto->derivatives) && sizeof($sob_dto->derivatives) > 0;
      $item = [];
      $item['class'] = [
        'derived-unit-item ',
        html_class_attribute_ref($sob_dto),
        $this->itemCollapsedStateClassAttribute(),
        $this->collapsibleItemClassAttribute($has_sub_derivatives),
        $this->subItemsClassAttribute($has_sub_derivatives)
      ];
      // data" element of the array is used as the contents of the list item
      $item['data'] = [];
      $unit_content_markup = '';
      if($this->with_details){
        $unit_dto_render_array = compose_cdm_specimen_or_observation_dto_details_grid($sob_dto);
        $this->applyUnitContentGrid($unit_dto_render_array);
        $unit_content_markup = drupal_render($unit_dto_render_array);
      }
      $item['data'] = $this->applyItemWrapper(
        $this->derived_units_tree_node_header($sob_dto)
        . $unit_content_markup
        , $has_sub_derivatives);
      if ($has_sub_derivatives) {
        usort($sob_dto->derivatives, 'compare_specimen_or_observation_dtos');
        // children are displayed in a nested list.
        $item['children'] = $this->derived_units_as_list_items($sob_dto->derivatives);
      }
      $list_items[] = $item;
    }

    return $list_items;
  }


  /**
   * @param $unit_dto_render_array
   *
   * @return mixed
   *
   * FIXME: merge into compose_cdm_specimen_or_observation_dto_details_grid() again
   */
  function applyUnitContentGrid(&$unit_dto_render_array){
    $unit_dto_render_array['#prefix'] = '<div class="unit-content derived-unit-details-grid">';
    $unit_dto_render_array['#suffix'] = '</div>';
    return $unit_dto_render_array;
  }

  function applyItemWrapper($item_markup, $has_children){
    $sub_item_line_class_attribute = $has_children ? ' item-wrapper-with-sub-items' : '';
    return '<div class="item-wrapper' . $sub_item_line_class_attribute . '">' // allows to apply the borders between .derived-unit-tree-root and .unit-content
      . $item_markup
    . '</div>';
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
    $collapse_open_icons = '';
    $unit_header_wrapper_sub_items_class_attr = '';
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
    $has_sub_derivatives = isset($sob_dto->derivatives) && sizeof($sob_dto->derivatives) > 0;
    if($has_sub_derivatives){
      if($this->isCollapsible()){
        $collapse_open_icons = '<span class="tree-node-symbol tree-node-symbol-collapsible">'
          . font_awesome_icon_markup('fa-' . SYMBOL_COLLAPSIBLE_CLOSED)
          . '</span>';
      } else {
        $collapse_open_icons = '<span class="tree-node-symbol">'
          . font_awesome_icon_markup('fa-' . SYMBOL_COLLAPSIBLE_OPEN)
          . '</span>';
      }
    }
    if($has_sub_derivatives){
      $unit_header_wrapper_sub_items_class_attr = ' unit-header-wrapper-with-sub-items';
    }
    if( $sob_dto->type == 'FieldUnit' ){
      $label = $sob_dto->label;
    } else {
      $label = $sob_dto->specimenIdentifier;
    }
    return '<div class="unit-header-wrapper' . $unit_header_wrapper_sub_items_class_attr . $focused_attribute . '"><div class="unit-header"><div class="unit-label' . $hover_effect_attribute .' ">' . $collapse_open_icons . '<span class="symbol">' . $this->symbol_markup($sob_dto) . '</span>' . $label . $icon_link_markup . '</div></div></div>';
  }

  /**
   * @param $sob_dto
   *
   * @return String
   */
  public function symbol_markup($sob_dto) {
    if(count($sob_dto->specimenTypeDesignations)){
      $base_of_record_symbol = font_awesome_icon_stack([
        symbol_for_base_of_record($sob_dto->recordBase->uuid,
          [
          'class' => ['fas', 'fa-stack-1x'],
          'style' => ['--fa-primary-color: red;']
        ]),
        font_awesome_icon_markup('fa-tag',
          [
            'class' => ['fas', 'fa-rotate-180', 'fa-stack-1x'],
            'style' => ['color: red; vertical-align:bottom; text-align:left; font-size:.75em; bottom: -0.35em;']
          ]
        )
      ]);
    } else {
      $base_of_record_symbol = symbol_for_base_of_record($sob_dto->recordBase->uuid);
    }
    return $base_of_record_symbol;
  }

}