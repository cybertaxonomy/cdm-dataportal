<?php

/**
 * @file
 * Functions for dealing with CDM entities from the package model.taxon
 *
 * @copyright
 *   (C) 2007-2016 EDIT
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
 * Functions which are composing Drupal render arrays
 *
 * The cdm_dataportal module needs to compose rather complex render arrays from
 * the data returned by the CDM REST service. The compose functions are
 * responsible for creating the render arrays.
 *
 * All these functions are also implementations of the compose_hook()
 * which is used in the proxy_content() function.
 * @}
 */

define('TAXON_TYPE_SYNONYM', 'Synonym');
define('TAXON_TYPE_MISAPPLIEDNAME', 'MisappliedName');

/**
 * Returns HTML for misapplied names and invalid designations.
 *
 * Both relation types are currently treated the same!
 *
 * @param object taxonRelationships
 *   A TaxonRelationshipsDTO, see taxon/{uuid}/taxonRelationshipsDTO
 *
 * @param object focusedTaxon
 *  The taxon being in the focus of the application
 *
 * @return string
 *    the rendered html
 */
function cdm_taxonRelationships($taxonRelationshipsDTO, $focusedTaxon){

  static $relationship_type_order_weights = [
    //
    UUID_PROPARTE_MISAPPLIEDNAME_FOR => 0,
    UUID_PARTIAL_MISAPPLIEDNAME_FOR => 0,
    //
    UUID_MISAPPLIED_NAME_FOR  => 1,
    //
    UUID_PROPARTE_SYNONYM_FOR  => 2,
    UUID_PARTIAL_SYNONYM_FOR  => 2
  ];

  if (!$taxonRelationshipsDTO || $taxonRelationshipsDTO->size < 1) {
    return null;
  }

  static $dedup_rel_type_uuids = array(
    UUID_MISAPPLIED_NAME_FOR,
    UUID_PARTIAL_MISAPPLIEDNAME_FOR,
    UUID_PROPARTE_MISAPPLIEDNAME_FOR
  );

  RenderHints::pushToRenderStack('taxon_relationships');
  $footnote_list_key = 'taxon_relationships';
  RenderHints::setFootnoteListKey($footnote_list_key);

  $misapplied = array();
  $joined_refs = array();

  $taxon_relationship_types = variable_get(CDM_TAXON_RELATIONSHIP_TYPES, unserialize(CDM_TAXON_RELATIONSHIP_TYPES_DEFAULT));

  // Aggregate misapplied names having the same fullname:
  //  - deduplicate misapplied names, so that the same name is not shown multiple times in case
  //    the duplicates only have different sensu references with detail and appended phrase (see #5647)
  //  - show only the author team as short citation for the sec reference
  //  - show the according reference as footnote to this short citation
  //
  // Example:
  // "Xenoxylum foobar" sensu Grumbach¹; sensu Lem²
  //    1. Novel marsian species, Grumbach, 2022
  //    2. Flora solaris, Lem, 2019
  if (isset($taxonRelationshipsDTO->relations[0])) {
    foreach($taxonRelationshipsDTO->relations as $taxon_relation){

      if (in_array($taxon_relation->type->uuid, $taxon_relationship_types)) {

        if (in_array($taxon_relation->type->uuid, $dedup_rel_type_uuids)) {

          RenderHints::pushToRenderStack('misapplied_name_for'); // TODO the render path string should in future come from $taxonRelation->type->...

          // full name with relation symbol, rel sec as de-duplication key
          // the sensu part will be removed from the key below in case it is present
          $name_text = join(' ', cdm_tagged_text_values($taxon_relation->taggedText, array('name')));
          $full_name_key = cdm_tagged_text_to_string($taxon_relation->taggedText, array('symbol', 'appendedPhrase'));
          $symbol_text = join(' ', cdm_tagged_text_values($taxon_relation->taggedText, array('symbol')));
          $full_name_key = $name_text . ' ' . str_replace($name_text, '', $full_name_key) . ' ' . $symbol_text;

          cdm_taxonRelationships_process_relationship_dto($taxon_relation, $misapplied, $joined_refs, $relationship_type_order_weights , $full_name_key);

          RenderHints::popFromRenderStack();
        } else {
          RenderHints::pushToRenderStack('other_taxon_relationship');
          // All relationship types except misapplied_name_for and invalid_designation_for.

          $taxon_relationships_lines[] = cdm_tagged_text_to_markup($taxon_relation->taggedText);

          RenderHints::popFromRenderStack();
        }
      }
    } // END loop over $taxonRelationshipsDTO->relations

  }

  // Sort the $joined_refs and create footnotes and footnote keys.
  uasort($joined_refs, function($a, $b) {
    if ($a['order_by_key'] == $b['order_by_key']) {
      return 0;
    }
    return ($a['order_by_key'] < $b['order_by_key']) ? -1 : 1;
  });


  foreach ($joined_refs as $ref_key => $sensu_strings) {
    if(!empty($sensu_strings)) {
      $ref_key_tokens = explode('#', $ref_key);
      $sensu_uuid = $ref_key_tokens[0];
      $sensu_reference = cdm_ws_get(CDM_WS_REFERENCE, $sensu_uuid);
      if(!$sensu_reference){
        drupal_set_message("Problem fetching sensu reference with uuid " . $sensu_uuid, 'error');
      }
      if($sensu_strings['order_by_key'] != $sensu_reference->titleCache){
        $sensu_reference_markup = cdm_reference_markup($sensu_reference);
        $footnote_key = FootnoteManager::addNewFootnote($footnote_list_key, $sensu_reference_markup);
        $footnote_key = render_footnote_key($footnote_key);
        $joined_refs[$ref_key]['markup'] = '<span class="sensu">' . $sensu_strings['markup'] . $footnote_key . '</span>';
      }
    }
  }

  // ---- Generate output ---- //
  $out = '<div class="taxon-relationships">';
  if (count($misapplied) > 0) {
    ksort($misapplied); // order the misapplied by scientific name
    $out .= '<ul class="misapplied">';

    // create the list entries per misapplied name
    foreach ($misapplied as $misapplied_name) {
      $misapplied_name_markup = $misapplied_name['out'];
      // all sensu and auct. for this MAN
      if (isset($misapplied_name['sensu_uuids'])) {
        $sensu_refs_with_fkey = array();
        foreach ($misapplied_name['sensu_uuids'] as $sensu_data) {
          if($sensu_data['uuid']){
            $joined_ref_key = $sensu_data['uuid'] . ($sensu_data['citation_detail'] ? '#' .  $sensu_data['citation_detail'] : '');
            $sensu_refs_with_fkey[$sensu_data['prefix'] . $joined_refs[$joined_ref_key]['order_by_key']] = $sensu_data['prefix'] . $joined_refs[$joined_ref_key]['markup'];
          } else {
            $sensu_refs_with_fkey[$sensu_data['prefix']] = $sensu_data['prefix'];
          }
        }
        ksort($sensu_refs_with_fkey);
        $sensu_refs_with_fkey_markup = join('; ', $sensu_refs_with_fkey);
        if(strpos($misapplied_name_markup, '{PLACEHOLDER_secReference}') !== false){
          $misapplied_name_markup = str_replace('{PLACEHOLDER_secReference}', $sensu_refs_with_fkey_markup, $misapplied_name_markup);
          // just remove the appendedPhrase placeholder as was included in the sensu_refs_with_fkey_markup
          $misapplied_name_markup = str_replace('{PLACEHOLDER_appendedPhrase}', '', $misapplied_name_markup);
        } else {
          // no sec ref so there could only be the appended phrase
          $misapplied_name_markup = str_replace('{PLACEHOLDER_appendedPhrase}', $sensu_refs_with_fkey_markup, $misapplied_name_markup);
        }
      }

      // append the err. sec. if there is any for this MAN
      if (isset($misapplied_name['relsec_uuids'])) {
        $relsec_refs_with_fkey = array();
        foreach ($misapplied_name['relsec_uuids'] as $relsec_data) {
          if($relsec_data['uuid']) {
            $relsec_refs_with_fkey[$relsec_data['prefix'] . $joined_refs[$relsec_data['uuid']]['order_by_key']] = $relsec_data['prefix'] . $joined_refs[$relsec_data['uuid']]['markup'];
          }
        }
        ksort($relsec_refs_with_fkey);
        $relsec_refs_with_fkey_markup = join('; ', $relsec_refs_with_fkey);
        $misapplied_name_markup = str_replace('{PLACEHOLDER_relSecReference}', $relsec_refs_with_fkey_markup, $misapplied_name_markup);
      }
      // final line
      $out .= '<li class="synonym"><span class="misapplied">' . $misapplied_name_markup . ' </span></li>';
    }
    $out .= '</ul>';
  }

  if (isset($taxon_relationships_lines) && is_array($taxon_relationships_lines) && count($taxon_relationships_lines) > 0) {
    $out .= '<ul class="taxonRelationships">';
    foreach ($taxon_relationships_lines as $taxon_relationship_line) {
      $out .= '<li class="synonym">' . $taxon_relationship_line . '</li>';
    }
    $out .= '</ul>';
  }

  $footnotes = render_footnotes($footnote_list_key, 'li');

  $out .= '<ul class="footnotes">' . $footnotes . '</ul>';
  $out .= '</div>';

  RenderHints::popFromRenderStack();
  return $out;
}

