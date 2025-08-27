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

      RenderHints::setAnnotationsAndSourceConfig(annotation_and_source_config_taxon_node());
      $annotations_and_sources = handle_annotations_and_sources($taxon_node);

      if(isset($taxon_node->status)){
        $state_label = strtolower($taxon_node->status->representation_L10n);
        if(isset($taxon_node->placementNote_L10n)){
          $state_label .= ' <span class="placement-note">(' . $taxon_node->placementNote_L10n . ')</span>';
        }
        $classification = cdm_ws_get(CDM_WS_CLASSIFICATION, array($taxon_node->classificationUUID));
        $status_markup_tokens[$state_label . '-' . $taxon_node->uuid]  = [
          'status' => '<span class="' . html_class_attribute_ref($taxon_node) .'">' . $state_label . $annotations_and_sources->footNoteKeysMarkup()  .  '%s</span>', // %s will be replaced by the $classification_markup or by '',
          'classification' => ' <span class="' . html_class_attribute_ref($classification) .'">[' . $classification->titleCache . ']</span>'
        ];
      }

    }
    if(count($status_markup_tokens) > 0){
      ksort($status_markup_tokens);
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
      $render_array['taxon-node-status'] = markup_to_render_array($label . ': ' . join('; ', $status_markup) . render_footnotes(RenderHints::getFootnoteListKey()));
      $render_array['taxon-node-status']['#prefix'] = '<div class="taxon-node-status">';
      $render_array['taxon-node-status']['#suffix'] = '</div>';
    }
  }
  RenderHints::setFootnoteListKey(null);
  RenderHints::popFromRenderStack();
  return $render_array;
}


function compose_taxon_node_new_status(array $taxon_nodes){

    $render_array = [];
    RenderHints::pushToRenderStack("taxon_nodes");
    RenderHints::setFootnoteListKey('taxon_nodes');
    if($taxon_nodes && count($taxon_nodes) > 0){
        $label_plural = false;
        $status_markup_tokens = [];
        foreach ($taxon_nodes as $taxon_node){

            RenderHints::setAnnotationsAndSourceConfig(annotation_and_source_config_taxon_node());
            $annotations_and_sources = handle_new_annotations_and_sources($taxon_node);
            $state_label = '';
            if(isset($taxon_node->status)) {
              if (is_string($taxon_node->status)) {
                $state_label = strtolower($taxon_node->status);
              }
            }

            if (isset_not_empty($taxon_node->placementNote)){
                $state_label .= ' <span class="placement-note">(' . $taxon_node->placementNote . ')</span>';
            }

            $classificationUuid = '';
            $classificationLabel = '';
            if (isset_not_empty($taxon_node->classificationUuid)){
              $classificationUuid = $taxon_node->classificationUuid;
              $classificationLabel = $taxon_node->classificationLabel;
            }
            if (isset_not_empty($state_label)){
              $state_label = str_replace("%", "%%", $state_label);
              $status_markup_tokens[$state_label . '-' . $taxon_node->uuid]  = [
                'status' => '<span class="cdm:TaxonNodeDto uuid:'. $taxon_node->uuid.'">' . $state_label . $annotations_and_sources->footNoteKeysMarkup()  .  '%s</span>', // %s will be replaced by the $classification_markup or by '',
                'classification' => ' <span class="cdm:Classification uuid:'. $classificationUuid.'">[' . $classificationLabel . ']</span>'
              ];
            }



        }
        if(count($status_markup_tokens) > 0){
            ksort($status_markup_tokens);
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
            $render_array['taxon-node-status'] = markup_to_render_array($label . ': ' . join('; ', $status_markup) . render_footnotes(RenderHints::getFootnoteListKey()));
            $render_array['taxon-node-status']['#prefix'] = '<div class="taxon-node-status">';
            $render_array['taxon-node-status']['#suffix'] = '</div>';
        }
    }
    RenderHints::setFootnoteListKey(null);
    RenderHints::popFromRenderStack();
    return $render_array;
}
