<?php

function cdm_get_nodetypes(){

	$nodetypes = module_invoke_all('cdm_nodetypes');
	return $nodetypes;

}

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
		$values['comment'] =  variable_get('comment_'. $nodetype, $comment_node_disabled);

		// preserve the current messages but before  saveing the node,
		$messages = drupal_set_message();

		$result = drupal_execute($node->type.'_node_form', $values, $node);
		// restore the messages
		$_SESSION['messages'] = $messages;

		if(!is_array($result)){
			// result should contain the path to the newly created node; e.g.: node/32
			$pathelements = explode('/', $result);
			$nid = array_pop($pathelements);

			// ---- checking for problems, see http://dev.e-taxonomy.eu/trac/ticket/2964
			if($nid == 0){
			  $message = t('Error creating node for ') . $nodetype . ',  cause: new node id was 0';
			  drupal_set_message($message ,'error');
			  watchdog('content', $message . ' $pathelements was :' . $pathelements, WATCHDOG_ERROR);
			  return null;
			}
			if(!referer_uri() || !isset($_SERVER['REMOTE_ADDR'])){
			  $message = t('Stopping creating node for ') . $nodetype . '.  cause: referer uri or host address was empty';
			  drupal_set_message($message ,'error');
			  watchdog('content', $message, WATCHDOG_ERROR);
			  return null;
			}
			// ---- END of checking for problems

			$node->nid = $nid;
            $hash = md5( variable_get('cdm_webservice_url', NULL) . $uuid ); // hash as a 32-character hexadecimal number.
			db_query('INSERT INTO {node_cdm} (nid, wsuri, hash, cdmtype, uuid) VALUES (%d, \'%s\', \'%s\', \'%s\', \'%s\');'
			, $nid, variable_get('cdm_webservice_url', NULL), $hash,  $nodetype, $uuid);

		} else {
            $message = t('Could not create node for') . $nodetype;
			drupal_set_message($message ,'error');
			watchdog('content', $message, WATCHDOG_ERROR);
			return null;
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
		// wrap content in cdm_dataportal specific container
        '#value' => '<div id="cdm_dataportal.node">' . $content . '</div>',
        '#weight' => variable_get('cdm_content_weight', -1)
	);

	$node->content['cdm'] = $cdm_content;
}

function cdm_delete_all_cdm_nodes(){

	$result = db_query("SELECT * FROM {node} WHERE type like '%s';", 'cdm_%');
	while ($node = db_fetch_object($result)) {
		node_delete($node->nid);
	}
}
