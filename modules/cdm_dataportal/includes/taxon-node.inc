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
  RenderHints::pushToRenderStack("taxon_nodes");
  RenderHints::setFootnoteListKey('taxon_nodes');
  if($taxon_nodes && count($taxon_nodes) > 0){
    $label_plural = false;
    $status_markup_tokens = [];
    foreach ($taxon_nodes as $taxon_node){

      $annotations_and_sources = handle_annotations_and_sources($taxon_node,
        array(
        'sources_as_content' => false,
        'link_to_name_used_in_source' => false,
        'link_to_reference' => true,
        'add_footnote_keys' => false,
        'bibliography_aware' => false),
        '',
        null);

      if(isset($taxon_node->status)){
        $state_label = strtolower($taxon_node->status);
        if(isset($taxon_node->statusNote_L10n->text)){
          $state_label .= ' <span class="status-note">(' . $taxon_node->statusNote_L10n->text . ')</span>';
        }
        $status_markup_tokens[]  = [
          'status' => '<span class="' . html_class_attribute_ref($taxon_node) .'">' . $state_label . $annotations_and_sources['foot_note_keys']  .  '%s</span>', // %s will be replaced by the $classification_markup or by '',
          'classification' => ' <span class="' . html_class_attribute_ref($taxon_node->classification) .'">[' . $taxon_node->classification->titleCache . ']</span>'
        ];
      }

    }
    if(count($status_markup_tokens) > 0){
      $status_markup = [];
      $label_plural |= count($status_markup_tokens) > 1;
      $label = $label_plural ? t('Placement status') : t('Placement status');
      if(count($status_markup_tokens) > 1){
        foreach ($status_markup_tokens as $tokes){
          $status_markup[] = sprintf($tokes['status'], $tokes['classification']);
        }
      } else {
        foreach ($status_markup_tokens as $tokes){
          $status_markup[] = sprintf($tokes['status'], '');
        }
      }
      $render_array['taxon-node-status'] = markup_to_render_array($label . ': ' . join('; ', $status_markup) . theme('cdm_footnotes', array('footnoteListKey' => RenderHints::getFootnoteListKey())));
      $render_array['taxon-node-status']['#prefix'] = '<div class="taxon-node-status">';
      $render_array['taxon-node-status']['#suffix'] = '</div>';
    }
  }
  RenderHints::getFootnoteListKey();
  RenderHints::popFromRenderStack();
  return $render_array;
}
