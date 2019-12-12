<?php

/**
 * @file
 * Hooks provided by the CDM DataPortal module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Alter the feature block list.
 *
 * Modules implementing this hook can add, remove or modify
 * feature blocks of the taxon general page part and of the
 * specimen page part.
 *
 * @param $block_list
 *   the array of blocks as returned by list_blocks()
 * @param $taxon
 *   Optional CDM Taxon instance, currently only given if
 *  called from within the taxon general page part
 *
 * @return array
 *  the altered $block_list
 *
 */
function hook_cdm_feature_node_blocks_alter(&$block_list, $taxon = NULL) {

  if ($taxon) {
    // taxon is only given if called from within the taxon general page part
    $numberOfChildren = count(
      cdm_ws_get(CDM_WS_PORTAL_TAXONOMY_CHILDNODES_OF_TAXON,
        [
          get_current_classification_uuid(),
          $taxon->uuid,
        ]
      )
    );
    $subRank = 'sub taxa';
    if ($taxon->name->rank->titleCache == "Genus") {
      $subRank = "species";
    }
    else {
      if ($taxon->name->rank->titleCache == "Species") {
        if ($numberOfChildren == 1) {
          $subRank = "infraspecific taxon";
        }
        else {
          $subRank = "infraspecific taxa";
        }
      }
    }
    if ($numberOfChildren > 0) {

      $pseudo_feature_weights = get_array_variable_merged(CDM_PSEUDO_FEATURE_BLOCK_WEIGHTS, CDM_PSEUDO_FEATURE_BLOCK_WEIGHTS_DEFAULT);
      $feature_num_of_taxa = make_pseudo_feature('Number of Taxa', PSEUDO_FEATURE_NUMBER_OF_TAXA);
      $block = feature_block(t('Number of Taxa'), $feature_num_of_taxa);
      $block->content[] = compose_feature_block_wrap_elements([
        markup_to_render_array(
          '<ul class="feature-block-elements"><li>'
          . $numberOfChildren . " " . $subRank
          . '</li></ul>')]
        , $feature_num_of_taxa);
      $block_list[$pseudo_feature_weights[PSEUDO_FEATURE_NUMBER_OF_TAXA]] = $block;
      cdm_toc_list_add_item('Number of Taxa', 'number-of-taxa', NULL, TRUE);
    }
  }

  return $block_list;
}

/**
 * Alter the content of a feature block.
 *
 * Modules implementing this hook can add, remove or modify
 * feature blocks contents of the taxon general page part and of the
 * specimen page part.
 *
 * @param array $block_content
 *   Drupal render array for the content of the feature block
 * @param object $feature
 *   The feature this block belongs to
 * @param object $elements
 *   An array of CDM DescriptionElement instances which are being displayed in
 *   this block. Even if this array is passed as reference it should not being
 *   altered. Passing by reference is only recommended to reduce the memory
 *   footprint.
 */
function hook_cdm_feature_node_block_content_alter(&$block_content, $feature, &$elements){
    if($feature->uuid == UUID_DISTRIBUTION){
        $block_content['my_custom_render_element'] = array(
            '#type' => 'markup',
            '#markup' => '<h1>Hello World</h1>',
            '#weight' => 99, // Show at the bottom of the block
        );
    }
}

/*
 * Alter the merged feature tree for a taxon profile page.
 *
 * @param $merged_tree
 *   The $merged_tree as produced by merged_taxon_feature_tree($taxon)
 * @param $taxon
 *   A CDM Taxon instance
 */
function hook_merged_taxon_feature_tree_alter($taxon, &$merged_tree){

  // find the distribution feature node
  $distribution_node =& cdm_feature_tree_find_node($merged_tree->root->childNodes, UUID_DISTRIBUTION);
  // remove all TextData
  $distribution_node->descriptionElements['TextData'] = array();
}

/**
 * @} End of "addtogroup hooks".
 */
