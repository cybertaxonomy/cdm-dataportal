<?php

function cdm_load_node($nodetype, $uuid, $title){
  
  // try to find node id
  $cdmnode = db_fetch_object(db_query('SELECT nid, cdmtype FROM {node_cdm} WHERE wsuri = \'%s\' AND cdmtype = \'%s\' AND uuid = \'%s\''
      , variable_get('cdm_webservice_url', NULL),  $nodetype, $uuid));
    
  if(is_numeric($cdmnode->nid)) {
    $node = node_load($cdmnode->nid);
  } else {
    
    // Create a new node
    $node->type = 'cdm_'.$nodetype;
    // use just the plain text of the HTML title
    $values['title'] = filter_xss($title, array()); 
    // limit length to the max length of the database field 128  
    $values['title'] = substr($values['title'], 0, 128);

    // preserve the current messages but before  saveing the node, 
    $messages = drupal_set_message();

    $result = drupal_execute($node->type.'_node_form', $values, $node);
    
    // restore the messages
    if(isset($messages)){
      $_SESSION['messages'] = $messages;
    }
    
    if(!is_array($result)){
      // result should contain the path the newly created node; e.g.: node/32
      $pathelements = explode('/', $result);
      $nid = array_pop($pathelements); 
      $node->nid = $nid;
      
      db_query('INSERT INTO {node_cdm} (nid, wsuri, cdmtype, uuid) VALUES (%d, \'%s\', \'%s\', \'%s\');'
        , $nid, variable_get('cdm_webservice_url', NULL),  $nodetype, $uuid);
      
    } else {
      drupal_set_message(t('Could not create node for ' . $nodetype),'error');
    }
  }
  
  return $node;
}


/**
 * 
 * @param $cdm_node_type one of the NODETYPE_* constants 
 * @param $uuid
 * @param $content
 * @param $title
 * 
 */
function cdm_node_show($cdm_node_type, $uuid, $title, $content){
    $node = cdm_load_node($cdm_node_type, $uuid, $title);
    drupal_set_title($title);
    cdm_add_node_content($node, $content, variable_get('cdm_content_weight', -1));
    return node_show($node, null); //TODO is using null for cid OK?
}

 
/**
 * 
 * @param $node
 * @param $content
 * @param $weight
 * @return unknown_type
 */
function cdm_add_node_content(&$node, &$content, $weight){
    $cdm_content = array(
        '#value' => $content, 
        '#weight' => variable_get('cdm_content_weight', -1)
    );
    $node->content['cdm'] = $cdm_content;
}
