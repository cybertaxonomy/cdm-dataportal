<?php
/**
 * @file
 * Node type definitions.
 */

define('NODETYPE_TAXON', 'taxon');
define('NODETYPE_MEDIA', 'media');
define('NODETYPE_REFERENCE', 'reference');
define('NODETYPE_NAME', 'name');

/**
 * Implements hook_cdm_nodetypes().
 */
function cdm_dataportal_cdm_nodetypes() {
  static $nodetypes;
  if (!$nodetypes) {
    $nodetypes = array(
      'cdm_' . NODETYPE_REFERENCE => NODETYPE_REFERENCE,
      'cdm_' . NODETYPE_TAXON => NODETYPE_TAXON,
      'cdm_' . NODETYPE_MEDIA => NODETYPE_MEDIA,
      'cdm_' . NODETYPE_NAME => NODETYPE_NAME,
    );
  }
  return $nodetypes;
}

/**
 * Implements hook_node_info().
 */
function cdm_dataportal_node_info() {
  $nodeinfo = array();
  foreach (cdm_get_nodetypes() as $nodeType => $type) {
    $nodeinfo[$nodeType] = array(
      'name' => t(ucfirst($type)),
      'has_title' => TRUE,
      'base' => 'cdm_dataportal',
      'description' => t(
        'This node type is being used internally to create peer nodes
         in drupal for cdm entities of the type !type.', array('!type' => $type)),
    );
  }

  return $nodeinfo;
}

/**
 * Implements hook_form().
 */
function cdm_dataportal_form(&$node) {

  $type = node_type_get_type($node);

  if (is_numeric($node->nid)) {
    $cdm_node_notice = t('In order to edit CDM content, please use the ') . l(t('Taxonomic Editor'), 'http://dev.e-taxonomy.eu/trac/wiki/TaxonomicEditor', array('fragment' => TRUE));
  }
  else {
    $cdm_node_notice = t('You cannot manually create a node of type ') . $type->name . '. ' . t('This node type is only created internally');
  }
  $form['cdm'] = array(
    '#value' => '<div class="cdm_node_notice warning">' . $cdm_node_notice . '</div>',
    '#weight' => -5,
  );

  // We need to define form elements for the node's title and body.
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => check_plain($type->title_label),
    '#required' => TRUE,
    '#disabled' => TRUE,
    '#default_value' => $node->title,
    '#weight' => -5,
  );

  return $form;
}

/**
 * @todo document this function.
 */
function cdm_get_nodetypes() {
  $nodetypes = module_invoke_all('cdm_nodetypes');
  return $nodetypes;
}
