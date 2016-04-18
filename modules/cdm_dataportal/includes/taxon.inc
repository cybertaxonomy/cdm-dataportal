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


/**
 * Returns HTML for misapplied names and invalid designations.
 *
 * Both relation types are currently treated the same!
 *
 * @param taxonRelationships
 * @param focusedTaxon
 *
 * @return string
 *    the rendered html
 */
function cdm_taxonRelationships($taxonRelationships, $focusedTaxon){

  if (!$taxonRelationships) {
    return null;
  }

  RenderHints::pushToRenderStack('taxonRelationships');
  $footnoteListKey = 'taxonRelationships';
  RenderHints::setFootnoteListKey($footnoteListKey);

  $misapplied = array();
  $joinedAuthorTeams = array();

  $taxon_relationship_types = variable_get(CDM_TAXON_RELATIONSHIP_TYPES, unserialize(CDM_TAXON_RELATIONSHIP_TYPES_DEFAULT));

  // Aggregate misapplied names having the same fullname:
  foreach ($taxonRelationships as $taxonRelation) {

    if (in_array($taxonRelation->type->uuid, $taxon_relationship_types)) {

      if ($taxonRelation->type->uuid == UUID_MISAPPLIED_NAME_FOR || $taxonRelation->type->uuid == UUID_INVALID_DESIGNATION_FOR) {

        $name = $taxonRelation->fromTaxon->name->titleCache;

        if(isset($taxonRelation->fromTaxon->sec)) {
          // taxa not always are have a sec reference (e.g. doubtful taxa)
          $authorteam = cdm_ws_get(CDM_WS_REFERENCE_AUTHORTEAM, $taxonRelation->fromTaxon->sec->uuid);
          $authorteam = $authorteam->titleCache;
        }

        if (!isset($misapplied[$name])) {
          // Render the first name found as representative for all others.
          $misapplied[$name]['out'] = cdm_related_taxon($taxonRelation->fromTaxon, UUID_MISAPPLIED_NAME_FOR);
        }
        else {
          // We need to add the anchors for all of the other mispplied names not
          // being rendered explicitly.
          $misapplied[$name]['out'] = uuid_anchor($taxonRelation->fromTaxon->uuid, $misapplied[$name]['out']);
        }

        // Collect all authors for this fullname.
        if (isset($authorteam)) {
          $misapplied[$name]['authorteam'][$authorteam] = '';
          $joinedAuthorTeams[$authorteam] = 'sensu ' . theme('cdm_reference', array('reference' => $taxonRelation->fromTaxon->sec));
        }
      }
      else {
        // All relationsship types but misapplied_name_for
        // invalid_designation_for.
        $taxon_relationships_lines[] = cdm_taxonRelationship($taxonRelation, TRUE, _is_invers_taxonRelationship($taxonRelation, $focusedTaxon));
      }
    }
  }

  // Sort the joinedAuthorTeams and create footnotes and footnotekeys.
  ksort($joinedAuthorTeams);
  foreach ($joinedAuthorTeams as $authorteam => $sensuCitation) {
    $footnoteKey = FootnoteManager::addNewFootnote($footnoteListKey, $sensuCitation);
    $joinedAuthorTeams[$authorteam] = '<span class="sensu">sensu '
      . $authorteam
      . theme('cdm_footnote_key', array('footnoteKey' => $footnoteKey))
      . '</span>';
  }

  // ---- Generate output ---- //

  $out = '<div class="taxon-relationships">';
  if (is_array($misapplied) && count($misapplied) > 0) {
    $out .= '<ul class="misapplied">';
    foreach ($misapplied as $misapplied_name) {

      $out .= '<li class="synonym"><span class="misapplied">' . $misapplied_name['out'] . ' </span>';

      if (isset($misapplied_name['authorteam'])) {
        // Fill authors with the renderedFootnoteKey and sorting 'em.
        foreach ($misapplied_name['authorteam'] as $authorteam => &$renderedFootnoteKey) {
          $renderedFootnoteKey = $joinedAuthorTeams[$authorteam];
        }
        ksort($misapplied_name['authorteam']);
        $out .= join('; ', $misapplied_name['authorteam']);
      }
      $out .= '</li>';
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

  $footnotes = theme('cdm_footnotes', array('footnoteListKey' => $footnoteListKey, 'enclosingTag' => 'li'));
  $footnotes .= theme('cdm_annotation_footnotes', array('footnoteListKey' => $footnoteListKey, 'enclosingTag' => 'li'));

// AK: why splitting footnotes at the sensu string ??? this is weired and hacky
//     TODO remove below dead code
//   $tr_footnotes_exploded = explode('sensu', $tr_footnotes);
//   $tr_footnotes_aux = '';
//   foreach ($tr_footnotes_exploded as $element) {
//     $tr_footnotes_aux .= $element;
//   }

  $out .= '<ul class="footnotes">' . $footnotes . '</ul>';

  $out .= '</div>';

  RenderHints::popFromRenderStack();
  return $out;
}


/**
 * Renders a representation of the given taxon relationship.
 *
 * According name relationships are also being rendered.
 *
 * @param unknown_type $taxonRelationship
 * @param boolean $doLinkTaxon
 *     whether to create a link to the related taxon
 * @param boolean $inverse
 *     whether the $taxonRelationship should be treaded as invers relation
 *
 * @return void|string
 */
function cdm_taxonRelationship($taxonRelationship, $doLinkTaxon = FALSE, $inverse = FALSE) {

  // Validate object.
  if (!(isset($taxonRelationship->toTaxon) && isset($taxonRelationship->fromTaxon) && isset($taxonRelationship->type))) {
    return null;
  }

  $taxonRelationType = $taxonRelationship->type;

  if ($inverse) {
    $toTaxon = $taxonRelationship->fromTaxon;
    $relsign = $taxonRelationType->inverseRepresentation_L10n_abbreviatedLabel;
    $reltype_representation = $taxonRelationType->inverseRepresentation_L10n;
  }
  else {
    $toTaxon = $taxonRelationship->toTaxon;
    $relsign = $taxonRelationType->representation_L10n_abbreviatedLabel;
    $reltype_representation = $taxonRelationType->representation_L10n;
  }

  return cdm_related_taxon($toTaxon, NULL, $relsign, $reltype_representation, $taxonRelationship->doubtful, $doLinkTaxon);
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
 */
function cdm_related_taxon($taxon, $reltype_uuid = NULL, $relsign = NULL, $reltype_representation = NULL, $doubtful=false, $doLinkTaxon = FALSE) {
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

  $skip_tags = array();

  $is_invalid = false;

  if (!$relsign) {

    switch ($reltype_uuid) {
      case UUID_HETEROTYPIC_SYNONYM_OF:
      case UUID_SYNONYM_OF:
        $relsign = $relsign_hetero;
        break;

      case UUID_HOMOTYPIC_SYNONYM_OF:
        $relsign = $relsign_homo;
        break;

      case UUID_MISAPPLIED_NAME_FOR:
      case UUID_INVALID_DESIGNATION_FOR:
        $skip_tags[] = 'authors';
        $is_invalid = true;
        $relsign = $relsign_invalid;

        break;

      default:
        $relsign = $relsign_invalid;
    }

  }

  if($doubtful) {
    $relsign = '?' . $relsign;
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

  if (isset($taxon->name->nomenclaturalReference)) {
    $referenceUri = url(path_to_reference($taxon->name->nomenclaturalReference->uuid));
  }
  $taxonUri = '';
  if ($doLinkTaxon) {
    $taxonUri = url(path_to_taxon($taxon->uuid, "synonymy"));
  }
  // Printing the taxonName and the handling the special case of annotations.
  if (!isset($referenceUri)) {
    $referenceUri = FALSE;
  }
  $out_taxon_part = render_taxon_or_name($taxon, $taxonUri, $referenceUri, TRUE, FALSE, $skip_tags, $is_invalid);
  $taxon_footnotes = theme('cdm_annotations_as_footnotekeys',
    array('cdmBase_list' => array(
      $taxon->name,
      $taxon,
    ),
      'footnote_list_key' => $footnoteListKey)
  );

  $homonyms = cdm_name_relationships_of($taxon);

  $out = '<span class="relation_sign" title="' . $reltype_representation . '">' . $relsign . '</span>'
    . $out_taxon_part . $taxon_footnotes . ' '  . $homonyms;

  $out = uuid_anchor($taxon->uuid, $out);

  RenderHints::popFromRenderStack();

  return $out;
}

