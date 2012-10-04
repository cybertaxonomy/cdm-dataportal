<?php

/**
 * Implements hook_node_view().
 */
function cdm_dataportal_node_view($node, $view_mode = 'full') {
  // see cdm_add_node_content
  switch ($node->type) {
    case 'cdm_' . NODETYPE_TAXON:
      $node->content['cdm'] = isset($node->cdm) ? $node->cdm : '';
      break;
    case 'cdm_' . NODETYPE_TAXON:
      $node->content['cdm'] = isset($node->cdm) ? $node->cdm : '';
      break;
  }
}

/**
 * Implements hook_node_delete().
 */
function cdm_dataportal_node_delete($node) {
  if (array_key_exists($node->type, cdm_get_nodetypes())) {
    // TODO Please review the conversion of this statement to the D7 database API syntax.
    /* db_query("DELETE FROM {node_cdm} WHERE nid = %d", $node->nid) */
    db_delete('node_cdm')->condition('nid', $node->nid)->execute();
  }
}
function cdm_load_node($nodetype, $uuid, $title) {
  
  // try to find node id
  $cdmnode = db_query('SELECT nid, cdmtype FROM {node_cdm} WHERE wsuri = :wsuri AND cdmtype = :cdmtype AND uuid = :uuid', array (
    ':wsuri' => variable_get('cdm_webservice_url', NULL), ':cdmtype' => $nodetype, ':uuid' => $uuid 
  ))->fetch();
  
  // Nid should not be 0 , if it is, something is wrong with the record. 
  if (isset($cdmnode->nid) && $cdmnode->nid == 0) {
    drupal_set_message(t('Something is wrong with the record for uuid=%uuid, please contact the helpdesk.', array('%uuid' => $uuid)),'error');
    return;
  }
  if (isset($cdmnode->nid) && is_numeric($cdmnode->nid) ) {
    $node = node_load($cdmnode->nid);
  }
  else {
    // Create a new node
    $node = new stdClass();
    $node->type = 'cdm_' . $nodetype;
    $node->language = LANGUAGE_NONE; // @WA TODO set to e.g. 'en' if locale is enabled

    // Set some default values for 'status', 'promote', 'sticky'. 'uid' and 'created'
    node_object_prepare($node); 
    
    // use just the plain text of the HTML title
    $title = filter_xss($title, array ());
    // limit length to the max length of the database field 128
    $title = substr($title, 0, 128);
    $node->title = $title;
    $node->uid = 0; // @WA this was used in the D5 module version. Remove this to change it to the current user.
    $node->comment = variable_get('comment__' . $node->type . $nodetype, false);
    
    // preserve the current messages but before saving the node,
    $messages = drupal_set_message();
    
    if ($node = node_submit($node)) { // Prepare node for saving by populating author and creation date
      // @WA: Note that node_save is using a helper function to save a revision with the uid of the current user
      // so the revision will not have uid = 0 but the uid of the current user.
      // I guess that is not a problem so I leave it like this. Remedy would be to alter that revision entry afterwards.
      node_save($node); //will create a watchdog log entry if it fails to create the node 
      //echo "Node with nid " . $node->nid . " saved!\n";
    } 
    
    // restore the messages
    $_SESSION['messages'] = $messages;
    
    //@WA I think http://dev.e-taxonomy.eu/trac/ticket/2964 is not relevant here anymore, since
    //node_save will roll_back if node cannot be created.
    if (!isset($node->nid)) {
      $message = t('Could not create node for ') . $nodetype . '('. $title . ')';
      drupal_set_message($message, 'error');
      watchdog('content', $message, WATCHDOG_ERROR);
      return null;
    }
    
    //hash as a 32-character hexadecimal number
    $hash = md5(variable_get('cdm_webservice_url') . $uuid); 
    
    $id = db_insert('node_cdm')->fields(array(
      'nid' => $node->nid,
      'wsuri' => variable_get('cdm_webservice_url'),
      'hash' => $hash,
      'cdmtype' => $nodetype,
      'uuid' => $uuid,
    ))->execute();    
  }
    
  return $node;
}

/**
 *
 * @param $cdm_node_type one
 *          of the NODETYPE_* constants
 * @param
 *          $uuid
 * @param
 *          $content
 * @param
 *          $title
 *
 */
function cdm_node_show($cdm_node_type, $uuid, $title, $content) {
  $node = cdm_load_node($cdm_node_type, $uuid, $title);
  drupal_set_title($title, PASS_THROUGH);
  cdm_add_node_content($node, $content, variable_get('cdm_content_weight', - 1));
  return node_show($node);
}

/**
 *
 * @param
 *          $node
 * @param
 *          $content
 * @param
 *          $weight
 * @return unknown_type
 */
function cdm_add_node_content(&$node, $content, $weight) {
  $cdm_content = array (
  // wrap content in cdm_dataportal specific container
  '#markup' => '<div id="cdm_dataportal.node">' . $content . '</div>', '#weight' => variable_get('cdm_content_weight', - 1) 
  );
  
  // @WA: for some reason $node->content is lost or recreated in node_show($node) in D7, 
  // so we attach to $node->cdm here and re-attach to $node->content in hook_node_view
  $node->cdm = $cdm_content;
}

/**
 * Deletes all cdm nodes, used when module is uninstalled
 */
function cdm_delete_all_cdm_nodes() {
  $result = db_query('SELECT n.nid FROM {node} n WHERE n.type like :type', array(':type' => 'cdm_%'));
  foreach ($result as $node) {
    node_delete($node->nid);
  }
}