/**
 * Returns HTML for misapplied names and invalid designations.
 *
 * Both relation types are currently treated the same!
 *
 * @param object taxonRelationships
 *   A TaxonRelationshipsDTO, see taxon/{uuid}/taxonRelationshipsDTO
 *
 * @param object focusedTaxon
 *  The taxon being in the focus of the application
 *
 * @return string
 *    the rendered html
 */
function cdm_new_taxonRelationships($taxonRelationshipsDTO, $focusedTaxon){

    static $relationship_type_order_weights = [
        //
        UUID_PROPARTE_MISAPPLIEDNAME_FOR => 0,
        UUID_PARTIAL_MISAPPLIEDNAME_FOR => 0,
        //
        UUID_MISAPPLIED_NAME_FOR  => 1,
        //
        UUID_PROPARTE_SYNONYM_FOR  => 2,
        UUID_PARTIAL_SYNONYM_FOR  => 2
    ];

    if (!$taxonRelationshipsDTO || $taxonRelationshipsDTO->count < 1) {
        return null;
    }

    static $dedup_rel_type_uuids = array(
        UUID_MISAPPLIED_NAME_FOR,
        UUID_PARTIAL_MISAPPLIEDNAME_FOR,
        UUID_PROPARTE_MISAPPLIEDNAME_FOR
    );

    RenderHints::pushToRenderStack('taxon_relationships');
    $footnote_list_key = 'taxon_relationships';
    RenderHints::setFootnoteListKey($footnote_list_key);

    $misapplied = array();
    $joined_refs = array();

    $taxon_relationship_types = variable_get(CDM_TAXON_RELATIONSHIP_TYPES, unserialize(CDM_TAXON_RELATIONSHIP_TYPES_DEFAULT));

    // Aggregate misapplied names having the same fullname:
    //  - deduplicate misapplied names, so that the same name is not shown multiple times in case
    //    the duplicates only have different sensu references with detail and appended phrase (see #5647)
    //  - show only the author team as short citation for the sec reference
    //  - show the according reference as footnote to this short citation
    //
    // Example:
    // "Xenoxylum foobar" sensu Grumbach¹; sensu Lem²
    //    1. Novel marsian species, Grumbach, 2022
    //    2. Flora solaris, Lem, 2019
   // if (isset($taxonRelationshipsDTO->relations[0])) {
    foreach($taxonRelationshipsDTO->items as $taxon_relation){

        if (in_array($taxon_relation->relTypeUuid, $taxon_relationship_types)) {

            if (in_array($taxon_relation->relTypeUuid, $dedup_rel_type_uuids)) {

                RenderHints::pushToRenderStack('misapplied_name_for'); // TODO the render path string should in future come from $taxonRelation->type->...

                // full name with relation symbol, rel sec as de-duplication key
                // the sensu part will be removed from the key below in case it is present
                $name_text = join(' ', cdm_tagged_text_values($taxon_relation->taggedLabel, array('name')));
                $full_name_key = cdm_tagged_text_to_string($taxon_relation->taggedLabel, array('symbol', 'appendedPhrase'));
                $symbol_text = join(' ', cdm_tagged_text_values($taxon_relation->taggedLabel, array('symbol')));
                $full_name_key = $name_text . ' ' . str_replace($name_text, '', $full_name_key) . ' ' . $symbol_text;

                cdm_new_taxonRelationships_process_relationship_dto($taxon_relation, $misapplied, $joined_refs, $relationship_type_order_weights , $full_name_key);

                RenderHints::popFromRenderStack();
            } else {
                RenderHints::pushToRenderStack('other_taxon_relationship');
                // All relationship types except misapplied_name_for and invalid_designation_for.

                $taxon_relationships_lines[] = cdm_tagged_text_to_markup($taxon_relation->taggedLabel);

                RenderHints::popFromRenderStack();
            }
        }
    } // END loop over $taxonRelationshipsDTO->relations

    //}

    // Sort the $joined_refs and create footnotes and footnote keys.
    uasort($joined_refs, function($a, $b) {
        if ($a['order_by_key'] == $b['order_by_key']) {
            return 0;
        }
        return ($a['order_by_key'] < $b['order_by_key']) ? -1 : 1;
    });

    // ---- Generate output ---- //
    $out = '<div class="taxon-relationships">';
    if (count($misapplied) > 0) {
        ksort($misapplied); // order the misapplied by scientific name
        $out .= '<ul class="misapplied">';

        // create the list entries per misapplied name
        foreach ($misapplied as $misapplied_name) {
            //first sort the sensu references then create footnotes

            $misapplied_name_markup = $misapplied_name['out'];
            // all sensu and auct. for this MAN
            if (isset($misapplied_name['sensu_uuids'])) {
                $sensu_refs_with_fkey = array();
                usort($misapplied_name['sensu_uuids'], 'compare_MAN_citation');
                foreach ($misapplied_name['sensu_uuids'] as $sensu_data) {

                    if($sensu_data['uuid']){
                        $citation_detail = '';
                        if (isset_not_empty($sensu_data['citation_detail'] )){
                            $citation_detail = '#' .$sensu_data['citation_detail'];
                        }
                        $joined_ref_key = $sensu_data['uuid'] . $citation_detail;
                        /****/

                        $sensu_reference_markup = $joined_refs[$joined_ref_key]['longCitation'];
                        $footnote_key = '';
                        if ($sensu_reference_markup != $joined_refs[$joined_ref_key]['citation']) {
                            $footnote_key = FootnoteManager::addNewFootnote($footnote_list_key, $joined_refs[$joined_ref_key]['markup']);
                            $footnote_key = render_footnote_key($footnote_key);
                        }
                        if (isset_not_empty($joined_refs[$joined_ref_key]['detail'])) {
                            $detail = ': ' . $joined_refs[$joined_ref_key]['detail'];
                        } else {
                            $detail = '';
                        }
                        $joined_refs[$joined_ref_key]['markup_with_footnote_key'] = '<span class="sensu">' . $joined_refs[$joined_ref_key]['citation'] .  $footnote_key . $detail . '</span>';


                        /****/

                        $sensu_refs_with_fkey[$sensu_data['prefix'] .  $joined_ref_key] = $sensu_data['prefix'] . $joined_refs[$joined_ref_key]['markup_with_footnote_key'];
                    } else {
                        $sensu_refs_with_fkey[$sensu_data['prefix']] = $sensu_data['out'];
                    }
                }

                $sensu_refs_with_fkey_markup = join('; ', $sensu_refs_with_fkey);
                if(strpos($misapplied_name_markup, '{PLACEHOLDER_secReference}') !== false){
                    $misapplied_name_markup = str_replace('{PLACEHOLDER_secReference}', $sensu_refs_with_fkey_markup, $misapplied_name_markup);
                    // just remove the appendedPhrase placeholder as was included in the sensu_refs_with_fkey_markup
                    $misapplied_name_markup = str_replace('{PLACEHOLDER_appendedPhrase}', '', $misapplied_name_markup);
                } else {
                    // no sec ref so there could only be the appended phrase
                    $misapplied_name_markup = str_replace('{PLACEHOLDER_appendedPhrase}', $sensu_refs_with_fkey_markup, $misapplied_name_markup);
                }
            }


            // append the err. sec. if there is any for this MAN
            if (isset($misapplied_name['relsec_uuids'])) {
                $relsec_refs_with_fkey = array();
                usort($misapplied_name['relsec_uuids'], 'compare_MAN_citation');
                foreach ($misapplied_name['relsec_uuids'] as $relsec_data) {
                    $citation_detail = '';
                    if (isset_not_empty($relsec_data['citation_detail'] )){
                        $citation_detail = '#' .$relsec_data['citation_detail'];
                    }
                    $joined_ref_key = $relsec_data['uuid'] . $citation_detail;
                    $sensu_reference_markup = $joined_refs[$joined_ref_key]['markup'];
                    $footnote_key = FootnoteManager::addNewFootnote($footnote_list_key, $sensu_reference_markup);
                    $footnote_key = render_footnote_key($footnote_key);
                    if (isset_not_empty($joined_refs[$joined_ref_key]['detail'])){
                        $detail = ': '.$joined_refs[$joined_ref_key]['detail'];
                    }else{
                        $detail = '';
                    }
                    $markup = '<span class="sensu">' . $joined_refs[$joined_ref_key]['citation'] .  $footnote_key . $detail . '</span>';

                    if($relsec_data['uuid']) {
                        $relsec_refs_with_fkey[$relsec_data['prefix'] . $joined_refs[$relsec_data['uuid']]['citation']] = $relsec_data['prefix'] . $markup;
                    }
                }

                $relsec_refs_with_fkey_markup = join('; ', $relsec_refs_with_fkey);
                $misapplied_name_markup = str_replace('{PLACEHOLDER_relSecReference}', $relsec_refs_with_fkey_markup, $misapplied_name_markup);
            }
            // final line
            $out .= '<li class="synonym"><span class="misapplied">' . $misapplied_name_markup . ' </span></li>';
        }
        $out .= '</ul>';
    }

    if (isset($taxon_relationships_lines) && is_array($taxon_relationships_lines) && count($taxon_relationships_lines) > 0) {
        $out .= '<ul class="taxonRelationships">';
        foreach ($taxon_relationships_lines as $taxon_relationship_line) {
            $out .= '<li class="synonym">' . $taxon_relationship_line . '</li>';
        }
        $out .= '</ul>';
    }

    $footnotes = render_footnotes($footnote_list_key, 'li');

    $out .= '<ul class="footnotes">' . $footnotes . '</ul>';
    $out .= '</div>';

    RenderHints::popFromRenderStack();
    return $out;
}


