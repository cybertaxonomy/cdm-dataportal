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
    $state_markup_tokens = [];
    foreach ($taxon_nodes as $taxon_node){
      if(isset($taxon_node->status)){
        $state_label = strtolower($taxon_node->status);
        $classification_markup = ' <span class="' . html_class_attribute_ref($taxon_node->classification) .'">[' . $taxon_node->classification->titleCache . ']</span>';
        $state_markup = '<span class="' . html_class_attribute_ref($taxon_node) .'">' . $state_label . '%s</span>'; // %s will be replaced by the $classification_markup or by ''
        $state_markup_tokens[] = [
          'states' => $state_markup,
          'classification' => $classification_markup,
          'closing_tag' => '</span>'
        ];
      }
    }
    if(count($state_markup_tokens) > 0){
      $state_markup = [];
      $label_plural |= count($state_markup_tokens) > 1;
      $label = $label_plural ? t('Placement status') : t('Placement status');
      if(count($state_markup_tokens) > 1){
        foreach ($state_markup_tokens as $tokes){
          $state_markup[] = sprintf($tokes['states'], $tokes['classification']);
        }
      } else {
        foreach ($state_markup_tokens as $tokes){
          $state_markup[] = sprintf($tokes['states'], '');
        }
      }
      $render_array['taxon-node-status'] = markup_to_render_array($label . ': ' . join('; ', $state_markup));
      $render_array['taxon-node-status']['#prefix'] = '<div class="taxon-node-status">';
      $render_array['taxon-node-status']['#suffix'] = '</div>';
    }
  }
  return $render_array;
}
