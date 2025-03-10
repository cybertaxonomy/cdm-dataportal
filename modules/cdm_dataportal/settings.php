<?php
/**
 * @file
 * CDM Dataportal settings.
 */


const FEATURE_BLOCK_WEIGHT_INCREMENT = 2;
const PSEUDO_FEATURE_AGGREGATION_DESCRIPTIONS = 'AGGREGATION_DESCRIPTIONS';
const PSEUDO_FEATURE_BIBLIOGRAPHY = 'BIBLIOGRAPHY';
const PSEUDO_FEATURE_NUMBER_OF_TAXA = 'NUMBER_OF_TAXA';

const CDM_NAME_PAGE_AUTOREDIRECT = 'cdm_name_page_autoredirect';
const CDM_SHOW_NAME_IDENTIFIER = 'cdm_show_name_identifier';
const CDM_SHOW_ACCESSION_TYPE = 'cdm_show_accession_type';

const CDM_NAME_PAGE_SECTION_TAXA = 'cdm_name_page_section_taxa';
const CDM_NAME_PAGE_SECTION_TAXA_DEFAULT = 1;

const CDM_MEDIA_GALLERY_VIEWER = 'image_gallery_viewer';
const CDM_MEDIA_GALLERY_VIEWER_DEFAULT = 'universalviewer';
const CDM_MEDIA_GALLERY_VIEWER_SIMPLE = 'default';
const CDM_META_DATA_SOURCE = 'media_meta_data_source';
const CDM_META_DATA_SOURCE_DEFAULT = 'mediaServer';

const CDM_ADDITIONAL_MEDIA_COPYRIGHT = 'cdm_additional_media_copyright';

const CDM_TAXONPAGE_TAXON_NODE_SHOW_STATES = 'cdm_taxonpage_taxon_node_show_status';
define('TAXONTREE_RANKLIMIT', 'cdm_taxontree_ranklimit');
define('TAXONTREE_RANKLIMIT_DEFAULT', 0);
  define('CDM_TAXONOMICTREE_UUID', 'cdm_taxonomictree_uuid');
  define('CDM_TAXONTREE_INCLUDES', 'taxontree_includes');

  define('CDM_SUB_TREE_FILTER_UUID', 'cdm_sub_tree_filter_uuid');

  define('NO_SORT', -1);
  define('SORT_HIERARCHICAL', 9);

  const CDM_SEARCH_RESULT_PAGE_SIZE_DEFAULT = 25;
  const CDM_SEARCH_RESULT_PAGE_SIZE = 'cdm_dataportal_search_items_on_page';

  define('SEARCH_RESULTS_SHOW_THUMBNAIL_CHECKBOX_DEFAULT', 1);
  define('SEARCH_RESULTS_SHOW_THUMBNAIL_CHECKBOX', 'search_results_show_thumbnail_checkbox');
 const CDM_DATAPORTAL_NOMREF_IN_TITLE = 'cdm_dataportal_nomref_in_title';
 const CDM_DATAPORTAL_NOMREF_IN_TITLE_DEFAULT =  1;
 const CDM_SPECIMEN_LIST_VIEW_MODE = 'cdm_specimen_list_view_mode';
 const CDM_SPECIMEN_LIST_VIEW_MODE_DEFAULT = 'derivate_table';
 const CDM_SPECIMEN_LIST_VIEW_MODE_OPTION_DERIVATE_TABLE = 'derivate_table';
 const CDM_SPECIMEN_LIST_VIEW_MODE_OPTION_DERIVATE_TREE = 'derivate_tree';
 const CDM_SPECIMEN_LIST_VIEW_MODE_OPTION_DERIVATE_PATH = 'derivate_path';
 const CDM_SPECIMEN_DERIVATE_TREE_OPTIONS = 'cdm_specimen_derivate_tree_options';
 const CDM_SPECIMEN_DERIVATE_TREE_OPTIONS_DEFAULT = ['field_unit_short_label' => 0];
 const CDM_SPECIMEN_RELATION = 'cdm_specimen_relation';
 const CDM_SPECIMEN_RELATION_DEFAULT = array('IA'=>'IA', 'TD'=>'TD', 'DET'=>'DET', 'CDET'=>'CDET');
 const CDM_SPECIMEN_RELATION_OPTION_IND_ASS = 'IA';
 const CDM_SPECIMEN_RELATION_OPTION_DET = 'DET';
 const CDM_SPECIMEN_RELATION_OPTION_TD = 'TD';
const CDM_SPECIMEN_RELATION_OPTION_CDET = 'CDET';
 
 const CDM_SEC_WITH_FOOTNOTE = 'footnote';
 const CDM_SEC_INTERNAL_LINK = 'internal_link';
 const CDM_SEC_WITHOUT_COMPLETE_REF = 'without_complete_ref';

 const CDM_DTO_PORTAL_PAGE = 'cdm_dto_mode';
 const CDM_DTO_LOADING = 'cdm_dto_loading';

  define('CDM_SYNONYMY_ACCEPTED_TAXON_SEC_SEPARATE', 'cdm_synonymy_accepted_taxon_sec_separate');
  define('CDM_SYNONYMY_SYN_SEC_PER_HOMOTYPIC_GROUP', 'cdm_synonymy_syn_sec_per_homotypic_group');
  define('CDM_SYNONYMY_ACCEPTED_TAXON_SEC_FOOTNOTE', 'cdm_synonymy_accepted_taxon_sec_footnote');
  define('CDM_SYNONYMY_ACCEPTED_TAXON_SEC_SEPARATE_LABEL', 'cdm_synonymy_accepted_taxon_sec_separate_label');
  define('CDM_SYNONYMY_ACCEPTED_TAXON_SEC_SEPARATE_LABEL_DEFAULT', 'Source');
  define('CDM_DATAPORTAL_ALL_FOOTNOTES', 0);
  define('CDM_DATAPORTAL_ANNOTATIONS_FOOTNOTES', 0);
  define('CDM_DATAPORTAL_LAST_VISITED_TAB_ARRAY_INDEX', 999);

  define('CDM_SEARCH_BLAST_SERVICE_URI', 'http://bgbm-import:9001/api/sequence');
  define('CDM_SEARCH_BLAST_ENABLED', 0);
  define('CDM_REGISTRATION_PRESISTENT_IDENTIFIER_AS_LINK', 'cdm_registration_presistent_identifier_as_link');

  /* annotationTypeKeys */
  $annotationTypeKeys = array_keys(cdm_vocabulary_as_option(UUID_ANNOTATION_TYPE));
  if (in_array(UUID_ANNOTATION_TYPE_TECHNICAL, $annotationTypeKeys)) {
    $annotationTypeKeys = array_flip($annotationTypeKeys);


    // Technical annotation are off by default.
    unset($annotationTypeKeys[UUID_ANNOTATION_TYPE_TECHNICAL]);
    $annotationTypeKeys = array_flip($annotationTypeKeys);
    $annotationTypeKeys[] = UUID_ANNOTATION_TYPE_UNTYPED;
    // Additional value for the NULL case.
    //$annotationTypeKeys[] = 'NULL_VALUE';
  }

  const ANNOTATION_TYPES_VISIBLE = 'annotations_types_as_footnotes';
  define('ANNOTATION_TYPES_VISIBLE_DEFAULT', serialize($annotationTypeKeys));

/* extensionTypeKeys */
$extensionTypeKeys = array_keys(cdm_vocabulary_as_option(UUID_EXTENSION_TYPE));
if (in_array(UUID_ANNOTATION_TYPE_TECHNICAL, $extensionTypeKeys)) {
  // Additional value for the NULL case.
  $extensionTypeKeys[] = 'NULL_VALUE';
}
const EXTENSION_TYPES_VISIBLE = 'extension_types_visible';
define('EXTENSION_TYPES_VISIBLE_DEFAULT', serialize([]));

  define('BIBLIOGRAPHY_FOR_ORIGINAL_SOURCE', 'bibliography_for_original_source');
  define('BIBLIOGRAPHY_FOR_ORIGINAL_SOURCE_DEFAULT', serialize(array(
    'enabled' => 0,
    'key_format' => 'ALPHA'
  )));
const ET_AL_POS = 'et_al_pos';

  /* taxonRelationshipTypes */
  define('CDM_TAXON_RELATIONSHIP_TYPES_DEFAULT', serialize(
    array(
      UUID_MISAPPLIED_NAME_FOR => UUID_MISAPPLIED_NAME_FOR,
      UUID_PROPARTE_MISAPPLIEDNAME_FOR => UUID_PROPARTE_MISAPPLIEDNAME_FOR,
      UUID_PARTIAL_MISAPPLIEDNAME_FOR => UUID_PARTIAL_MISAPPLIEDNAME_FOR,
      UUID_PROPARTE_SYNONYM_FOR => UUID_PROPARTE_SYNONYM_FOR,
      UUID_PARTIAL_SYNONYM_FOR => UUID_PARTIAL_SYNONYM_FOR
    )
  ));


  /* ---- MAP SETTING CONSTANTS ---- */
  /**
   * @var array of URIs eg. http://edit.africamuseum.be"
   *   An options array
   */
  define('EDIT_MAPSERVER_URI', serialize(
      array(
        'http://edit.africamuseum.be'=>'Primary (http://edit.africamuseum.be)'
      )
    )
  );
  define('EDIT_MAPSERVER_PATH', '/edit_wp5');
  /**
   * @var array of versions eg. "v1.2"
   *   An options array
   */
  define('EDIT_MAPSERVER_VERSION', serialize(
      array(
//        'v1' => 'v1' ,           // no longer recommended
//        'v1.1' => 'v1.1',        // no longer recommended
//        'v1.2_dev' => 'v1.2_dev',// no longer recommended
        'v1.2' => 'v1.2',
        'v1.3_dev' => 'v1.3_dev (not recommended)',
        'v1.4_dev' => 'v1.4_dev (latest features)'
      )
    )
  );
  define('EDIT_MAPSERVER_URI_DEFAULT', 'http://edit.africamuseum.be');
  define('EDIT_MAPSERVER_VERSION_DEFAULT', 'v1.2');

    // --- Taxon profile settings --- /
  define('DISTRIBUTION_TEXTDATA_DISPLAY_ON_TOP', 'distribution_textdata_on_top');
  define('CDM_TAXON_PROFILE_IMAGE', 'cdm_taxon_profile_image');
  define('CDM_TAXON_PROFILE_IMAGE_DEFAULT', serialize(
      array(
          'show' => 0,
          'maxextend' => 184,
          'media_uri_query' => '',
          'custom_placeholder_enabled' => 1,
          'custom_placeholder_image_on' => 0,
          'custom_placeholder_image_fid' => ''
      )
    )
  );


define('FEATURE_BLOCK_SETTINGS', 'feature_block_settings');

define('CDM_PSEUDO_FEATURE_BLOCK_WEIGHTS', 'cdm_pseudo_feature_block_weights');
define('CDM_PSEUDO_FEATURE_BLOCK_WEIGHTS_DEFAULT', serialize(
  [
    PSEUDO_FEATURE_NUMBER_OF_TAXA => 0 - FEATURE_BLOCK_WEIGHT_INCREMENT,
    PSEUDO_FEATURE_AGGREGATION_DESCRIPTIONS => 100,
    PSEUDO_FEATURE_BIBLIOGRAPHY => 102
  ]
));

define('DISTRIBUTION_CONDENSED', 'distribution_condensed');
define('DISTRIBUTION_CONDENSED_INFO_PATH', 'distribution_condensed_info_path');
define('DISTRIBUTION_CONDENSED_INFO_PATH_DEFAULT', 'cdm_dataportal/help/condensed_distribution');
define('DISTRIBUTION_CONDENSED_RECIPE', 'distribution_condensed_recipe');
define('DISTRIBUTION_CONDENSED_RECIPE_DEFAULT', 'EuroPlusMed');

define('DISTRIBUTION_STATUS_COLORS', 'distribution_status_colors');
define('DISTRIBUTION_ORDER_MODE', 'distribution_order_mode');
define('DISTRIBUTION_ORDER_MODE_DEFAULT', 'TREE');
define('DISTRIBUTION_TREE_OMIT_LEVELS', 'distribution_tree_omit_levels');

define('CDM_SYNONYMY_AS_TAB', 'cdm_synonymy_as_tab');
define('CDM_SYNONYMY_AS_TAB_DEFAULT', 1);

/**
 * Returns the array of implemented taxon page tabs.
 * The array has fixed integer keys which must not be changed.
 */
function get_taxon_tabs_list() {
  $opts = array(
    0 => 'General',
    1 => 'Synonymy',
    2 => 'Images',
    3 => 'Specimens',
    4 => 'Keys',
    5 => 'Experts',
  );
  if(variable_get(CDM_SYNONYMY_AS_TAB, CDM_SYNONYMY_AS_TAB_DEFAULT) !== 1){
    // skip the Synonymy if it is shown in  the general tab
    unset($opts[1]);
  }
  return $opts;
}

define('CDM_TAXONPAGE_TAB_WEIGHT', 'cdm_taxonpage_tab_weight');
define('CDM_TAXONPAGE_TAB_WEIGHT_DEFAULT', serialize(
  array(
    'general' => '-3',
    'synonymy' => '-2',
    'images' => '0',
    'specimens' => '1',
    'keys' => '3',
    'experts' => '5',
    )
));


// CDM_TAXONPAGE_TAB_LABELS_DEFAULT
define('CDM_TAXONPAGE_TAB_LABELS', 'cdm_taxonpage_tab_labels');
define('CDM_TAXONPAGE_TAB_LABELS_DEFAULT', serialize(
  array(
    'general' => null,
    'synonymy' => null,
    'images' => null,
    'specimens' => null,
    'keys' => null,
    'experts' => null,
  )
));

$taxon_tab_options = get_taxon_tabs_list();
$taxon_tab_options[CDM_DATAPORTAL_LAST_VISITED_TAB_ARRAY_INDEX] = 'Last visited tab';

define('CDM_DATAPORTAL_DEFAULT_TAXON_TAB', serialize($taxon_tab_options));

/**
 * @todo document this function.
 */
function get_taxon_options_list() {
  $taxon_tab_options = array_flip(get_taxon_tabs_list());
  foreach ($taxon_tab_options as $key => $value) {
    $taxon_tab_options[$key] = t('@key', array('@key' => $key));
  }
  return $taxon_tab_options;

}

define('CDM_SEARCH_TAXA_MODE','cdm_search_taxa_mode');
define('CDM_SEARCH_TAXA_MODE_DEFAULT', serialize(
    // to unset a default entry set the value to 0
    array(
      'doTaxa'=>'doTaxa',
      'doSynonyms' => 'doSynonyms',
      'doTaxaByCommonNames' => 0,
      'doMisappliedNames' => 'doMisappliedNames'
    )
  )
);

define('CDM_SEARCH_AREA_FILTER_PRESET', 'cdm_search_area_filter_preset');

define('SIMPLE_SEARCH_USE_LUCENE_BACKEND', 'simple_search_use_lucene_backend');
define('SIMPLE_SEARCH_IGNORE_CLASSIFICATION', 'simple_search_ignore_classification');
define('SIMPLE_SEARCH_AUTO_SUGGEST', 'cdm_search_taxa_auto_suggest');

/* Gallery variables. */
$gallery_settings = array(
    "cdm_dataportal_show_taxon_thumbnails" => 1,
    "cdm_dataportal_show_synonym_thumbnails" => 0,
    "cdm_dataportal_show_thumbnail_captions" => 1,
    "cdm_dataportal_media_maxextend" => 120,
    "cdm_dataportal_media_cols" => 3,
    "cdm_dataportal_media_maxRows" => 1,
);

define('TAXONPAGE_VISIBILITY_OPTIONS_DEFAULT', serialize(get_taxon_options_list()));
define('CDM_DATAPORTAL_GALLERY_SETTINGS', serialize($gallery_settings));
define('CDM_DATAPORTAL_SPECIMEN_GALLERY_NAME', 'specimen_gallery');
define('CDM_DATAPORTAL_TYPE_SPECIMEN_GALLERY_NAME', 'type_specimen_gallery');
define('CDM_DATAPORTAL_DESCRIPTION_GALLERY_NAME', "description_gallery");
define('CDM_DATAPORTAL_TAXON_MEDIA_GALLERY_NAME_TAB', "taxon_tab_media_gallery");
define('CDM_DATAPORTAL_SEARCH_GALLERY_NAME', "search_gallery");
define('CDM_DATAPORTAL_DISPLAY_TAXON_RELATIONSHIPS', 'cdm_dataportal_display_taxon_relationships');
define('CDM_DATAPORTAL_DISPLAY_NAME_RELATIONSHIPS', 'cdm_dataportal_display_name_relations');
// define('CDM_DATAPORTAL_DISPLAY_NAME_RELATIONSHIPS_2', array("default" => t('Display all')));
define('CDM_DATAPORTAL_DISPLAY_TAXON_RELATIONSHIPS_DEFAULT', 1);
define('CDM_DATAPORTAL_DISPLAY_NAME_RELATIONSHIPS_DEFAULT', 1);

define('CDM_STANDARD_IMAGE_VIEWER', "cdm_standard_image_viewer");
define('CDM_STANDARD_IMAGE_VIEWER_DEFAULT', serialize(array(
  'media_representation_details_enabled' => 0
)));

/**
 * The drupal variable key for the array containing the uuids of the taxon relationship types to display in
 * the synonymy.
 *
 * @var string
 */
define('CDM_TAXON_RELATIONSHIP_TYPES', 'cdm_taxon_relationship_types');

define('CDM_NAME_RELATIONSHIP_INLINE_TYPES', 'cdm_name_relationship_types');
define('CDM_NAME_RELATIONSHIP_INLINE_TYPES_DEFAULT', serialize(
    array(
      UUID_NAMERELATIONSHIPTYPE_LATER_HOMONYM => UUID_NAMERELATIONSHIPTYPE_LATER_HOMONYM,
      UUID_NAMERELATIONSHIPTYPE_TREATED_AS_LATER_HOMONYM => UUID_NAMERELATIONSHIPTYPE_TREATED_AS_LATER_HOMONYM,
      UUID_NAMERELATIONSHIPTYPE_BLOCKING_NAME_FOR => UUID_NAMERELATIONSHIPTYPE_BLOCKING_NAME_FOR,
      UUID_NAMERELATIONSHIPTYPE_AVOIDS_HOMONYM_OF => UUID_NAMERELATIONSHIPTYPE_AVOIDS_HOMONYM_OF,
      UUID_NAMERELATIONSHIPTYPE_IS_NOT => UUID_NAMERELATIONSHIPTYPE_IS_NOT
    )
  )
);

define('CDM_NAME_RELATIONSHIP_LIST_TYPES', 'cdm_name_relationship_list_types');

/**
 * The drupal variable for the configuration of the information aggregation along
 * the taxon relation ships. The mapped array is associative and holds two elements:
 *    - direct: the uuids of the taxon relationship types to take into account in invers
 *      direction.
 *    - invers: the uuids of the taxon relationship types to take into account in direct
 *      direction.
 *
 * @var String
 */
define('CDM_AGGREGATE_BY_TAXON_RELATIONSHIPS', 'cdm_aggregate_by_taxon_relationships');
define('CDM_AGGREGATE_BY_TAXON_RELATIONSHIPS_DEFAULT', serialize(
    array(
        'direct'=>array(),
        'invers'=>array()
     )
   ));
define('CDM_PROFILE_FEATURETREE_UUID', 'cdm_dataportal_featuretree_uuid');
define('CDM_OCCURRENCE_FEATURETREE_UUID', 'cdm_occurrence_featuretree_uuid');
define('CDM_DATAPORTAL_STRUCTURED_DESCRIPTION_FEATURETREE_UUID', 'cdm_dataportal_structdesc_featuretree_uuid');
define('CDM_PROFILE_MARKER_TYPE', 'cdm_dataportal_marker_type');

define('CDM_DISTRIBUTION_FILTER', 'cdm_distribution_filter');
define('CDM_DISTRIBUTION_FILTER_DEFAULT', serialize(
      array(
      'filter_rules' => array(
        'statusOrderPreference' => 1,
        'subAreaPreference' => 1,
      ),
      'hiddenAreaMarkerType' => array()
     )
));

define('CDM_AREATREE_UUID', 'cdm_areatree_uuid');
define('CDM_STATUSTREE_UUID', 'cdm_statustree_uuid');
define('DISTRIBUTION_HIERARCHY_STYLE', 'distribution_hierarchy_style');
define('DISTRIBUTION_HIERARCHY_STYLE_DEFAULT', serialize(array(
  "level_0" => array(
    'label_suffix' => '',
    'status_glue' => '',
    'item_suffix' => ': ',
    'item_glue' => ', ',
    'item_group_prefix' => '',
    'item_group_postfix' => ''
  ),
  'level_1' => array(
    'label_suffix' => '',
    'status_glue' => '‒ ', // &#8210;
    'item_suffix' => '',
    'item_glue' => '; ',
    'item_group_prefix' => '',
    'item_group_postfix' => ''
  ),
  'level_2' => array(
    'label_suffix' => '',
    'status_glue' => '‒ ', // &#8210; 
    'item_suffix' => '',
    'item_glue' => ', ',
    'item_group_prefix' => ' (',
    'item_group_postfix' => ')'
  )
)));

/**
 * Constant for the drupal variable key distribution_map_visibility
 *
 * possible values:
 *  - never
 *  - automatic
 *  - always
 */
define('DISTRIBUTION_MAP_VISIBILITY', 'distribution_map_visibility');
define('DISTRIBUTION_MAP_VISIBILITY_DEFAULT', 'automatic');

/**
 * Constant for the drupal variable key specimen_map_visibility
 *
 * possible values:
 *  - never
 *  - automatic
 *  - always
 */
define('SPECIMEN_MAP_VISIBILITY', 'specimen_map_visibility');
define('SPECIMEN_MAP_VISIBILITY_DEFAULT', 'automatic');

define('CDM_TAXON_MEDIA_FILTER', 'cdm_taxon_media_filter');
define('CDM_TAXON_MEDIA_FILTER_DEFAULT', serialize(
    array(
        'includeTaxonDescriptions' => 'includeTaxonDescriptions',
        'includeOccurrences' => 0,
        'includeTaxonNameDescriptions' => 0//,
        //'includeOriginals' => 0
     )
  ));