/**
 * @param $taxon_relation
 * @param $misapplied
 * @param $joined_refs
 * @param $relationship_type_weights
 * @param $name_dedup_key
 */
function cdm_taxonRelationships_process_relationship_dto($taxon_relation, &$misapplied, &$joined_refs, $relationship_type_weights, $name_dedup_key) {

  $appended_phrase_text = join(' ', cdm_tagged_text_values($taxon_relation->taggedText, array('appendedPhrase')));
  // remove/extract appendedPhrase, secReference, relSecReference and add placeholders if needed
  tagged_text_extract($taxon_relation->taggedText, 'appendedPhrase', true);
  $sensu_tagged_text = tagged_text_extract_reference_and_detail($taxon_relation->taggedText, "secReference", true);
  $relsec_tagged_text = tagged_text_extract_reference_and_detail($taxon_relation->taggedText, "relSecReference", true);

  // prepend the weight value for the relationship type to the $name_dedup_key so that the key can be used for ordering the
  // entries in the $misapplied array
  if(isset($relationship_type_weights[$taxon_relation->type->uuid])){
    $reltype_weight = $relationship_type_weights[$taxon_relation->type->uuid];
  } else {
    $reltype_weight = 99;
  }
  $name_dedup_key = str_pad( $reltype_weight, 2, '0', STR_PAD_LEFT) . '-' . $name_dedup_key;
  $skipTags = array();
  $skipTags[] = "secMicroReference";
  if (isset($sensu_tagged_text[1])) {
    // for de-duplication everything else needs to be equal except for appendedPhrase + MAN.sec + MAN.secDetail. see #7658#note-21
    $name_dedup_key = str_replace(cdm_tagged_text_to_string($sensu_tagged_text), ' ', $name_dedup_key);
    $appended_phrase_text = $appended_phrase_text . array_shift($sensu_tagged_text)->text; // remove first element which contains the "sensu", this will be added later in this code
    $sensu_citation_detail = trim(join(' ', cdm_tagged_text_values($sensu_tagged_text, array('secMicroReference'))));
    //TODO: add footnote placeholder in front of the secMicroReference
    $sensu_citation_short_markup = cdm_tagged_text_to_markup($sensu_tagged_text);
    //only one footnote for a reference undependent of detail
    $sensu_citation_short = cdm_tagged_text_to_string($sensu_tagged_text, array('secMicroReference'));
    $sensu_uuid = $sensu_tagged_text[0]->entityReference->uuid;
    $name_dedup_key = preg_replace('/\s+/', ' ', $name_dedup_key); // sanitize multiple whitespace characters
    $misapplied[$name_dedup_key]['sensu_uuids'][] = array('uuid' => $sensu_uuid, 'prefix' => $appended_phrase_text, 'citation_detail' => $sensu_citation_detail);
    $ref_key = $sensu_uuid . ($sensu_citation_detail ? '#' . $sensu_citation_detail : '');
    if (!isset($joined_refs[$ref_key])) {
      $joined_refs[$ref_key] = array(
        'order_by_key' => $sensu_citation_short,
        'markup' => $sensu_citation_short_markup // the footnote key will be appended later
      );
    }
  } else if ($appended_phrase_text) {
    // appended phrase without reference
    $name_dedup_key = preg_replace('/\s+/', ' ', $name_dedup_key); // sanitize multiple whitespace characters
    $misapplied[$name_dedup_key]['sensu_uuids'][] = array('uuid' => null, 'prefix' => $appended_phrase_text);
  }

  if (isset($relsec_tagged_text[1])) {
    $appended_phrase_text = array_shift($relsec_tagged_text)->text; // remove first element which contains the "err. sec", this will be added later in this code
    $relsec_citation_detail = trim(join(' ', cdm_tagged_text_values($relsec_tagged_text, array('secMicroReference'))));
    $relsec_citation_short_markup = cdm_tagged_text_to_markup($relsec_tagged_text);
    $relsec_citation_short = cdm_tagged_text_to_string($relsec_tagged_text, array('secMicroReference'));
    $relsec_uuid = $relsec_tagged_text[0]->entityReference->uuid;
    $misapplied[$name_dedup_key]['relsec_uuids'][] = array('uuid' => $relsec_uuid, 'prefix' => $appended_phrase_text, 'citation_detail' => $relsec_citation_detail);;
    $ref_key = $relsec_uuid . ($relsec_citation_detail ? '#' . $relsec_citation_detail : '');
    if (!isset($joined_refs[$ref_key])) {
      $joined_refs[$ref_key] = array(
        'order_by_key' => $relsec_citation_short,
        'markup' => $relsec_citation_short_markup // the footnote key will be appended later
      );
    }
  }

  if (!isset($misapplied[$name_dedup_key]['out'])) {
    // helpful for debugging: // cdm_tagged_text_add_options($taxon_relation->taggedText, array(array('filter-type' => 'symbol', 'attributes' => array('title' =>  array($taxon_relation->type->representations[0]->label)))));
    $misapplied[$name_dedup_key]['out'] = cdm_tagged_text_to_markup($taxon_relation->taggedText);
  } else {
    // We need to add the anchors for all of the other misapplied names not
    // being rendered explicitly.
    $misapplied[$name_dedup_key]['out'] = uuid_anchor($taxon_relation->taxonUuid, $misapplied[$name_dedup_key]['out']);
  }

}
/**
 * @param $taxon_relation
 * @param $misapplied
 * @param $joined_refs
 * @param $relationship_type_weights
 * @param $name_dedup_key
 */
