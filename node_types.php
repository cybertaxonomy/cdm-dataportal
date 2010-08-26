<?php

define(NODETYPE_TAXON, 'taxon');
define(NODETYPE_MEDIA, 'media');
define(NODETYPE_REFERENCE, 'reference');
define(NODETYPE_NAME, 'name');

function cdm_dataportal_get_nodetypes(){
	static $nodetypes;
	  if(!$nodetypes){
	    $nodetypes = array('cdm_'.NODETYPE_REFERENCE => NODETYPE_REFERENCE, 'cdm_'.NODETYPE_TAXON => NODETYPE_TAXON, 'cdm_'.NODETYPE_MEDIA => NODETYPE_MEDIA, 'cdm_'.NODETYPE_NAME => NODETYPE_NAME); 
	  }
	  return $nodetypes; 
}

/**
 * Implementation of hook_node_info().
 */
function cdm_dataportal_node_info() {
	
  $nodeinfo = array();
  foreach( cdm_dataportal_get_nodetypes() as $nodeType=>$type){
    $nodeinfo[$nodeType] = array(
      'name' => t(ucfirst($type)),
      'has_title' => TRUE,
      'module' => 'cdm_dataportal',
      'description' => t('Node type with reflects cdm entities of the type' . $type . '.')
    );
  }
  
  return $nodeinfo;
}


/**
 *  Implementation of hook_form()
 */
function cdm_dataportal_form(&$node) {
  
  $type = node_get_types('type', $node);
  
  if(is_numeric($node->nid)){
    $cdm_node_notice = t('In order to edit CDM content, please use the '). l(t('Taxonomic Editor'), 'http://dev.e-taxonomy.eu/trac/wiki/TaxonomicEditor', NULL, NULL, TRUE);   
  } else {
    $cdm_node_notice = t('You cannot manually create a node of type ').$type->name . '. '. t('This node type is only created internally');
  }
  $form['cdm'] = array(
    '#value' => '<div class="cdm_node_notice warning">' . $cdm_node_notice . '</div>',
    '#weight' => -5
  );
  // We need to define form elements for the node's title and body.
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => check_plain($type->title_label),
    '#required' => TRUE,
    '#disabled' => TRUE,
    '#default_value' => $node->title,
    '#weight' => -5
  );
  
  return $form;
}


/**
 * Implementation of hook_nodeapi().
 */
function cdm_dataportal_nodeapi(&$node, $op, $teaser, $page) {
  switch ($op) {
    case 'view':
      switch($node->type){
        case 'cdm_'.NODETYPE_TAXON : 
          if(arg(0) == 'node'){
            $cdmnode = db_fetch_object(db_query('SELECT * FROM {node_cdm} WHERE nid = \'%d\''
              , $node->nid));
            $taxonpage = cdm_dataportal_taxon_view($cdmnode->uuid);
            drupal_set_title($taxonpage->title);
            cdm_add_node_content($node, $taxonpage->content, variable_get('cdm_content_weight', -1));
          }
          break;
        case 'cdm_'.NODETYPE_NAME : 
          if(arg(0) == 'node'){
            $cdmnode = db_fetch_object(db_query('SELECT * FROM {node_cdm} WHERE nid = \'%d\''
              , $node->nid));
            $namepage = cdm_dataportal_name_view($cdmnode->uuid);
            drupal_set_title($namepage->title);
            cdm_add_node_content($node, $namepage->content, variable_get('cdm_content_weight', -1));
          }
          break;  
      }
      break;   
  }
}