define('CDM_MAP_DISTRIBUTION', 'cdm_map_distribution');
define('CDM_MAP_DISTRIBUTION_DEFAULT', serialize(array(
  // needs to be merged with user setting by drupal_array_merge_deep()
  // 'width' => 512, // optimum size for OSM layers is 512
  // 'height' => 512 / 2, // optimum size for OSM layers 256
  'aspect_ratio' => 2,
  'bbox' => '', // empty to allow automatic zooming to extend
  'maxZoom' => 13, // see https://dev.e-taxonomy.eu/redmine/issues/9458
  'show_labels' => FALSE,
  'caption' => '',
  'distribution_opacity' => '0.5',
  'map_type' => 1, //  1 = 'openlayers', 0 = 'image'
  'image_map' => array(
    'base_layer' => '', // none, formerly this was cyprusdivs
    'bg_color' => '1874CD',
    'layer_style' => 'ffffff,606060,,',
  ),
  'openlayers' => array(
    'base_layers' =>  array(
      // A layer MUST NOT BE SET in the defaults,
      // otherwise it can not be overidden by the user settings:
      // 'mapproxy_vmap0' => 'mapproxy_vmap0',
      // it is sufficient to define the preferred layer,
      // since it will automatically be enabled:
      'PREFERRED' => 'open_topomap'),
    'custom_wms_base_layer' => array(
      'name' => NULL,
      'url' => NULL,
      'params' =>  NULL,
      'projection' => NULL,
      'proj4js_def' => NULL,
      'max_extent' => NULL,
      'units' => NULL,
      'untiled' => FALSE
    ),
    'wms_overlay_layer' => array(
      'name' => NULL,
      'url' => NULL,
      'params' =>  NULL,
      'untiled' => FALSE
    ),
    'show_layer_switcher' => TRUE,
    'display_outside_max_extent' => FALSE,
    'google_maps_api_key' => NULL,
  ),
  'legend' => array(
    'show' => TRUE,
    'opacity' => '0.5',
    'font_size' => 10,
    'font_style' => FALSE,
    'icon_width' => 35,
    'icon_height' => 15
  )
)));

/**
 * Merges the named array variable with the array of defaults.
 *
 * IMPORTANT: The array keys must be strings. When the keys are integers
 * the merging will not take place for these entities. Number keyed entities
 * are just appended to the result array.
 *
 * @param string $variable_name
 *     The variable name
 * @param string | array $default
 *     The array containing the default values either as array or serialized as string.
 *     Unserialization is cared for if necessary
 * @return array
 *     The merged array as returned by drupal_array_merge_deep()
 *
 * TODO compare with mixed_variable_get() duplicate functions? => result use this function instead of mixed_variable_get()
 * TODO force $default being an array
 */
function get_array_variable_merged($variable_name, $default){

    // unserialize if nessecary
    if(!is_array($default)){
      $default = unserialize($default);
    }
    $variable = variable_get($variable_name, array());
    $result = drupal_array_merge_deep($default, $variable);
    return $result;
}

/**
 * @todo document this function.
 */
function getGallerySettings($gallery_config_form_name) {
  return get_array_variable_merged($gallery_config_form_name, CDM_DATAPORTAL_GALLERY_SETTINGS);
}

/**
 * Returns the string representation of the default tab.
 *
 * @param bool $returnTabIndex
 *   Defaults to FALSE, if set true this function will return the index number
 *   of the default tab. (used to supply default values to form elements).
 */
function get_default_taxon_tab($returnTabIndex = FALSE) {

  global $user;
  $values = unserialize(CDM_DATAPORTAL_DEFAULT_TAXON_TAB);
  $user_tab_active = 'cdm_dataportal_' . $user->uid . '_default_tab_active';
  $user_tab = 'cdm_dataportal_' . $user->uid . '_default_tab';
  // Get the user value if the used has chosen to overwrite the system settings.
  $user_tab_on = variable_get($user_tab_active, FALSE);
  if ($user_tab_on) {
    $user_value = variable_get($user_tab, 0);
    $index_value = $user_value;
    // Get the system value.
  }
  else {
    $system_value = variable_get('cdm_dataportal_default_tab', 0);
    $index_value = $system_value;
  }

  // Return the index value or the string representation.
  if ($returnTabIndex) {
    return $index_value;
  }
  else {
    return ($values[$index_value]);
  }
}

/**
 * Provides the feature block settings for a specific feature which matches the $feature_uuid parameter.
 *
 * In case specifically configured settings array, like these which are stored in the drupal variables, is missing
 * one or more fields these fields are taken from the default. That is the specific settings are always merges
 * with the default.
 *
 * Note: These settings only apply to feature blocks which do not have a special rendering
 * the specially handled features (e.g.: Distribution, CommonNames) may make use of the
 * 'special' element of the settings
 *
 * @param $feature_uuid
 *   The uuid string representation of the feature to return the settings for
 *
 * @return array
 *  an associative array of settings, with the following fields:
 *    - as_list: string
 *        this setting will be used in compose_feature_block_wrap_elements() as $enclosing_tag
 *        possible values are:
 *          div: not as list,
 *          ul: as bullet list,
 *          ol: as numbered list,
 *          dl: as definition list
 *        The tag used for the inner element, thus depends on the value of this field. The
 *        inner tag name can be retrieved by the function cdm_feature_block_element_tag_name()
 *    - link_to_reference: boolean,
 *        render the reference as link, ignored if the element is NOT a DescriptionElementSource
 *    - link_to_name_used_in_source": boolean
 *        whether to show name in source information as link which will point to the according name page
 *    - sources_as_content (boolean)
 *        TRUE (int: 1):
 *          1. If element is of the CDM type TextData and the text is not empty the source references will be
 *             appended in brackets like "text (source references)". If the original source has name in source
 *             information it will be appended to the citation string,
 *             like : "(citation, as name in source; citation, as name in source)"
 *          2. if the text of the TextData is empty, the original source citations are the only content
 *             (e.g. use case CITATION) and are not put into brackets. In this case the nameInSource is
 *             prepended to the citation string like: "name in source: citation"
 *        FALSE (int: 0):
 *          Original sources are put into the bibliography(=references) pseudo feature block. If the original source
 *          citations are the only content, the resulting feature block content would only consist of footnotes.
 *          In this case the display of the respective feature block is suppressed.
 *          TODO if the bibliography is not enabled the sources will be treated as footnotes like annotations,
 *               in future however they will in this case be shown in a separate references section for each
 *               feature block.
 *    - sources_as_content_to_bibliography  (boolean)
 *        Only valid if sources_as_content == TRUE, will cause the sources to be also shown
 *        in the bibliography.
 *    - sort_elements
 *        whether and how to sort the elements
 *        possible values are the constants SORT_ASC, SORT_DESC, NULL,
 *        some feature types (Distribution) also support: SORT_HIERARCHICAL (
 *        TODO option to exclude levels, put in special?,
 *        TODO make use of this setting in compose_feature_block_wrap_elements())
 *    - element_tag
 *        specifies the tag to be used for creating the elements, only applies if "as_list" == 'div'
 *        possible values are span | div. the proper inner tag name can be retrieved by the function
 *        cdm_feature_block_element_tag_name()
 *    - special: array()
 *        an array with further settings, this field can be used for special
 *        settings for specialized rendering like for distributions
 *  }
 *
 */
function get_feature_block_settings($feature_uuid = 'DEFAULT') {
  // the default must conform to the default parameter values of
  // compose_feature_block_wrap_elements() : $glue = '', $sort = FALSE, $enclosing_tag = 'ul'
  // compose_description_element_text_data() : asListElement = NULL

  // see #3257 (implement means to define the features to show up in the taxonprofile and in the specimen descriptions)

  // ---- DEFAULTS settings

  // only needed as final option, when the settings are not having a default
  $default = array(
    'DEFAULT' => array(
      'as_list' => 'div',
      'link_to_reference' => 0,
      'link_to_name_used_in_source' => 1,
      'sources_as_content' => 0,
      'sources_as_content_to_bibliography' => 0,
      'sort_elements' => NO_SORT,
      'glue' => '',
      'element_tag' => NULL
    ),
    // settings for pseudo feature bibliography
    // only hard coded here
    'BIBLIOGRAPHY' => array(
      'as_list' => 'div',
      'link_to_reference' => 0,
      'link_to_name_used_in_source' => 1,
      'sources_as_content' => 0,
      'sources_as_content_to_bibliography' => 0,
      'sort_elements' => NO_SORT,
      'glue' => '',
      'element_tag' => NULL
    )
  );

  // will be used as preset in the settings
  $other_themes_default = array(
    'DEFAULT' => array(
      'as_list' => 'div',
      'link_to_reference' => 0,
      'link_to_name_used_in_source' => 1,
      'sources_as_content' => 0,
      'sources_as_content_to_bibliography' => 0,
      'sort_elements' => NO_SORT,
      'glue' => '',
      'element_tag' => NULL
    ),
    UUID_CITATION => array(
      'as_list' => 'div',
      'link_to_reference' => 0,
      'link_to_name_used_in_source' => 0,
      'sources_as_content' => 1,
      'sources_as_content_to_bibliography' => 0,
      'sort_elements' => SORT_ASC,
      'glue' => '',
      'element_tag' => 'div'
    ),
    UUID_DISTRIBUTION => array(
      'as_list' => 'div', // currently ignored
      'link_to_reference' => 0,
      'link_to_name_used_in_source' => 0,
      'sources_as_content' => 0,
      'sources_as_content_to_bibliography' => 0,
      'sort_elements' => NO_SORT, // will cause ...
      'glue' => '',
      'element_tag' => 'div',
      'special' => array()
    ),
    UUID_COMMON_NAME => array(
      'as_list' => 'div',
      'link_to_reference' => 0,
      'link_to_name_used_in_source' => 1,
      'sources_as_content' => 0,
      'sources_as_content_to_bibliography' => 0,
      'sort_elements' => NO_SORT,
      'glue' => '',
      'element_tag' => 'span'
    ),
  );

  // ---- Special DEFAULTS for existing portals
  // TODO:
  // this can be removed once the feature block
  // settings have been deployed for the first time to these portals

  $cichorieae_default = array(
    'DEFAULT' => array(
      'as_list' => 'div',
      'link_to_reference' => 1,
      'link_to_name_used_in_source' => 1,
      'sources_as_content' => 1,
      'sources_as_content_to_bibliography' => 0,
      'sort_elements' => NO_SORT,
      'glue' => '',
      'element_tag' => 'div'
    ),
    UUID_CITATION => array(
      'as_list' => 'div',
      'link_to_reference' => 0,
      'link_to_name_used_in_source' => 0,
      'sources_as_content' => 1,
      'sources_as_content_to_bibliography' => 0,
      'sort_elements' => SORT_ASC,
      'glue' => '',
      'element_tag' => 'div'
    ),
    UUID_CHROMOSOMES_NUMBERS => array(
      'as_list' => 'ul',
      'link_to_reference' => 1,
      'link_to_name_used_in_source' => 1,
      'sources_as_content' => 1,
      'sources_as_content_to_bibliography' => 0,
      'sort_elements' => NO_SORT,
      'glue' => '',
      'element_tag' => 'div'
    ),
    UUID_CHROMOSOMES => array(
      'as_list' => 'ul',
      'link_to_reference' => 0,
      'link_to_name_used_in_source' => 1,
      'sources_as_content' => 1,
      'sources_as_content_to_bibliography' => 0,
      'sort_elements' => NO_SORT,
      'glue' => '',
      'element_tag' => 'div'
    ),
    UUID_COMMON_NAME => array(
      'as_list' => 'div',
      'link_to_reference' => 0,
      'link_to_name_used_in_source' => 1,
      'sources_as_content' => 0,
      'sources_as_content_to_bibliography' => 0,
      'sort_elements' => NO_SORT,
      'glue' => '',
      'element_tag' => 'span'
    ),
  );

  $palmweb_default = array(
    'DEFAULT' => array(
      'as_list' => 'ul',
      'link_to_reference' => 1,
      'link_to_name_used_in_source' => 1,
      'sources_as_content' => 1,
      'sources_as_content_to_bibliography' => 1,
      'sort_elements' => NO_SORT,
      'glue' => '',
      'element_tag' => NULL
    ),
    UUID_CITATION => array(
      'as_list' => 'ul',
      'link_to_reference' => 1,
      'link_to_name_used_in_source' => 1,
      'sources_as_content' => 0,
      'sources_as_content_to_bibliography' => 1,
      'sort_elements' => SORT_ASC,
      'glue' => '',
      'element_tag' => 'div'
    ),
    UUID_DISTRIBUTION => array(
      'as_list' => 'div', // currently ignored
      'link_to_reference' => 1,
      'link_to_name_used_in_source' => 1,
      'sources_as_content' => 1, // FIXME seems to have no effect see Acanthophoenix rousselii (palmae)
      'sources_as_content_to_bibliography' => 1,
      'sort_elements' => NO_SORT, // will cause ...
      'glue' => ', ',
      'element_tag' => 'span',
      'special' => array()
    ),
  );

  $cyprus_default = $cichorieae_default;
  $cyprus_default[UUID_DISTRIBUTION] = array(
    'as_list' => 'div', // currently ignored
    'link_to_reference' => 0,
    'link_to_name_used_in_source' => 0,
    'sources_as_content' => 0,
    'sources_as_content_to_bibliography' => 0,
    'sort_elements' => NO_SORT, // will cause ...
    'glue' => ' ',
    'element_tag' => 'div',
    'special' => array()
  );

  $default_theme = variable_get('theme_default', NULL);

  switch ($default_theme) {
    case 'garland_cichorieae':
      $settings_for_theme = $cichorieae_default;
      break;
    case 'cyprus':
      // cyprus: no longer used in production,
      // but is required for selenium tests see class eu.etaxonomy.dataportal.pages.PortalPage
      $settings_for_theme = $cyprus_default;
      break;
    case 'flore_afrique_centrale':
    case 'flora_malesiana':
    case 'flore_gabon':
      $settings_for_theme = $cichorieae_default;
      $settings_for_theme[UUID_CITATION]['as_list'] = 'ul';
      break;
    case 'palmweb_2':
      $settings_for_theme = $palmweb_default;
      break;
    default:
      $settings_for_theme = $other_themes_default;
  }
  // add pseudo feature settings
  $settings_for_theme['BIBLIOGRAPHY'] = $default['BIBLIOGRAPHY'];

  // ---- END of DEFAULTS

  $saved_settings = variable_get(FEATURE_BLOCK_SETTINGS, NULL);

  $feature_block_setting = null;

  if (isset($saved_settings[$feature_uuid])) {
    $feature_block_setting = $saved_settings[$feature_uuid];
  }
  else if (isset($settings_for_theme[$feature_uuid])) {
    $feature_block_setting = $settings_for_theme[$feature_uuid];
  }
  else if (isset($settings_for_theme['DEFAULT'])) {
    $feature_block_setting = $settings_for_theme['DEFAULT'];
  }

  // now merge the default and specific settings
  $settings_to_merge = array($default['DEFAULT']);
  if(is_array($saved_settings)){
    $settings_to_merge[] = $saved_settings['DEFAULT'];
  }
  if(isset($feature_block_setting)){
    $settings_to_merge[] = $feature_block_setting;
  }
  $feature_block_setting = drupal_array_merge_deep_array($settings_to_merge);

  return $feature_block_setting;
}

/**
 * returns the current setting for the original source bibliography
 *
 * Caches internally
 *
 * @return array
 *  the setting for the original source bibliography see BIBLIOGRAPHY_FOR_ORIGINAL_SOURCE:
 *   - 'enabled': 1|0
 *   - 'key_format': one of 'latin', 'ROMAN', 'roman', 'ALPHA', 'alpha'
 */