function cdm_new_taxonRelationships_process_relationship_dto($taxon_relation, &$misapplied, &$joined_refs, $relationship_type_weights, $name_dedup_key) {

    $appended_phrase_text = join(' ', cdm_tagged_text_values($taxon_relation->taggedLabel, array('appendedPhrase')));
    // remove/extract appendedPhrase, secReference, relSecReference and add placeholders if needed
    tagged_text_extract($taxon_relation->taggedLabel, 'appendedPhrase', true);
    $sensu_tagged_text = tagged_text_extract_reference_and_detail($taxon_relation->taggedLabel, "secReference", true);
    $relsec_tagged_text = tagged_text_extract_reference_and_detail($taxon_relation->taggedLabel, "relSecReference", true);

    // prepend the weight value for the relationship type to the $name_dedup_key so that the key can be used for ordering the
    // entries in the $misapplied array
    if(isset($relationship_type_weights[$taxon_relation->relTypeUuid])){
        $reltype_weight = $relationship_type_weights[$taxon_relation->relTypeUuid];
    } else {
        $reltype_weight = 99;
    }
    $name_dedup_key = str_pad( $reltype_weight, 2, '0', STR_PAD_LEFT) . '-' . $name_dedup_key;

    if (isset($sensu_tagged_text[1])) {
        // for de-duplication everything else needs to be equal except for appendedPhrase + MAN.sec + MAN.secDetail. see #7658#note-21
        $name_dedup_key = str_replace(cdm_tagged_text_to_string($sensu_tagged_text), ' ', $name_dedup_key);
        $appended_phrase_text = $appended_phrase_text . array_shift($sensu_tagged_text)->text; // remove first element which contains the "sensu", this will be added later in this code
        $sensu_citation_detail = trim(join(' ', cdm_tagged_text_values($sensu_tagged_text, array('secMicroReference'))));
        $sensu_citation_long = '';
        $sensu_citation_sortable_date = '';
        if (isset_not_empty($taxon_relation->secSource) && isset_not_empty($taxon_relation->secSource->label[0])){
            $sensu_citation_long = $taxon_relation->secSource->label[0]->label;

            $sensu_citation_sortable_date = $taxon_relation->secSource->sortableDate;
        }
        $sec_without_microReference = $taxon_relation->secSource;
        if (isset_not_empty($sensu_citation_detail)){
            $sensu_citation_detail_page = '';
            if (is_numeric($sensu_citation_detail) && $sensu_citation_detail>0 && $sensu_citation_detail < 10000){
                $sensu_citation_detail_page = 'p ' . $sensu_citation_detail;
            }
            $label = $taxon_relation->secSource->label[0]->label;
            $label = str_replace($sensu_citation_detail_page, '', $label);
            $sec_without_microReference->label[0]->label = $label;
        }

        $sensu_citation_long_markup = render_original_source($sec_without_microReference, FALSE);
        $sensu_citation_short = cdm_tagged_text_to_string($sensu_tagged_text);
        $sensu_citation_short_without_detail = cdm_tagged_text_to_string($sensu_tagged_text, array('secMicroReference'));
        $sensu_uuid = $sensu_tagged_text[0]->entityReference->uuid;
        $name_dedup_key = preg_replace('/\s+/', ' ', $name_dedup_key); // sanitize multiple whitespace characters
        $misapplied[$name_dedup_key]['sensu_uuids'][] = array('uuid' => $sensu_uuid, 'prefix' => $appended_phrase_text, 'citation_detail' => $sensu_citation_detail, 'sensu_citation'=> $sensu_citation_short_without_detail, 'sortable_date' => $sensu_citation_sortable_date);
        $ref_key = $sensu_uuid . ($sensu_citation_detail ? '#' . $sensu_citation_detail : '');
        if (!isset($joined_refs[$ref_key])) {
            $joined_refs[$ref_key] = array(
                'order_by_key' => $sensu_citation_sortable_date . $sensu_citation_short_without_detail,
                'markup' => $sensu_citation_long_markup,
                'detail' => $sensu_citation_detail,
                'longCitation' => $sensu_citation_long,// the footnote key will be appended later
                'citation' => $sensu_citation_short_without_detail
            );
        }
    } else if ($appended_phrase_text) {
        // appended phrase without reference
        $name_dedup_key = preg_replace('/\s+/', ' ', $name_dedup_key); // sanitize multiple whitespace characters

        $out = '';
        $annotations_and_sources = handle_annotations_and_sources($taxon_relation);
        $out .= $appended_phrase_text . $annotations_and_sources->footNoteKeysMarkup();

        $misapplied[$name_dedup_key]['sensu_uuids'][] = array('uuid' => null, 'prefix' => $appended_phrase_text, 'out' => $out);

    }

    if (isset($relsec_tagged_text[1])) {
        $appended_phrase_text = array_shift($relsec_tagged_text)->text; // remove first element which contains the "err. sec", this will be added later in this code
        $relsec_citation_detail = trim(join(' ', cdm_tagged_text_values($relsec_tagged_text, array('secMicroReference'))));
        $relsec_citation_long_markup = render_original_source($taxon_relation->relSource, FALSE);
        $relsec_citation_long = '';
        $relsec_citation_sortable_date = '';
        if (isset_not_empty($taxon_relation->relSource) && isset_not_empty($taxon_relation->relSource->label[0])){
            $relsec_citation_long = $taxon_relation->relSource->label[0]->label;
            $relsec_citation_sortable_date = $taxon_relation->relSource->sortableDate;
        }
        $relsec_citation_short_without_detail = cdm_tagged_text_to_string($relsec_tagged_text, array('secMicroReference'));
        $relsec_citation_short = cdm_tagged_text_to_string($relsec_tagged_text);
        $relsec_uuid = $relsec_tagged_text[0]->entityReference->uuid;
        $misapplied[$name_dedup_key]['relsec_uuids'][] = array('uuid' => $relsec_uuid, 'prefix' => $appended_phrase_text, 'citation_detail' => $relsec_citation_detail, 'relsec_citation'=> $relsec_citation_short_without_detail, 'sortable_date' => $relsec_citation_sortable_date);;
        $misapplied[$name_dedup_key]['relsec_uuids'][] = array('uuid' => $relsec_uuid, 'prefix' => $appended_phrase_text, 'citation_detail' => $relsec_citation_detail, 'relsec_citation'=> $relsec_citation_short_without_detail, 'sortable_date' => $relsec_citation_sortable_date);;
        $ref_key = $relsec_uuid . ($relsec_citation_detail ? '#' . $relsec_citation_detail : '');
        if (!isset($joined_refs[$ref_key])) {
            $joined_refs[$ref_key] = array(
                'order_by_key' => $relsec_citation_sortable_date . $relsec_citation_short_without_detail,
                'markup' => $relsec_citation_long_markup,
                'detail' => $relsec_citation_detail,
                'longCitation' => $relsec_citation_long,// the footnote key will be appended later
                'citation' => $relsec_citation_short_without_detail
            );
        }
    }

    if (!isset($misapplied[$name_dedup_key]['out'])) {
        // helpful for debugging: // cdm_tagged_text_add_options($taxon_relation->taggedText, array(array('filter-type' => 'symbol', 'attributes' => array('title' =>  array($taxon_relation->type->representations[0]->label)))));
        $out = cdm_tagged_text_to_markup($taxon_relation->taggedLabel);
        $out = uuid_anchor($taxon_relation->relTaxonUuid, $out);
        $annotations_and_sources = handle_annotations_and_sources($taxon_relation);
        $out .= $annotations_and_sources->footNoteKeysMarkup();
        $misapplied[$name_dedup_key]['out'] = $out;
    } else {

        // We need to add the anchors for all of the other misapplied names not
        // being rendered explicitly. TODO!!
       // $misapplied[$name_dedup_key]['out'] = uuid_anchor($taxon_relation->taxonUuid, $misapplied[$name_dedup_key]['out']);
    }

}


