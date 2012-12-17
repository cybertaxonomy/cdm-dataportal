<?php
/**
 * @file
 * CDM Node functions.
 */

/**
 * Implements hook_node_view().
 *
 * Comment @WA should this also be used for other cdm node types like name page?
 */
function cdm_dataportal_node_view($node, $view_mode = 'full') {
  // See cdm_add_node_content.
  switch ($node->type) {
    case 'cdm_' . NODETYPE_TAXON:
      if (!isset($node->cdm) && arg(0) == 'node') {
        // If a node page is loaded directly, e.g. node/%nid instead of
        // cdm_dataportal/taxon/%uuid, try to load the taxon page content
        // into $node->cdm.
        // only do this for node pages (where arg(0) = node),
        // not for pages like comment/reply/%nid.
        $cdmnode = db_query('SELECT uuid FROM {node_cdm} WHERE nid = :nid', array(
          ':nid' => $node->nid,
        ))->fetch();
        if (isset($cdmnode->uuid)) {
          cdm_dataportal_taxon_page_view($cdmnode->uuid);
        }
      }
      $node->content['cdm'] = isset($node->cdm) ? $node->cdm : '';
      break;
  }

}

/**
 * Implements hook_node_delete().
 */
function cdm_dataportal_node_delete($node) {
  if (array_key_exists($node->type, cdm_get_nodetypes())) {
    db_delete('node_cdm')->condition('nid', $node->nid)->execute();
  }
}

/**
 * Loads the node if one exist for $uuid, or else creates one.
 *
 * @param string $nodetype
 *   The node type.
 * @param string $uuid
 *   UUID for which to load the node.
 * @param string $title
 *   The node title to display for the node.
 *
 * @return mixed
 *   The node object for $uuid.
 */
function cdm_load_node($nodetype, $uuid, $title) {

  // Try to find node id.
  $cdmnode = db_query('SELECT nid, cdmtype FROM {node_cdm} WHERE wsuri = :wsuri AND cdmtype = :cdmtype AND uuid = :uuid', array(
    ':wsuri' => variable_get('cdm_webservice_url', NULL),
    ':cdmtype' => $nodetype,
    ':uuid' => $uuid,
  ))->fetch();

  // Nid should not be 0 , if it is, something is wrong with the record.
  if (isset($cdmnode->nid) && $cdmnode->nid == 0) {
    drupal_set_message(t('Something is wrong with the record for uuid=%uuid,
      please contact the helpdesk.', array('%uuid' => $uuid)), 'error');
    return;
  }
  if (isset($cdmnode->nid) && is_numeric($cdmnode->nid)) {
    $node = node_load($cdmnode->nid);
  }
  else {
    // Create a new node.
    $node = new stdClass();
    $node->type = 'cdm_' . $nodetype;
    // Comment @WA TODO set to e.g. 'en' if locale is enabled.
    $node->language = LANGUAGE_NONE;

    // Set some default values for:
    // 'status', 'promote', 'sticky', 'uid' and 'created'.
    node_object_prepare($node);

    // Use just the plain text of the HTML title.
    $title = filter_xss($title, array());

    // Limit length to the max length of the database field 128.
    $title = substr($title, 0, 128);
    $node->title = $title;

    // Comment @WA: this was used in the D5 module version.
    // Remove this to change it to the current user.
    $node->uid = 0;

    // 2 = comments on, 1 = comments off.
    $node->comment = variable_get('comment_' . $node->type);

    // Preserve the current messages but before saving the node.
    $messages = drupal_set_message();

    if ($node = node_submit($node)) {
      // Prepare node for saving by populating author and creation date.
      // Comment @WA: Note that node_save is using a helper function to save a
      // revision with the uid of the current user so the revision will not
      // have uid = 0 but the uid of the current user.
      // I guess that is not a problem so I leave it like this. Remedy would be
      // to alter that revision entry afterwards.
      // Will create a watchdog log entry if it fails to create the node.
      node_save($node);
    }

    // Restore the messages.
    $_SESSION['messages'] = $messages;

    // Comment @WA: I think http://dev.e-taxonomy.eu/trac/ticket/2964 is not
    // relevant here anymore, since node_save will roll_back if node cannot be
    // created.
    if (!isset($node->nid)) {
      $message = t('Could not create node for !nodetype (!title).', array(
        '!nodetype' => $nodetype,
        '!title' => $title,
      ));
      drupal_set_message($message, 'error');
      watchdog('content', $message, WATCHDOG_ERROR);
      return NULL;
    }

    // Hash as a 32-character hexadecimal number.
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
 * Wrapper function around node_show()
 *
 * Just like the drupal function node_show() this function will generate an
 * array which displays a node detail page. Prior calling node_show() this
 * function assures that the special cdm node types are undegone the nessecary
 * preprocessing.
 *
 * This function will be called by a cdm_dataportal_{CDM_NODE_TYPE}_view function.
 *
 *
 * @param String $cdm_node_type
 *     one of the cdm content type names as defined in
 *     the file node_types.php. Possible values are 'taxon', 'media', 'reference', 'name'.
 *     you may want to use the according contstants instred of the string: NODETYPE_TAXON,
 *     NODETYPE_MEDIA, NODETYPE_REFERENCE, NODETYPE_NAME.
 * @param String $uuid
 *     the UUID string of the cdm entitiy to be shown. The cdm type is of cource defined by
 *     the  $cdm_node_type value
 * @param String $title
 *     the Page title
 * @param String or render array? $content
 *
 * @return
 *     A $page element suitable for use by drupal_render().
 */
function cdm_node_show($cdm_node_type, $uuid, $title, $content) {
  // tell drupal code to load the node
  $node = cdm_load_node($cdm_node_type, $uuid, $title);
  // set the title coming supplied by a cdm_dataportal_{CDM_NODE_TYPE}_view function
  drupal_set_title($title, PASS_THROUGH);

  cdm_add_node_content($node, $content);
  return node_show($node);
}

/**
 * Sets the $content given a paramater to the $node object
 *
 * The $content can either be a string or an array.
 *
 * see:
 *  - element_children()
 *  - drupal_render()
 *  - http://api.drupal.org/api/drupal/includes!common.inc/function/drupal_render/7#comment-6644
 *
 * TODO see notes near bottom of function
 *
 * @param object $node
 *   A $node object
 * @param string|array $content
 *   The content to set for the $node
 *
 */
function cdm_add_node_content(&$node, $content) {

  if(is_array($content)) {
    // $content seems to be a render array suitable for drupal_render()
    $cdm_content = array(
        // Wrap content in cdm_dataportal specific container.
        '#prefix' => '<div id="cdm_dataportal.node">',
        '#suffix' => '</div>',
        // the key of child elements can be chosen arbitrarily it only must not start with a '#'
        'content' => $content,
        '#weight' => variable_get('cdm_content_weight', -1),
    );
  } else {
    $cdm_content = array(
      // Wrap content in cdm_dataportal specific container.
      '#markup' => '<div id="cdm_dataportal.node">' . $content . '</div>',
      '#weight' => variable_get('cdm_content_weight', -1),
    );
  }

  // Comment @WA: for some reason $node->content is lost or recreated in
  //   node_show($node) in D7, so we attach to $node->cdm here and re-attach to
  //   $node->content in hook_node_view.
  //
  // Followup by @AK:
  //   $node->content is removed in node_build_content() we need to
  //   implement the 'view' hook in order to set the  $node->content
  //   properly in the drupal way. => TODO
  //
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