function get_bibliography_settings($clear_cache = false){
  static $bibliography_settings = null;
  if(!$bibliography_settings || $clear_cache){
    $bibliography_settings = get_array_variable_merged(
      BIBLIOGRAPHY_FOR_ORIGINAL_SOURCE,
      BIBLIOGRAPHY_FOR_ORIGINAL_SOURCE_DEFAULT
    );
  }
  return $bibliography_settings;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function cdm_dataportal_menu_admin(&$items) {
  // Display section on admin/config page.
  $items['admin/config/cdm_dataportal'] = array(
    'title' => 'CDM Dataportal',
    'description' => 'Settings for the CDM DataPortal.',
    'position' => 'right',
    'weight' => 10,
    'page callback' => 'system_admin_menu_block_page',
    'access arguments' => array('administer cdm_dataportal'),
    'file' => 'system.admin.inc',
    'file path' => drupal_get_path('module', 'system'),
  );
  $items['admin/config/cdm_dataportal/settings'] = array(
    'title' => 'Settings',
    'description' => 'Settings for the CDM DataPortal.',
    'weight' => 0,
    'page callback' => 'drupal_get_form',
    'page arguments' => array('cdm_settings_general'),
    'access arguments' => array('administer cdm_dataportal'),
    'type' => MENU_NORMAL_ITEM,
  );
  $items['admin/config/cdm_dataportal/settings/general'] = array(
    'title' => 'General',
    'description' => 'General',
    'weight' => 0,
    'page callback' => 'drupal_get_form',
    'page arguments' => array('cdm_settings_general'),
    'access arguments' => array('administer cdm_dataportal'),
    'type' => MENU_DEFAULT_LOCAL_TASK,
  );

  $items['admin/config/cdm_dataportal/settings/cachesite'] = array(
    'title' => 'Cache',
    'description' => 'Cache',
    'access arguments' => array('administer cdm_dataportal'),
    'page callback' => 'drupal_get_form',
    'page arguments' => array('cdm_settings_cache'),
    'weight' => 10,
    'type' => MENU_LOCAL_TASK,
  );

  $items['admin/config/cdm_dataportal/settings/geo'] = array(
    'title' => 'Geo & Map',
    'description' => 'Geo & Map',
    'access arguments' => array('administer cdm_dataportal'),
    'page callback' => 'drupal_get_form',
    'page arguments' => array('cdm_settings_geo'),
    'weight' => 1,
    'type' => MENU_LOCAL_TASK,
  );

  $items['admin/config/cdm_dataportal/settings/layout'] = array(
    'title' => 'Layout',
    'description' => 'Configure and adjust the layout of your DataPortal ',
    'access arguments' => array('administer cdm_dataportal'),
    'page callback' => 'drupal_get_form',
    'page arguments' => array('cdm_settings_layout'),
    'weight' => 2,
    'type' => MENU_LOCAL_TASK,
  );

  $items['admin/config/cdm_dataportal/settings/layout/taxon'] = array(
    'title' => 'Taxon page',
    'description' => 'Configure and adjust the layout of your DataPortal ',
    'access arguments' => array('administer cdm_dataportal'),
    'page callback' => 'drupal_get_form',
    'page arguments' => array('cdm_settings_layout_taxon'),
    'weight' => 1,
    'type' => MENU_LOCAL_TASK,
  );
  $items['admin/config/cdm_dataportal/settings/layout/name-page'] = array(
    'title' => 'Name page',
    'description' => 'Configure and adjust the layout of your DataPortal ',
    'access arguments' => array('administer cdm_dataportal'),
    'page callback' => 'drupal_get_form',
    'page arguments' => array('cdm_settings_layout_name_page'),
    'weight' => 1,
    'type' => MENU_LOCAL_TASK,
  );
  $items['admin/config/cdm_dataportal/settings/layout/search'] = array(
    'title' => 'Search',
    'description' => 'Configure and adjust the layout of your DataPortal ',
    'access arguments' => array('administer cdm_dataportal'),
    'page callback' => 'drupal_get_form',
    'page arguments' => array('cdm_settings_layout_search'),
    'weight' => 2,
    'type' => MENU_LOCAL_TASK,
  );

  $items['admin/config/cdm_dataportal/settings/layout/media'] = array(
    'title' => 'Media',
    'description' => 'Configure and adjust the layout of your DataPortal ',
    'access arguments' => array('administer cdm_dataportal'),
    'page callback' => 'drupal_get_form',
    'page arguments' => array('cdm_settings_layout_media'),
    'weight' => 3,
    'type' => MENU_LOCAL_TASK,
  );

}

/**
 * @todo document this function.
 */
function cdm_help_general_cache() {
  $form = array();
  $form['cache_help'] = array(
    '#type' => 'fieldset',
    '#title' => t('Help'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['cache_help']['test'] = array('#value' => t('probando'));
  return drupal_render($form);
  $res = array();
  $res['default'] = drupal_render($help);
  return $res;
}

/**
 * Configures the settings form for the CDM-API module.
 *
 * @return array
 *   Drupal settings form.
 */
function cdm_settings_general() {

  $form['cdm_webservice'] = array(
    '#type' => 'fieldset',
    '#title' => t('CDM Server'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#description' => t('The <em>CDM Server</em> exposes data stored in a
        CDM data base to the web via RESTful web services and thus is the source of the data
        to be displayed by a CDM DataPotal.'),
  );

  $form['cdm_webservice']['cdm_webservice_url'] = array(
    '#type' => 'textfield',
    '#title' => t('CDM web service URL') . ':',
    '#description' => t('This is the URL to the CDM-Server exposing your data
      e.g. <em>"http://myserver.net/cdmserver/myproject"</em>'),
    '#default_value' => variable_get('cdm_webservice_url', NULL),
  );

  $form['cdm_webservice']['cdm_webservice_url_ssl_verify'] = array(
    '#type' => 'checkbox',
    '#title' => t('Verify SSL/TLS certificate') . ':',
    '#description' => 'The ssl/tsl certificate of the CDM web service will not be verified if this options is disabled. 
    This can be helpful if you are running your server with self-signed certificates. Since this is 
    discouraged, disabling this options should only be done if you are fully aware of the security consequences.',
    '#default_value' => variable_get('cdm_webservice_url_ssl_verify', 1),
  );

  $form['cdm_webservice']['cdm_webservice_debug'] = array(
    '#type' => 'markup',
    '#markup' => '<b>Debug CDM Web Service:</b> Debugging web services is possible via the ' . l('CDM web service debug block', 'admin/structure/block/manage/cdm_api/cdm_ws_debug/configure')
      . ' visible only for administrators',
  );

  $form['cdm_webservice']['freetext_index'] = array(
    '#type' => 'fieldset',
    '#title' => t('Freetext index'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

  // Check the cdmserver port number and display a waring if it is not port 80
  preg_match("#http[s]?://[0-9\p{L}\.]*:([0-9]*)/.*#u", variable_get('cdm_webservice_url', ''), $portNumberMatch, PREG_OFFSET_CAPTURE);
  if (isset($portNumberMatch[1]) && $portNumberMatch[1] != '80') {
    $form['cdm_webservice']['freetext_index']['message'] = array(
      '#markup' => "<div class=\"description\">"
      . t("The CDM web service URL contains a portnumber other than default HTTP port 80: '!port'
           Due to this the reindex and purge fuctions may not be working if there is a firewall in between you and the CDM Server.
           You may want to contact the maintainer of the according CDM Server in order to solve this problem.",
          array('!port' => $portNumberMatch[1][0])
        )
      . "</div>",
    );
  };

  $frontentURL = urlencode(variable_get('cdm_webservice_url', ''));
  $trigger_link_options = array(
    'attributes' => array(
      'class' => 'index-trigger'
    ),
  );
  $form['cdm_webservice']['freetext_index']['operations'] = array(
    '#markup' => "<div>" . t('Operations: !url1 !url2', array(
        '!url1' => l(t("Purge"), cdm_compose_ws_url(CDM_WS_MANAGE_PURGE, NULL, 'frontendBaseUrl=' . $frontentURL), $trigger_link_options),
        '!url2' => l(t("Reindex"), cdm_compose_ws_url(CDM_WS_MANAGE_REINDEX, NULL, 'frontendBaseUrl=' . $frontentURL), $trigger_link_options),
      ))
    . '<div id="index-progress"></div></div>',
  );

  $form['cdm_webservice']['freetext_index']['cdm_login'] = array(
    // this must not be stored, it is only used by the _add_js_cdm_ws_progressbar
    '#type' => 'textfield',
    '#title' => t('Login'),
    '#description' => t('Your cdm user credentials in the following form: <code>user:password</code>')
  );
  drupal_add_js('
        jQuery(document).ready(function() {
             jQuery("#edit-cdm-login").change(function (e) {
                var login = jQuery(e.target).val();
                jQuery("#edit-freetext-index .index-trigger").each(function(index){
                   var url = jQuery(this).attr("href");
                   url = url.replace(/:\/\/[^@]+@|:\/\//, "://" + login + "@");
                   jQuery(this).attr("href", url);
                });
            });
        });
      ',
    array(
      'type' => 'inline',
      'scope' => 'footer'
    )
  );
  _add_js_cdm_ws_progressbar(".index-trigger", "#index-progress");

  $form['cdm_webservice']['proxy'] = array(
    '#type' => 'fieldset',
    '#title' => t('Proxy'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['cdm_webservice']['proxy']['cdm_webservice_proxy_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Proxy URL') . ':',
    '#description' => t('If this proxy url is set the cdm api tries
    to connect the web service over the given proxy server.
    Otherwise proxy usage is deactivated.'),
    '#default_value' => variable_get('cdm_webservice_proxy_url', FALSE),
  );

  $form['cdm_webservice']['proxy']['cdm_webservice_proxy_port'] = array(
    '#type' => 'textfield',
    '#title' => t('Proxy port') . ':',
    '#default_value' => variable_get('cdm_webservice_proxy_port', '80'),
  );

  $form['cdm_webservice']['proxy']['cdm_webservice_proxy_usr'] = array(
    '#type' => 'textfield',
    '#title' => t('Login') . ':',
    '#default_value' => variable_get('cdm_webservice_proxy_usr', FALSE),
  );

  $form['cdm_webservice']['proxy']['cdm_webservice_proxy_pwd'] = array(
    '#type' => 'textfield',
    '#title' => t('Password') . ':',
    '#default_value' => variable_get('cdm_webservice_proxy_pwd', FALSE),
  );

  $form['sub_tree_filter'] = array(
    '#type' => 'fieldset',
    '#title' => t('Subtree filter'),
    '#collapsible' => FALSE,
    '#collapsed' => TRUE,
    '#description' => t('The Data Portal can be restricted to the taxa which belong to a specific classification subtree. Other taxa will not be shown or accessible otherwise.'),
  );

  $sub_tree_filter_uuid_value = variable_get(CDM_SUB_TREE_FILTER_UUID, FALSE);
  $taxon_node_info = '';
  if($sub_tree_filter_uuid_value){
    $taxon_node_dto = cdm_ws_get(CDM_WS_TAXONNODE, array($sub_tree_filter_uuid_value));
    if(isset_not_empty($taxon_node_dto) && isset_not_empty($taxon_node_dto->taggedTitle)){
      $taxon_node_info = " Current taxon node: <strong>" . cdm_tagged_text_to_markup($taxon_node_dto->taggedTitle) . " [" . $taxon_node_dto->treeIndex . "]</strong>";
    }
  }
  $form['sub_tree_filter'][CDM_SUB_TREE_FILTER_UUID] = array(
    '#type' => 'textfield',
    '#title' => t('Taxon Node UUID') . ':',
    '#default_value' => variable_get(CDM_SUB_TREE_FILTER_UUID, FALSE),
    '#description' => "The uuid of the TaxonNode entity to be used as filter." . $taxon_node_info
  );

  // TODO: settings are still incomplete, compare with
  // trunk/dataportal/inc/config_default.php.inc.
  $form['taxon_tree'] = array(
    '#type' => 'fieldset',
    '#title' => t('Taxon Tree'),
    '#collapsible' => FALSE,
    '#collapsed' => TRUE,
    '#description' => t('<p>When you explore your collection, you can navigate
      it through a tree structure also called <em>Taxon Tree</em>.</p><p>To be
      able to navigate through your collection the
      <a href="http://drupal.org/handbook/blocks">drupal block</a>
      <em>CDM Taxon Tree</em> should be visible for users. Enable the block at
      <a href="./?q=admin/build/block">Administer&#45&#62Site building&#45&#62Blocks
      </a></p>'),
  );

  $form['taxon_tree'][CDM_TAXONOMICTREE_UUID] = array(
    '#type' => 'select',
    '#title' => t('Default classification') . ':',
    '#default_value' => variable_get(CDM_TAXONOMICTREE_UUID, FALSE),
    '#options' => cdm_get_taxontrees_as_options(),
    '#description' => t('The default taxa classification to be used in the whole portal. 
      This affects most obviously the classification browser and the search in case it is restricted 
      to a specific classification. See also <i>Included Classifications</i> below.'),
  );

  $form['taxon_tree'][TAXONTREE_RANKLIMIT] = array(
    '#type' => 'select',
    '#title' => t('Rank of highest displayed taxon') . ':',
     // Before DEFAULT_TAXONTREE_RANKLIMIT_UUID.
    '#default_value' => variable_get(TAXONTREE_RANKLIMIT, TAXONTREE_RANKLIMIT_DEFAULT),
    '#options' => cdm_vocabulary_as_option(UUID_RANK, null, true),
    '#description' => t('This is the rank of the highest displayed taxon in the
      <em>taxon tree</em>. You can select here which rank should be at the top
      level of the tree structure.'),
  );

  $classification_uuids = array_keys(cdm_get_taxontrees_as_options());
  $taxontree_includes_default = array_combine($classification_uuids, $classification_uuids);
  $form['taxon_tree'][CDM_TAXONTREE_INCLUDES] = array(
      '#type' => 'checkboxes',
      '#title' => t('Included Classifications') . ':',
      '#default_value' => variable_get(CDM_TAXONTREE_INCLUDES, $taxontree_includes_default),
      '#options' => cdm_get_taxontrees_as_options(),
      '#description' => t('Only the checked classifications will be available 
        in the classification selection element of the taxon tree browser and in search forms.'),
  );

  $form['distribution'] = array(
      '#type' => 'fieldset',
      '#title' => t('Distributions'),
      '#collapsible' => FALSE,
      '#description' => 'This section covers general settings regarding distributions, map related settings are found in the '
          . l('geo & map tab', 'admin/config/cdm_dataportal/settings/geo') .
          '. Further settings regarding the Distribution feature block can be found in the Layout/Taxon tab at two distinct places: '
          . l('Distribution appearance', 'admin/config/cdm_dataportal/settings/layout/taxon', array('fragment' => 'edit-distribution-layout')) .', '
          . l('Taxon profile feature block settings', 'admin/config/cdm_dataportal/settings/layout/taxon', array('fragment' => 'edit-feature-block-settings')) .
          '<p>
          </p>',
  );

  $form['distribution'][CDM_DISTRIBUTION_FILTER] = array(
      '#type' => 'fieldset',
      '#title' => 'Distribution filter',
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
      '#tree' => TRUE,
      '#description' => 'The Distribution filter offers the following options
      <ul>
      <li><strong>Status order preference rule:</strong> In case of multiple distribution status (<code>PresenceAbsenceTermBase</code>) for
        the same area the status with the highest order is preferred, see <code>OrderedTermBase.compareTo(OrderedTermBase)</code>.</li>
      <li><strong>Sub area preference rule:</strong> If there is an area with a direct sub-area and both areas have the same
        computed status only information on the sub-area should be reported, whereas the super-area should be ignored.</li>
      <li><strong>Marked area filter:</strong> Skip distributions for areas having a <code>true</code> marker with one of the specified marker types.
        Existing sub-areas of a marked area must also be marked with the same marker type, otherwise the marked
        area acts as a fallback area for the sub-areas. An area is a <em>fallback area</em> if it is marked to
        be hidden and if it has at least one sub-area which is not marked to be hidden. The <em>fallback area</em>
        will be shown on a map and used in a condensed distributions string if there is no distribution for any of the non-hidden
        sub-areas. In the textual representation it will shown if distribution data for the fallback area itself exists. For more detailed discussion on
        <em>fallback areas</em> see the ticket <a href="https://dev.e-taxonomy.eu/redmine/issues/4408">#4408</a>.</li>
      </ul>'
  );

  $cdm_distribution_filter = get_array_variable_merged(CDM_DISTRIBUTION_FILTER, CDM_DISTRIBUTION_FILTER_DEFAULT);
  $form['distribution'][CDM_DISTRIBUTION_FILTER]['filter_rules'] = array(
      '#type' => 'checkboxes',
      '#title' => 'Filter rules',
      '#default_value' => $cdm_distribution_filter['filter_rules'],
      '#options' => array(
          'statusOrderPreference' => 'Status order preference rule',
          'subAreaPreference' => 'Sub area preference rule'
      ),
  );

  $marker_type_options = cdm_terms_by_type_as_option('MarkerType');
  $form['distribution'][CDM_DISTRIBUTION_FILTER]['hiddenAreaMarkerType'] = array(
      '#type' => 'checkboxes',
      '#title' => 'Fallback area filter',
      '#default_value' => $cdm_distribution_filter['hiddenAreaMarkerType'],
      '#options' => $marker_type_options,
      '#description' => 'Check one or more MarkerTypes to define the "fallback area" filter.',
  );

  $form['aggregation'] = array(
      '#type' => 'fieldset',
      '#title' => t('Aggregation of data'),
      '#collapsible' => FALSE,
      '#description' => 'This section covers the different aspects of aggregating information.
          <p>
          </p>',
  );

  $form['aggregation'][CDM_TAXON_MEDIA_FILTER] = array(
      '#type' => 'checkboxes',
      '#title' => 'Taxon media filter',
      '#default_value' => variable_get(CDM_TAXON_MEDIA_FILTER, unserialize(CDM_TAXON_MEDIA_FILTER_DEFAULT)),
      '#options' => array(
          'includeTaxonDescriptions' => 'Media in taxon descriptions',
          'includeTaxonNameDescriptions' => 'Media in name descriptions',
          'includeOccurrences' => 'Media related to specimens and occurrences'//,
          //'includeOriginals' => 'Media related to the originals of specimens and occurrences',
      ),
      '#description' => 'This filter configures which images should be taken into account.',
  );

  $form['aggregation']['notice'] = array(
      '#markup' => '<strong>NOTICE:</strong> The below aggregation settings can slow down the data portal, so you may want to sensibly apply these setting and you may also
          want to make use of the caching capabilities of the dataportal.',
  );

  $form['aggregation']['media_aggregation'] = array(
      '#type' => 'fieldset',
      '#title' => t('Media aggregation'),
      '#collapsible' => FALSE,
      '#collapsed' => TRUE,
      '#description' => t("The media aggregation is also affected by the settings in \"<strong>Aggregation via taxon relationships</strong>\" below."),

  );
  $form['aggregation']['media_aggregation']['cdm_images_include_children'] = array(
      '#type' => 'select',
      '#title' => t('Aggregation of taxon pictures') . ':',
      '#default_value' => variable_get('cdm_images_include_children', FALSE),
      '#options' => array(
          0 => "Show only pictures of the current taxon",
          1 => "Include pictures of taxonomic children",
      ),
      '#description' => t("Choose whether to include the images of the taxonomic children. This will affect the <em>Images</em> tab and image thumbnails like in the search results."),
  );

  $form['aggregation']['aggregate_by_taxon_relationships'][CDM_AGGREGATE_BY_TAXON_RELATIONSHIPS] = array(
      '#type' => 'fieldset',
      '#attributes' => array('class' => array('clearfix')),
      '#title' => t('Aggregation via taxon relationships'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#tree' => TRUE,
      '#description' => t('Information on taxa will be aggregated along the below chosen
          taxon relation ships. This will affect images and occurrences (specimens).
          Taxon relation ships are directed and point form one taxon to another. The taxon
          relationships to be taken into account can therefore configured for the direct direction
          and for the inverse.'),
  );

  $taxonRelationshipTypeOptions = cdm_vocabulary_as_option(UUID_TAXON_RELATIONSHIP_TYPE, '_cdm_relationship_type_term_label_callback');
  $aggregate_by_taxon_relationships = variable_get(CDM_AGGREGATE_BY_TAXON_RELATIONSHIPS, unserialize(CDM_AGGREGATE_BY_TAXON_RELATIONSHIPS_DEFAULT));

  $form['aggregation']['aggregate_by_taxon_relationships'][CDM_AGGREGATE_BY_TAXON_RELATIONSHIPS]['direct'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Direct'),
      '#options' => $taxonRelationshipTypeOptions,
      '#default_value' => $aggregate_by_taxon_relationships['direct'],
  );
  $form['aggregation']['aggregate_by_taxon_relationships'][CDM_AGGREGATE_BY_TAXON_RELATIONSHIPS]['invers'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Invers'),
      '#options' => $taxonRelationshipTypeOptions,
      '#default_value' => $aggregate_by_taxon_relationships['invers'],
  );

  $form['drupal_integration'] = array(
    '#type' => 'fieldset',
    '#attributes' => array('class'=> array('clearfix')),
    '#title' => t('Drupal integration'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#tree' => FALSE
  );

  $form['drupal_integration'][CDM_DRUPAL_NODE_CREATION] = array(
    '#type' => 'checkbox',
    '#title' => 'Create drupal nodes',
    '#default_value' => variable_get(CDM_DRUPAL_NODE_CREATION, FALSE),
    '#description' => 'Content für cdm_dataportal pages is directly retrieved from the 
    CDM webservice configured above. In order to use other drupal modules like the "Comments" module together with the 
    cdm pages it is required that drupal nodes are created and stored in the database.'
  );



  $form['drupal_integration']['drop_all_cdm_nodes_warning_pre'] = array(
    '#markup' => '<h6 style="color:red;">WARNING:</h6>
    <div class="description">Using this button, you will lose all content associated with the cdm drupal nodes which is stored in the drupal data base.</div>',
  );
  $form['drupal_integration']['drop_all_cdm_nodes'] = array(
    '#type' => 'submit',
    '#value' => t('Drop all cdm nodes'),
    '#submit' => array('drop_all_cdm_nodes_submit')
  );

  $form['drupal_integration']['drop_all_cdm_nodes_warning_post'] = array(
    '#markup' => '<div class="description">All Drupal nodes created for cdm content can be deleted at once using this button.</div>',
  );

    // ----------------------
  $form['cdm_js_devel_mode'] = array(
      '#type' => 'checkbox',
      '#title' => 'Java-script developer mode',
      '#default_value' => variable_get('cdm_js_devel_mode', FALSE),
      '#description' => 'In production envirionments the java script libraries
      the cdm_dataportal is making use of are compressed and optimized. This
      is good for performance but a caveat if you need to debug java-script. When the
      java-script developer mode is enabled the uncompressed and commented developer
      versions of java-script libraries will be used where possible.
      <br/><strong>Do not use this option in production!</strong>'
  );
  // ----------------------
  $form['cdm_debug_mode'] = array(
      '#type' => 'checkbox',
      '#title' => 'CDM page debug mode',
      '#default_value' => variable_get('cdm_debug_mode', FALSE),
      '#description' => 'When CDM page debug mode enabled the start and end of cdm entity page
      creation is logged as well as any http request send via the cdm_api. The log is written to a file in the temporary
      folder configured in the' . l('File system settings', 'admin/config/media/file-system') .
       '. For this site the file is <code> ' . file_directory_temp() . '/drupal_debug.txt</code>
      The log is written by the drupal devel module function <code>dd()</code>.
      <br/><strong>Note:</strong> The start and end of the page creation is currently only logged for taxon pages only.'
  );



  // Comment @WA: D7 form api does not support reset buttons,
  // so to mimic the D5 reset button we add one like this.
  $form['actions']['reset'] = array(
    '#markup' => '<input id="reset" type="reset" class="form-submit" value="' . t('Reset to defaults') . '" />',
    '#weight' => 1000,
  );

  $form['#submit'][] = 'cdm_settings_general_submit';

  return system_settings_form($form);
}

/**
 * Submit callback; drops all cdm nodes.
 *
 * @ingroup forms
 */
function drop_all_cdm_nodes_submit($form, &$form_state) {
  cdm_delete_all_cdm_nodes();
  drupal_set_message(t('All cdm nodes dropped.'));
}


/**
 * LAYOUT settings
 *
 * @return array
 *   The form structure.
 */
function cdm_settings_layout() {

  $form = array();

  $form['about'] = array(
    '#markup' => '<h4>' . t('Portal Layout') . '</h4><p>' . t('These settings contain the general configurations
      layout. If you want to configure the specific sites layout visit the
      respective configuration site for taxon, search or media.') . '</p>',
  );

  $form['annotations'] = array(
    '#type' => 'fieldset',
    '#title' => t('Annotations'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#description' => t('This sections allows configuring global settings regarding annotations and thus will affect annotations dispayed as footnote and others.'),
  );

  $annotation_type_options = cdm_terms_by_type_as_option('AnnotationType');
  // Additional option for the NULL case.
  $annotation_type_options[UUID_ANNOTATION_TYPE_UNTYPED] = t('untyped');
  $annotations_types_visible = variable_get(ANNOTATION_TYPES_VISIBLE, unserialize(EXTENSION_TYPES_VISIBLE_DEFAULT));
  $form['annotations'][ANNOTATION_TYPES_VISIBLE] = array(
    '#type' => 'checkboxes',
    '#title' => t('Visbility of annotation types'),
    '#description' => t("Only annotations of the selected type will be displayed. You may want to turn 'technical annotations' off."),
    '#options' => $annotation_type_options,
    '#default_value' => $annotations_types_visible
  );

  $form['extensions'] = array(
    '#type' => 'fieldset',
    '#title' => t('Extensions'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#description' => t('This sections allows configuring global settings regarding the data type extensions.'),
  );

  $extension_type_options = cdm_terms_by_type_as_option('ExtensionType');
  $extension_types_visible = variable_get(EXTENSION_TYPES_VISIBLE, unserialize(EXTENSION_TYPES_VISIBLE_DEFAULT));
  $form['extensions'][EXTENSION_TYPES_VISIBLE] = array(
    '#type' => 'checkboxes',
    '#title' => t('Visbility of extension types'),
    '#description' => t("Only extensions of the selected type will be displayed."),
    '#options' => $extension_type_options,
    '#default_value' => $extension_types_visible
  );


  // ---- original source --- //
  $form[BIBLIOGRAPHY_FOR_ORIGINAL_SOURCE] = array(
      '#type' => 'fieldset',
      '#tree' => TRUE,
      '#title' => t('Source Citations'),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
  );

  $bibliography_settings = get_bibliography_settings(true);

  $form[BIBLIOGRAPHY_FOR_ORIGINAL_SOURCE]['enabled'] = array(
      '#type' => 'checkbox',
      '#title' => t('Original Source in bibliography'),
      '#default_value' => $bibliography_settings['enabled'],
      '#description' => t('Show original source citations in bibliography block, instead of rendering them with other
       annotations in each feature block.<br/><br/>Whether the Original Source reference of a Feature Block is actually put 
       into the bibliography also depends on the settings in the ' .
        l("Taxon profile feature block settings", "admin/config/cdm_dataportal/settings/layout/taxon") .
        '. For more information please refer to the description on the settings "<em>Sources as content</em>" & "<em>Sources as content to bibliography</em>" in that settings page.'),
  );

  $form[BIBLIOGRAPHY_FOR_ORIGINAL_SOURCE]['key_format'] = array(
    '#type' => 'select',
    '#title' => t('The format of the key numerals'),
    '#default_value' => $bibliography_settings['key_format'],
    '#options' => array('latin' => 'Latin',
      'ROMAN' => 'Roman (upper case)',
      'roman' => 'Roman (lower case)',
      'ALPHA'=> 'Alphabet (upper case)',
      'alpha' => 'Alphabet (lower case)')
  );

  // --- Advanced Search --- //
  $form['asearch'] = array(
      '#type' => 'fieldset',
      '#title' => t('Advanced search'),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
  );
  $form['asearch']['cdm_dataportal_show_advanced_search'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show advanced search link'),
      '#default_value' => variable_get('cdm_dataportal_show_advanced_search', 1),
      '#description' => t('Check this box if the link to advanced search should be show below the search box.'),
  );

  // --- Name page autoredirect feature --- //
  $form['name_page'] = array(
    '#type' => 'fieldset',
    '#title' => t('Name page'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['name_page'][CDM_NAME_PAGE_AUTOREDIRECT] = array(
    '#type' => 'checkbox',
    '#title' => 'Always redirect to taxon',
    '#default_value' => variable_get(CDM_NAME_PAGE_AUTOREDIRECT, 0),
    '#description' => t('By checking this option you can globally enable the redirection behavior of the name page. 
    Depending on the context from which a user navigates to the name page the data portal chooses to show the name page or it redirects to the related taxon if there is only one. 
    This option allows to enable this behavior also for contexts in which the redirection normally is not active.'),
  );

  // --- Registrations --- //
  $form['registrations'] = array(
    '#type' => 'fieldset',
    '#title' => t('Registrations'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['registrations']['cdm_registration_presistent_identifier_as_link'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use the persistent http identifier as link'),
    '#default_value' => variable_get('cdm_registration_presistent_identifier_as_link', 0),
    '#description' => t('Switch the portal from using the drupal path <code>registration/{url encoded persistent http identifier}</code> to using the persistent http identifier directly as link.'),
  );


    // ---- et al position --- //
	$form['et_al_pos'] = array(
	    '#type' => 'fieldset',
	    '#title' => t('Et al. position'),
	    '#collapsible' => FALSE,
	    '#collapsed' => FALSE,
	  );

    $form['et_al_pos'][ET_AL_POS] = array(
        '#type' => 'textfield',
        '#title' => t('Position of <em>& al.</em> in nomenclatural teams'),
        '#default_value' => variable_get(ET_AL_POS, NULL),
        '#description' => t('If a team has more members then the given number & al. is used. E.g. if the team is <em>Rivas Mart., Wildpret, del Arco, O. Rodr.</em> and the number is <em>3</em> the team is shown as <em>Rivas Mart., Wildpret & al.</em> instead.
                             <br/>If this value is empty or < 2 the full team is shown.'),
    );

  // ---- Taxon Name Rendering --- //
  $form['taxon_name'] = array(
      '#type' => 'fieldset',
      '#title' => t('Taxon name display'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#description' => t('The display of taxon names is configured by two parts.
          The <srong>name render templates</strong> define the parts of the name to be displayed in the different areas of the data portal pages.
          The name parts are defined in the <stong>part definitions</strong>'),
  );

  $nameRenderConfiguration = new NameRenderConfiguration();

  $partDefinitionConfigStatus = $nameRenderConfiguration->partDefinitionConfigurationStatus();
  switch($partDefinitionConfigStatus){
    case NameRenderConfiguration::PRE380_CONFIGURATION:
      $which_version_message = '(These are the old default part definition from before EDIT platform release 3.8.0, you may want to reset these by clearing the text area and and submitting the form.)';
      break;
    case NameRenderConfiguration::CUSTOM_CONFIGURATION:
      $which_version_message = '(This are custom part definitions, clearing the text area and and submitting the form will reset it to the default)';
      break;
    case NameRenderConfiguration::DEFAULT_CONFIGURATION:
    default:
      $which_version_message = '(These are the default part definition.)';
  }
  $current_part_definition_json = $nameRenderConfiguration->getCurrentPartDefinitionJson();
  $default_part_definition_json = $nameRenderConfiguration->getDefaultPartDefinitionJson();
  $diff_viewer_markup = '';
  if($partDefinitionConfigStatus == NameRenderConfiguration::CUSTOM_CONFIGURATION){
    $diff_viewer_markup = diff_viewer($default_part_definition_json, $current_part_definition_json);
  }

  $which_version_message = '<div style="color:#ff0000; font-weight: bold;">'
    . $which_version_message
    . '</div>'
    . $diff_viewer_markup;

  $form['taxon_name'][NameRenderConfiguration::CDM_PART_DEFINITIONS] = array(
      '#type' => 'textarea',
      '#title' => t('Part definitions'),
      '#element_validate' => array('form_element_validate_json'),
      '#default_value' =>  $current_part_definition_json,
      '#description' => '
          <p>' . $which_version_message . '</p>
          <p>
           The part definitions define the specific parts of which a rendered taxon name plus additional information will consist.
          </p>
          <p>
           A full taxon name plus additional information can consist of the following elements:
          <ul>
             <li>name: the taxon name inclugin rank nbut without author</li>
             <li>authors:  The authors of a reference, also used in taxon names</li>
             <li>reference: the nomenclatural reference,</li>
             <li>microreference:  Volume, page number etc.</li>
             <li>status:  The nomenclatural status of a name</li>
             <li>description: name descriptions like protologues etc ...</li>
          </ul>
          </p>
          <p>
           These elements are combined in the part definitions array to from the specific parts to be rendered.
           (The taxon name "Lapsana communis L., Sp. Pl.: 811. 1753" shall be an example in the following)
           The following parts can be formed and are recognized by the system:
          <ul>
            <li>namePart: the name and rank (for example: "Lapsana communis")</li>
            <li>authorshipPart: the author (for example: "L.")</li>
            <li>nameAuthorPart: the combination of name and author part (for example: "Lapsana communis L.").</li>
               This is useful for zoological names where the authorshipPart belongs to the name and both should</li>
               be combined when a link to the taxon is rendered.</li>
            <li>referencePart: the nomencaltural reference (for example: "Sp. Pl. 1753")</li>
          <li>referenceYearPart: the publication year of the nomencaltural reference (for example: "1753")</li>
            <li>microreferencePart: usually the page number (for example ": 811.")</li>
            <li>statusPart: the nomenclatorical status</li>
            <li>descriptionPart: name descriptions like protologues etc ...</li>
          </ul>
          </p>
          <p>
           Each set of parts is dedicated to render a specific TaxonName type, the type names are used as keys for the
           specific parts part definitions:
          <ul>
            <li>BotanicalName</li>
            <li>ZoologicalName</li>
            <li>#DEFAULT: covers ViralNames and other NonViralNames
          </ul>
           An example:
          <pre>
           ' . $default_part_definition_json . '
           </pre>',
  );

  $nameRenderTemplateConfStatus = $nameRenderConfiguration->nameRenderTemplateConfigurationStatus();

  switch($nameRenderTemplateConfStatus){
    case NameRenderConfiguration::PRE380_CONFIGURATION:
      $which_version_message = '(These are the old default render templates from before EDIT platform release 3.8.0, you may want to reset these by clearing the text area and and submitting the form.)';
      break;
    case NameRenderConfiguration::CUSTOM_CONFIGURATION:
      $which_version_message = '(These are custom render templates, clearing the text area and and submitting the form will reset it to the default)';
      break;
    case NameRenderConfiguration::DEFAULT_CONFIGURATION:
    default:
      $which_version_message = '(These are the default render templates.)';
  }
  $default_render_templates_json = $nameRenderConfiguration->getDefaultRenderTemplatesJson();
  $current_render_templates_json = $nameRenderConfiguration->getCurrentRenderTemplatesJson();
  $diff_viewer_markup = '';
  if($nameRenderTemplateConfStatus == NameRenderConfiguration::CUSTOM_CONFIGURATION){
    $diff_viewer_markup = diff_viewer($default_render_templates_json, $current_render_templates_json);
  }

  $which_version_message = '<div style="color:#ff0000; font-weight: bold;">'
    . $which_version_message
    . '</div>'
    . $diff_viewer_markup;

  $form['taxon_name'][NameRenderConfiguration::CDM_NAME_RENDER_TEMPLATES] = array(
      '#type' => 'textarea',
      '#title' => t('Name render templates'),
      '#element_validate' => array('form_element_validate_json'),
      '#default_value' =>  $current_render_templates_json,
      '#description' => '
          <p>' . $which_version_message . '</p>
          <p>
          The render templates array contains one or more name render templates to be used within the page areas identified by the
          render path. The render path of taxon names can be made visible by adding the URI query parameter 
          <strong><code>RENDER_PATH=1</code></strong> to the page request.<br />
          The render path is used as key of the array sub subelements whereas the name render template array is set as value.
          The following render Path keys are currently recognized:
          <ul>
            <li>list_of_taxa</li>
            <li>acceptedFor</li>
            <li>homonym</li>
            <li>taxon_page_synonymy</li>
            <li>typedesignations</li>
            <li>taxon_page_title</li>
            <li>polytomousKey</li>
            <li>na: name + authorship</li>
            <li>nar:name + authorship + reference</li>
            <li>#DEFAULT</li>
          </ul>
          A single render template can be used for multiple render paths. In this case the according key of the render templates
          array element should be a comma separated list of render paths, without any whitespace!.
          </p>
          <p>
          A render template is an associative array. The keys of this array are referring to the keys as defined in the part
          definitions array. See <a href="#edit-cdm-part-definitions">Part definitions</a> above for more information.
          <p>
          The value of the render template element must be set to TRUE in order to let this part being rendered.
          For some parts can <strong>links</strong> can be created which lead to the accoring intity page:</br>
          The <strong>namePart</strong>, <strong>nameAuthorPart</strong>, <strong>referencePart</strong> and <strong>secReferencePart</strong> can also hold an associative array with a single
          element: array(\'#uri\' => TRUE). The value of the #uri element will be replaced by the according
          links if the paramters $nameLink or $refenceLink are given to the name render function
          (this is hard coded and cannot be configured here).',
  );

    $form['taxon_name'][CDM_SHOW_NAME_IDENTIFIER] = array(
        '#type' => 'checkbox',
        '#title' => 'Show Name Identifier',
        '#default_value' => variable_get(CDM_SHOW_NAME_IDENTIFIER, 1),
        '#description' => t('By checking this option you can enable or disable the display of name identifiers .'),
    );

    $form['taxon_name'][CDM_SHOW_ACCESSION_TYPE] = array(
        '#type' => 'checkbox',
        '#title' => 'Show Accession Type in Typus Information',
        '#default_value' => variable_get(CDM_SHOW_ACCESSION_TYPE, 1),
        '#description' => t('By checking this option you can enable or disable the display of name identifiers .'),
    );

  // @WA: D7 form api does not support reset buttons,
  // so to mimic the D5 reset button we add one like this.
  $form['actions']['reset'] = array(
    '#markup' => '<input id="reset" type="reset" class="form-submit" value="' . t('Reset to defaults') . '" />',
    '#weight' => 1000,
  );

  $form['#submit'] = array('submit_json_as_php_array');
  // #json_elements especially defined for submit_json_as_php_array()
  $form['#json_elements'] = array(NameRenderConfiguration::CDM_NAME_RENDER_TEMPLATES, NameRenderConfiguration::CDM_PART_DEFINITIONS);
  return system_settings_form($form);
}


/**
 * @param $form_name
 * @param $form_title
 * @param $collapsed
 * @param string $form_description
 *   The description for the fieldset of the gallery setting.
 * @return mixed
 */
function cdm_dataportal_create_gallery_settings_form($form_name, $form_title, $collapsed, $form_description = '') {
  $form[$form_name] = array(
    '#type' => 'fieldset',
    '#title' => t('@form-title', array('@form-title' => $form_title)),
    '#collapsible' => TRUE,
    '#collapsed' => $collapsed,
    '#tree' => TRUE,
    '#description' => $form_description,
  );

  $default_values = unserialize(CDM_DATAPORTAL_GALLERY_SETTINGS);
  $gallery_settings = variable_get($form_name, $default_values);
  if ($form_name == CDM_DATAPORTAL_SEARCH_GALLERY_NAME) {
    /*
    TODO: why cdm_dataportal_search_items_on_page does not save the value on $test???
    $form[$form_name]['cdm_dataportal_search_items_on_page'] = array(
    '#type' => 'textfield',
    '#title' => t('Search Page Size'),
    '#default_value' => $test,
    '#description' => t('Number of Names to display per page in search results.')
    );
    */
    $form[$form_name]['cdm_dataportal_show_taxon_thumbnails'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show media thumbnails for accepted taxa'),
      '#default_value' => $gallery_settings['cdm_dataportal_show_taxon_thumbnails'],
    );

    $form[$form_name]['cdm_dataportal_show_synonym_thumbnails'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show media thumbnails for synonyms'),
      '#default_value' => $gallery_settings['cdm_dataportal_show_synonym_thumbnails'],
      '#description' => '',
    );
  }

  // $showCaption = variable_get('cdm_dataportal_findtaxa_show_thumbnail_captions', 0);
  $form[$form_name]['cdm_dataportal_show_thumbnail_captions'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show captions under thumbnails'),
    '#default_value' => $gallery_settings['cdm_dataportal_show_thumbnail_captions'],
    '#description' => '',
  );

  $form[$form_name]['cdm_dataportal_media_maxextend'] = array(
    '#type' => 'textfield',
    '#title' => t('Thumbnail size (max of width or height)') . ':',
    '#default_value' => $gallery_settings['cdm_dataportal_media_maxextend'],
    '#description' => t('The maximum extend in either dimension, width or height, in pixels for the thumbnail images in the gallery.'),
  );

  $form[$form_name]['cdm_dataportal_media_cols'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of columns') . ':',
    '#default_value' => $gallery_settings['cdm_dataportal_media_cols'],
    '#description' => t('Group the thumbnails in columns: select how many
      columns the gallery should display.'),
  );

  if ($form_name == CDM_DATAPORTAL_SEARCH_GALLERY_NAME) {
    $form[$form_name]['cdm_dataportal_media_maxRows'] = array(
      '#type' => 'textfield',
      '#title' => t('Maximum number of rows') . ':',
      '#default_value' => $gallery_settings['cdm_dataportal_media_maxRows'],
      '#description' => t('You can group the thumbnails in rows, select in how
        many rows should be the thumbnails grouped.<br/><strong>Note:</strong>
        If you want an unlimited number of rows please set to 0.'),
    );
  }

  return $form;
}

/**
 * @return array
 *   The form structure.
 */
function cdm_settings_layout_taxon() {
  $collapsed = FALSE;
  $form = array();

  $form['#submit'][] = 'cdm_settings_layout_taxon_submit';
    $form['taxon_access'] = array(
        '#type' => 'fieldset',
        '#title' => t('Taxon access (experimental)'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        //'#description' => 'Data can be accessed as cdm objects or as so named data transfer objects. The current standard is accessing the cdm objects.'
    );
    $form['taxon_access'][CDM_DTO_PORTAL_PAGE] = array(
        '#type' => 'checkbox',
        '#title' => 'Use Dto page',
        '#collapsible' => FALSE,
        '#collapsed' => FALSE,
        '#default_value' => variable_get(CDM_DTO_PORTAL_PAGE, FALSE),
        '#default_value' => variable_get(CDM_DTO_PORTAL_PAGE, FALSE),
        '#description' => 'When this is activated the data are loaded as data portal optimized DTOs which results in less webservice calls and smaller objects.
        This should improve the performance but at the moment it is only usable for some portals and is in experimental state. 
        Please contact the EDIT team for more information.'
    );
    $form['taxon_access'][CDM_DTO_LOADING] = array(
        '#type' => 'checkbox',
        '#title' => 'Use Dto Loading',
        '#collapsible' => FALSE,
        '#collapsed' => FALSE,
        '#default_value' => variable_get(CDM_DTO_LOADING, FALSE),
        '#default_value' => variable_get(CDM_DTO_LOADING, FALSE),
        '#description' => 'This setting can only be activated in combination with DTO loading, if this is activated the data are loaded as data portal optimized DTOs and 
        the loading of the elements is also done with dto loading. 
        Please contact the EDIT team for more information.'
    );

  // --------- TABBED TAXON ------- //
  $form['taxon_node'] = array(
    '#type' => 'fieldset',
    '#title' => t('Taxon node'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => 'Taxa are organized in a classification tree by taxon nodes. Display of taxon node related information like states can be manged here.'
  );
  $form['taxon_node'][CDM_TAXONPAGE_TAXON_NODE_SHOW_STATES] = array(
    '#type' => 'checkbox',
    '#title' => t('Show taxon node status'),
    '#default_value' => variable_get(CDM_TAXONPAGE_TAXON_NODE_SHOW_STATES, 1),
    '#description' => t('Show the taxon node status excluded, unplaced, placement doubtful ...'),
  );

  // --------- TABBED TAXON ------- //
  $form['taxon_tabs'] = array(
    '#type' => 'fieldset',
    '#title' => t('Taxon tabs'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => 'A taxon page consists of various sections, that is content blocks, each displaying a different kind of information.'
  );

  $form['taxon_tabs']['cdm_dataportal_taxonpage_tabs'] = array(
    '#type' => 'checkbox',
    '#title' => t('Tabbed taxon page'),
    '#default_value' => variable_get('cdm_dataportal_taxonpage_tabs', 1),
    '#description' => t('If enabled the sections of a taxon page will be displayed as individual tabs'),
  );

  $form['taxon_tabs'][CDM_SYNONYMY_AS_TAB] = array(
    '#type' => 'checkbox',
    '#title' => t('Synonymy as tab'),
    '#default_value' => variable_get(CDM_SYNONYMY_AS_TAB, CDM_SYNONYMY_AS_TAB_DEFAULT),
    '#description' => t('The synonymy can be moved to its own tab. This is only applicable when the tabbed taxon page option is activated.'),
    '#disabled' =>  variable_get('cdm_dataportal_taxonpage_tabs', 1) !== 1
  );

  $form['taxon_tabs']['cdm_taxonpage_tabs_visibility'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Section/Tab visibility') . ':',
    '#default_value' => variable_get('cdm_taxonpage_tabs_visibility', get_taxon_options_list()),
    '#options' => get_taxon_options_list()
  );

  // WEIGHT
  $taxon_tabs_weights = get_array_variable_merged(CDM_TAXONPAGE_TAB_WEIGHT, CDM_TAXONPAGE_TAB_WEIGHT_DEFAULT);
  $form['taxon_tabs'][CDM_TAXONPAGE_TAB_WEIGHT] = array(
    '#title'  => 'Section/Tab order',
    '#type' => 'fieldset',
    '#collapsible' => false,
    '#tree' => true,
    '#description' => 'The weight value defines the order of the section/tab.'
  );
  // Weights range from -delta to +delta, so delta should be at least half
  // of the amount of tabs present.
  $tab_weight_delta = round(count(get_taxon_tabs_list()) / 2) + 1;
  foreach (get_taxon_tabs_list() as $label) {
    $key = strtolower($label); // turn in to string, since we need to use strings as keys
    $form['taxon_tabs'][CDM_TAXONPAGE_TAB_WEIGHT][$key] = array(
        '#title' => $label,
        '#type'  => 'weight',
        '#default_value' => $taxon_tabs_weights[$key],
        '#delta' => $tab_weight_delta
    );
  }

  $taxon_tabs_labels = get_array_variable_merged(CDM_TAXONPAGE_TAB_LABELS, CDM_TAXONPAGE_TAB_LABELS_DEFAULT);
  $form['taxon_tabs'][CDM_TAXONPAGE_TAB_LABELS] = array(
    '#title'  => 'Section/Tab label override',
    '#type' => 'fieldset',
    '#collapsible' => false,
    '#tree' => true,
    '#description' => 'Setting a label for a section/tab will override the default label. 
      Please enter the label text in the default language of the portal.'
  );
  foreach (get_taxon_tabs_list() as $label) {
    $key = strtolower($label); // turn in to string, since we need to use strings as keys
    $form['taxon_tabs'][CDM_TAXONPAGE_TAB_LABELS][$key] = array(
      '#title' => $label,
      '#type'  => 'textfield',
      '#default_value' => $taxon_tabs_labels[$key]
    );
  }

  $form['taxon_tabs']['cdm_dataportal_default_tab'] = array(
    '#type' => 'select',
    '#title' => t('Default tab to display') . ':',
    '#default_value' => variable_get('cdm_dataportal_default_tab', 0),
    '#options' => unserialize(CDM_DATAPORTAL_DEFAULT_TAXON_TAB),
    '#description' => t('<p>Select the default tab to display when visiting a
      taxon page. Only available if Tabbed Taxon Page is enable.</p>
      <strong>Note:</strong> After performing a search and clicking in any
      synonym, the taxon tab to be rendered will be the synonymy of the accepted
      taxon and not the above selected tab.'),
  );

  /* ======  TAXON_PROFILE ====== */
  $form['taxon_profile'] = array(
    '#type' => 'fieldset',
    '#title' => t('Taxon profile (tab)'),
    '#description' => t('<p>This section covers the settings related to the taxon
      profile tab, also known as the <strong>"General"</strong> tab.</p>'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  // ---- PROFILE PICTURE ----//

  $form['taxon_profile'][CDM_TAXON_PROFILE_IMAGE] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#title' => t('Taxon profile picture'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('This sections allows configuring the display of the so called taxon profile image which is displayed in the taxon profile tab.'),
  );

  //FIXME migrate variables:
  //  cdm_dataportal_show_default_image ---> CDM_TAXON_PROFILE_IMAGE['show']
  // FIXME
  //  enable file module in profile and in update,(a.kohlbecker, 4.9.2014: is this still an open issue?)

  $taxon_profile_image_settings = variable_get(CDM_TAXON_PROFILE_IMAGE, unserialize(CDM_TAXON_PROFILE_IMAGE_DEFAULT));

  /*
   * 'show' => 1,
   * 'maxextend' => 184,
   * 'media_uri_query' => ''
   * 'custom_placeholder_image_on' => 1,
   * 'custom_placeholder_image_fid' => ''
   */
  $form['taxon_profile'][CDM_TAXON_PROFILE_IMAGE]['show'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable profile picture'),
    '#description' => t('Show the profile picture.'),
    '#default_value' => $taxon_profile_image_settings['show'],
  );

  $form['taxon_profile'][CDM_TAXON_PROFILE_IMAGE]['maxextend'] = array(
      '#type' => 'textfield',
      '#tree' => TRUE,
      '#title' => t('Profile picture maximum extend'),
      '#default_value' =>  $taxon_profile_image_settings['maxextend'],
      '#field_suffix' => 'px',
      '#maxlength' => 4,
      '#size' => 4,
      '#description' => t('The maximum extend in either dimension, width or height, of the profile picture in pixels.')
  );

  $form['taxon_profile'][CDM_TAXON_PROFILE_IMAGE]['media_uri_query'] = array(
      '#type' => 'textfield',
      '#tree' => TRUE,
      '#title' => t('Additional URI query parameter'),
      '#default_value' =>  $taxon_profile_image_settings['media_uri_query'],
      '#maxlength' => 1024,
      '#size' => 60,
      '#description' => t('Additional query parameters to be used when requesting for the  
            profile image. E.g.: <code>width=400&height=300&quality=95&format=jpeg</code>.
            The query parameters will be appended to the uri of the media representation part
            as stored in the cdm. The query parameter string must not start with a \'&\' or  \'?\'')
  );

  $form['taxon_profile'][CDM_TAXON_PROFILE_IMAGE]['custom_placeholder_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show the placeholder image'),
    '#description' => t("A placeholder image will be shown if no taxon profile picture is available."),
    '#default_value' => $taxon_profile_image_settings['custom_placeholder_enabled']
  );

  $form['taxon_profile'][CDM_TAXON_PROFILE_IMAGE]['custom_placeholder_image_on'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use a custom placeholder image'),
      '#description' => t("This image is shown as replacement if no image of the taxon is available."),
      '#default_value' => $taxon_profile_image_settings['custom_placeholder_image_on']
  );

  if($taxon_profile_image_settings['custom_placeholder_image_on'] == 1){
    $form['taxon_profile'][CDM_TAXON_PROFILE_IMAGE]['custom_placeholder_image_fid'] = array(
        '#type' => 'managed_file',
        '#title' => t('Custom placeholder image file'),
        '#progress_indicator' => 'bar',
        '#default_value' => $taxon_profile_image_settings['custom_placeholder_image_fid'],
    //       '#name' => 'custom_placeholder_image',
        '#upload_location' => 'public://' . CDM_TAXON_PROFILE_IMAGE .'/'
    );

    if($taxon_profile_image_settings['custom_placeholder_image_fid']){
      $profile_image_file = file_load($taxon_profile_image_settings['custom_placeholder_image_fid']);
      $url = file_create_url($profile_image_file->uri);
      $form['taxon_profile'][CDM_TAXON_PROFILE_IMAGE]['preview'] = array(
                '#type' => 'item',
                '#markup' => '<div class="image-preview"><img src="' . $url . '"/></div>',
      );
    }
  } else {
    $form['taxon_profile'][CDM_TAXON_PROFILE_IMAGE]['custom_placeholder_image_fid'] = array(
      '#type' => 'hidden',
      '#default_value' => $taxon_profile_image_settings['custom_placeholder_image_fid']
    );
  }

  $options = cdm_vocabulary_as_option(UUID_RANK, null, true);
  array_unshift($options, '-- DISABLED --');
  $form['taxon_profile'][CDM_TAXON_PROFILE_IMAGE]['image_hide_rank'] = array(
    '#type' => 'select',
    '#title' => t('Hide profile picture for higher ranks') . ':',
    '#default_value' => variable_get('image_hide_rank', '0'),
    '#options' => $options,
    '#description' => t('The taxon profile picture will not be shown for taxa with rank higher that the selected.'),
  );

  // -- MEDIA THUMBNAILS -- //
  $form_name = CDM_DATAPORTAL_DESCRIPTION_GALLERY_NAME;
  $form_title = 'Taxon Profile Images';
  $form_description = '<p>The different sections in the taxon  profile can have images associated with them. These images are displayed in a gallery of thumbnails wich can be configuered here:</p>';
  $form['taxon_profile'][] = cdm_dataportal_create_gallery_settings_form($form_name, $form_title, $collapsed, $form_description);

  // ---- FEATURE TREE BLOCKS ---- //
  $form['taxon_profile']['feature_blocks'] = array(
    '#type' => 'fieldset',
    '#title' => t('Feature Blocks'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t("This section covers settings related to the taxon's
      <em>Feature Tree</em>. The <em>feature tree</em> are the taxon's
      features such as description, distribution, common names"),
  );


  $featureTrees = cdm_get_featureTrees_as_options(TRUE, TRUE);
  $profile_feature_tree = get_profile_feature_tree();
  $profile_feature_tree_uuid = $profile_feature_tree->uuid;
  if(!isset($featureTrees['options'][$profile_feature_tree_uuid])) {
    $profile_feature_tree_uuid = UUID_DEFAULT_FEATURETREE;
  }

  $form['taxon_profile']['feature_blocks'][CDM_PROFILE_FEATURETREE_UUID] = array(
    '#type' => 'radios',
    '#title' => t('Taxon profile feature tree') . ':',
    '#default_value' => $profile_feature_tree_uuid,
    '#options' =>  $featureTrees['options'],
    '#pre_render' => array('form_pre_render_conditional_form_element', 'radios_prepare_options_suffix'),
    '#options_suffixes' => $featureTrees['treeRepresentations'],
    '#description' => t('The Feature Tree selected defines the type and order 
    of the according feature blocks visible in the taxon profile page. A feature block 
    only is shown if any data for it is present. The block weight is shown after the feature label in brackets'
    ),
  );
  $marker_types = cdm_terms_by_type_as_option('MarkerType', CDM_ORDER_BY_TITLE_CACHE_ASC, NULL, TRUE);

  $marker_type = variable_get(CDM_PROFILE_MARKER_TYPE, NULL);
  $form['taxon_profile']['feature_blocks'][CDM_PROFILE_MARKER_TYPE] = array(
    '#type' => 'select',
    '#title' => t('Marker type to filter factual data'),
    '#description' => t("Only factual data set without selected marker will be displayed."),
    '#options' => $marker_types,
    '#default_value' => $marker_type
  );
  $pseudo_feature_weights = get_array_variable_merged(CDM_PSEUDO_FEATURE_BLOCK_WEIGHTS, CDM_PSEUDO_FEATURE_BLOCK_WEIGHTS_DEFAULT);
  $form['taxon_profile']['feature_blocks'][CDM_PSEUDO_FEATURE_BLOCK_WEIGHTS] = array(
    '#title'  => 'Pseudo feature block weight',
    '#type' => 'fieldset',
    '#collapsible' => false,
    '#tree' => true,
    '#description' => 'Weights for the pseudo feature blocks. The weight value defines the 
    position in the list of blocks. The weight of normal feature is defined by the position 
    of the according feature. Please see the specific feature details above to find the feature weight.'
  );
  foreach ([PSEUDO_FEATURE_NUMBER_OF_TAXA, PSEUDO_FEATURE_AGGREGATION_DESCRIPTIONS, PSEUDO_FEATURE_BIBLIOGRAPHY] as $ps_feature) {
    $form['taxon_profile']['feature_blocks'][CDM_PSEUDO_FEATURE_BLOCK_WEIGHTS][$ps_feature] = array(
      '#title' => $ps_feature,
      '#type'  => 'textfield',
      '#element_validate' => array('element_validate_number'),
      '#size' => 4,
      '#default_value' => $pseudo_feature_weights[$ps_feature]
    );
  }


  // ---- FEATURE TREE BLOCKS > LAYOUT PER FEATURE BLOCK ---- //
  $profile_feature_tree = get_profile_feature_tree();

  if (isset($profile_feature_tree->root->childNodes)) {

    $form_feature_block_layout = array(
      '#type' => 'fieldset',
      '#tree' => true,
      '#title' => t('Taxon profile feature block settings'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#description' => 'This section let\'s you define how each of the feature blocks is displayed.
      A sub form for each of the the currently selected feature tree allows to configure each feature block individually.
      The subforms have the following settings in common:<br />
      <h6>List type:</h6><div>Whether the description elements are displayed as list or not. Three different list types are available</div>
      <h6>Link to reference:</h6><div>Render the reference as link, ignored if the element is NOT a DescriptionElementSource</div>
      <h6>Link to name used in source:</h6><div>Whether to show name in source information as link which will point to the according name page</div>
      <h6>Sources as content:</h6><div><strong>If enabled:</strong><br />
            <ol>
            <li>If the element is a CDM TextData instance and if the text is not empty the source references will be
                appended in brackets like "text (source references)". If the original source has name in source
                information it will be appended to the citation string,
                like : "(citation-A, as name-in-source-A; citation-B, as name-in-source-B)"</li>
             <li>if the text of the TextData is empty, the original source citations are the only content
                (e.g. use case CITATION) and are not put into brackets. In this case the nameInSource is
                prepended to the citation string like: "name in source: citation"</li>
            </ol>
            <strong>If disabled:</strong><br />
             Original sources are put into the bibliography(=references) pseudo feature block. If the original source
             citations are the only content, the resulting feature block content would only consist of footnotes.
             In this case the display of the respective feature block is suppressed.</div>
      </dl>
      <h6>Sources as content to bibliography:</h6><div>Only valid if <em>Sources as content</em> is enabled, will cause the sources to be also shown
           in the bibliography. For this to work the bibliography must be enabled the <em>' .l(
            'Layout Settings', 'admin/config/cdm_dataportal/settings/layout', array('fragment'=>'edit-bibliography-for-original-source'))
        . '</em></div>
      <h6>Sort elements:</h6><div>Whether and how to sort the elements
           possible values are the constants SORT_ASC, SORT_DESC, NULL,
           some feature types (Distribution) also support: SORT_HIERARCHICAL</div>
      <h6>Element tag:</h6><div>The tag to be used for creating the elements, only applies if "List type" is set to <em>No list</em>
           possible values are span or div. Developers: The proper inner tag name can be retrieved by the function
           cdm_feature_block_element_tag_name()</div>
       <h6>Element glue:</h6><div>This string is used to concatenate individual items together. The glue is ignored when the items are displayed 
           as list.<b>NOTE:</b>The glue is not yet fully implemented for all features and thus may not work as expected, see '
           . l('#6831', 'https://dev.e-taxonomy.eu/redmine/issues/6831') . '</div>',
    );


    $feature_list_layout_settings_disabled = FALSE;

    // creating helper object to retrieve the default settings
    $featureNode = new stdClass();
    $featureNode->term = new stdClass();
    $featureNode->term->uuid="DEFAULT";
    $featureNode->term->representation_L10n = "Default";
    array_unshift($profile_feature_tree->root->childNodes, $featureNode);

    foreach ($profile_feature_tree->root->childNodes as $featureNode) {

      if (!$feature_list_layout_settings_disabled && isset($featureNode->term)) {

        // $subform_id must not exceed 45 characters, a uuid has 36 characters
        $subform_id = $featureNode->term->uuid;
        $feature_block_setting = get_feature_block_settings($featureNode->term->uuid);

//        $settings = mixed_variable_get($subform_id, FEATURE_TREE_LAYOUT_DEFAULTS);

        $form_feature_block_layout[$subform_id] = array(
          '#type' => 'fieldset',
          '#tree' => TRUE,
          '#title' => $featureNode->term->representation_L10n,
          '#collapsible' => FALSE,
          '#collapsed' => FALSE,
        );
        if($featureNode->term->uuid == "DEFAULT"){
          $form_feature_block_layout[$subform_id]['#description']='These are the defaults which apply to
          all feature blocks for which no specific settings have been defined. for consistency enabling links for <em>source
          references</em> and <em>names in source</em> is only possible in the defaults';
        }

        $form_feature_block_layout[$subform_id]['as_list'] = array(
          '#type' => 'select',
          '#title' => 'List type',
          '#default_value' => $feature_block_setting['as_list'],
          '#options' => array(
            'div' => 'not as list',
            'ul' => 'bullet list',
            'ol' => 'numbered list',
            'dl' => 'definition list'
          ),
        );

        if($featureNode->term->uuid == "DEFAULT"){
          $form_feature_block_layout[$subform_id]['link_to_reference'] = array(
            '#type' => 'checkbox',
            '#title' => t('Link to reference'),
            '#default_value' => $feature_block_setting['link_to_reference'],
          );

          $form_feature_block_layout[$subform_id]['link_to_name_used_in_source'] = array(
            '#type' => 'checkbox',
            '#title' => 'Link to name used in source',
            '#default_value' => $feature_block_setting['link_to_name_used_in_source'],
          );
        }

        $form_feature_block_layout[$subform_id]['sources_as_content'] = array(
          '#type' => 'checkbox',
          '#title' => 'Sources as content',
          '#default_value' => $feature_block_setting['sources_as_content'],
        );

        $form_feature_block_layout[$subform_id]['sources_as_content_to_bibliography'] = array(
          '#type' => 'checkbox',
          '#title' => 'Put sources also as content to bibliography',
          '#default_value' => $feature_block_setting['sources_as_content_to_bibliography'],
        );

        $form_feature_block_layout[$subform_id]['sort_elements'] = array(
          '#type' => 'select',
          '#title' => t('Sort elements'),
          '#default_value' => $feature_block_setting['sort_elements'],
          '#options' => array(
            NO_SORT => 'No sorting',
            SORT_ASC => 'Ascending',
            SORT_DESC => 'Descending',
            SORT_HIERARCHICAL => 'Hierarchical'
          ),
          '#description' => 'NOT YET FULLY USED! only in preparation (works partially for distributions)
          <dl>
          <dr><dt>No sorting</dt><dd>Sorting undefined</dd></dr>
          <dr><dt>Ascending</dt><dd>Alphabetically in ascending order</dd></dr>
          <dr><dt>Descending</dt><dd>Alphabetically in descending order</dd></dr>
          <dr><dt>Hierarchical</dt><dd>Use the order of items and their hierarchy. This is not possible for all feature and item types.</dd></dr>
          </dl>',
        );

        $form_feature_block_layout[$subform_id]['element_tag'] = array(
          '#type' => 'select',
          '#title' => t('Element tag'),
          '#options' => array(
            'span' => 'span',
            'div' => 'div',
            'p' => 'p'
          ),
          '#default_value' => $feature_block_setting['element_tag'],
        );
        $form_feature_block_layout[$subform_id]['glue'] = array(
          '#type' => 'textfield',
          '#title' => t('Element glue'),
          '#default_value' => $feature_block_setting['glue'],
          '#size' => 10
      );

      }
      $form['taxon_profile']['feature_blocks'][FEATURE_BLOCK_SETTINGS] = $form_feature_block_layout;
    }
  }

  // ---- STRUCTURED DESCRIPTION FEATURE TREE ---- //
  $form['taxon_profile']['structured_description_featuretree'] = array(
    '#type' => 'fieldset',
    '#title' => t('Structured Description Feature Tree'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $featureTrees = cdm_get_featureTrees_as_options();
  $profile_feature_tree_uuid = variable_get(CDM_DATAPORTAL_STRUCTURED_DESCRIPTION_FEATURETREE_UUID, UUID_DEFAULT_FEATURETREE);
  if(!isset($featureTrees['options'][$profile_feature_tree_uuid])) {
    $profile_feature_tree_uuid = NULL;
  }
  $form['taxon_profile']['structured_description_featuretree'][CDM_DATAPORTAL_STRUCTURED_DESCRIPTION_FEATURETREE_UUID] = array(
    '#type' => 'radios',
    '#title' => t('Natural language representation of structured descriptions') . ':',
    '#default_value' => $profile_feature_tree_uuid,
    '#options' => $featureTrees['options'],
    '#pre_render' => array('form_pre_render_conditional_form_element', 'radios_prepare_options_suffix'),
    '#options_suffixes' => $featureTrees['treeRepresentations'],
    '#description' => t('Taxon descriptions can be stored in a highly structured
      form. The feature tree selected here will be used to generate textual
      representation in natural language.'
    ),
  );



  // ---- DISTRIBUTION LAYOUT ---- //
  $form['taxon_profile']['distribution_layout'] = array(
    '#title' => t('Distribution'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#type' => 'fieldset',
    '#description' => 'This section covers general settings regarding the textual representation of distributions and the visibility of the map.
        Map settings regarding the geometry, layers, etc are found in the '
      . l('geo & map tab', 'admin/config/cdm_dataportal/settings/geo') .
      '. Further settings regarding the distribution feature block can be found in above in this tab at '
      . l(
        'Taxon profile feature block settings', 'admin/config/cdm_dataportal/settings/layout/taxon',
        array('fragment' => 'edit-feature-block-settings')
      )
      . ' More general settings regrading the filtering of Distributions are found at '
      . l('Distribution appearance', 'admin/config/cdm_dataportal/settings', array('fragment' => 'edit-distribution'))
      . '. (These settings here will be merged in future releases into the feature block settings)',

  );

  $form['taxon_profile']['distribution_layout'][DISTRIBUTION_MAP_VISIBILITY] = _cdm_map_visibility_setting('distribution');

  $form['taxon_profile']['distribution_layout'][DISTRIBUTION_CONDENSED] = array(
    '#type' => 'checkbox',
    '#title' => t('Condensed distribution'),
    '#default_value' => variable_get(DISTRIBUTION_CONDENSED, 0),
    '#description' => 'This option enables the display of a very compact representation
    of the distribution which includes also information on the status.',
  );

  $form['taxon_profile']['distribution_layout'][DISTRIBUTION_CONDENSED_RECIPE] = array(
    '#type' => 'select',
    '#title' => t('Condensed distribution recipe'),
    '#default_value' => variable_get(DISTRIBUTION_CONDENSED_RECIPE, DISTRIBUTION_CONDENSED_RECIPE_DEFAULT),
    '#options' => array('EuroPlusMed' => 'Euro+Med', 'FloraCuba' => 'Flora of Cuba'),
    '#description' => 'Recipe for creating the condensed distribution.',
  );

  $form['taxon_profile']['distribution_layout'][DISTRIBUTION_CONDENSED_INFO_PATH] = array(
    '#type' => 'textfield',
    '#title' => t('Condensed distribution info path'),
    '#default_value' => variable_get(DISTRIBUTION_CONDENSED_INFO_PATH, DISTRIBUTION_CONDENSED_INFO_PATH_DEFAULT),
    '#description' => 'By default the help page ' .l(DISTRIBUTION_CONDENSED_INFO_PATH_DEFAULT, DISTRIBUTION_CONDENSED_INFO_PATH_DEFAULT)
      . ' is used as target for the info link which is shown at the end of the condensed distribution string.',
  );


  $form['taxon_profile']['distribution_layout'][DISTRIBUTION_ORDER_MODE] = array(
    '#type' => 'radios',
    '#title' => t('Display mode') . ':',
    '#default_value' => variable_get(DISTRIBUTION_ORDER_MODE, DISTRIBUTION_ORDER_MODE_DEFAULT),
    '#options' => array(
      'FLAT_ALPHA' => t('Flat list'),
      'TREE' => t('Hierarchically ordered'),
    ),
    '#description' => 'Taxon distribution information is displayed with
    focus on the area of the distribution. The list of areas can either be shown
    as flat list ordered alphabetically or in the hierarchical of the parent
    area and subarea relationship. Fall back areas areas with no Distribution data
    are hidden from the area hierarchy so that their sub areas will move one level up.
    See ' . l('Distribution appearance', 'admin/config/cdm_dataportal/settings', array('fragment' => 'edit-distribution')) .
    ' for details on the <em>Marked area filter</em>.',
  );

  $form['taxon_profile']['distribution_layout'][DISTRIBUTION_HIERARCHY_STYLE] = array(
    '#type' => 'fieldset',
    '#tree' => true,
    '#title' => t('Distribution hierarchy style')
  );

  $hierarchy_styles = get_array_variable_merged(DISTRIBUTION_HIERARCHY_STYLE, DISTRIBUTION_HIERARCHY_STYLE_DEFAULT);
  foreach(array_keys($hierarchy_styles) as $level) {
    $form['taxon_profile']['distribution_layout'][DISTRIBUTION_HIERARCHY_STYLE][$level] = array(
      '#type' => 'fieldset',
      '#tree' => true,
      '#title' => t('@area-level', array('@area-level' => drupal_ucfirst((str_replace('_', ' ', $level))))),
      '#attributes' => array('class' => array('fieldset-float'))
    );
    foreach ($hierarchy_styles[$level] as $key => $value) {
      $form['taxon_profile']['distribution_layout'][DISTRIBUTION_HIERARCHY_STYLE][$level][$key] = array(
        '#type' => 'textfield',
        '#title' => t('@area-level-style', array('@area-level-style' => drupal_ucfirst((str_replace('_', ' ', $key))))),
        '#default_value' => $hierarchy_styles[$level][$key],
        '#maxlength' => 4,
        '#size' => 4
      );
    }
  }

  $level_options = cdm_vocabulary_as_option(UUID_NAMED_AREA_LEVEL, NULL, FALSE, NULL, CDM_ORDER_BY_ORDER_INDEX_ASC);
  $form['taxon_profile']['distribution_layout'][DISTRIBUTION_TREE_OMIT_LEVELS] = array(
    '#type' => 'checkboxes',
    '#title' => 'Omit area levels',
    '#options' => $level_options,
    '#default_value' => variable_get(DISTRIBUTION_TREE_OMIT_LEVELS, array()),
    '#description' => 'This option ins only applicable when distributions are hierachically orderd (see option above)!
    Areas which belong to the selected area levels will be hidden in the portal.',
  );
    $form['taxon_profile']['distribution_layout'][DISTRIBUTION_TEXTDATA_DISPLAY_ON_TOP] = array(
        '#type' => 'checkbox',
        '#title' => t('Show TextData elements on top of the map'),
        '#default_value' => variable_get(DISTRIBUTION_TEXTDATA_DISPLAY_ON_TOP, 0),
        '#description' => t('Check this if you want to appear all <code>TextData</code>
      elements on top of the map. Otherwise all <code>TextData</code>
      distribution elements will be listed below the other area elements.
      This option is useful if you need to have descriptive texts for each
      distribution map.'),
    );
    /* ====AREA TREE ===*/
    // ---- AREA TREE BLOCKS ---- //
    $form['taxon_profile']['distribution_layout']['area_trees'] = array(
        '#type' => 'fieldset',
        '#title' => t('Area Tree'),
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
        '#description' => t("This section covers settings related to the distribution  
      <em>area tree</em>."),
    );
    $areaTrees = cdm_get_areaTrees_as_options();
    $areaTrees_temp['options'][] = "Default area tree (computed from vocabularies in use)";
    $options = array_merge($areaTrees_temp['options'], $areaTrees['options']);
    $areaTrees['options'] = $options;
     //$profile_area_tree = get_profile_area_tree();
    //$profile_area_tree_uuid = $profile_area_tree->uuid;
    //if(!isset($areaTrees['options'][$profile_area_tree_uuid])) {
    //    $profile_area_tree_uuid = UUID_DEFAULT_AREATREE;
    //}
    $form['taxon_profile']['distribution_layout']['area_trees'][CDM_AREATREE_UUID] = array(
        '#type' => 'radios',
        '#title' => t('Distribution area tree') . ':',
        //  '#default_value' => $profile_area_tree_uuid,
        '#options' =>  $areaTrees['options'],
        '#pre_render' => array('form_pre_render_conditional_form_element', 'radios_prepare_options_suffix'),
        '#options_suffixes' => $areaTrees['treeRepresentations'],
        '#default_value' => variable_get(CDM_AREATREE_UUID, array()),
        '#description' => t('The area tree selected defines a filter and order (including hierarchical order) 
           on the distribution data visible in the distribution section of the taxon profile page.'
        ),
    );
    /* ====STATUS TREE ===*/
    // ----STATUS TREE BLOCKS ---- //
    $form['taxon_profile']['distribution_layout']['status_trees'] = array(
        '#type' => 'fieldset',
        '#title' => t('Status Tree'),
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
        '#description' => t("This section covers settings related to the distribution  
      <em>status tree</em>."),
    );
    $statusTrees = cdm_get_statusTrees_as_options();
    $statusTrees_temp['options'][] = "Default status tree (computed from vocabularies in use)";
    $options = array_merge($statusTrees_temp['options'], $statusTrees['options']);
    $statusTrees['options'] = $options;

    $form['taxon_profile']['distribution_layout']['status_trees'][CDM_STATUSTREE_UUID] = array(
        '#type' => 'radios',
        '#title' => t('Distribution status tree') . ':',
        //  '#default_value' => $profile_area_tree_uuid,
        '#options' =>  $statusTrees['options'],
        '#pre_render' => array('form_pre_render_conditional_form_element', 'radios_prepare_options_suffix'),
        '#options_suffixes' => $statusTrees['treeRepresentations'],
        '#default_value' => variable_get(CDM_STATUSTREE_UUID, array()),
        '#description' => t('The status tree selected defines a filter (but no order) on
           the distribution data visible in the distribution section of the taxon profile page.'
        ),
    );


    $form['taxon_profile']['distribution_layout']['status_colours'] = array(
        '#type' => 'fieldset',
        '#title' => t('Distribution Status Colours'),
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,

    );
  $form['taxon_profile']['distribution_layout']['status_colours'][DISTRIBUTION_STATUS_COLORS] = array(
      '#type' => 'textarea',
      '#title' => t('Custom status colors'),
      '#element_validate' => array('form_element_validate_json'),
      '#default_value' => variable_get(DISTRIBUTION_STATUS_COLORS, ''),
      '#description' => t('Defines the distribution status colors of the distribution map.<BR>
          <strong>NOTE:</strong><i>The preferred way to define the distribution status coloring is to set the value
          in the term itself (e.g. by using the term editor in the TaxEditor). This setting here is only for overriding
          the values defined in the terms. This is only necessary if more than 1 dataportal accesses the data in the
          database and if there are at least 2 dataportals having different requirements for the coloring.</i><BR><BR>
          A json map object with either a distribution status uuid or a distribution status idInVocabulary as key 
          and a hex color as value. e.g: <code>{"n":"#ff0000","cef81d25-501c-48d8-bbea-542ec50de2c2":"#00ff00"}</code>.
          The idInVocabulary version must always refer to a term in the CDM default distribution status vocabulary.
          For other terms it does not work therefore for these the uuid must be taken as key.<Br>
          <Br>
          Reference list of the idInVocabulary values of the default distribution status vocabulary:
<pre>
Presence Term
e	endemic for the relevant area
p	present
n	native
nd	native: doubtfully native
pd	present: doubtfully present
in	introduced: naturalized
ia	introduced: adventitious (casual)
i	introduced
ip	introduced: uncertain degree of naturalisation
id	introduced: doubtfully introduced (perhaps cultivated only)
ni	naturalized invasive
niq	naturalized invasive: presence questionable
nn	naturalized non-invasive
nnq	naturalized non-invasive: presence questionable
ic	introduced: cultivated
c	cultivated
?e	endemic: presence questionable
pd	present: presence questionable
nq	native: presence questionable
inq	naturalised: presence questionable
iaq	introduced: adventive (casual): presence questionable
iq	introduced: presence questionable
cq	cultivated, presence questionable
ne	present but not endemic for the relevant area
ue	present but with unknown endemism for the relevant area
??	undefined

AbsenceTerm
a	absent
ne	native: formerly native
-e	endemic: reported in error
f	reported in error
nf	native: reported in error
-d	doubtfully native: reported in error
if	introduced: reported in error
ie	introduced: formerly introduced
-n	naturalised: reported in error
iaf	casual: reported in error
cf	cultivated: reported in error

</pre>'),
  );



  /* ====== SYNONYMY ====== */
  $form['taxon_synonymy'] = array(
    '#type' => 'fieldset',
    '#title' => t('Taxon synonymy (tab)'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('This section covers the settings related to the taxon
      <strong>synonymy</strong> tab.'),
  );

  $form['taxon_synonymy']['cdm_dataportal_nomref_in_title'] = array(
    '#type' => 'checkbox',
    '#title' => t('Accepted taxon on top of the synonymy'),
    '#default_value' => variable_get('cdm_dataportal_nomref_in_title', CDM_DATAPORTAL_NOMREF_IN_TITLE_DEFAULT),
    '#description' => t('If checked, the first homotypic taxon is a repetition
      of the accepted taxon most likely with the full nomenclatural reference, 
      depending on the ' . l('Name render templates', 'admin/config/cdm_dataportal/settings/layout') . '.'),
  );
  $form['taxon_synonymy'][CDM_SYNONYMY_SYN_SEC_PER_HOMOTYPIC_GROUP] = array(
    '#type' => 'checkbox',
    '#title' => t('Secundum referenence of synonyms per homotypic group.'),
    '#default_value' => variable_get(CDM_SYNONYMY_SYN_SEC_PER_HOMOTYPIC_GROUP, 0),
    '#description' => t('You may want to show the secundum reference(s) not for every synonym but at the end of an homotypic group like typedesignations '),

  );


    $form['taxon_synonymy'][CDM_SYNONYMY_ACCEPTED_TAXON_SEC_SEPARATE] = array(
    '#type' => 'checkbox',
    '#title' => t('Secundum referenence as separate line above the accepted taxon.'),
    '#default_value' => variable_get(CDM_SYNONYMY_ACCEPTED_TAXON_SEC_SEPARATE, 0),
    '#description' => t('You may want to remove the <code>"secReferencePart": true,</code> entry from <code>"accepted_taxon.taxon_page_synonymy"{</code> the in the '
      . l('Name render templates', 'admin/config/cdm_dataportal/settings/layout')
      . ' (Only applicable when the "Show accepted taxon on top of the synonymy" option above is enabled.)'),
    '#disabled' =>  !variable_get(CDM_DATAPORTAL_NOMREF_IN_TITLE, CDM_DATAPORTAL_NOMREF_IN_TITLE_DEFAULT)
  );

    $form['taxon_synonymy'][CDM_SYNONYMY_ACCEPTED_TAXON_SEC_SEPARATE_LABEL] = array(
        '#type' => 'textfield',
        '#description' => 'Label for the secundum referenence.',
        '#default_value' => variable_get(CDM_SYNONYMY_ACCEPTED_TAXON_SEC_SEPARATE_LABEL, CDM_SYNONYMY_ACCEPTED_TAXON_SEC_SEPARATE_LABEL_DEFAULT),
        '#disabled' =>  !variable_get('cdm_dataportal_nomref_in_title', 0)
    );
    $form['taxon_synonymy'][CDM_SYNONYMY_ACCEPTED_TAXON_SEC_FOOTNOTE] = array(
        '#type' => 'radios',
        '#title' => t('Complete secundum referenence in footnote or as internal link.'),
        '#default_value' => variable_get(CDM_SYNONYMY_ACCEPTED_TAXON_SEC_FOOTNOTE, CDM_SEC_WITHOUT_COMPLETE_REF),
        '#options' => [
            CDM_SEC_WITH_FOOTNOTE => 'Footnote',
            CDM_SEC_INTERNAL_LINK => 'Internal link',
            CDM_SEC_WITHOUT_COMPLETE_REF => 'Nothing'
        ],
        '#description' => 'Available view modes for secundum references are:
        <ul>
         <li>' . CDM_SEC_WITH_FOOTNOTE . ': Long citation in a footnote.</li>
         <li>' . CDM_SEC_INTERNAL_LINK . ': Detailed informations with an internal link</li>
         <li>' . CDM_SEC_WITHOUT_COMPLETE_REF . ': No link to the long citation or details</li>
       </ul>'
    );







  $form['taxon_synonymy']['taxon_relations'] = array(
    '#type' => 'fieldset',
    '#title' => t('Taxon relationships'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE
  );

  $form['taxon_synonymy']['taxon_relations'][CDM_DATAPORTAL_DISPLAY_TAXON_RELATIONSHIPS] = array(
    '#type' => 'checkbox',
    '#title' => t('Show taxon relations ships of accepted taxon'),
    '#default_value' => variable_get(CDM_DATAPORTAL_DISPLAY_TAXON_RELATIONSHIPS, CDM_DATAPORTAL_DISPLAY_TAXON_RELATIONSHIPS_DEFAULT),
    '#description' => t('If this option is enabled the synonymy will show the
      below selected taxon relationships of accepted taxa.'),
  );

  $taxon_relationship_type_options = cdm_vocabulary_as_option(UUID_TAXON_RELATIONSHIP_TYPE, '_cdm_relationship_type_term_label_callback');
  $taxon_relationship_type_defaults = variable_get(CDM_TAXON_RELATIONSHIP_TYPES, unserialize(CDM_TAXON_RELATIONSHIP_TYPES_DEFAULT));
  $form['taxon_synonymy']['taxon_relations'][CDM_TAXON_RELATIONSHIP_TYPES] = array(
    '#type' => 'checkboxes',
    '#title' => t('Taxon relationship types') . ':',
    '#description' => 'Only taxon relationships of the selected type will be displayed',
    '#options' => $taxon_relationship_type_options,
    '#default_value' => $taxon_relationship_type_defaults,
    '#disabled' => !variable_get(CDM_DATAPORTAL_DISPLAY_TAXON_RELATIONSHIPS, CDM_DATAPORTAL_DISPLAY_TAXON_RELATIONSHIPS_DEFAULT),
  );

  $form['taxon_synonymy']['name_relations'] = array(
    '#type' => 'fieldset',
    '#title' => t('Name relationships'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE
  );

  $name_relationship_type_options = cdm_vocabulary_as_option(
      UUID_NAME_RELATIONSHIP_TYPE,
      '_cdm_relationship_type_term_label_callback',
      false,
      array('uuid' => '/' .UUID_NAMERELATIONSHIPTYPE_LATER_HOMONYM . '|'
        . UUID_NAMERELATIONSHIPTYPE_TREATED_AS_LATER_HOMONYM . '|'
        . UUID_NAMERELATIONSHIPTYPE_CONSERVED_AGAINST . '|'
        . UUID_NAMERELATIONSHIPTYPE_BLOCKING_NAME_FOR . '|'
        . UUID_NAMERELATIONSHIPTYPE_IS_NOT . '|'
        . UUID_NAMERELATIONSHIPTYPE_MISSPELLING . '|'
        . UUID_NAMERELATIONSHIPTYPE_AVOIDS_HOMONYM_OF . '|'
        . UUID_NAMERELATIONSHIPTYPE_ORTHOGRAPHIC_VARIANT . '/' )
  );
  $form['taxon_synonymy']['name_relations'][CDM_NAME_RELATIONSHIP_INLINE_TYPES] = array(
    '#type' => 'checkboxes',
    '#title' => t('Name relationship types') . ':',
    '#description' => 'This setting only affects specific types of name relations which are displayed appended to scientific name. 
    A full listing of all name relationships for a scientific name is provided by the taxon ' . l('name page', 'admin/config/cdm_dataportal/settings/layout/name-page') . '.',
    '#options' => $name_relationship_type_options,
    '#default_value' => variable_get(CDM_NAME_RELATIONSHIP_INLINE_TYPES, unserialize(CDM_NAME_RELATIONSHIP_INLINE_TYPES_DEFAULT)),
  );

  // ====== SPECIMENS ====== //
  $form['taxon_specimens'] = array(
    '#type' => 'fieldset',
    '#title' => t('Taxon specimens (tab)'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('This section covers the settings related to the taxon
      <strong>specimens</strong> tab.'),
  );


    $form['taxon_specimens'][CDM_SPECIMEN_RELATION] = array(
        '#type' => 'checkboxes',
        '#title' => 'Relations to the selected taxon of displayed specimens or occurrences:',
        '#default_value' => variable_get(CDM_SPECIMEN_RELATION, CDM_SPECIMEN_RELATION_DEFAULT),
        '#options' => [
            CDM_SPECIMEN_RELATION_OPTION_IND_ASS => 'Taxon Fact',
            CDM_SPECIMEN_RELATION_OPTION_TD=> 'Type Designation',
            CDM_SPECIMEN_RELATION_OPTION_DET => 'Determination',
            CDM_SPECIMEN_RELATION_OPTION_CDET => 'Current Determination'
        ],
        '#description' => 'Available relations between taxa and specimens or observations are :
        <ul>
         <li>' . 'Taxon Fact' . ': Related by taxon fact for example individual association</li>
         <li>' . 'Type Designation' . ': Mentioned as a type designation</li>
         <li>' . 'Determination' . ': Mentioned in a determination </li>
         <li>' . 'Current Determination' . ': Mentioned as current determination, if <i>Determination</i> is selected this option has no effect</li>         
       </ul>' . 'If nothing is selected all related specimens are displayed. To fully remove specimens please deselect the specimen tab from the tabs area above.'
    );


  $form['taxon_specimens'][CDM_SPECIMEN_LIST_VIEW_MODE] = array(
      '#type' => 'radios',
      '#title' => 'View mode for lists of specimens or occurrences.',
      '#default_value' => variable_get(CDM_SPECIMEN_LIST_VIEW_MODE, CDM_SPECIMEN_LIST_VIEW_MODE_DEFAULT),
      '#options' => [
        CDM_SPECIMEN_LIST_VIEW_MODE_OPTION_DERIVATE_TABLE => 'Compressed derivate table',
        CDM_SPECIMEN_LIST_VIEW_MODE_OPTION_DERIVATE_TREE => 'Derivate tree',
        CDM_SPECIMEN_LIST_VIEW_MODE_OPTION_DERIVATE_PATH => 'Derivate path'
      ],
      '#description' => 'Available view modes for listing specimens and occurrences are:
        <ul>
         <li>' . 'Compressed derivate table' . ': As compressed table where every row represents
    a collection. Rows can be expanded to get an overview of the specimens and their derivatives.</li>
         <li>' . 'Derivate tree' . ': As tree of all derivations stemming from the field unit</li>
         <li>' . 'Derivate path' . ': As the path of derivatives from the specimen to the field unit</li>
       </ul>'
  );

  $specimen_derivate_tree_options = get_array_variable_merged(CDM_SPECIMEN_DERIVATE_TREE_OPTIONS, CDM_SPECIMEN_DERIVATE_TREE_OPTIONS_DEFAULT);
  $form['taxon_specimens'][CDM_SPECIMEN_DERIVATE_TREE_OPTIONS] = [
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#title' => 'Derivate tree options',
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#description' => t('These setting only apply to the <i>Derivate tree</i> view mode (see above).'),
  ];

  $form['taxon_specimens'][CDM_SPECIMEN_DERIVATE_TREE_OPTIONS]['field_unit_short_label'] = [
    '#type' => 'checkbox',
    '#title' => 'Field unit short label',
    '#default_value' => $specimen_derivate_tree_options['field_unit_short_label'],
    '#description' => t('Use the short collecting string for field units 
    instead of the long summary label, which also includes location information.'),
  ];

  $featureTrees = cdm_get_featureTrees_as_options(TRUE);
  $profile_feature_tree_uuid = variable_get(CDM_OCCURRENCE_FEATURETREE_UUID, UUID_DEFAULT_FEATURETREE);
  if(!isset($featureTrees['options'][$profile_feature_tree_uuid])) {
    $profile_feature_tree_uuid = UUID_DEFAULT_FEATURETREE;
  }
  $form['taxon_specimens']['feature_trees'][CDM_OCCURRENCE_FEATURETREE_UUID] = array(
    '#type' => 'radios',
    '#title' => t('Specimen description feature tree') . ':',
    '#default_value' => $profile_feature_tree_uuid,
    '#options' =>  $featureTrees['options'],
    '#pre_render' => array('form_pre_render_conditional_form_element', 'radios_prepare_options_suffix'),
    '#options_suffixes' => $featureTrees['treeRepresentations'],
    '#description' => t('Select the feature tree to be used for displaying specimen descriptions. Click "Show Details" to see the Feature Tree elements.'
    ),
  );

  $form_name = CDM_DATAPORTAL_SPECIMEN_GALLERY_NAME;
  $form_title = t('Specimen media');
  $form_description = t('Specimens may have media which is displayed at the
     Specimen tab/section as a gallery. It is possible to configure the
     thumbnails gallery here, however for configuring how a single media should
     be displayed please go to !url.</p>',
     array(
       '!url' => l(t('Layout -> Media'), 'admin/config/cdm_dataportal/settings/layout/media'),
     ));
  $form['taxon_specimens'][] = cdm_dataportal_create_gallery_settings_form($form_name, $form_title, FALSE, $form_description);

  // --- MEDIA GALLERY ---- //
  $form_name = CDM_DATAPORTAL_TAXON_MEDIA_GALLERY_NAME_TAB;
  $form_title = 'Media gallery (tab)';
  $form_description = '<p>This section covers the settings related to the taxon <strong>images</strong> tab.
   Taxa may have media (usually images) and they are displayed as thumbnails. It is possible to configure
   the thumbnails gallery here, however for configuring how a single media should be displayed please go to
   <a href="./?q=admin/config/cdm_dataportal/settings/layout/media">Layout -&gt; Media</a></p>
   <p><strong>Note:</strong> These settings are only taken into account when the standard
   gallery viewer is selected at <a href="./?q=admin/config/cdm_dataportal/settings/layout/media">Layout -&gt; Media</a>.</p>';
  $form['taxon_media'][] = cdm_dataportal_create_gallery_settings_form($form_name, $form_title, TRUE, $form_description);

  // Comment @WA: D7 form api does not support reset buttons,
  // so to mimic the D5 reset button we add one like this.
  $form['actions']['reset'] = array(
    '#markup' => '<input id="reset" type="reset" class="form-submit" value="' . t('Reset to defaults') . '" />',
    '#weight' => 1000,
  );
  return system_settings_form($form);
}

/**
 * Creates a form element for the constants DISTRIBUTION_MAP_VISIBILITY, SPECIMEN_MAP_VISIBILITY.
 *
 * @param $map_id
 * @param $form
 * @return mixed
 */
function _cdm_map_visibility_setting($map_id)
{
  return array(
    '#type' => 'select',
    '#title' => t(ucfirst($map_id) . ' map visibility'),
    '#default_value' => variable_get(constant(strtoupper($map_id) . '_MAP_VISIBILITY'), constant(strtoupper($map_id) . '_MAP_VISIBILITY_DEFAULT')),
    '#options' => array('always' => 'always', 'automatic' => 'automatic', 'never' => 'never'),
    '#description' => "The visibility of the map can managed <b>automatically</b> depending on whether there is data to show or not. 
        The map also can forced to show up <b>always</b> or <b>never</b>."
  );
}

/**
 * @return array
 *   The form structure.
 */
function cdm_settings_layout_search() {

  $form = array();

  $form['#submit'][] = 'cdm_settings_layout_search_submit';

  $form['search_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Taxa Search'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#description' => t('<p>The data portal allows the users to perform searchs.</p><p>To perform searchs
         the block <em>CDM Taxon Search</em> should be enabled and visible for users
         where they can write the text to be searched. You can find Drupal block configuration
         site at <a href="./?q=admin/build/block">Administer&#45&#62Site building&#45&#62Blocks</a></p> '),
  );

  $form['search_settings'][SIMPLE_SEARCH_IGNORE_CLASSIFICATION] = array(
      '#type' => 'checkbox',
      '#title' => t('Ignore the chosen classification in simple search'),
      '#default_value' => variable_get(SIMPLE_SEARCH_IGNORE_CLASSIFICATION, 0),
      '#description' => t('The simple search, which can be executed via the search block,
          will by default search on the classification selected in the classification browser
          selector. Set the tick if you want your portal to search on all classifications.'),
  );

  $form['search_settings'][SIMPLE_SEARCH_USE_LUCENE_BACKEND] = array(
    '#type' => 'checkbox',
    '#title' => t('Run simple search with free-text search backend.'),
    '#default_value' => variable_get(SIMPLE_SEARCH_USE_LUCENE_BACKEND, 0),
    '#description' => t('The simple search uses by default another search
      backend as the advances search. By checking this option the simple search can be
      configured to also use the free-text search backend.'),
  );

  $form['search_settings'][SIMPLE_SEARCH_AUTO_SUGGEST] = array(
    '#type' => 'checkbox',
    '#title' => t('(EXPERIMENTAL) Enable auto-suggest for taxon search'),
    '#default_value' => variable_get(SIMPLE_SEARCH_AUTO_SUGGEST, 0),
    '#description' => t('If enabled, the taxon search field will suggest taxon names while typing in a search query.
    This function works on indexed taxon names. If you experience any delay maybe you have to reindex (see above).'),
  );

  $form['search_settings'][CDM_SEARCH_RESULT_PAGE_SIZE] = array(
    '#type' => 'textfield',
    '#title' => t('Results per page') . ':',
    '#default_value' => variable_get(CDM_SEARCH_RESULT_PAGE_SIZE, CDM_SEARCH_RESULT_PAGE_SIZE_DEFAULT),
    '#description' => t('Number of results to display per page.'),
  );

  $form['search_settings'][SEARCH_RESULTS_SHOW_THUMBNAIL_CHECKBOX] = array(
    '#type' => 'checkbox',
    '#title' => t('Show the') .  ' <i>' . t('Display image thumbnails') . '</i>' . t('button') . ':',
    '#default_value' => variable_get(SEARCH_RESULTS_SHOW_THUMBNAIL_CHECKBOX, SEARCH_RESULTS_SHOW_THUMBNAIL_CHECKBOX_DEFAULT),
    '#description' => t('The search results page will offer a button to toggle the display of image thumbnails.'),
  );

  $search_mode_default = get_array_variable_merged(CDM_SEARCH_TAXA_MODE, CDM_SEARCH_TAXA_MODE_DEFAULT);
  $form['search_settings']['cdm_search_taxa_mode'] = array(
      '#type' => 'checkboxes',
      '#title' => 'Search mode',
      '#description' => 'The taxon search can operate in different modes in order to find only taxa, synonyms,
          taxa by its common name and even taxa which have been used as misapplied names. The settings made here will affect the default
          for the advance search form and the behaviour of the simple search form which always will behave according to the
          defaults set here.',
      '#options' => drupal_map_assoc(array_keys(unserialize(CDM_SEARCH_TAXA_MODE_DEFAULT))),
      '#default_value' => $search_mode_default
      );

  $form['search_settings'][CDM_SEARCH_AREA_FILTER_PRESET] = array(
    '#type' => 'textarea',
    '#title' => t('area_filter_preset') . ':',
    '#default_value' => variable_get(CDM_SEARCH_AREA_FILTER_PRESET, ''), // '05b0dd06-30f8-477d-bf4c-30d9def56320' =>  Caucasia (Ab + Ar + Gg + Rf(CS)) (Cc)

    '#description' => t('Area uuids, comma separated, no whitespace. EXPERIMENTAL!!!!'),
  );

    $form['blast_search_settings'] = array(
        '#type' => 'fieldset',
        '#title' => t('Blast Search'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        '#description' => t('<p>To perform blast searchs
         the block <em>CDM Taxon Search</em> should be enabled and visible for users
         where they can write the text to be searched. You can find Drupal block configuration
         site at <a href="./?q=admin/build/block">Administer&#45&#62Site building&#45&#62Blocks</a></p>
         <p>To perform a blast search a blast database for the cdm instance is needed.</p> '),
    );

    $form['blast_search_settings'][CDM_SEARCH_BLAST_ENABLED] = array(
        '#type' => 'checkbox',
        '#title' => t('Activate Blast search') . ':',
        '#default_value' => variable_get(CDM_SEARCH_BLAST_ENABLED, 0), // '05b0dd06-30f8-477d-bf4c-30d9def56320' =>  Caucasia (Ab + Ar + Gg + Rf(CS)) (Cc)

        '#description' => t('Activate the blast search for this portal, this works only with an existing blast database!'),
    );
    $form['blast_search_settings'][CDM_SEARCH_BLAST_SERVICE_URI] = array(
        '#type' => 'textfield',
        '#title' => t('Webservice URL for blast search') . ':',
        '#default_value' => variable_get(CDM_SEARCH_BLAST_SERVICE_URI, 'http://130.133.70.28:9001/api/sequence'),

        '#description' => t('Enter the webservice URL for blast search'),
    );

  // --- SEARCH TAXA GALLERY ---- //
  $items = variable_get(CDM_SEARCH_RESULT_PAGE_SIZE, CDM_SEARCH_RESULT_PAGE_SIZE_DEFAULT);
  $collapsed = FALSE;
  $form_name = CDM_DATAPORTAL_SEARCH_GALLERY_NAME;
  $form_title = 'Taxa Search thumbnails';
  $form_description = 'Search results may show thumbnails. ';
  $form[] = cdm_dataportal_create_gallery_settings_form($form_name, $form_title, $collapsed, $form_description);

  // Comment @WA: D7 form api does not support reset buttons,
  // so to mimic the D5 reset button we add one like this.
  $form['actions']['reset'] = array(
    '#markup' => '<input id="reset" type="reset" class="form-submit" value="' . t('Reset to defaults') . '" />',
    '#weight' => 1000,
  );
  return system_settings_form($form);
}


/**
 * @return array
 *   The form structure.
 */
function cdm_settings_layout_name_page()
{

  $form = array();


  $form['name_relations'] = array(
    '#type' => 'fieldset',
    '#title' => t('Name relationship listings'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE
  );

  $name_relationship_type_options = cdm_vocabulary_as_option(
    UUID_NAME_RELATIONSHIP_TYPE,
    '_cdm_relationship_type_term_label_callback',
    false
  );
  $form['name_relations'][CDM_NAME_RELATIONSHIP_LIST_TYPES] = array(
    '#type' => 'checkboxes',
    '#title' => t('Name relationship types') . ':',
    '#description' => 'This setting only affects specific types of name relations which are displayed as list. Another representation 
    is the inline style used in the synonymy which may show a different (reduced) set of name relations. 
    The according settings can be adjusted in the ' . l(' synonmy section of the taxon page settings', 'admin/config/cdm_dataportal/settings/layout/taxon') . '.',
    '#options' => $name_relationship_type_options,
    '#default_value' => variable_get(CDM_NAME_RELATIONSHIP_LIST_TYPES, cdm_vocabulary_as_defaults(UUID_NAME_RELATIONSHIP_TYPE)),
  );

  $form[CDM_NAME_PAGE_SECTION_TAXA] = array (
    '#type' => 'checkbox',
    '#title' => 'Show the associated taxa section',
    '#default_value' => variable_get(CDM_NAME_PAGE_SECTION_TAXA, CDM_NAME_PAGE_SECTION_TAXA_DEFAULT)

  );

  return system_settings_form($form);
}

/**
 * @return array
 *   The form structure.
 */
function cdm_settings_layout_media() {

  $form = array();

  $form['media_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Media settings'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#description' => 'This section covers layout settings for media pages.'
      . 'Further media related settings may be found under the taxon layout settings and on the general settings.',
  );

  $form['media_settings'][CDM_MEDIA_GALLERY_VIEWER] = array(
    '#type' => 'select',
    '#title' => t('Image viewer') . ':',
    '#default_value' => variable_get(CDM_MEDIA_GALLERY_VIEWER, CDM_MEDIA_GALLERY_VIEWER_DEFAULT),
    '#options' => array(
      CDM_MEDIA_GALLERY_VIEWER_DEFAULT => t('Universalviewer'),
      'fsi' => t('FSI viewer (requires FSI server!)'),
      'default' => t('Simple viewer'),
    ),
    '#description' => 'The <b>Universalviewer</b> is a feature rich image gallery and viewer which provides support
            for mobile devices and thus is highly recommended over the other options.<br/>
            <b>Simple viewer</b> combines a HTML gallery with a simple image viewer and only provides basic functionality 
            and does not work well on mobile devices.<br />
            <b>FSI Viewer</b> can only be used in conjunction with images served by an FSI server.'
  );

  // --- STANDARD_IMAGE_VIEWER ---- //
  if(variable_get(CDM_MEDIA_GALLERY_VIEWER, CDM_MEDIA_GALLERY_VIEWER_DEFAULT) == CDM_MEDIA_GALLERY_VIEWER_SIMPLE) {
    $form['media_settings'][CDM_STANDARD_IMAGE_VIEWER] = array(
      '#type' => 'fieldset',
      '#tree' => true,
      '#title' => t('Standard image viewer settings'),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
      );

    $cdm_standard_image_viewer_settings = get_array_variable_merged(CDM_STANDARD_IMAGE_VIEWER, CDM_STANDARD_IMAGE_VIEWER_DEFAULT);
    $form['media_settings'][CDM_STANDARD_IMAGE_VIEWER]['media_representation_details_enabled'] = array (
      '#type' => 'checkbox',
      '#title' => 'Show media representations',
      '#default_value' => $cdm_standard_image_viewer_settings['media_representation_details_enabled']
    );

  }
  if(variable_get(CDM_MEDIA_GALLERY_VIEWER, 'default') == CDM_MEDIA_GALLERY_VIEWER_DEFAULT) {
    $form['media_settings'][CDM_ADDITIONAL_MEDIA_COPYRIGHT] = [
      '#type' => 'textarea',
      '#title' => 'Additional media copyright statement',
      '#default_value' => variable_get(CDM_ADDITIONAL_MEDIA_COPYRIGHT, NULL),
      '#description' => 'This statement will be shown below the <b>Universal imgage viewer only</b>'
    ];

      $form['media_settings'][CDM_META_DATA_SOURCE] = array (
         '#type' => 'select',
          '#title' => t('Meta data source') . ':',
          '#default_value' => variable_get(CDM_META_DATA_SOURCE, CDM_META_DATA_SOURCE_DEFAULT),
          '#options' => array(
                  'cdm' => t('Prefer CDM'),
                  'mediaServer' => t('Prefer Media Server'),
                  'onlyCdm' => t('Only CDM'),
                  'onlyMediaServer' => t('Only Media Server')
              ),
          '#description' => 'The <b>Universalviewer</b> can show meta data provided by the cdm instance or from the media server, please choose the source of the shown meta data. 
A special case is if all information in cdm description field'
      );

  }

  // @WA: D7 form api does not support reset buttons,
  // so to mimic the D5 reset button we add one like this.
  $form['actions']['reset'] = array(
    '#markup' => '<input id="reset" type="reset" class="form-submit" value="' . t('Reset to defaults') . '" />',
    '#weight' => 1000,
  );
  return system_settings_form($form);
}

/**
 * GEOSERVICE and Map settings.
 *
 *  @return array
 *   The form structure.
 */
function cdm_settings_geo($form, &$form_state) {

  $current_geoserver_settings = get_edit_map_service_settings();
  $map_distribution = get_array_variable_merged(CDM_MAP_DISTRIBUTION, CDM_MAP_DISTRIBUTION_DEFAULT);


  $form = array();

  $dummy_distribution_query = NULL;
  if($map_distribution['map_type'] != 1){
    // we need to apply a dummy query since the map service requires for image maps
    // at least as and ad to be defined
    $dummy_distribution_query = "as=a:339966&ad=tdwg1:a:1,2,3,4,5,6,7,8,9";
  }

  $form['map_preview'] = array(
      '#type' => 'fieldset',
      '#tree' => FALSE,
      '#title' => t('Map preview'),
      '#collapsible' => FALSE,
      '#description' => 'The preview of the map'
       . ($dummy_distribution_query != null ?
           ' may not be accurate in case of image maps, please check the map display in the taxon pages.':
           '.<br/>Hold down Strg and drag with your mouse to select a bbox to zoom to. <br/>The bbox of the visible area of the map is always displayed below the map.')
  );
  $form['map_preview']['openlayers_map'] = compose_map('settings-preview', NULL, NULL, $dummy_distribution_query, NULL, [
    'move' => "this.cdmOpenlayersMap.printInfo",
    '#execute' => "this.cdmOpenlayersMap.printInfo"
  ], TRUE // resizable
  );

  /*
  $form['map_preview']['map'] = compose_map(NULL, $dummy_distribution_query, NULL, array(), 0 // force image map
  );
  */

  /*
   * GEO SERVER
   */
  $form['edit_map_server'] = array(
    '#type' => 'fieldset',
    '#tree' => true,
    '#title' => t('EDIT map service'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('Configuration and selection of your geo server.
      The Geo Server is responsible for generating the maps.'),
  );

  $form['edit_map_server']['base_uri'] = array(
    '#type' => 'select',
    '#title' => t('EDIT map service') . ':',
    '#default_value' => $current_geoserver_settings['base_uri'],
    '#options' => unserialize(EDIT_MAPSERVER_URI),
    '#description' => t('Select the EDIT map server you want to use within your data portal.'),
  );
  $form['edit_map_server']['version'] = array(
      '#type' => 'select',
      '#title' => t('Version') . ':',
      '#default_value' => $current_geoserver_settings['version'],
      '#options' => unserialize(EDIT_MAPSERVER_VERSION),
      '#description' => t('The version of the EDIT map services'),
  );

  /*
   * MAP SETTINGS
   */

  $form[CDM_MAP_DISTRIBUTION] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#title' => t('Maps settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('General configuration for all map types.'),
  );

  $form[CDM_MAP_DISTRIBUTION]['map_type'] = array(
    '#type' => 'radios',
    '#title' => 'Map types',
    '#options' => array(
      1 => "OpenLayers dynamic map viewer",
      0 => "Plain image",
    ),
    '#default_value' => $map_distribution['map_type'],
    '#description' => 'Two different map types are available :
      <ul><li><em>OpenLayers</em>: Display the maps in an interactive viewer
      which allows zooming and panning. If enabled you can configure the default layer
      (background of your maps) below.</li>
      <li><em>Plain image</em>: The map will be static non interactive
      image.</li></ul>',
  );
  $open_layers_is_enabled = $map_distribution['map_type'] == 1;

  /*
   * settings for the distribution map are used also for specimens map!!!!
   */

  $form[CDM_MAP_DISTRIBUTION]['aspect_ratio'] = array(
      '#type' => 'textfield',
      '#title' => 'Aspect ratio',
      '#default_value' => $map_distribution['aspect_ratio'],
      '#maxlength' => 4,
      '#size' => 4,
      '#element_validate' => array('element_validate_number'),
      '#description' => 'The ratio of width to height of the map. Instead of expressing the aspect ratio as usually as
      two numbers separated by a colon (x:y), this field requires a the value which is the result of the division of the
      width by the height:</br>
      <pre>aspect ratio = w / h</pre>
      For a landscape oriented map with an aspect ratio of 2:1 use <strong>2</strong> as value,</br>
      for a square map use <strong>1</strong>.',
  );

  $form[CDM_MAP_DISTRIBUTION]['bbox'] = array(
    '#type' => 'textfield',
    '#title' => 'Bounding box',
    '#default_value' => $map_distribution['bbox'],
    '#description' =>
      'The bounding box (left, bottom, right, top) defines the area to be initially displayed in maps. The bbox must be given in coordinates of the projection of the chosen layer. 
      For the whole world use Use <code>-180,-90,180,90</code>" for EPSG:4326 layers, for Web Mercator maps Open Street Map or 
      Google maps the maximum bounds of the world are <code>-27395030.933594,-13697515.466797,27395030.933594,13697515.466797</code>.
      Leave <strong>empty</strong> to let the map <strong>automatically zoom</strong> to the bounds enclosing the shown data.</p>
      <strong>TIP:</strong> You can use the map preview above to choose the <span class="map-extent-bbox"><strong class="layer-value">map extent bbox</strong></span> from the details information.
      Hold down Strg and drag with your mouse to select a bbox to zoom to. The bbox of the visible area of the map is always displayed
      below the map from where you can copy the bbox string.</p>',
  );

  $form[CDM_MAP_DISTRIBUTION]['maxZoom'] = array(
    '#type' => 'select',
    '#title' => 'Max zoom level',
    '#default_value' => $map_distribution['maxZoom'],
    '#options' => array(0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20)
  );

  $form[CDM_MAP_DISTRIBUTION]['show_labels'] = array(
    '#type' => 'checkbox',
    '#title' => 'Display area labels',
    '#default_value' => $map_distribution['show_labels'],
    '#description' => t('The map will show name labels of the areas'),
  );

  $form[CDM_MAP_DISTRIBUTION]['caption'] = array(
    '#type' => 'textfield',
    '#title' => 'Map caption',
    '#default_value' => $map_distribution['caption'],
    '#description' => t('The caption will be shown below the map.'),
  );

  $form[CDM_MAP_DISTRIBUTION]['distribution_opacity'] = array(
    '#type' => 'textfield',
    '#title' => 'Distribution layer opacity',
    '#default_value' => $map_distribution['distribution_opacity'],
    '#description' => t('Valid values range from 0.0 to 1.0. Value 1.0 means the distributions
    (the countries or regions) will fully visible, while a value near to 0.0 will be not much visible.'),
  );

  // --- Plain Image Settings --- //
  $form[CDM_MAP_DISTRIBUTION]['image_map'] = array(
    '#type' => 'fieldset',
    '#title' => 'Plain image map settings',
    '#tree' => TRUE,
    '#collapsible' => TRUE,
    '#collapsed' => $open_layers_is_enabled,
    '#description' => 'The settings in this section are still expertimental
      and can only be used with the EDIT map service version 1.1 or above.',
  );
  $edit_mapserver_version = get_edit_map_service_version_number();
  if ($edit_mapserver_version < 1.1) {
    $form[CDM_MAP_DISTRIBUTION]['image_map']['#description'] = '<div class="messages warning">'
      . t("The chosen EDIT map service version (@edit-mapserver-version) is too low, it must be at least 1.1",
        array('@edit_mapserver_version' => '$edit_mapserver_version')) . '</div>'
      . $form[CDM_MAP_DISTRIBUTION]['image_map']['#description'];
  }

  $form[CDM_MAP_DISTRIBUTION]['image_map']['width'] = array(
    '#type' => 'textfield',
    '#title' => 'Width',
    '#default_value' => $map_distribution['image_map']['width'],
    '#maxlength' => 4,
    '#size' => 4,
    '#description' => 'Width of the map. The height is calculated from the <strong>Aspect ratio</strong> set in the section above. ',
  );

  $form[CDM_MAP_DISTRIBUTION]['image_map']['base_layer'] = array(
    '#type' => 'textfield',
    '#title' => 'Background layer',
    '#default_value' => $map_distribution['image_map']['base_layer'],
    '#description' => t('Background layer. For available layers inspect !url1 or !url2.', array(
      '!url1' => l('deegree-csw', 'http://edit.africamuseum.be:8080/deegree-csw/md_search.jsp'),
      '!url2' => l('geoserver layers', 'http://edit.africamuseum.be/geoserver/web/'),
    )),
  );

  $form[CDM_MAP_DISTRIBUTION]['image_map']['bg_color'] = array(
    '#type' => 'textfield',
    '#title' => 'Background color',
    '#default_value' => $map_distribution['image_map']['bg_color'],
  );

  $form[CDM_MAP_DISTRIBUTION]['image_map']['layer_style'] = array(
    '#type' => 'textfield',
    '#title' => 'Background layer style',
     // Only line color by now.
    '#default_value' => $map_distribution['image_map']['layer_style'],
    '#description' => 'Syntax: {Area fill color},{Area stroke color},{Area stroke width},{Area stroke dash style}',
  );

  $form[CDM_MAP_DISTRIBUTION]['image_map']['projection'] = array(
      '#type' => 'textfield',
      '#title' => 'Projection',
      '#default_value' => drupal_array_get_nested_value($map_distribution, array('image_map', 'projection')),
      '#description' => 'Spatial Reference System (SRS) identifier ) optional ( Defines projections in WMS GetMap request.
        Using EPSG:4326 (WGS84 lat/long) is the default but can be changed
        on-the-fly to different UTM and much more zone specific. Examples: EPSG:4326, EPSG:900913, EPSG:3857, EPSG:7777777',
  );


  // --- OpenLayers Settings --- //
  $form[CDM_MAP_DISTRIBUTION]['openlayers'] = array(
    '#type' => 'fieldset',
    '#title' => 'OpenLayers settings',
    '#tree' => TRUE,
    '#collapsible' => TRUE,
    '#collapsed' => !$open_layers_is_enabled,
    '#description' => '',
  );


  $form[CDM_MAP_DISTRIBUTION]['openlayers']['display_outside_max_extent'] = array(
      '#type' => 'checkbox',
      '#title' => 'Display outside max extent',
      '#default_value' => $map_distribution['openlayers']['display_outside_max_extent'],
      '#description' => t('Allows the map to display parts of the layers which are outside
         the max extent if the aspect ratio of the map and of the baselayer
         are not equal.'),
  );


  $form[CDM_MAP_DISTRIBUTION]['openlayers']['show_layer_switcher'] = array(
      '#type' => 'checkbox',
      '#title' => 'Show Layer Switcher',
      '#default_value' => $map_distribution['openlayers']['show_layer_switcher'],
      '#description' => 'The Layer Switcher control displays a table of contents
      for the map.  This allows the user interface to switch between
      base layers and to show or hide overlays.  By default the switcher is
      shown minimized on the right edge of the map, the user may expand it
      by clicking on the handle.',
  );

  if (!$open_layers_is_enabled) {
    $form[CDM_MAP_DISTRIBUTION]['openlayers']['#description'] = '<div class="messages warning">'
        . 'The Openlayers viewer is currently not enabled! (see section Maps settings above )</div>'
        . $form[CDM_MAP_DISTRIBUTION]['openlayers']['#description'];
  }

  // The default layer must always be enabled
  $preferred_layer = $map_distribution['openlayers']['base_layers']['PREFERRED'];
  $map_distribution['openlayers']['base_layers'][$preferred_layer] = $preferred_layer;

  $baselayer_options = array(
    /*
   NOTICE: must correspond to the layers defined in
   js/openlayers_,ap.js#getLayersByName()
   */
    'osgeo_vmap0' => "Metacarta Vmap0 (OSGeo server) - instable!", // EPSG:4326: EPSG:900913
    'metacarta_vmap0' => "Metacarta Vmap0 (MetaCarta Labs server)  - instable!", // EPSG:4326, EPSG:900913
    'mapproxy_vmap0' => "Metacarta Vmap0 (OSGeo server) - via fast EDIT MapProxy",
    //'mapproxy_etopo1' => "ETOPO1 Global Relief Model - via fast EDIT MapProxy",
    //'edit-etopo1' => "ETOPO1 Global Relief Model",
    // all others EPSG:900913
    'mapnik' => 'OpenStreetMap (mapnik)',
    'stamen_terrain' => 'Stamen Terrain',
    'open_topomap' => 'OpenTopoMap',
    // map quest is no longer free. it is required to sign up for a test plan.
    // 'mapquest_open' => "MapQuest",
    // 'mapquest_sat' => "MapQuest Sattelite",
    'groadmap' => 'Google Roadmap',
    'gsatellite' => 'Google Satellite',
    'ghybrid' => 'Google Hybrid',
    'gterrain' => 'Google Terrain',
//     'veroad' => 'Virtual Earth Roads',
//     'veaer' => 'Virtual Earth Aerial',
//     'vehyb' => 'Virtual Earth Hybrid',
    // 'yahoo' => 'Yahoo Street',
    // 'yahoosat' => 'Yahoo Satellite',
    // 'yahoohyb' => 'Yahoo Hybrid',
     'custom_wms_base_layer_1' => 'Custom WMS base layer (needs to be manually configured below!)',
  );

  $form[CDM_MAP_DISTRIBUTION]['openlayers']['base_layers'] = array(
    '#type' => 'checkboxes_preferred',
    '#title' => 'Base Layers',
    '#options' => $baselayer_options,
    '#default_value' =>  $map_distribution['openlayers']['base_layers'],
    '#description' => 'Choose the baselayer layer you prefer to use as map background in the OpenLayers dynamic mapviewer.',
  );

  $google_maps_api_key = null;
  if(isset($map_distribution['openlayers']['google_maps_api_key'])){
    $google_maps_api_key = $map_distribution['openlayers']['google_maps_api_key'];
  }
  $form[CDM_MAP_DISTRIBUTION]['openlayers']['google_maps_api_key'] = array(
    '#type' => 'textfield',
    '#title' => 'Google Maps API Key',
    '#default_value' => $google_maps_api_key,
    '#description' => 'In order to use any of the Google map layers you need to provide 
        your <a href="https://developers.google.com/maps/documentation/javascript/get-api-key">Google Maps API Key</a>. ',
  );

  $form[CDM_MAP_DISTRIBUTION]['openlayers']['custom_wms_base_layer'] = wms_layer_settings(
    $map_distribution['openlayers']['custom_wms_base_layer'],
    'Custom WMS base layer',
    'Here you an define a custom wms layer as additional base layer. You need to enable this layer in the base layers section above.',
    true // add projection settings
    );

  $form[CDM_MAP_DISTRIBUTION]['openlayers']['wms_overlay_layer'] = wms_layer_settings(
    $map_distribution['openlayers']['wms_overlay_layer'],
    'WMS overlay layer',
    'Here you an define a wms layer which will overlay all other layers in the map viewer. 
                You can actually combine multiple layers for this overlay. 
                For details please refer to the wms query parameter <code>Layers</code> .'
  );

  $form[CDM_MAP_DISTRIBUTION]['openlayers']['wms_overlay_layer']['is_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => 'Enable overlay layer',
    '#weight' => -100,
    '#default_value' => isset($map_distribution['openlayers']['wms_overlay_layer']['is_enabled']) && $map_distribution['openlayers']['wms_overlay_layer']['is_enabled'] === 1  ? 1 : 0
  );

  /*
   * Map Legend
   */
  $form[CDM_MAP_DISTRIBUTION]['legend'] = array(
    '#type' => 'fieldset',
    '#title' => 'Map legend',
    '#tree' => TRUE,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => 'Configure the maps legend.',
  );

  $form[CDM_MAP_DISTRIBUTION]['legend']['show'] = array(
    '#type' => 'checkbox',
    '#title' => 'Display a map legend',
    '#default_value' => $map_distribution['legend']['show'],
    '#description' => 'Check this if you like a legend to be displayed with the maps.',
  );

  $form[CDM_MAP_DISTRIBUTION]['legend']['opacity'] = array(
    '#type' => 'textfield',
    '#title' => 'Legend opacity',
    '#default_value' => $map_distribution['legend']['opacity'],
    '#description' => 'Valid values range from 0.0 to 1.0. Value 1.0 means the legend will be fully visible, while a value near
                         to 0.0 will be not much visible.',
  );

  $form[CDM_MAP_DISTRIBUTION]['legend']['font_size'] = array(
    '#type' => 'textfield',
    '#title' => 'Font size',
    '#default_value' => $map_distribution['legend']['font_size'],
    '#description' => 'Font size in pixels.',
  );

  $fontStyles = array(
    0 => "plane",
    1 => "italic",
  );
  $form[CDM_MAP_DISTRIBUTION]['legend']['font_style'] = array(
    '#type' => 'select',
    '#title' => 'Available font styles',
    '#default_value' => $map_distribution['legend']['font_style'],
    '#options' => $fontStyles,
    '#description' => 'Select a font style for the map legend.',
  );

  $form[CDM_MAP_DISTRIBUTION]['legend']['icon_width'] = array(
    '#type' => 'textfield',
    '#title' => 'Icon width',
    '#default_value' => $map_distribution['legend']['icon_width'],
    '#description' => 'Legend icon width in pixels.',
  );
  $form[CDM_MAP_DISTRIBUTION]['legend']['icon_height'] = array(
    '#type' => 'textfield',
    '#title' => 'Icon height',
    '#default_value' => $map_distribution['legend']['icon_height'],
    '#description' => 'Legend icon height in pixels.',
  );

  // @WA: D7 form api does not support reset buttons,
  // so to mimic the D5 reset button we add one like this.
  $form['actions']['reset'] = array(
    '#markup' => '<input id="reset" type="reset" class="form-submit" value="' . t('Reset to defaults') . '" />',
    '#weight' => 1000,
  );

  return system_settings_form($form);
}

/**
 * @param $default_settings
 * @param $title
 * @param $description
 * @param bool $add_projection_settings
 * @return array
 */
function wms_layer_settings($default_settings, $title, $description, $add_projection_settings = false)
{
  $form_elements = array(
    '#type' => 'fieldset',
    '#title' => $title,
    '#tree' => TRUE,
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#description' => $description,
  );

  $form_elements['name'] = array(
    '#type' => 'textfield',
    '#title' => 'Layer name',
    '#default_value' => $default_settings['name'],
    '#description' => 'A arbitrary name for the layer.',
  );
  $form_elements['url'] = array(
    '#type' => 'textfield',
    '#title' => 'WMS url',
    '#default_value' => $default_settings['url'],
    '#description' => 'Base url for the WMS (e.g.  http://edit.africamuseum.be/geoserver/topp/wms, http://wms.jpl.nasa.gov/wms.cgi)'
  );
  $form_elements['untiled'] = array(
    '#type' => 'checkbox',
    '#title' => 'Untiled',
    '#default_value' => $default_settings['untiled'],
    '#description' => 'If the layer contains labels you may want to check this option to avoid label duplication or erratic invisibility.'
  );
  $form_elements['params'] = array(
    '#type' => 'textarea',
    '#title' => 'WMS parameters',
    '#element_validate' => array('form_element_validate_json'),
    '#default_value' => $default_settings['params'],
    '#description' => 'An javasript object with key/value pairs representing the GetMap query string parameters and parameter values ('
      .l('Geoserver WMS parameter reference', 'http://docs.geoserver.org/stable/en/user/services/wms/reference.html#getmap' )
      . '), entered in valid JSON. For example:
<pre> {
  "Layers": "topp:em_tiny_jan2003",
  "Format": "image/png",
  "BGCOLOR": "0xe0faff"
}
</pre>
    You can supply and web accessible SLD file by using the <code>sld</code> or <coded>sld_body</coded> parameters.'
  );

  if($add_projection_settings){

    $form_elements['projection'] = array(
      '#type' => 'textfield',
      '#title' => 'Projection',
      '#default_value' => $default_settings['projection'],
      '#description' => 'The desired projection for the layer (e.g. EPSG:4326, EPSG:900913, EPSG:3857)'
    );
    $form_elements['proj4js_def'] = array(
      '#type' => 'textfield',
      '#maxlength' => 256,
      '#title' => 'proj4js definition',
      '#default_value' => $default_settings['proj4js_def'],
      '#description' => 'The <a href="http://trac.osgeo.org/openlayers/wiki/Documentation/Dev/proj4js">proj4js definition</a> for the projection named above.
              The definitions for
              EPSG:102067, EPSG:102757, EPSG:102758, EPSG:21781, EPSG:26591, EPSG:26912, EPSG:27200, EPSG:27563, EPSG:3857,
              EPSG:41001, EPSG:4139, EPSG:4181, EPSG:42304, EPSG:4272, EPSG:4302, EPSG:900913
              are already predefined and must NOT be added here again.  If your dont know the defintion of your desired projection,
              go to  <a href="http://spatialreference.org/">http://spatialreference.org/</a>, search for your projection and
              choose to display the proj4js definition string.
              <h5>Quick Reference on the common proj4js definition parameters:</h5>
              <pre>
  +a         Semimajor radius of the ellipsoid axis
  +alpha     ? Used with Oblique Mercator and possibly a few others
  +axis      Axis orientation (new in 4.8.0)
  +b         Semiminor radius of the ellipsoid axis
  +datum     Datum name (see `proj -ld`)
  +ellps     Ellipsoid name (see `proj -le`)
  +k         Scaling factor (old name)
  +k_0       Scaling factor (new name)
  +lat_0     Latitude of origin
  +lat_1     Latitude of first standard parallel
  +lat_2     Latitude of second standard parallel
  +lat_ts    Latitude of true scale
  +lon_0     Central meridian
  +lonc      ? Longitude used with Oblique Mercator and possibly a few others
  +lon_wrap  Center longitude to use for wrapping (see below)
  +nadgrids  Filename of NTv2 grid file to use for datum transforms (see below)
  +no_defs   Don\'t use the /usr/share/proj/proj_def.dat defaults file
  +over      Allow longitude output outside -180 to 180 range, disables wrapping (see below)
  +pm        Alternate prime meridian (typically a city name, see below)
  +proj      Projection name (see `proj -l`)
  +south     Denotes southern hemisphere UTM zone
  +to_meter  Multiplier to convert map units to 1.0m
  +towgs84   3 or 7 term datum transform parameters (see below)
  +units     meters, US survey feet, etc.
  +vto_meter vertical conversion to meters.
  +vunits    vertical units.
  +x_0       False easting
  +y_0       False northing
  +zone      UTM zone
              </pre>
            For the full reference please refer to <a href="http://proj4.org/parameters.html">http://proj4.org/parameters.html</a>.'
    );
    $form_elements['max_extent'] = array(
      '#type' => 'textfield',
      '#title' => 'Maximum extent',
      '#default_value' => $default_settings['max_extent'],
      '#description' => 'The maximum extent of the map as bounding box (left, bottom, right, top) in the units of the map.'
    );
    $form_elements['units'] = array(
      '#type' => 'textfield',
      '#title' => 'Units',
      '#default_value' => $default_settings['units'],
      '#description' => 'The layer map units.  Defaults to null.  Possible values are ‘degrees’ (or ‘dd’), ‘m’, ‘ft’, ‘km’, ‘mi’, ‘inches’.  Normally taken from the projection.  Only required if both map and layers do not define a projection, or if they define a projection which does not define units.'
    );

  }
  return $form_elements;
}


/**
 * @return array
 *   The form structure.
 */
function cdm_settings_cache() {

  $form = array();

  $form['cache_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Cache Settings'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#description' => t('<p>When caching is enabled all single taxon sites are
      stored in an internal drupal cache doing the portal response of taxa pages
      faster. This is possible because the sites are loaded from the cache and
      are not created from scratch.</p>'),
  );

  $form['cache_settings']['cdm_webservice_cache'] = array(
    '#type' => 'checkbox',
    '#title' => t('<strong>Enable caching</strong>'),
    '#options' => cdm_help_general_cache(),
    '#default_value' => variable_get('cdm_webservice_cache', 1),
    '#description' => t('<p>Enable drupal to load taxa pages from the cache.</p>
       <p><strong>Note:</strong> If taxa are modified by the editor or any other
       application the changes will be not visible till the cache is erased.
       Therefore developers should deactived this feature when they are working
       on the CDM Dataportal Module.</p>'),
  );

  $form['cache_settings']['cdm_run_cache'] = array(
    '#markup' => cdm_view_cache_site(),
  );

  // @WA: D7 form api does not support reset buttons,
  // so to mimic the D5 reset button we add one like this.
  $form['actions']['reset'] = array(
    '#markup' => '<input id="reset" type="reset" class="form-submit" value="' . t('Reset to defaults') . '" />',
    '#weight' => 1000,
  );
  return system_settings_form($form);
}

/**
 * Walk and cache all taxon pages.
 */
function cdm_view_cache_site() {

  $out = '';

  drupal_add_js(drupal_get_path('module', 'cdm_dataportal') . '/js/cache_all_taxa.js');

  $request_params = array();
  $request_params['class'] = "Taxon";

  $cdm_ws_page_taxa_url = cdm_compose_ws_url(CDM_WS_TAXON . ".json", NULL, queryString($request_params));
  $cdm_ws_page_taxa_url = uri_uriByProxy($cdm_ws_page_taxa_url);
  $cdm_ws_page_taxa_url = rtrim($cdm_ws_page_taxa_url, '/');

  $out .= t('<p><strong>Cache all taxon pages</strong></p>');
  $out .= '<p>When you launch the cache process the cache is filled and ready to be enabled.<br/>
  Remember that when you load the taxa from the cache last changes on taxa will be not visible till you erase
  the cache and fill it again.</p>';
  $out .= '<p>Before  running the cache bot you have to empty the cache manually.</p>';

  $out .= '<div>' . t('This caching process may take long time and could cause heavy load on your server') . '</div>';
  $out .= '<div id="progress"></div>';

  // Comment @WA: A form within a form is not valid html and not needed here.
  // Also, it would be recommended just to include this part of the form in the
  // rest of the form array in cdm_settings_cache().
  // $out .= '<form id="cache_site">';
  $out .= '<input type="hidden" name="pageTaxaUrl" value="' . $cdm_ws_page_taxa_url . '"/>';
  $out .= '<input type="hidden" name="taxonPageUrl" value="' . url('cdm_dataportal/taxon/') . '"/>';
  $out .= '<input type="button" name="start" value="' . t('Start') . '"/>';
  $out .= '<input type="button" name="stop" value="' . t('Stop') . '"/>';
  // $out .= '</form>';
  $out .= '</div>';
  /*
  foreach($taxonPager->records as $taxon){
    cdm_dataportal_taxon_view($uuid);
  }
  */
  return $out;
}


function cdm_settings_layout_taxon_submit($form, &$form_state){
  if (isset($form_state['values'][CDM_TAXON_PROFILE_IMAGE]['custom_placeholder_image_fid'])) {
    $file = file_load($form_state['values'][CDM_TAXON_PROFILE_IMAGE]['custom_placeholder_image_fid']);
    if(is_object($file)){
      $file->status = FILE_STATUS_PERMANENT;
      file_save($file);
      file_usage_add($file, 'cdm_dataportal', CDM_TAXON_PROFILE_IMAGE, 0);
    }
  }
  // rebuild the menu if the specific tabs setting have changed, otherwise the change will not have a consistent effect
  $tab_lables_modified = serialize(get_array_variable_merged(CDM_TAXONPAGE_TAB_LABELS, CDM_TAXONPAGE_TAB_LABELS_DEFAULT)) != serialize($form_state['values'][CDM_TAXONPAGE_TAB_LABELS]);
  $tabs_enabled_modified = variable_get('cdm_dataportal_taxonpage_tabs', 1) != $form_state['values']['cdm_dataportal_taxonpage_tabs'];
  if($tab_lables_modified || $tabs_enabled_modified){
    // we first need to set the variable to persist the changes setting
    variable_set('cdm_dataportal_taxonpage_tabs', $form_state['values']['cdm_dataportal_taxonpage_tabs']);
    variable_set(CDM_TAXONPAGE_TAB_LABELS, $form_state['values'][CDM_TAXONPAGE_TAB_LABELS]);
    menu_rebuild();
  }
}

function cdm_settings_layout_search_submit($form, &$form_state){
  // the visibility of media thumbnails also affects the ui of the search results
  // so reset the according session variable
  //    1. in order to give the user immediate
  //       feedback on potential setting changes
  //    2. let refresh the default if it has changed
  if (isset($_SESSION['pageoption']['searchtaxa']['showThumbnails'])) {
    unset($_SESSION['pageoption']['searchtaxa']['showThumbnails']);
  }
}

/**
 * Form validation handler for cdm_settings_general
 *
 * @param $form
 * @param $form_state
 */
function cdm_settings_general_validate($form, &$form_state) {

  if (!str_endsWith($form_state['values']['cdm_webservice_url'], '/')) {
    $form_state['values']['cdm_webservice_url'] .= '/';
  }

}

/**
 * Form submit handler for settings general.
 *
 * tasks performed:
 *  - clear the [cdm][taxonomictree_uuid] session variable since this taxonomictree_uuid might no longer bee valid
 *
 * @param $form
 * @param $form_state
 */
function cdm_settings_general_submit($form, &$form_state){
  // clear the [cdm][taxonomictree_uuid] session variable since this taxonomictree_uuid might no longer bee valid
  unset($_SESSION['cdm']['taxonomictree_uuid']);
  unset($_SESSION['cdm_login']);
}

/**
 * Form validation handler for cdm_settings_cache
 */
function cdm_settings_cache_validate($form, &$form_state) {
  if ($form_state['values']['cdm_webservice_cache'] != variable_get('cdm_webservice_cache', 1)) {
    cache_clear_all(NULL, 'cache_cdm_ws');
    // Better clear secref_cache since I can not be sure if the cache has not
    // be used during this response.
    cdm_api_secref_cache_clear();
  }

}

/**
 * Returns an associative array of the currently chosen settings for the EDIT map service or the defaults as
 * specified in EDIT_MAPSERVER_URI_DEFAULT and EDIT_MAPSERVER_VERSION_DEFAULT:
 *  - base_uri: the protocol and host part , e.g.: http://edit.africamuseum.be
 *  - version: the version, e.g.: v1.1
 *
 * @return array
 *    An associative array of the currently chosen settings for the EDIT map service or the defaults.
 */
function get_edit_map_service_settings() {

  $settings = variable_get('edit_map_server', array(
      'base_uri' => EDIT_MAPSERVER_URI_DEFAULT,
      'version' => EDIT_MAPSERVER_VERSION_DEFAULT
      )
  );

  return $settings;
}

/**
 * Returns the full edit map service URI e.g.: http://edit.africamuseum.be/edit_wp5/v1.1
 *
 * @return string
 *   The full edit map service URI e.g.: http://edit.africamuseum.be/edit_wp5/v1.1
 */
function get_edit_map_service_full_uri() {
  $settings = get_edit_map_service_settings();
  return $settings['base_uri'] . EDIT_MAPSERVER_PATH .  '/' . $settings['version'];
}


/**
 * Returns the version number of the currently selected edit mapserver as a float
 *
 * @return float
 *   The version number of the currently selected edit mapserver as a float.
 *   Returns 0 on error.
 */
function get_edit_map_service_version_number() {

  $pattern = '/v([\d\.]+).*$/';

  $settings = get_edit_map_service_settings();
  preg_match($pattern, $settings['version'], $matches, PREG_OFFSET_CAPTURE);
  if (isset($matches[1])) {
    // Convert string to float.
    $version = 1 + $matches[1][0] - 1;
    return $version;
  }
  else {
    // Report error.
    drupal_set_message(t(" Invalid EDIT map service version number: '!version'",
        array(
          '!version' => $settings['version'],
          'warning')
        )
      );
    return 0;
  }
}

/**
 * Returns the array of selected items in a options array
 *
 * @param array $options
 *   An options array as generated by a form element like checkoxes, select ...,
 */
function get_selection($options) {
  $selection = array();
  foreach ($options as $key=>$val) {
    if (!empty($val)) {
      $selection[] = $val;
    }
  }
  return $selection;
}


/**
 * Implements hook_element_info().
 *
 * Allows modules to declare their own Form API element types and specify their default values.
 *
 * @see http://api.drupal.org/api/drupal/modules!system!system.api.php/function/hook_element_info/7
 */
function cdm_dataportal_element_info() {
  $type['checkboxes_preferred'] = array(
    '#input' => TRUE,
    '#process' => array('checkboxes_preferred_expand'),
    '#after_build' => array('checkboxes_preferred_after_build'),
    '#theme' => array('checkboxes_preferred'),
    // '#theme_wrapper' => array('form_element'),
  );
  return $type;
}

/**
 * #process callback function for the custom form element type 'checkbox_preferred'
 *
 *
 */
function checkboxes_preferred_expand($element, &$form_state, $form) {

  // First of all create checkboxes for each of the elements
  $element = form_process_checkboxes($element);

  // compose the element name
  $parents = array();
  array_deep_copy($element['#parents'], $parents);
  $parents[count($parents) -1 ] .= '_preferred';
  $element_name = $parents[0];
  for ($i=1; $i < count($parents); $i++){
    $element_name .= '[' . $parents[$i] . ']';
  }

  $children = element_children($element);

  $element['table_start'] = array(
    '#markup' => '<table class="checkboxes_preferred"><tr><th></th><th>' . t('Enabled') . '</th><th>' . t('Default') . '</th></tr>',
    '#weight' => -1,
  );

  // prepare first part each of the table rows which contains the row label
  $weight = 0;
  foreach ($children as $key) {
    $odd_even = $weight % 4 == 0 ? 'odd' : 'even';
    $element[$key]['#weight'] = $weight;
    $element[$key]['#prefix'] = '<tr class="' . $odd_even . '"><td>' . t('@row-label', array('@row-label' => $element['#options'][$key])) . '</td><td>';
    $element[$key]['#suffix'] = '</td>';
    unset($element[$key]['#title']);
    $weight += 2;
  }
  $weight = 0;

  // add a radio button to each of the checkboxes, the
  // check boxes have already been created at the beginning
  // of this function
  if (count($element['#options']) > 0) {
    foreach ($element['#options'] as $key => $choice) {
      if (!isset($element[$key . '_preferred'])) {
        $element[$key . '_preferred'] = array(
          '#type' => 'radio',
          '#name' => $element_name,
          '#return_value' => check_plain($key),
          '#default_value' => empty($element['#default_value_2']) ? NULL : $element['#default_value_2'],
          '#attributes' => $element['#attributes'],
          '#parents' => $element['#parents'],
          // '#spawned' => TRUE,
          '#weight' => $weight + 1,
          '#prefix' => '<td>',        // add a prefix to start a new table cell
          '#suffix' => '</td></tr>',  // add a prefix to close the tabel row
        );
      }
      $weight += 2;
    }
  }

  // end the table
  $element['table_end'] = array(
    '#markup' => '</table>',
    '#weight' => $weight++,
  );

  return $element;
}

/**
 * Theme function for the custom form field 'checkboxes_preferred'.
 */
function theme_checkboxes_preferred($variables) {
  $element = $variables['element'];
  $out = '<div id="edit-baselayers-wrapper" class="form-item">';
  $out .= '<label for="edit-baselayers">' . $element['#title'] . '</label>';
  $out .= drupal_render_children($element);
  $out .= '<div class="description">' . $element['#description'] . '</div>';
  $out .= '</div>';
  return $out;
}

/**
 * Callback for checkboxes preferred for widget which will
 * be called after the form or element is built. The call
 * back is configured in the form element by setting it as
 * #after_build parameter.
 *
 * @see http://api.drupal.org/api/drupal/developer!topics!forms_api_reference.html/7#after_build
 *
 * @param $element
 *   Nested array of form elements that comprise the form.
 * @param $form_state
 *   A keyed array containing the current state of the form.
 *   This includes the current persistent storage data for the form.
 *   Additional information, like the sanitized $_POST data,
 *   is also accumulated here in $form_state['input']
 *
 * @return the modified form array
 */
function checkboxes_preferred_after_build($element, &$form_state) {

  $parent_id = $element['#parents'][count($element['#parents']) - 1];

  if ($_POST && count($_POST) > 0) {
    // TODO use  $form_state['input'] instead of POST !!!
    // First pass of form processing.
    $parents = array();
    array_deep_copy($element['#parents'], $parents);
    $parents[count($parents) - 1] .= '_preferred';
    $preferred_layer = drupal_array_get_nested_value($_POST, $parents);
    $element['#value']['PREFERRED'] = $preferred_layer;
//     $form_state[$parent_id] = $element['#value'];
//     $form_state['values']['baselayers'] = $element['#value'];
    $form_state_element_values = &drupal_array_get_nested_value($form_state['values'], $element['#parents']);
    $form_state_element_values = $element['#value'];
  }
  else {
    // Second pass of form processing.
    $preferred_layer = $element['#value']['PREFERRED'];
  }

  // Also set the chosen value (not sure if this is good Drupal style ....).
  foreach ($children = element_children($element) as $key) {
    if (!empty($element[$key]['#type']) && $element[$key]['#type'] == 'radio') {
      $element[$key]['#value'] = $preferred_layer;
    }
  }
  // The default layer must always be enabled.
  $element[$preferred_layer]['#value'] = $preferred_layer;

  return $element;
}

function radios_prepare_options_suffix(&$elements){

  $childrenKeys = element_children($elements);
  foreach($childrenKeys as $key){
    if(!is_array($elements[$key]['#theme_wrappers'])){
      $elements[$key]['#theme_wrappers'] = array();
    }
    if(isset($elements['#options_suffixes'][$key])){
      $elements[$key]['#theme_wrappers'][] = 'radio_options_suffix';
      $elements[$key]['#options_suffix'] = $elements['#options_suffixes'][$key];
    }
  }
  return $elements;

}

/**
 * TODO
 * @param unknown $variables
 */
function theme_radio_options_suffix($variables) {
  $element = $variables['element'];
  if(isset($element['#options_suffix'])) {
    $element['#children'] .= $element['#options_suffix'];
  }
  return $element['#children'];
}


/**
 * Element validate callback for text field and arrays containing json.
 *
 * @param $element
 *   The form element to validate
 * @param $form_state
 *   A keyed array containing the current state of the form.
 * @param $form
 *   Nested array of form elements that comprise the form.
 */
function form_element_validate_json($element, &$form_state, $form) {
   if (!empty($element['#value'])) {
     json_decode($element['#value']);
     if(json_last_error() != JSON_ERROR_NONE){
       form_error($element,
         t('The form element %title contains invalid JSON. You can check the syntax with ', array('%title' => $element['#title']))
         . l('JSONLint', 'http://jsonlint.com/')
       );
     }
   }
}

/**
 * Form submission handler for textareas and textfields containing JSON.
 *
 * The contained JSON will be converted into an php array
 * or object and will be stored in the variables as such.
 *
 * @see http://api.drupal.org/api/drupal/developer!topics!forms_api_reference.html/7#submit
 *
 * @param $form
 *   Nested array of form elements that comprise the form.
 * @param $form_state
 *   A keyed array containing the current state of the form.
 *
 */
function submit_json_as_php_array($form, &$form_state) {
  if (is_array($form['#json_elements'])) {
    foreach ($form['#json_elements'] as $element){
      if (trim($form_state['values'][$element])) {
        $form_state['values'][$element] = (array) json_decode($form_state['values'][$element]);
      } else {
        $form_state['values'][$element] = NULL;
      }
    }
  }
}