/**
 * Renders a representation of the given taxon relationship.
 *
 * According name relationships are also being rendered.
 *
 * @param $taxon
 *  The CDM TaxonBase entity
 * @param $reltype_uuid
 *  The UUID of the TaxonRelationshipType
 * @param $relsign
 *  Optional. Can be  used to override the internal decision strategy on finding a suitable icon for the relationship
 * @param $reltype_representation
 *   Optional: Defines the value for the title attribute of the html element enclosing the relsign
 * @param $doubtful
 *   TODO
 * @param $doLinkTaxon
 *   The taxon will be rendered as clickable link when true.
 *
 * @return string
 *   Markup for the taxon relationship.
 *
 * @throws Exception
 */
function cdm_related_taxon($taxon, $reltype_uuid = NULL) {
  static $relsign_homo = '≡';
  static $relsign_hetero = '=';
  static $relsign_invalid = '&ndash;';
  static $nom_status_invalid_type_uuids =  array(
    UUID_NOMENCLATURALSTATUS_TYPE_INVALID,
    UUID_NOMENCLATURALSTATUS_TYPE_NUDUM,
    UUID_NOMENCLATURALSTATUS_TYPE_COMBINATIONINVALID,
    UUID_NOMENCLATURALSTATUS_TYPE_PROVISIONAL
  );

  // 'taxonRelationships';
  $footnoteListKey = NULL;

  $skip_render_template_keys = array();

  $is_invalid = false;


  switch ($reltype_uuid) {
    case UUID_HETEROTYPIC_SYNONYM_OF:
    case UUID_SYNONYM_OF:
      $relsign = $relsign_hetero;
      break;

    case UUID_HOMOTYPIC_SYNONYM_OF:
      $relsign = $relsign_homo;
      break;

    case UUID_MISAPPLIED_NAME_FOR:
      $skip_render_template_keys[] = 'nameAuthorPart';
      $is_invalid = true;
      $relsign = $relsign_invalid;

      break;

    default:
      $relsign = $relsign_invalid;
  }


  /*
  Names with status invalid or nudum are to be displayed with the
  $relsign_invalid, these names appear at the end of all names in their
  homotypic group (ordered correctly by the java cdm_lib).
  */
  if (isset($taxon->name->status) && is_array($taxon->name->status)) {
    foreach ($taxon->name->status as $status) {
      if (in_array($status->type->uuid , $nom_status_invalid_type_uuids)) {
        $relsign = $relsign_invalid;
        break;
      }
    }
  }

  // Now rendering starts ..
  RenderHints::pushToRenderStack('related_taxon');

  if (isset($taxon->name->nomenclaturalSource->citation)) {
    $referenceUri = url(path_to_reference($taxon->name->nomenclaturalSource->citation->uuid));
  }
  $taxonUri = '';

  // Printing the taxonName and the handling the special case of annotations.
  if (!isset($referenceUri)) {
    $referenceUri = FALSE;
  }
  $out_taxon_part = render_taxon_or_name($taxon, $taxonUri, $referenceUri, TRUE, TRUE, FALSE, $skip_render_template_keys, $is_invalid);
  $name_relations = cdm_name_relationships_for_taxon($taxon);
  $name_relations_render_array = compose_name_relationships_inline($name_relations, $taxon->name->uuid, $taxon->uuid);

  $out = '<span class="relation_sign">' . $relsign . '</span>'
    . $out_taxon_part;
  if(isset($name_relations_render_array['list']['items'][0])){
    $out .= ' '  . drupal_render($name_relations_render_array);
  }

  $out = uuid_anchor($taxon->uuid, $out);

  RenderHints::popFromRenderStack();

  return $out;
}
/**
 * Renders a representation of the given taxon relationship.
 *
 * According name relationships are also being rendered.
 *
 * @param $taxon_dto
 *  The CDM TaxonBase dto
 * @param $reltype_uuid
 *  The UUID of the TaxonRelationshipType
 * @param $relsign
 *  Optional. Can be  used to override the internal decision strategy on finding a suitable icon for the relationship
 * @param $reltype_representation
 *   Optional: Defines the value for the title attribute of the html element enclosing the relsign
 * @param $doubtful
 *   TODO
 * @param $doLinkTaxon
 *   The taxon will be rendered as clickable link when true.
 *
 * @return string
 *   Markup for the taxon relationship.
 *
 * @throws Exception
 */
