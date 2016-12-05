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

  static $NULL_AUTHORTEAM = 'NULL_AUTHORTEAM';

  if (!$taxonRelationships) {
    return null;
  }

  RenderHints::pushToRenderStack('taxon_relationships');
  $footnoteListKey = 'taxon_relationships';
  RenderHints::setFootnoteListKey($footnoteListKey);

  $misapplied = array();
  $joinedAuthorTeams = array();

  $taxon_relationship_types = variable_get(CDM_TAXON_RELATIONSHIP_TYPES, unserialize(CDM_TAXON_RELATIONSHIP_TYPES_DEFAULT));

  // Aggregate misapplied names having the same fullname:
  //  - deduplicate misapplied names, so that the same name it not shown multiple times in case it
  //    the duplicates only have different sensu references/author teams (see #5647)
  //  - show the author team as sec reference
  //  - show the according reference as footnote to this author team
  //  - if the sec reference has no author team it should show instead the title cache
  //
  // Example:
  // "Xenoxylum foobar" sensu Grumbach¹; sensu Lem²
  //    1. Novel marsian species, Grumbach, 2022
  //    2. Flora solaris, Lem, 2019
  foreach ($taxonRelationships as $taxonRelation) {

    if (in_array($taxonRelation->type->uuid, $taxon_relationship_types)) {

      if ($taxonRelation->type->uuid == UUID_MISAPPLIED_NAME_FOR || $taxonRelation->type->uuid == UUID_INVALID_DESIGNATION_FOR) {

        RenderHints::pushToRenderStack('misapplied_name_for'); // TODO the render path string should in future come from $taxonRelation->type->...

        $name = $taxonRelation->fromTaxon->name->titleCache;

        $sensu_citation_footnote_str = null;

        if(isset($taxonRelation->fromTaxon->sec)) {

          $sensu_citation_footnote_str = theme('cdm_reference', array('reference' => $taxonRelation->fromTaxon->sec));
          $authorteam = cdm_ws_get(CDM_WS_REFERENCE_AUTHORTEAM, $taxonRelation->fromTaxon->sec->uuid);

          if(isset($authorteam->titleCache)){
            $authorteam_str = $authorteam->titleCache;
            if($taxonRelation->fromTaxon->sec->titleCache == $authorteam->titleCache){
              // no need for a footnote in case the reference only consists of the author team
              $sensu_citation_footnote_str = '';
            }
          } else {
            $authorteam_str = $sensu_citation_footnote_str;
            // no need for a footnote in case in case it is used as replacement for missing author teams
            $sensu_citation_footnote_str = '';
          }
        } else {
          // taxa not always are have a sec reference (e.g. doubtful taxa)
          // use the NULL_AUTHORTEAM in this case
          $authorteam_str = $NULL_AUTHORTEAM;
        }

        if (!isset($misapplied[$name])) {
          // Render the first name found as representative for all others.
          $misapplied[$name]['out'] = cdm_related_taxon($taxonRelation->fromTaxon, UUID_MISAPPLIED_NAME_FOR);
        }
        else {
          // We need to add the anchors for all of the other misapplied names not
          // being rendered explicitly.
          $misapplied[$name]['out'] = uuid_anchor($taxonRelation->fromTaxon->uuid, $misapplied[$name]['out']);
        }

        // Collect all authors for this fullname.
        if (isset($authorteam_str) && $authorteam_str != $NULL_AUTHORTEAM) {
          // prepare entry for the footnote key of the sensu citation footnote
          $misapplied[$name]['authorteam'][$authorteam_str] = '';
          // map sensu citation footnote to the authorteam string
          $joinedAuthorTeams[$authorteam_str] = $sensu_citation_footnote_str;

        }

      }
      else {
        RenderHints::pushToRenderStack('other_taxon_relationship');
        // All relationsship types but misapplied_name_for
        // invalid_designation_for.
        $taxon_relationships_lines[] = cdm_taxonRelationship($taxonRelation, TRUE, _is_invers_taxonRelationship($taxonRelation, $focusedTaxon));
      }

      RenderHints::popFromRenderStack();
    }
  }

  // Sort the joinedAuthorTeams and create footnotes and footnotekeys.
  ksort($joinedAuthorTeams);
  $author_team_cnt = 0;
  foreach ($joinedAuthorTeams as $authorteam_str => $sensu_citation) {
    $footnoteKey = '';
    if(!empty($sensu_citation)) {
      $footnoteKey = FootnoteManager::addNewFootnote($footnoteListKey, 'sensu ' . $sensu_citation);
      $footnoteKey = theme('cdm_footnote_key', array('footnoteKey' => $footnoteKey));
    }
    $sensu = '';
    if($authorteam_str != $NULL_AUTHORTEAM){
      $sensu = ++$author_team_cnt == 0 ? '' : 'sensu ' . $authorteam_str;
    }
    $joinedAuthorTeams[$authorteam_str] = '<span class="sensu">' . $sensu . $footnoteKey .'</span>';
  }

  // ---- Generate output ---- //

  $out = '<div class="taxon-relationships">';
  if (is_array($misapplied) && count($misapplied) > 0) {
    $out .= '<ul class="misapplied">';
    foreach ($misapplied as $misapplied_name) {

      $out .= '<li class="synonym"><span class="misapplied">' . $misapplied_name['out'] . ' </span>';

      if (isset($misapplied_name['authorteam'])) {
        // Fill authors with the renderedFootnoteKey and sorting 'em.
        foreach ($misapplied_name['authorteam'] as $authorteam_str => &$renderedFootnoteKey) {
          $renderedFootnoteKey = $joinedAuthorTeams[$authorteam_str];
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
 * @param $taxonRelationship
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
    if (isset($synonym->name->nomenclaturalReference)) {
      $referenceUri = url(path_to_reference($synonym->name->nomenclaturalReference->uuid));
    }
    $out .= render_taxon_or_name($synonym->name, NULL, $referenceUri);
    $out .= theme('cdm_annotations_as_footnotekeys', array('cdmBase_list' => $synonym));
    $out .= '</span>';
  }
  RenderHints::popFromRenderStack();
  return $out;
}

/**
 * Compose function for a list of taxa.
 *
 * This function is for example used to display search results or the taxa for a taxon name in the name page.
 *
 * @param $taxon_list array
 *   The list of CDM Taxon entities. e.g. The records array as contained in a pager object.
 * @param $freetext_search_results array
 * @param $show_classification boolean
 *
 * @ingroup compose
 *
 */
function compose_list_of_taxa($taxon_list, $freetext_search_results = array(), $show_classification = false) {

  $unclassified_snippet = '<span class="unclassified">' . t('unclassified') . '</span>';

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
  $misappied_uuids = array();
  foreach ($taxon_list as $taxon) {
    if ($taxon->class == "Synonym") {
      if (!array_key_exists($taxon->uuid, $synonym_uuids)) {
        $synonym_uuids[$taxon->uuid] = $taxon->uuid;
      }
    }
    elseif (!_cdm_dataportal_acceptedByCurrentView($taxon)) {
      // Assuming that it is a misappied name, will be further examined below.
      $misappied_uuids[$taxon->uuid] = $taxon->uuid;
    }
  }

  // Batch service not jet implemented:
  // $table_of_accepted = cdm_ws_property(CDM_WS_PORTAL_TAXON_ACCEPTED,
  // join(',', $synonym_uuids));
  // thus ...
  $table_of_accepted = array();

  foreach ($synonym_uuids as $relatedUuid) {
    $table_of_accepted[$relatedUuid] = cdm_ws_get(CDM_WS_PORTAL_TAXON_ACCEPTED,
      array($relatedUuid),
      'classificationFilter=' . $searched_in_classification_uuid
    );
  }

  foreach ($misappied_uuids as $relatedUuid) {
    $taxonRelations = cdm_ws_get(CDM_WS_PORTAL_TAXON_RELATIONS, array(
      $relatedUuid,
    ));
    foreach ($taxonRelations as $relation) {
      if ($relation->type->uuid == UUID_MISAPPLIED_NAME_FOR && _cdm_dataportal_acceptedByCurrentView($relation->toTaxon)) {
        $table_of_accepted[$relatedUuid][] = $relation->toTaxon;
      }
    }
  }

  $out = '<ul class="cdm_names" style="background-image: none;">';
  $itemCnt = -1;
  foreach ($taxon_list as $taxon) {
    $itemCnt++;
    if (isset($table_of_accepted[$taxon->uuid])) {
      // Its a synonym or misapplied name.
      $is_synonym = isset($synonym_uuids[$taxon->uuid]); //TODO better use the $taxon->class attribute?
      $taxon_type = $is_synonym ? "Synonym" : "misapplied-name";

      $acceptedTaxon = $table_of_accepted[$taxon->uuid];

      if (is_object($acceptedTaxon)) {

        $taxonUri = uri_to_synonym($taxon->uuid, $acceptedTaxon->uuid);
        $referenceUri = '';
        if (isset($acceptedTaxon->name->nomenclaturalReference)) {
          $referenceUri = url(path_to_reference($acceptedTaxon->name->nomenclaturalReference->uuid));
        }
        $taxon_or_name = $is_synonym ? $taxon->name : $taxon;
        // $taxon_or_name this is a trick to suppress the sec reference for sysnonyms
        // supplying the name will cause render_taxon_or_name() to not show the sec reference
        $out .= '<li class="' . $taxon_type . '">' . render_taxon_or_name($taxon_or_name, $taxonUri, $referenceUri);
        if ($show_classification) {
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
          $out .= ' : <span class="classifications">' . implode(', ', $classification_titles) . '</span>';
        }
        $out .= theme('cdm_annotations_as_footnotekeys', array('cdmBase_list' => $taxon));
        if ($showMedia_synonyms) {
          $out .= theme('cdm_taxon_list_thumbnails', array('taxon' => $acceptedTaxon));
        }
      }
    }
    else {
      // Its a Taxon.
      $taxonUri = url(path_to_taxon($taxon->uuid));
      $referenceUri = '';
      if (isset($taxon->name->nomenclaturalReference)) {
        $referenceUri = url(path_to_reference($taxon->name->nomenclaturalReference->uuid));
      }
      $out .= '<li class="Taxon">' . render_taxon_or_name($taxon, $taxonUri, $referenceUri);
      if ($show_classification) {
        $classifications = get_classifications_for_taxon($taxon);
        $classification_titles = array();
        foreach ($classifications as $classification) {
          if (isset($classification->titleCache)) {
            $classification_titles[] = $classification->titleCache;
          }
        }
        if(count($classification_titles) == 0){
          $classification_titles[] = $unclassified_snippet;
        }
        $out .= ' : <span class="classifications">' . implode(', ', $classification_titles) . '</span>';
      }
      $out .= theme('cdm_annotations_as_footnotekeys', array('cdmBase_list' => $taxon));

      if ($showMedia_taxa) {
        $out .= theme('cdm_taxon_list_thumbnails', array('taxon' => $taxon));
      }
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

    $out .= '</li>';
  }

  $out .= '</ul>';
  RenderHints::popFromRenderStack();

  return markup_to_render_array($out); // TODO create render array of all list items in function
}


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

