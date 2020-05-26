<?php

/**
 * @file
 * Taxon Node functions.
 *
 * @copyright
 *   (C) 2007-2020 EDIT
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
 * Created a render array with taxon node information like states.
 *
 * @param array $taxon_nodes
 *
 * @gropup compose
 */
function compose_taxon_node_status(array $taxon_nodes){

  $render_array = [];
  if($taxon_nodes && count($taxon_nodes) > 0){
    $label_plural = false;
    $states_markup_tokens = [];
    foreach ($taxon_nodes as $taxon_node){
      $enabled_states = [];
      if($taxon_node->excluded === TRUE){
        $enabled_states[] = t('excluded');
      }
      if($taxon_node->doubtful === TRUE){
        $enabled_states[] = t('doubtful');
      }
      if($taxon_node->unplaced === TRUE){
        $enabled_states[] = t('unplaced');
      }
      if(count($enabled_states) > 0){
        $label_plural |= count($enabled_states) > 1;
        $classification_markup = ' <span class="' . html_class_attribute_ref($taxon_node->classification) .'">[' . $taxon_node->classification->titleCache . ']</span>';
        $states_markup = '<span class="' . html_class_attribute_ref($taxon_node) .'">' . join(', ', $enabled_states) . '%s</span>'; // %s will be replaced by the $classification_markup or by ''

        $states_markup_tokens[] = [
          'states' => $states_markup,
          'classification' => $classification_markup,
          'closing_tag' => '</span>'
        ];
      }
    }
    if(count($states_markup_tokens) > 0){
      $states_markup = [];
      $label_plural |= count($states_markup_tokens) > 1;
      $label = $label_plural ? t('Classificatorical states') : t('Classificatorical state');
      if(count($states_markup_tokens) > 1){
        foreach ($states_markup_tokens as $tokes){
          $states_markup[] = sprintf($tokes['states'], $tokes['classification']);
        }
      } else {
        foreach ($states_markup_tokens as $tokes){
          $states_markup[] = sprintf($tokes['states'], '');
        }
      }
      $render_array['taxon-node-status'] = markup_to_render_array($label . ': ' . join('; ', $states_markup));
      $render_array['taxon-node-status']['#prefix'] = '<div class="taxon-node-status">';
      $render_array['taxon-node-status']['#suffix'] = '</div>';
    }
  }
  return $render_array;
}