function cdm_new_related_taxon($taxon_dto, $reltype_uuid = NULL) {
    static $relsign_homo = '≡';
    static $relsign_hetero = '=';
    static $relsign_invalid = '&ndash;';
    static $nom_status_invalid_type_uuids =  array(
        UUID_NOMENCLATURALSTATUS_TYPE_INVALID,
        UUID_NOMENCLATURALSTATUS_TYPE_NUDUM,
        UUID_NOMENCLATURALSTATUS_TYPE_COMBINATIONINVALID,
        UUID_NOMENCLATURALSTATUS_TYPE_PROVISIONAL
    );

    // 'taxonRelationships';
    $footnoteListKey = NULL;

    $skip_render_template_keys = array();

    $is_invalid = false;
    $is_synonym = false;

    switch ($reltype_uuid) {
        case UUID_HETEROTYPIC_SYNONYM_OF:
        case UUID_SYNONYM_OF:
            $relsign = $relsign_hetero;
            $is_synonym = true;
            break;

        case UUID_HOMOTYPIC_SYNONYM_OF:
            $relsign = $relsign_homo;
            $is_synonym = true;
            break;

        case UUID_MISAPPLIED_NAME_FOR:
            $skip_render_template_keys[] = 'nameAuthorPart';
            $is_invalid = true;
            $relsign = $relsign_invalid;

            break;

        default:
            $relsign = $relsign_invalid;
    }


    /*
    Names with status invalid or nudum are to be displayed with the
    $relsign_invalid, these names appear at the end of all names in their
    homotypic group (ordered correctly by the java cdm_lib).
    */
    //TODO: how it is implemented in new dto???
    if ($taxon_dto->invalid ) {
        $relsign = $relsign_invalid;
    }

    // Now rendering starts ..
    RenderHints::pushToRenderStack('related_taxon');
//TODO: actually the uuid of the nomenclatural source is not available in taxon dto
    /*if (isset($taxon_dto->name->nomenclaturalSource->citation)) {
        $referenceUri = url(path_to_reference($taxon->name->nomenclaturalSource->citation->uuid));
    }*/
    $taxonUri = '';

    // Printing the taxonName and the handling the special case of annotations.
    if (!isset($referenceUri)) {
        $referenceUri = FALSE;
    }

    $out_taxon_part = render_new_taxon_or_name($taxon_dto, true, $taxonUri, $referenceUri, TRUE, TRUE, FALSE, $skip_render_template_keys, $is_invalid);
    $name_relations = cdm_name_relationships_for_taxon_new($taxon_dto);
    //TODO the name uuid is not available!!!
    $name_relations_render_array = compose_new_name_relationships_inline($name_relations, $taxon_dto->uuid, $taxon_dto->nameType);

    $out = '<span class="relation_sign">' . $relsign . '</span>'
        . $out_taxon_part;
    if(isset($name_relations_render_array['list']['items'][0])){
        $out .= ' '  . drupal_render($name_relations_render_array);
    }

    $out = uuid_anchor($taxon_dto->uuid, $out);

    RenderHints::popFromRenderStack();

    return $out;
}
/**
 * Creates markup for a taxon which is the accepted of another one
 *
 * @param $accepted_for_uuid
 *   The uuid of the accepted taxon
 */
function cdm_accepted_for($accepted_for_uuid) {

  if(!is_uuid($accepted_for_uuid)){
    return '';
  }

  RenderHints::pushToRenderStack('acceptedFor');
  $out = '';

  $synonym = cdm_ws_get(CDM_WS_PORTAL_TAXON, $accepted_for_uuid);
  if ($synonym) {
    $out .= '<span class="acceptedFor">';
    $out .= t('is accepted for ');
    $referenceUri = null;
    if (isset($synonym->name->nomenclaturalSource->citation)) {
      $referenceUri = url(path_to_reference($synonym->name->nomenclaturalSource->citation->uuid));
    }
    $out .= render_taxon_or_name($synonym->name, NULL, $referenceUri);
    RenderHints::setAnnotationsAndSourceConfig(synonymy_annotations_and_source_config());
    $annotations_and_sources = handle_annotations_and_sources($synonym);
    $out .= $annotations_and_sources->footNoteKeysMarkup();
    $out .= '</span>';
  }
  RenderHints::popFromRenderStack();
  return $out;
}

/**
 * Compose function for a list of taxa.
 *
 * This function is for used to:
 *
 * 1. Display search results
 * 2. List the taxa for a taxon name in the name page.
 *
 * @param $taxon_list array
 *   The list of CDM Taxon entities. e.g. The records array as contained in a pager object.
 * @param $freetext_search_results array
 * @param $show_classification boolean
 *
 * @ingroup compose
 */
