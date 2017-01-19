<?php
/**
 * @file
 * Only contains the hook_theme() implementation.
 *
 * @copyright
 *   (C) 2007-2012 EDIT
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
 * Implements hook_theme().
 *
 * Registering themes with a theme hook is needed since Drupal 6.
 *
 * @author W. Addink <w.addink@eti.uva.nl>
 */
function cdm_dataportal_theme() {
  return array(
    // Themes in help.php.
    'cdm_dataportal_admin_config_cdm_dataportal_general' => array('variables' => array()),
    'cdm_dataportal_admin_config_cdm_dataportal_geo' => array('variables' => array()),
    'cdm_dataportal_admin_config_cdm_dataportal_layout' => array('variables' => array()),
    'cdm_dataportal_admin_config_cdm_dataportal_cachesite' => array('variables' => array()),
    'cdm_dataportal_admin_config_more_help_link' => array('variables' => array()),

    // Themes in settings.php.
    'checkboxes_preferred' => array('render element' => 'element'),
    'radio_options_suffix' => array('render element' => 'element'),

    // Themes in cdm_dataportal.common.theme.
    'cdm_annotations_as_footnotekeys' => array('variables' => array('cdmBase_list' => NULL, 'footnote_list_key' => NULL)),
    'cdm_annotation_footnotes' => array('variables' => array('footnoteListKey' => NULL, 'enclosingTag' => 'span')),
    'cdm_annotation_content' => array('variables' => array('AnnotationTO' => NULL)),
    'cdm_list_of_annotations' => array('variables' => array('annotationElements' => NULL)),
    'cdm_footnote_key' => array('variables' => array(
        'footnoteKey' => NULL,
        'separator' => '',
        'highlightable' => TRUE,
        'separator_off' => FALSE,
    )),
    'cdm_footnote' => array('variables' => array('footnoteKey' => NULL, 'footnoteText' => NULL, 'enclosing_tag' => 'span')),
    'cdm_footnotes' => array('variables' => array(
        'footnoteListKey' => NULL,
        'enclosingTag' => 'span',
    )),
    'cdm_annotations' => array('variables' => array('annotations' => NULL, 'enclosingTag' => 'span')),
    'cdm_marker' => array('variables' => array('label' => NULL, 'attributes' => NULL)),
    'cdm_pager' => array('variables' => array('pager' => NULL, 'path' => NULL, 'parameters' => NULL)),
    'cdm_pager_link' => array('variables' => array(
        'text' => NULL,
        'linkIndex' => NULL,
        'pager' => NULL,
        'path' => NULL,
        'parameters' => array(),
        'attributes' => NULL,
    )),
    'cdm_back_to_image_gallery_button' => array('variables' => array()),
    'cdm_print_button' => array('variables' => array()),

    // Themes in cdm_dataportal.descriptions.theme.
    'cdm_feature_name' => array('variables' => array('feature_name' => NULL)),
    'FeatureTree_hierarchy' => array('variables' => array('FeatureTreeUuid' => NULL)),
    'FeatureTree_hierarchy_children' => array('variables' => array('node' => NULL)),
    'cdm_IdentificationKey' => array('variables' => array(
        'identificationKey' => NULL,
        'doLinkToKeyPage' => TRUE,
        'showIdentificationKeyTitle' => TRUE,
    )),
    'cdm_polytomousKey' => array('variables' => array('polytomousKey' => NULL)),
    'cdm_polytomousKey_linkedStyle_subgraph' => array('variables' => array('polytomousKeyNode' => NULL)),
    'cdm_poytomousKeyNode_modifyingText' => array('variables' => array('modifyingText' => NULL)),
    'cdm_list_IdentificationKeys' => array('variables' => array('type' => NULL, 'taxonUuid' => NULL)),
    'cdm_block_IdentificationKeys' => array('variables' => array('taxonUuid' => NULL)),
    'cdm_UseDescription' => array('variables' => array('description' => NULL, 'taxonUuid' => NULL)),
    'cdm_block_Uses' => array('variables' => array('taxonUuid' => NULL)),

    // Themes in cdm_dataportal.media.theme.
    'cdm_media_mime_application' => array('variables' => array('mediaRepresentation' => NULL, 'feature' => NULL)),
    'cdm_media_mime_image' => array('variables' => array('mediaRepresentation' => NULL, 'feature' => NULL)),
    'cdm_media_mime_text' => array('variables' => array('mediaRepresentation' => NULL, 'feature' => NULL)),
    'cdm_media_caption' => array('variables' => array(
        'media' => NULL,
        'elements' => array('title', 'description', 'artist', 'location', 'rights'),
        'sources_as_content' => FALSE
    )),
    // Themes in cdm_dataportal.name.theme.
    'cdm_typedesignations' => array('variables' => array('typeDesignations' => array())),
    'cdm_nameRelationships' => array('variables' => array('nameRelationships' => NULL, 'skipTypes' => FALSE)),
    'cdm_homotypicSynonymLine' => array('variables' => array('taxon' => NULL)),
    'cdm_heterotypicSynonymyGroup' => array('variables' => array('homotypicalGroup' => NULL)),
    'cdm_homotypicSynonymyGroup' => array('variables' => array(
        'synonymList' => NULL,
        'accepted_taxon_name_uuid' => NULL,
        'prependedSynonyms' => array(),
    )),
    'get_nameRenderTemplate' => array('variables' => array()),
    'get_partDefinition' => array('variables' => array()),

    // Themes in cdm_dataportal.occurrence.theme.
    'cdm_specimen' => array('variables' => array('specimenTypeDesignation' => NULL)),

    // Themes in cdm_dataportal.page.theme.
    'cdm_specimen_page_title' => array('variables' => array('specimen' => NULL)),
    'cdm_taxon_page_title' => array('variables' => array('taxon' => NULL)),
    'cdm_name_page_title' => array('variables' => array('taxon_name' => NULL)),
    'cdm_taxon_page_synonymy' => array('variables' => array('taxon' => NULL, 'addAcceptedTaxon' => NULL)),
    'cdm_reference_page' => array('variables' => array('reference' => NULL)),
    'cdm_media_page' => array('variables' => array(
        'media' => NULL,
        'mediarepresentation_uuid' => FALSE,
        'partId' => FALSE,
    )),
    'cdm_polytomousKey_page' => array('variables' => array('polytomousKey' => NULL)),

    // Themes in cdm_dataportal.references.theme.
    'cdm_reference' => array('variables' => array(
        'reference' => NULL,
        'microReference' => NULL,
        'doLink' => FALSE,
        'referenceStyle' => NULL,
    )),
    'cdm_OriginalSource' => array('variables' => array('source' => NULL, 'doLink' => TRUE, 'do_link_to_name_used_in_source' => FALSE)),

    // Themes in cdm_dataportal.taxon.theme.
    'cdm_search_results' => array('variables' => array(
        'pager' => NULL,
        'path' => NULL
    )),
    'cdm_taxon_list_thumbnails' => array('variables' => array('taxon' => NULL)),

    // Themes in includes/pre-drupal8.inc
    'description_list' => array('variables' => array('items' => array(), 'groups' => array(), 'title' => NULL, 'attributes' => array())),
  );
}