function compose_list_of_taxa($taxon_list, $freetext_search_results = array(), $show_classification = false) {

    RenderHints::pushToRenderStack('list_of_taxa');

    $gallery_settings = getGallerySettings(CDM_DATAPORTAL_SEARCH_GALLERY_NAME);

    $showMedia_taxa = $gallery_settings['cdm_dataportal_show_taxon_thumbnails'];
    $showMedia_synonyms = $gallery_settings['cdm_dataportal_show_synonym_thumbnails'];
    $searched_in_classification = cdm_dataportal_searched_in_classification();
    $searched_in_classification_uuid = null;
    if(isset($searched_in_classification->uuid)){
        $searched_in_classification_uuid = $searched_in_classification->uuid;
    }

    // .. Well, for sure not as performant as before, but better than nothing.
    $synonym_uuids = array();
    $misapplied_uuids = array();
    foreach ($taxon_list as $taxon) {
        if ($taxon->class == "Synonym") {
            if (!array_key_exists($taxon->uuid, $synonym_uuids)) {
                $synonym_uuids[$taxon->uuid] = $taxon->uuid;
            }
        }
        elseif (!_cdm_dataportal_acceptedByCurrentView($taxon)) {
            // Assuming that it is a misapplied name, will be further examined below.
            $misapplied_uuids[$taxon->uuid] = $taxon->uuid;
        }
    }

    // Batch service not jet implemented:
    // $table_of_accepted = cdm_ws_property(CDM_WS_PORTAL_TAXON_ACCEPTED,
    // join(',', $synonym_uuids));
    // thus ...
    $table_of_accepted = array();

    foreach ($synonym_uuids as $misapplication_uuid) {
        $classification_filter = '';
        if($searched_in_classification_uuid){
            $classification_filter = 'classificationFilter=' . $searched_in_classification_uuid;
        }
        $table_of_accepted[$misapplication_uuid] = cdm_ws_get(
            CDM_WS_PORTAL_TAXON_ACCEPTED,
            array($misapplication_uuid),
            $classification_filter
        );
    }

    foreach ($misapplied_uuids as $mispplication_uuid) {
        $taxonRelationsDTO = cdm_ws_get(CDM_WS_PORTAL_TAXON_RELATIONS_DTO, array(
            $mispplication_uuid,
        ));
        if(isset($taxonRelationsDTO->relations)){
            foreach ($taxonRelationsDTO->relations as $relation) {
                if ($relation->type->uuid == UUID_MISAPPLIED_NAME_FOR && _cdm_dataportal_matches_current_view($relation->classificationsUUIDs)) {
                    $table_of_accepted[$mispplication_uuid][] = cdm_ws_get(CDM_WS_TAXON, $relation->taxonUuid);
                }
            }
        }
    }

    $out = '<div class="cdm-item-list" style="background-image: none;">';
    $itemCnt = -1;
    foreach ($taxon_list as $taxon) {
        $itemCnt++;

        if (isset($table_of_accepted[$taxon->uuid])) {
            // Its a synonym or misapplied name.
            $is_synonym = isset($synonym_uuids[$taxon->uuid]); //TODO better use the $taxon->class attribute?
            $taxon_type = $is_synonym ? TAXON_TYPE_SYNONYM :TAXON_TYPE_MISAPPLIEDNAME;

            $acceptedTaxa = $table_of_accepted[$taxon->uuid];

            if (!is_array($acceptedTaxa)) {
                $acceptedTaxa = array($acceptedTaxa);
            }

            foreach ($acceptedTaxa as $acceptedTaxon) {
                if (is_object($acceptedTaxon)) {

                    $taxonUri = uri_to_synonym($taxon->uuid, $acceptedTaxon->uuid);
                    $referenceUri = '';
                    if (isset($acceptedTaxon->name->nomenclaturalSource->citation)) {
                        $referenceUri = url(path_to_reference($acceptedTaxon->name->nomenclaturalSource->citation->uuid));
                    }
                    $skip_render_template_parts = $is_synonym ? ['secReferencePart'] : [];
                    // $taxon_or_name this is a trick to suppress the sec reference for synonyms
                    // supplying the name will cause render_taxon_or_name() to not show the sec reference
                    $out .= "<div class=\"item " . $taxon_type . "\">" . render_taxon_or_name($taxon, $taxonUri, $referenceUri, true, false, $skip_render_template_parts);
                    if($taxon_type == TAXON_TYPE_MISAPPLIEDNAME && isset_not_empty($taxon->secSource) && isset_not_empty($taxon->secSource->citation)) {
                        if (isset($taxon->secSource->citation->authorship)) {
                            $authorship = $taxon->secSource->citation->authorship->titleCache;
                        }
                        else {
                            $authorship = $taxon->secSource->citation->titleCache;
                        }
                        $out .= ' sensu ' . $authorship;
                    }
                    if ($show_classification) {
                        $out .= render_classifications_for_taxon($taxon);
                    }
                    if ($showMedia_synonyms) {
                        $out .= theme('cdm_taxon_list_thumbnails', array('taxon' => $acceptedTaxon));
                    }
                }
            }
        }
        else {
            // Its a Taxon.
            $taxonUri = url(path_to_taxon($taxon->uuid));
            $referenceUri = '';
            if (isset($taxon->name->nomenclaturalSource->citation)) {
                $referenceUri = url(path_to_reference($taxon->name->nomenclaturalSource->citation->uuid));
            }
            $out .= '<div class="item Taxon">' . render_taxon_or_name($taxon, $taxonUri, $referenceUri, false);
            if ($show_classification) {
                $out .= render_classifications_for_taxon($taxon);
            }
            if ($showMedia_taxa) {
                $out .= theme('cdm_taxon_list_thumbnails', array('taxon' => $taxon));
            }
        }

        /*
         * the score field will be empty in case of MultiTermQueries like
         * WildcardQueries, since these are  constant score by default
         * since Lucene 2.9
         */
        //currently only complete matches are returned, so we can remove this.
        if(isset($freetext_search_results[$itemCnt]) && $freetext_search_results[$itemCnt]->score && $freetext_search_results[$itemCnt]->maxScore && isset_not_empty($freetext_search_results[$itemCnt]->fieldHighlightMap)){
            $percentage =  ( $freetext_search_results[$itemCnt]->score / $freetext_search_results[$itemCnt]->maxScore ) * 100;
            $out .= '<div class="score-bar"><div class="score-bar-indicator" style="width:' . $percentage .'% "></div></div>';
            $out .= '<div class="score-bar-value">' . number_format($percentage, 2) .'%</div>';
        }

        // Render highlighted fragments, these are made available by free text
        // searches.
        if (isset($freetext_search_results[$itemCnt]->fieldHighlightMap)) {
            $field_fragments = (array) $freetext_search_results[$itemCnt]->fieldHighlightMap;
            if (count($field_fragments) > 0) {
                $fragments_out = '';
                foreach ($field_fragments as $fieldName => $fragments) {
                    $fragments_out .= '... <span class="' . $fieldName. '">' . filter_xss(join(" ... ", $fragments), array('b') ) . '</span>';
                }
                $out .= '<div class="fragment_highlight">' . $fragments_out . ' ...</div>';
            }
        }

        $out .= '</div>';
    }

    $out .= '</div>';
    RenderHints::popFromRenderStack();

    return markup_to_render_array($out); // TODO create render array of all list items in function
}

/**
 * Compose function for a list of taxa.
 *
 * This function is for used to:
 *
 * 1. Display search results
 * 2. List the taxa for a taxon name in the name page.
 *
 * @param $taxon_list array
 *   The list of CDM Taxon entities. e.g. The records array as contained in a pager object.
 * @param $freetext_search_results array
 * @param $show_classification boolean
 *
 * @ingroup compose
 */
function compose_list_of_taxon_search_result($taxon_list, $freetext_search_results = array(), $show_classification = false) {

    RenderHints::pushToRenderStack('list_of_taxa');

    $gallery_settings = getGallerySettings(CDM_DATAPORTAL_SEARCH_GALLERY_NAME);

    $showMedia_taxa = $gallery_settings['cdm_dataportal_show_taxon_thumbnails'];
    $showMedia_synonyms = $gallery_settings['cdm_dataportal_show_synonym_thumbnails'];
    $searched_in_classification = cdm_dataportal_searched_in_classification();
    $searched_in_classification_uuid = null;
    if(isset($searched_in_classification->uuid)){
        $searched_in_classification_uuid = $searched_in_classification->uuid;
    }

    // .. Well, for sure not as performant as before, but better than nothing.
    $misapplied_uuids = array();
    foreach ($taxon_list as $taxon) {
        if ($taxon->entity->class == 'Taxon' && !_cdm_dataportal_acceptedByCurrentView($taxon)) {
            // Assuming that it is a misapplied name, will be further examined below.
            $misapplied_uuids[$taxon->entity->uuid] = $taxon->entity->uuid;
        }
    }

    // Batch service not jet implemented:
    // $table_of_accepted = cdm_ws_property(CDM_WS_PORTAL_TAXON_ACCEPTED,
    // join(',', $synonym_uuids));
    // thus ...
    $table_of_accepted = array();
    foreach ($misapplied_uuids as $mispplication_uuid) {
        $taxonRelationsDTO = cdm_ws_get(CDM_WS_PORTAL_TAXON_RELATIONS_DTO, array(
            $mispplication_uuid,
        ));
        if(isset($taxonRelationsDTO->relations)){
            $taxon_relationship_types = variable_get(CDM_TAXON_RELATIONSHIP_TYPES, unserialize(CDM_TAXON_RELATIONSHIP_TYPES_DEFAULT));
            foreach ($taxonRelationsDTO->relations as $relation) {
                if (in_array($relation->type->uuid, $taxon_relationship_types) && in_array($searched_in_classification->uuid,$relation->classificationsUUIDs)  )  {
                //if (($relation->type->uuid == UUID_MISAPPLIED_NAME_FOR || $relation->typeUuid == UUID_MISAPPLIED_NAME_FOR || $relation->type->uuid == UUID_PROPARTE_SYNONYM_FOR || $relation->typeUuid == UUID_PROPARTE_SYNONYM_FOR) && _cdm_dataportal_matches_current_view($relation->classificationsUUIDs)) {
                 //   $table_of_accepted[$mispplication_uuid][] = cdm_ws_get(CDM_WS_TAXON, $relation);
                    $table_of_accepted[$mispplication_uuid][] = $relation;
                }
            }
        }
    }

    $out = '<div class="cdm-item-list" style="background-image: none;">';
    $itemCnt = -1;
    foreach ($taxon_list as $taxon) {
        $itemCnt++;
       if ($taxon->entity->class == "Synonym") {
        $synonym = $taxon;
        $taxonUri = uri_to_synonym($synonym->entity->uuid, $synonym->acceptedTaxonUuid);
        $taxon_type = TAXON_TYPE_SYNONYM;
        $out .=createOutput($taxon_type, $taxon, $taxonUri, "");
       }else {
         if (isset($table_of_accepted[$taxon->entity->uuid])) {
           // Its a misapplied name.
           $taxon_type = TAXON_TYPE_MISAPPLIEDNAME;
           $relations = $table_of_accepted[$taxon->entity->uuid];

           if (!is_array($relations)) {
               $relations = array($relations);
           }
           $taxa_count = count($relations)-1;
           $index = 0;
           foreach ($relations as $relation) {
             if (is_object($relation)) {
                 $taxonUri = uri_to_synonym($taxon->entity->uuid, $relation->taxonUuid);
                 $referenceUri = '';
                 //if (isset($acceptedTaxon->name->nomenclaturalSource->citation)) {
                 //    $referenceUri = url(path_to_reference($acceptedTaxon->name->nomenclaturalSource->citation->uuid));
                 //}
                 $out .= createOutput($taxon_type, $taxon, $taxonUri, $referenceUri);
                 $out .= " as ". cdm_tagged_text_to_markup($relation->taggedText, ['reference', 'secReference', 'relSecReference']);
                 if ($index<$taxa_count) {
                     $out .= "</div>";
                 }
                 $index++;
             }
           }
         }else{
           if (isset_not_empty($taxon->entity) && isset_not_empty($taxon->entity->uuid)){
              $taxonUri = url(path_to_taxon($taxon->entity->uuid));
              $taxon_type = "Taxon";                                                                                                                             
              $referenceUri = '';                                                                                                                                
              if (isset_not_empty($taxon->entity->name->nomenclaturalSource) && isset_not_empty($taxon->entity->name->nomenclaturalSource->citation)) {          
                 $referenceUri = url(path_to_reference($taxon->entity->name->nomenclaturalSource->citation->uuid));                                              
              }                                                                                                                                                  
              $out .= createOutput($taxon_type, $taxon, $taxonUri, $referenceUri);                                                                               
           }
         }

        }

        //$out .= '<div class="item Taxon">' . render_taxon_or_name($taxon, $taxonUri, $referenceUri, false);
       // $out = createOutput($taxon_type, $taxon, $taxonUri, $referenceUri, $out);
        if ($show_classification) {
            $out .= render_classifications_for_taxon($taxon-> entity);
        }
        if ($showMedia_taxa) {
            $out .= theme('cdm_taxon_list_thumbnails', array('taxon' => $taxon));
        }

        

        /*
         * the score field will be empty in case of MultiTermQueries like
         * WildcardQueries, since these are  constant score by default
         * since Lucene 2.9
         */
        if(isset($freetext_search_results[$itemCnt]) && $freetext_search_results[$itemCnt]->score && $freetext_search_results[$itemCnt]->maxScore){
            $percentage =  ( $freetext_search_results[$itemCnt]->score / $freetext_search_results[$itemCnt]->maxScore ) * 100;
            $out .= '<div class="score-bar"><div class="score-bar-indicator" style="width:' . $percentage .'% "></div></div>';
            $out .= '<div class="score-bar-value">' . number_format($percentage, 2) .'%</div>';
        }

        // Render highlighted fragments, these are made available by free text
        // searches.
        if (isset($freetext_search_results[$itemCnt]->fieldHighlightMap)) {
            $field_fragments = (array) $freetext_search_results[$itemCnt]->fieldHighlightMap;
            if (count($field_fragments) > 0) {
                $fragments_out = '';
                foreach ($field_fragments as $fieldName => $fragments) {
                    $fragments_out .= '... <span class="' . $fieldName. '">' . filter_xss(join(" ... ", $fragments), array('b') ) . '</span>';
                }
                $out .= '<div class="fragment_highlight">' . $fragments_out . ' ...</div>';
            }
        }

        $out .= '</div>';
    }

    $out .= '</div>';
    RenderHints::popFromRenderStack();

    return markup_to_render_array($out); // TODO create render array of all list items in function
}

/**
 * @param string $taxon_type
 * @param mixed $taxon
 * @param string $taxonUri
 * @param string $referenceUri
 * @param string $out
 * @return string
 */
function createOutput($taxon_type, $taxon, $taxonUri, $referenceUri): string
{
    $out = "";
    $skip_render_template_parts = $taxon_type == TAXON_TYPE_SYNONYM ? ['secReferencePart'] : [];
    $skip_render_template_parts = $taxon_type == TAXON_TYPE_MISAPPLIEDNAME ? ['referencePart', 'secReferencePart'] : [];
    $out .= "<div class=\"item " . $taxon_type . "\">" . render_new_taxon_or_name($taxon, true, $taxonUri, $referenceUri, false, false, false, $skip_render_template_parts);
    return $out;
}


function render_classifications_for_taxon($taxon) {
  $unclassified_snippet = '<span class="unclassified">' . t('unclassified') . '</span>';

  $classifications = get_classifications_for_taxon($taxon);
  $classification_titles = array();
  foreach ($classifications as $classification) {
    if (isset($classification->titleCache)) {
      $classification_titles[] = $classification->titleCache;
    }
  }
  if (count($classification_titles) == 0) {
    $classification_titles[] = $unclassified_snippet;
  }
  return '<span class="classifications"><span class="separator"> : </span>' . implode(', ', $classification_titles) . '</span>';
}

/**
 * Compose function for the taxonomic children
 *
 * @param $taxon_uuid
 *    The uuuid of the taxon to compose the list of taxonomic children for
 * @return
 *   A drupal render array.
 *
 * @ingroup compose
 */
function compose_taxonomic_children($taxon_uuid){

  $render_array = array();
  
  if($taxon_uuid) {
    $children = cdm_ws_get(CDM_WS_PORTAL_TAXONOMY_CHILDNODES_OF_TAXON, array(
      get_current_classification_uuid(),
      $taxon_uuid
      ));
    if($children){
      $taxonomic_children = theme('cdm_taxontree', array('tree' => $children));
      $render_array = markup_to_render_array($taxonomic_children);
    }
  }
  return $render_array;
}

